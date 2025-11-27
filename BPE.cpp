#include "BPE.h"
#include "SimpleMap.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

// Build a frequency table of adjacent character pairs
SimpleMap<int, int> BPE::buildFrequencyTable(const string &text)
{
    SimpleMap<int, int> freqTable;
    if (text.size() < 2)
        return freqTable; // Safety check

    // Iterate up to size - 1
    for (size_t i = 0; i < text.size() - 1; ++i)
    {
        unsigned char left = (unsigned char)text[i];
        unsigned char right = (unsigned char)text[i + 1];

        int pair = (left << 8) | right;
        ++freqTable[pair];
    }
    return freqTable;
}

// Find the most frequent adjacent pair
int BPE::findMostFrequentPair(SimpleMap<int, int> &freqTable)
{
    int bestPair;
    int maxCount = 0;
    auto keys = freqTable.getKeys();
    for (auto &key : keys)
    {
        int count = freqTable[key];
        if (count > maxCount)
        {
            maxCount = count;
            bestPair = key;
        }
    }
    if (!bestPair || maxCount < 2)
        return 0;
    return bestPair;
}

// Apply pair replacement using a single character symbol
string BPE::applyPair(const string &text, const int &pair, unsigned char newSymbol)
{
    string out;
    out.reserve(text.size()); // Optimization

    unsigned char first = (pair >> 8) & 0xFF;
    unsigned char second = pair & 0xFF;

    for (size_t i = 0; i < text.size(); ++i)
    {
        // Check if we have a pair match
        if (i < text.size() - 1 &&
            (unsigned char)text[i] == first &&
            (unsigned char)text[i + 1] == second)
        {

            out += (char)newSymbol;
            i++; // Skip the next character (we just consumed it)
        }
        else
        {
            out += text[i];
        }
    }
    return out;
}

// Compress XML input safely
CompressedData BPE::compress(const string &xmlText)
{
    CompressedData result;
    string text = xmlText;

    // USE INT HERE!
    int nextToken = 128;

    int fileSize = xmlText.size();
    // Prevent excessively long loops on huge files
    int MAX_ITER = std::min(255 - 128, fileSize / 10);

    int iter = 0;
    while (iter < MAX_ITER && nextToken <= 255)
    {
        auto freq = buildFrequencyTable(text);
        int best = findMostFrequentPair(freq);

        if (best == 0)
            break; // No pairs found

        int pairFreq = freq[best];
        // Only compress if it saves space (Frequency > 1)
        if (pairFreq < 2)
            break;

        // Unpack for storage
        unsigned char first = (best >> 8) & 0xFF;
        unsigned char second = best & 0xFF;
        string bestPair = {(char)first, (char)second};

        // Store in order.
        // Token 128 replaces "AB"
        // Token 129 replaces "C" + Token 128
        result.dictionary.push_back(bestPair);

        // Apply
        text = applyPair(text, best, (unsigned char)nextToken);

        if (100 * iter / MAX_ITER % 10 == 0)
        {
            cout << "\n[+] Completed: " << 100 * iter / MAX_ITER << "%\n";
        }

        nextToken++;
        iter++;
    }

    result.encodedText = text;
    cout << "\n[+] Final encoded text length (bytes): " << result.encodedText.size() << "\n";
    return result;
}

string BPE::decompress(const CompressedData &compressedData)
{
    string currentText = compressedData.encodedText;

    // Iterate backwards through the dictionary (LIFO order)
    size_t tokens = compressedData.dictionary.size();
    for (int i = tokens - 1; i >= 0; --i)
    {

        string pair = compressedData.dictionary[i];
        unsigned char targetSymbol = (unsigned char)(128 + i); // The token to find

        // 1. Create a buffer for the NEXT version of the text
        string nextText;
        // Optimization: Reserve slightly more memory to prevent re-allocations
        nextText.reserve(currentText.size() * 1.1);

        // 2. Scan the text ONCE
        for (char c : currentText)
        {
            if ((unsigned char)c == targetSymbol)
            {
                // FOUND IT! Append the pair instead of the token
                nextText += pair;
            }
            else
            {
                // No match, just copy the character
                nextText += c;
            }
        }

        // 3. Update currentText for the next loop iteration
        currentText = std::move(nextText);
        
        if (100 * (tokens - i) / tokens % 10 == 0) {
            cout << "\n[+] Completed: " << 100 * (tokens - i) / tokens << "%\n";
        }
    }

    return currentText;
}

bool BPE::saveToFile(const string &path, const CompressedData &data)
{
    ofstream file(path, ios::binary);
    if (!file)
        return false;

    // Write Header
    size_t count = data.dictionary.size();
    file.write((char *)&count, sizeof(size_t));

    // Write Dictionary (Fixed 2 bytes per entry)
    for (const auto &pair : data.dictionary)
    {
        file.write(pair.data(), 2); // Safe for binary
    }

    // Write Separator (Optional, helps debugging)
    char sep = 0x00;
    file.write(&sep, 1);

    // Write Body
    file.write(data.encodedText.data(), data.encodedText.size());
    return true;
}

CompressedData BPE::loadFromFile(const string &path)
{
    ifstream file(path, ios::binary);
    CompressedData out;
    if (!file)
        return out;

    // Read Count
    size_t count = 0;
    file.read((char *)&count, sizeof(size_t));

    // Read Dictionary
    for (size_t i = 0; i < count; ++i)
    {
        char buffer[2];
        file.read(buffer, 2);
        out.dictionary.push_back(string(buffer, 2));
    }

    // Read Separator
    char sep;
    file.read(&sep, 1);

    // Read Rest of file
    stringstream ss;
    ss << file.rdbuf();
    out.encodedText = ss.str();

    return out;
}
