#ifndef BPECOMPRESSOR_H
#define BPECOMPRESSOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
using namespace std;
struct CompressedData{
    vector<pair<string,string>> dictionary;
    string encodedText;

};


class BPECompressor {
public:
    CompressedData compress(const string& xmlText);
    string decompress(const CompressedData& compressedData);

    bool saveToFile(const string& path, const CompressedData& data);
    CompressedData loadFromFile(const string& path);


private:
    unordered_map<string,int> buildFrequencyTable(const string& text);
    pair<string,string> findMostFrequentPair(const unordered_map<string,int>& freqTable);
    string applyPair(const string &text, const string &A, const string &B, char newSymbol);

};
#endif // BPECOMPRESSOR_H