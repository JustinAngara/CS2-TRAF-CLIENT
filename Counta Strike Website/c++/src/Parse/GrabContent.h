#pragma once
#include "Parse.h"
#include "Item.h"

namespace GrabContent
{
	void Run(Parse& parse);
	void GenerateBlocks(Parse& parse, std::string content);
	void PrintBlocks(Parse& parse);

    // helper functions
    std::string TrimWS(const std::string& str);
    bool shouldskip(const std::string& trimmed);
    std::string ExtractName(const std::string& trimmed, size_t startPos, const std::string& delimiters);
    void StartBlock(const std::string& line, const std::string& trimmed, bool isEnum, bool& inBlock, bool& isEnumClass, std::string& blockName, std::string& currentBlock);
    void FinishBlock(Parse& parse, bool isEnumClass, const std::string& blockName, std::string& currentBlock, bool& inBlock);

	void ParseItems(Block& block, const std::string& blockContent); 
	Item ParseLine(const std::string& line, bool isEnumClass);  

}
