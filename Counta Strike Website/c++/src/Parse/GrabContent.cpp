#include "GrabContent.h"
#include "Item.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


// edit the parse attributes now
void GrabContent::Run(Parse& parse)
{
	std::string fileLoc{ parse.getFileLoc() };
	std::string content{ };

	std::ifstream in(fileLoc, std::ios::binary);
	if (!in) throw std::runtime_error("Failed to open: " + fileLoc);

	std::ostringstream ss;
	ss << in.rdbuf();
	content = ss.str();

	// we want to generate the blocks
	GenerateBlocks(parse, content);
};



void GrabContent::GenerateBlocks(Parse& parse, std::string content)
{
    std::istringstream stream(content);
    std::string line;
    std::string currentBlock;
    bool inBlock = false;
    bool isEnumClass = false;
    std::string blockName;
    
    while (std::getline(stream, line)) {
        std::string trimmed = TrimWS(line);
        
        if (!inBlock) {
            if (shouldskip(trimmed)) continue;
            
            if (trimmed.find("enum class") == 0) {
                StartBlock(line, trimmed, true, inBlock, isEnumClass, blockName, currentBlock);
            }
            else if (trimmed.find("namespace") == 0) {
                StartBlock(line, trimmed, false, inBlock, isEnumClass, blockName, currentBlock);
            }
        }
        else {
            currentBlock += line + "\n";
            
            if (trimmed == "};" || trimmed == "}") {
                FinishBlock(parse, isEnumClass, blockName, currentBlock, inBlock);
            }
        }
    }
}

void GrabContent::FinishBlock(Parse& parse, bool isEnumClass, const std::string& blockName, 
                              std::string& currentBlock, bool& inBlock)
{
    Block block(isEnumClass, blockName);
    
    ParseItems(block, currentBlock);
    
    parse.add(block);
    currentBlock.clear();
    inBlock = false;
}
void GrabContent::ParseItems(Block& block, const std::string& blockContent)
{
    std::istringstream stream(blockContent);
    std::string line;
    bool isFirstLine = true;
    int lineNum = 0;
    
    while (std::getline(stream, line)) {
        lineNum++;
        
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        
        std::string trimmed = TrimWS(line);
        
        if (trimmed.empty() || trimmed.find("//") == 0 || trimmed == "};" || trimmed == "}") {
            continue;
        }
        
        
        Item item = ParseLine(trimmed, block.getIsEnumClass());
        if (!item.getName().empty()) {
			block.addItem(item);
        }
    }
}

Item GrabContent::ParseLine(const std::string& line, bool isEnumClass)
{
    if (isEnumClass) {
        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) return Item();
        
        std::string name = TrimWS(line.substr(0, equalPos));
        
        std::string offsetPart = line.substr(equalPos + 1);
        size_t commaPos = offsetPart.find(',');
        if (commaPos != std::string::npos) {
            offsetPart = offsetPart.substr(0, commaPos);
        }
        std::string offset = TrimWS(offsetPart);
        
        return Item(name, "", offset);
    }
    else {
        
        if (line.find("constexpr") == std::string::npos) {
            return Item();
        }
        
        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) return Item();
        
        size_t semicolonPos = line.find(';');
        if (semicolonPos == std::string::npos) return Item();
        
        std::string beforeEqual = TrimWS(line.substr(0, equalPos));  
        
        // check name after last space/tab
        size_t lastSpace = beforeEqual.find_last_of(" \t");
        if (lastSpace == std::string::npos) return Item();
        
        std::string name = TrimWS(beforeEqual.substr(lastSpace + 1));
        
        // extract offset (between '=' and ';')
        std::string offset = TrimWS(line.substr(equalPos + 1, semicolonPos - equalPos - 1));
        
        // extract type from comment (after '//')
        std::string type = "";
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos && commentPos < line.length() - 2) {
            type = TrimWS(line.substr(commentPos + 2));
        }
        
        return Item(name, type, offset);
    }
}

std::string GrabContent::TrimWS(const std::string& str)
{
    std::string trimmed = str;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
    trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
    return trimmed;
}

bool GrabContent::shouldskip(const std::string& trimmed)
{
    return trimmed.empty() || trimmed.find("//") == 0;
}

std::string GrabContent::ExtractName(const std::string& trimmed, size_t startPos, const std::string& delimiters)
{
    size_t nameEnd = trimmed.find_first_of(delimiters, startPos);
    if (nameEnd == std::string::npos) nameEnd = trimmed.length();
    
    std::string name = trimmed.substr(startPos, nameEnd - startPos);
    return TrimWS(name);
}

void GrabContent::StartBlock(const std::string& line, const std::string& trimmed, bool isEnum, 
                             bool& inBlock, bool& isEnumClass, std::string& blockName, std::string& currentBlock)
{
    inBlock = true;
    isEnumClass = isEnum;
    
    size_t nameStart = isEnum ? 10 : 9;
    std::string delimiters = isEnum ? " :{" : "{";
    
    blockName = ExtractName(trimmed, nameStart, delimiters);
    currentBlock = line + "\n";
}

void GrabContent::PrintBlocks(Parse& parse)
{
	std::vector<Block> blocks = parse.getContent();

	std::cout << "\n========================================" << std::endl;
	std::cout << "TOTAL BLOCKS FOUND: " << blocks.size() << std::endl;
	std::cout << "========================================\n"
			  << std::endl;

	for (size_t i = 0; i < blocks.size(); ++i)
	{
		std::cout << "\n========================================" << std::endl;
		std::cout << "BLOCK #" << (i + 1) << std::endl;
		std::cout << "Type: " << (blocks[i].getIsEnumClass() ? "ENUM CLASS" : "NAMESPACE") << std::endl;
		std::cout << "Name: " << blocks[i].getName() << std::endl;
		std::cout << "Items: " << blocks[i].getContent().size() << std::endl;
		std::cout << "========================================" << std::endl;

		for (const auto& item : blocks[i].getContent())
		{
			std::cout << "  Name: " << item.getName()
					  << " | Offset: " << item.getOffset();
			if (!item.getType().empty())
			{
				std::cout << " | Type: " << item.getType();
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}
