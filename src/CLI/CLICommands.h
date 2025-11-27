#include "FileUtils.h"
#include<XMLValidator.h>
#include <vector>

class CLICommands
{
public:
    int handle(int argc, char* argv[]);
private:
    int verifyCommand(const std::vector<std::string>& args);
};