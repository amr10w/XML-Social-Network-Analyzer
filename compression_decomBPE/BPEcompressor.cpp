#include "BPECompressor.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

// Build frequency table of adjacent character pairs
unordered_map<string,int> BPECompressor::buildFrequencyTable(const string& text) {
    unordered_map<string,int> freqTable;
    for (size_t i = 0; i + 1 < text.size(); ++i) {
        string pair = text.substr(i, 2);
        freqTable[pair]++;
    }
    return freqTable;
}

// Find the most frequent adjacent pair
pair<string,string> BPECompressor::findMostFrequentPair(const unordered_map<string,int>& freqTable) {
    string bestPair;
    int maxCount = 0;
    for (const auto& entry : freqTable) {
        if (entry.second > maxCount) {
            maxCount = entry.second;
            bestPair = entry.first;
        }
    }
    if (bestPair.empty() || maxCount < 2) return {"",""}; // only consider frequency >= 2
    return {bestPair.substr(0,1), bestPair.substr(1,1)};
}

// Apply pair replacement using a single character symbol
string BPECompressor::applyPair(const string& text, const string& A, const string& B, char newSymbol) {
    vector<char> out;
    size_t i = 0;
    while (i < text.size()) {
        if (i+1 < text.size() && text[i]==A[0] && text[i+1]==B[0]) {
            out.push_back(newSymbol);
            i += 2;
        } else {
            out.push_back(text[i]);
            i++;
        }
    }
    return string(out.begin(), out.end());
}

// Compress XML input safely
CompressedData BPECompressor::compress(const string& xmlText) {
    CompressedData result;
    string text = xmlText;

    set<char> usedSymbols(text.begin(), text.end()); // track symbols already in text
    unsigned char nextSymbol = 128; // extended ASCII

    const int MAX_RULES = 200; // safety limit
    int iter = 0;

    while (iter < MAX_RULES) {
        auto freq = buildFrequencyTable(text);
        auto best = findMostFrequentPair(freq);
        if (best.first.empty()) break; // no more useful pair

        // Ensure newSymbol is not in the text
        while (usedSymbols.count(nextSymbol)) nextSymbol++;
        if (nextSymbol > 255) break; // ran out of replacement symbols

        // Check if replacement actually reduces size
        int pairFreq = freq[best.first + best.second];
        int savings = pairFreq * (2 - 1); // pair length 2, symbol length 1
        if (savings <= 0) break;

        char newSymbol = nextSymbol++;
        usedSymbols.insert(newSymbol);

        result.dictionary.push_back({best.first + best.second, string(1,newSymbol)});
        text = applyPair(text, best.first, best.second, newSymbol);

        iter++;
    }

    result.encodedText = text;
    return result;
}

// Decompress
string BPECompressor::decompress(const CompressedData& compressedData) {
    string text = compressedData.encodedText;
    for (int i = compressedData.dictionary.size()-1; i >=0; i--) {
        const auto& rule = compressedData.dictionary[i];
        string pair = rule.first;
        string symbol = rule.second;
        size_t pos = 0;
        while ((pos = text.find(symbol, pos)) != string::npos) {
            text.replace(pos, symbol.size(), pair);
            pos += pair.size();
        }
    }
    return text;
}

// Save compressed data to file
bool BPECompressor::saveToFile(const string& path, const CompressedData& data) {
    ofstream file(path, ios::binary);
    if (!file) return false;

    file << "COMP1\n";
    file << data.dictionary.size() << "\n";
    for (auto& rule : data.dictionary)
        file << rule.first << "|" << rule.second << "\n";
    file << "-----------\n";
    file << data.encodedText;
    return true;
}

// Load compressed data from file
CompressedData BPECompressor::loadFromFile(const string& path) {
    ifstream file(path, ios::binary);
    CompressedData out;
    if (!file) return out;

    string header;
    getline(file, header);

    int rulesCount;
    file >> rulesCount;
    file.ignore();

    for (int i=0;i<rulesCount;i++){
        string line;
        getline(file,line);
        size_t sep = line.find('|');
        out.dictionary.push_back({line.substr(0,sep), line.substr(sep+1)});
    }

    string delimiter;
    getline(file,delimiter);

    stringstream ss;
    ss << file.rdbuf();
    out.encodedText = ss.str();

    return out;
}
