#include "FileUtils.h"
#include "XMLValidator.h"
#include "XMLMinifier.h"
#include "Tree.h"
#include "XML_Compress.h"
#include "XML_Decompress.h"
#include "Graph.h"
#include "Mutual.h"
#include "GraphVisualizer.h"
#include <vector>

enum ErrorCode {
    OK = 0,
    ERR_UNKNOWN_COMMAND = 1,
    ERR_MISSING_ARGUMENT = 2,
    ERR_INVALID_OPTION = 3,
    ERR_FILE_NOT_FOUND = 4,
};
class CLICommands
{
public:
    int handle(int argc, char* argv[]);
private:
    int verifyCommand(const std::vector<std::string>& args);
    int minifyCommand(const std::vector<std::string>& args);
    int prettifyCommand(const std::vector<std::string>& args);
    int xmlToJsonCommand(const std::vector<std::string>& args);
    int compressCommand(const std::vector<std::string>& args);
    int decompressCommand(const std::vector<std::string>& args);
    int mutualCommand(const std::vector<std::string>& args);
    int visualizerCommand(const std::vector<std::string>& args);
};