#include "File.h"
#include <string>
#include <fstream>

// "../../output/output.txt"
void File::PutInTo(std::string fileTarget, std::string content)
{
	
	std::ofstream MyFile(fileTarget);

	MyFile << content;

	MyFile.close();
}
