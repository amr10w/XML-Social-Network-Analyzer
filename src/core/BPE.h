#ifndef BPE_H
#define BPE_H
#include <string>
#include <vector>
#include "SimpleMap.h"

struct CompressedData{
    std::vector<std::string> dictionary;
    std::string encodedText;

};


class BPE {
public:
    CompressedData compress(const std::string& xmlText);
    std::string decompress(const CompressedData& compressedData);

    std::string to_string(const CompressedData& data);
    CompressedData from_string(const std::string& path);


private:
    SimpleMap<int, int> buildFrequencyTable(const std::string &text);
    int findMostFrequentPair(SimpleMap<int,int>& freqTable);
    std::string applyPair(const std::string &text, const int &pair, unsigned char newSymbol);

};
#endif // BPE_H