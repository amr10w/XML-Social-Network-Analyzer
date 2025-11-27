#include "CLICommands.h"

int CLICommands::handle(int argc, char *argv[])
{
     if(argc < 2) {
        std::cout << "Usage: XNet <command> [options]\n";
        return 1;
    }

    std::string cmd = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    if(cmd == "verify") return verifyCommand(args);

    std::cerr << "Unknown command: " << cmd << "\n";
    return 1;

}

int CLICommands::verifyCommand(const std::vector<std::string> &args)
{
    if(args.size() < 2 || args[0] != "-i") {
        std::cout << "Usage: verify -i <filename>\n";
        return 1;
    }

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    std::cout << verify(content) << "\n";

    return 0;
}
