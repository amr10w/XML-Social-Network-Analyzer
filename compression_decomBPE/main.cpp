#include "BPE.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <chrono>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        cerr << "Usage: BPE <compress|decompress> -i <input> -o <output>" << endl;
        return 1;
    }

    // 2. Convert to string for safe comparison
    string operation = argv[1];
    string flagInput = argv[2];
    string inputPath = argv[3];
    string flagOutput = argv[4];
    string outputPath = argv[5];
    if (flagInput == "-i" && flagOutput == "-o")
    {
        if (operation == "compress")
        {
            ifstream input(inputPath.c_str());
            if (!input)
            {
                cerr << "Error opening \"" << inputPath << "\"" << endl;
                return 1;
            }
            ofstream output(outputPath.c_str());
            if (!output)
            {
                cerr << "Output file path \"" << outputPath << "\" is wrong" << endl;
                return 1;
            }
            string xml((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());



            BPE compressor;

            auto start = chrono::high_resolution_clock::now();

            cout << "\n[+] Started compressing \"" << inputPath << "\"\n";

            auto compressed = compressor.compress(xml);

            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

            int original_size = xml.size();
            int compression_size = compressed.encodedText.size() + compressed.dictionary.size() * 2 + sizeof(int);
            double ratio = (1 - (double)compression_size / original_size) * 100;
            cout << "\n[+] Compression complete!\n";
            cout << "\n[+] Compression ratio: " << ratio << "%\n";
            cout << "\n[+] Compression time: " << duration.count() << " ms\n";
            cout << "\n[+] Compression size(encoding): " << compression_size << " bytes\n";
            compressor.saveToFile(outputPath, compressed);
            cout << "\n[+] Saved output to \"" << outputPath << "\"" << endl;
            return 0;
        }
        else if (operation == "decompress")
        {
            ifstream input(inputPath.c_str());
            if (!input)
            {
                cerr << "Couldn't open \"" << inputPath << "\"" << endl;
                return 1;
            }
            ofstream output(outputPath.c_str());
            if (!output)
            {
                cerr << "Couldn't output to \"" << outputPath << "\"" << endl;
                return 1;
            }

            BPE decompressor;

            auto loaded = decompressor.loadFromFile(inputPath);
            auto start = chrono::high_resolution_clock::now();
            cout << "\n[+] Started decompressing \"" << inputPath << "\"\n";
            string decompressed = decompressor.decompress(loaded);
            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

            output << decompressed;
            cout << "\n[+] Decompression complete!\n";
            cout << "\n[+] Decompression time: " << duration.count() << " ms\n";
            cout << "\n[+] Saved output to \"" << outputPath << "\"\n";
            input.close();
            output.close();
            return 0;
        }
        else
        {
            cerr << "Error" << endl;
            return 1;
        }
    }
    else
    {
        cerr << "Please, enter a right command" << endl;
        return 1;
    }
    return 0;
}