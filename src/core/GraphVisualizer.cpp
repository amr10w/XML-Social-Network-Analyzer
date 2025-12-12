#include "GraphVisualizer.h"
#include <fstream>
#include <cstdlib>
#include <string>
#include <iostream>

bool Visualizer(const Graph &graph, const std::string &file_name)
{
    #ifdef ENABLE_GRAPHVIZ
        // 1. Calculate Stats for the Header
        int num_users = 0;
        int num_connections = 0;
        
        const auto& adj_list = graph.getGraph();
        num_users = adj_list.size();
        
        for(const auto& entry : adj_list) {
            num_connections += entry.second.size();
        }

        std::ofstream file("network.dot");
        file << "digraph Network {\n";

        // ======== GRAPH THEME & LAYOUT ========
        file << "    // General graph styling\n";
        file << "    graph [\n";
        file << "        layout=\"circo\",\n";            // <--- KEY: Forces Circular Layout
        file << "        bgcolor=\"#1e1e1e\",\n";
        file << "        fontcolor=\"white\",\n";
        file << "        fontname=\"Arial\",\n";          // Looks cleaner
        file << "        fontsize=16,\n";                 // Title size
        file << "        labelloc=\"t\",\n";              // Place label at the Top
        file << "        label=\"Social Network Graph\\nUsers: " 
             << num_users << " | Connections: " << num_connections << "\",\n"; 
        file << "        splines=true,\n";
        file << "        overlap=false\n";
        file << "    ];\n";

        file << "    node [\n";
        file << "        shape=circle,\n";
        file << "        style=filled,\n";
        file << "        fillcolor=\"#4e89ff\",\n";
        file << "        fontcolor=\"white\",\n";
        file << "        fontname=\"Arial\",\n";
        file << "        fontsize=12,\n";
        file << "        penwidth=2,\n";
        file << "        fixedsize=true,\n";              // Makes circles uniform
        file << "        width=0.6\n";
        file << "    ];\n";

        file << "    edge [\n";
        file << "        color=\"#dddddd\",\n";
        file << "        arrowsize=0.7,\n";
        file << "        penwidth=1.5\n";
        file << "    ];\n";
        // ===============================
        
        for(const auto& entry : adj_list)
        {
            for(int j=0; j<(int)entry.second.size(); j++){
                // Writing the edge
                file << "\t" << entry.second[j] << " -> " << entry.first << ";\n";
            }
        }
        file << "}\n";
        file.close();

        // 2. Command Update: Use -Kcirco to ensure the circular engine is used
        std::string command = "dot -Kcirco -Tpng network.dot -o " + file_name;
        
        int result = std::system(command.c_str());
        
        if (result == 0) {
            std::cout << "Graph generated successfully at " << file_name << std::endl;
            return true;
        } else {
            std::cerr << "Error: Graphviz failed to run. Code: " << result << std::endl;
            return false;
        }
    #else
        std::cout << "Graphviz not installed. Skipping draw." << std::endl;
        return false;
    #endif
}