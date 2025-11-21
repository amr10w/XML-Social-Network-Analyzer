#include "BPECompressor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{

    ifstream input("C:\\Users\\mocam\\OneDrive\\Desktop\\Senior 1 Fall 2025\\Data structures and algorthims\\Project\\sample2.xml");
    if(!input)
    {
        cerr << "Error opening input.xml" << endl;
        return 1;
    }

    string xml((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());

    BPECompressor compressor;
    //compress the XML
    auto compressed = compressor.compress(xml);
    cout << "[+] Compression complete\n";
    cout << "    Dictionary entries: "
              << compressed.dictionary.size() << "\n";
    cout << "    Encoded size: "
              << compressed.encodedText.size() << " bytes\n";

    //save to file
    if (compressor.saveToFile("output_file.comp", compressed))
        cout << "[+] Saved compressed file: output_file.comp\n";
    else
        cout << "[-] Error saving output_file.comp!\n";


    // load back
    auto loaded = compressor.loadFromFile("output_file.comp");
    cout << "[+] Loaded compressed file: output_file.comp\n";

    //decompress
    string decompressed = compressor.decompress(loaded);
    cout << "[+] Decompression complete\n";


    //verify

    if(decompressed == xml)
    {
        cout << "[+] Decompressed data matches original XML\n";
    }
    else
    {
        cout << "[-] Decompressed data does NOT match original XML\n";
        return 1;
    }


    // save decompressed XML
    ofstream output("decompressed.xml");
    output << decompressed;
    output.close();


    cout << "[+] Saved decompressed XML to: decompressed.xml\n";
    return 0;


}