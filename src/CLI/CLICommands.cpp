#include "CLICommands.h"


int CLICommands::handle(int argc, char *argv[])
{
    if(argc < 2) {
        std::cout << "Usage: xml_editor <command> [options]\n";
        return ERR_MISSING_ARGUMENT;
    }

    std::string cmd = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    if(cmd == "verify") return verifyCommand(args);
    if(cmd == "mini") return minifyCommand(args);
    if(cmd == "format") return prettifyCommand(args);
    if(cmd == "json") return xmlToJsonCommand(args);
    if(cmd == "compress") return compressCommand(args);
    if(cmd == "decompress") return decompressCommand(args);
    if(cmd == "mutual") return mutualCommand(args);
    if(cmd == "draw") return visualizerCommand(args);
    if(cmd == "search") return searchCommand(args);
    if(cmd == "most_active") return mostActiveCommand(args);
    if(cmd == "most_influencer") return mostInfluencerCommand(args);
    if(cmd == "suggest") return suggestCommand(args);

    

    std::cerr << "Unknown command: " << cmd << "\n";
    return ERR_UNKNOWN_COMMAND;

}

int CLICommands::verifyCommand(const std::vector<std::string> &args)
{
    if(args.size() < 2 || args[0] != "-i") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: verify -i <filename>\n";
        return ERR_INVALID_OPTION;
    }
    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content=="") return ERR_FILE_NOT_FOUND;
    bool result = verify(content);

    if(result)
    {
        std::cout << "Verification succeeded.\n";
        return OK;
    }
    else{
        std::cout << "Verification failed.\n";
    }

    ErrorInfo errors = countErrorSummary(content);
    std::cout<<"Number Of Errors: "<<errors.count<<"\n";
    
    for(int i=0;i<errors.lines.size();i++)
    {
        std::cout<<errors.lines[i]<<" "<<errors.descriptions[i]<<"\n";
    }
    

    if(!result&&args.size()> 2 )
    {
        if(args[2] == "-f" && args[3] == "-o")
        {
            std::string out =fixXml(content,errors);
            writeToFile(args[4],out);
            return OK;
        }
        else if(args[2] != "-f" || args[3] != "-o")
        {
            std::cerr<<"Invalid option\n";
            std::cerr << "Usage: verify -i <filename> -f -o <filename>\n";
            return ERR_INVALID_OPTION;
        }
        if(args[4].length() < 4 || args[4].substr(args[4].length() - 4) != ".xml") {
            std::cerr << "Invalid option\n";
            std::cerr << "Output file must be a .xml file\n";
            return ERR_INVALID_OPTION;
        }
        
    }
    
    return OK;
}

int CLICommands::minifyCommand(const std::vector<std::string> &args)
{
    if(args.size() < 2 || args[0] != "-i" || args[2]!= "-o") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: mini -i <filename> -o <filename>\n";
        return ERR_INVALID_OPTION;
    }
    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }
    if(args[3].length() < 4 || args[3].substr(args[3].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Output file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    
    if(content == "") return ERR_FILE_NOT_FOUND;

    std::string miniContent = minifyXML(content);

    writeToFile(args[3],miniContent);
    return OK;
}

int CLICommands::prettifyCommand(const std::vector<std::string> &args)
{
    if(args.size() < 2 || args[0] != "-i" || args[2]!= "-o") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: format -i <filename> -o <filename>\n";
        return ERR_INVALID_OPTION;
    }
    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }
    if(args[3].length() < 4 || args[3].substr(args[3].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Output file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }
    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;

    std::vector<Token> tokens = tokenizeXML(content);

    Tree<std::string>* tree = buildTree(tokens);
    tree->print_prettified();
    writeToFile(args[3],tree->getPrettifyingString());

    return OK;
}

int CLICommands::xmlToJsonCommand(const std::vector<std::string> &args)
{
    if(args.size() < 2 || args[0] != "-i" || args[2]!= "-o") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: json -i <filename> -o <filename>\n";
        return ERR_INVALID_OPTION;
    }
    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    } 
    if(args[3].length() < 5 || args[3].substr(args[3].length() - 5) != ".json") {
        std::cerr << "Invalid option\n";
        std::cerr << "Output file must be a .json file\n";
        return ERR_INVALID_OPTION;
    }
    std::string content = readFileToString(args[1]);
    Tree<std::string>* root = buildTree(tokenizeXML(content));
    writeToFile(args[3], JsonConverter::convert(root->getRoot()));

    return OK;
}

int CLICommands::compressCommand(const std::vector<std::string> &args)
{
    if(args.size() < 4 || args[0] != "-i" || args[2]!= "-o") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: compress -i <filename> -o <filename>\n";
        return ERR_INVALID_OPTION;
    }
    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml" && args[1].substr(args[1].length() - 5) != ".json") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml or .json file\n";
        return ERR_INVALID_OPTION;
    } 
    if(args[3].length() < 5 || args[3].substr(args[3].length() - 5) != ".comp") {
        std::cerr << "Invalid option\n";
        std::cerr << "Output file must be a .comp file\n";
        return ERR_INVALID_OPTION;
    }

    return BPE_compress(args[1], args[3]);
}

int CLICommands::decompressCommand(const std::vector<std::string> &args)
{
    if(args.size() < 4 || args[0] != "-i" || args[2]!= "-o") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: decompress -i <filename> -o <filename>\n";
        return ERR_INVALID_OPTION;
    }
    if(args[1].length() < 5 || args[1].substr(args[1].length() - 5) != ".comp") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .comp file\n";
        return ERR_INVALID_OPTION;
    }
    if(args[3].length() < 4 || args[3].substr(args[3].length() - 4) != ".xml" && args[3].substr(args[3].length() - 5) != ".json") {
        std::cerr << "Invalid option\n";
        std::cerr << "Output file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }
    return BPE_decompress(args[1], args[3]);
}

int CLICommands::mutualCommand(const std::vector<std::string> &args)
{
    if(args.size() != 4 || args[0] != "-i"  || args[2]!= "-ids") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: mutual -i <filename> -ids <ids>\n";
        return ERR_INVALID_OPTION;
    }

    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }

    // xml_editor mutual -i input_file.xml -ids 1,2,3
    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;

    Graph graph(content);

    std::string ids = args[3];
    std::vector<int> idsVector;

    std::stringstream ss(ids);
    std::string token;

    while (std::getline(ss, token, ',')) {
        try {
            int num = std::stoi(token);
            idsVector.push_back(num);
        } catch (...) {
            std::cerr << "Invalid option\n";
            std::cerr << "Ids must be integers\n";
            return ERR_INVALID_OPTION;
        }
    }

    if (idsVector.size() < 2) {
    std::cerr << "Error: At least 2 IDs are required to compute mutual followers\n";
    std::cerr << "Usage: mutual -i <filename> -ids <id1,id2,...>\n";
    return ERR_INVALID_OPTION;
    }

    std::vector<int> mutual = computeMutualFollowers(graph, idsVector);
    printVector(mutual);

    return OK;
}

int CLICommands::visualizerCommand(const std::vector<std::string> &args)
{
    if(args.size() != 4 || args[0] != "-i" || args[2] != "-o") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: draw -i <filename.xml> -o <filename.png>\n";
        return ERR_INVALID_OPTION;
    }

    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }
    if(args[3].length() < 4 || args[3].substr(args[3].length() - 4) != ".png") {
        std::cerr << "Invalid option\n";
        std::cerr << "Output file must be a .png file\n";
        return ERR_INVALID_OPTION;
    }
    
    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;
    
    Graph graph(content);
    Visualizer(graph,args[3]);
    return OK;
}

int CLICommands::searchCommand(const std::vector<std::string> &args){


if( args.size() != 4 || args[2] != "-i" ) {
        std::cerr<<"Invalid option\n";
        
        std::cerr << "Usage:  -w word -i input_file.xml\n";
        std::cerr << "or\n";
         std::cerr << "Usage:  -t topic -i input_file.xml\n";

        return ERR_INVALID_OPTION;
    }

    if(args[3].length() < 4 || args[3].substr(args[3].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }
    std::string filename = args[3];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;
    

if(args[0]=="-w")
{

        const std::string wordToSearch = args[1];  

    std::vector<PostMatch> results = searchPostsByWord(content, wordToSearch);
        printMatches(results);
           


        return OK;
        


}
else if(args[0]=="-t")
{
                

        const std::string topicToSearch = args[1];
         std::vector<PostMatch> results = searchPostsByTopic(content, topicToSearch);
        printMatchesTopic(results);
                return OK;




}
else
{

  std::cerr << "Usage start with :  -w or -t\n";
  return ERR_INVALID_OPTION;

}
}

// xml_editor most_active -i input_file.xml
// xml_editor most_influencer -i input_file.xm
int CLICommands::mostActiveCommand(const std::vector<std::string> &args)
{
    if(args.size() != 2 || args[0] != "-i") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: most_active -i <filename.xml>\n";
        return ERR_INVALID_OPTION;
    }

    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;
    
    Graph graph(content);
    int mostActivePersonId = graph.getMostActivePersonId();
    std::string mostActivePersonName = graph.getName(mostActivePersonId);
    std::cout << mostActivePersonId << ": " << mostActivePersonName << std::endl;
    return OK;
}

int CLICommands::mostInfluencerCommand(const std::vector<std::string> &args)
{
    if(args.size() != 2 || args[0] != "-i") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: most_influencer -i <filename.xml>\n";
        return ERR_INVALID_OPTION;
    }

    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;
    
    Graph graph(content);
    int mostInfluencerPersonId = graph.getMostInfluencerId();
    std::string mostInfluencerPersonName = graph.getName(mostInfluencerPersonId);
    std::cout << mostInfluencerPersonId << ": " << mostInfluencerPersonName << std::endl;
    return OK;
}

// xml_editor suggest -i input_file.xml -id 1
int CLICommands::suggestCommand(const std::vector<std::string> &args)
{
    if(args.size() != 4 || args[0] != "-i" || args[2] != "-id") {
        std::cerr<<"Invalid option\n";
        std::cerr << "Usage: suggest -i <filename.xml>\n";
        return ERR_INVALID_OPTION;
    }

    if(args[1].length() < 4 || args[1].substr(args[1].length() - 4) != ".xml") {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a .xml file\n";
        return ERR_INVALID_OPTION;
    }

    if(args[3].length() > 1 || !std::isdigit(args[3][0])) {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a (one)number\n";
        return ERR_INVALID_OPTION;
    }

    int id = std::stoi(args[3]);
    if(id < 0) {
        std::cerr << "Invalid option\n";
        std::cerr << "Input file must be a positive number\n";
        return ERR_INVALID_OPTION;
    }

    

    std::string filename = args[1];
    std::string content = readFileToString(filename);
    if(content == "") return ERR_FILE_NOT_FOUND;
    
    Graph graph(content);
    std::vector<int> suggests = suggest(graph, id);
    printSuggestions(suggests);

   
    return OK;
}