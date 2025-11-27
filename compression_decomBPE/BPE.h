#ifndef BPE_H
#define BPE_H

#include <string>
#include <vector>

#include <utility>
#include "SimpleMap.h"
using namespace std;
struct CompressedData{
    vector<string> dictionary;
    string encodedText;

};


class BPE {
public:
    CompressedData compress(const string& xmlText);
    string decompress(const CompressedData& compressedData);

    bool saveToFile(const string& path, const CompressedData& data);
    CompressedData loadFromFile(const string& path);


private:
    SimpleMap<int, int> buildFrequencyTable(const string &text);
    int findMostFrequentPair(SimpleMap<int,int>& freqTable);
    string applyPair(const string &text, const int &pair, unsigned char newSymbol);

};
#endif // BPE_H