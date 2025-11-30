#ifndef BPE_H
#define BPE_H
#include "SimpleMap.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

struct CompressedData{
    std::vector<std::string> dictionary;
    std::string encodedText;

};


class BPE {
public:
    CompressedData compress(const std::string& xmlText);
    std::string decompress(const CompressedData& compressedData);

    void write_to_file(const std::string& path, const CompressedData& data);
    CompressedData load_from_file(const std::string& path);


private:
    SimpleMap<int, int> buildFrequencyTable(const std::string &text);
    int findMostFrequentPair(SimpleMap<int,int>& freqTable);
    std::string applyPair(const std::string &text, const int &pair, unsigned char newSymbol);

};
#endif // BPE_H