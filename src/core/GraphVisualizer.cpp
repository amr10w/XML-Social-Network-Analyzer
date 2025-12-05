#include "GraphVisualizer.h"
#include "GraphvizWrapper.h"
#include <fstream>
#include <cstdlib> 

bool Visualizer(const Graph &graph , const std::string &file_name)
{
    #ifdef ENABLE_GRAPHVIZ
        std::ofstream file("network.dot");
        file << "digraph Network {\n";

        // ======== GRAPH THEME ========
        file << "    // General graph styling\n";
        file << "    graph [\n";
        file << "        bgcolor=\"#1e1e1e\",\n";          // Dark background
        file << "        fontcolor=\"white\",\n";
        file << "        fontsize=12,\n";
        file << "        splines=true,\n";
        file << "        overlap=false\n";
        file << "    ];\n";

        file << "    node [\n";
        file << "        shape=circle,\n";
        file << "        style=filled,\n";
        file << "        fillcolor=\"#4e89ff\",\n";       // Blue nodes
        file << "        fontcolor=\"white\",\n";
        file << "        fontsize=12,\n";
        file << "        penwidth=2\n";
        file << "    ];\n";

        file << "    edge [\n";
        file << "        color=\"#dddddd\",\n";           // Light gray edges
        file << "        arrowsize=0.7,\n";
        file << "        penwidth=1.5\n";
        file << "    ];\n";
        // ===============================
        
        for(int i=1;i<(int)graph.getGraph().size();i++)
        {
            for(int j=0;j<(int)graph.getGraph()[i].size();j++){
                file<<"\t"<< i << " -> "<< graph.getGraph()[i][j] <<";\n";
            }
        }
        file << "}\n";
        file.close();

        std::string command = "dot -Tpng network.dot -o " + file_name;
        int result = std::system(command.c_str());
        if (result == 0) {
            std::cout << "Graph generated successfully at " << file_name << std::endl;
            return true;
        } else {
            std::cerr << "Error: 'dot.exe' failed to run. Code: " << result << std::endl;
            return false;
        }
    #else
        std::cout << "Graphviz not installed. Skipping draw." << std::endl;
        return false;
    #endif
}