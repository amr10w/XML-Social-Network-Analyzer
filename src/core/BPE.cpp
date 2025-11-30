#include "BPE.h"


SimpleMap<int, int> BPE::buildFrequencyTable(const std::string &text)
{
    SimpleMap<int, int> freqTable;
    if (text.size() < 2)
        return freqTable;

    for (size_t i = 0; i < text.size() - 1; ++i)
    {
        unsigned char left = (unsigned char)text[i];
        unsigned char right = (unsigned char)text[i + 1];

        int pair = (left << 8) | right;
        ++freqTable[pair];
    }
    return freqTable;
}

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

std::string BPE::applyPair(const std::string &text, const int &pair, unsigned char newSymbol)
{
    std::string out;
    out.reserve(text.size());

    unsigned char first = (pair >> 8) & 0xFF;
    unsigned char second = pair & 0xFF;

    for (size_t i = 0; i < text.size(); ++i)
    {
        if (i < text.size() - 1 &&
            (unsigned char)text[i] == first &&
            (unsigned char)text[i + 1] == second)
        {

            out += (char)newSymbol;
            i++;
        }
        else
        {
            out += text[i];
        }
    }
    return out;
}

CompressedData BPE::compress(const std::string &xmlText)
{
    CompressedData result;
    std::string text = xmlText;

    int nextToken = 128;

    int fileSize = xmlText.size();
    int MAX_ITER = std::min(255 - 128, fileSize / 10);

    int iter = 0;
    while (iter < MAX_ITER && nextToken <= 255)
    {
        auto freq = buildFrequencyTable(text);
        int best = findMostFrequentPair(freq);

        if (best == 0)
            break;

        int pairFreq = freq[best];
        if (pairFreq < 2)
            break;

        unsigned char first = (best >> 8) & 0xFF;
        unsigned char second = best & 0xFF;
        std::string bestPair = {(char)first, (char)second};

        result.dictionary.push_back(bestPair);

        text = applyPair(text, best, (unsigned char)nextToken);

        if (100 * iter / MAX_ITER % 10 == 0)
        {
            std::cout << "\n[+] Completed: " << 100 * iter / MAX_ITER << "%\n";
        }

        nextToken++;
        iter++;
    }

    result.encodedText = text;
    std::cout << "\n[+] Final encoded text length (bytes): " << result.encodedText.size() << "\n";
    return result;
}

std::string BPE::decompress(const CompressedData &compressedData)
{
    std::string currentText = compressedData.encodedText;

    size_t tokens = compressedData.dictionary.size();
    for (int i = tokens - 1; i >= 0; --i)
    {

        std::string pair = compressedData.dictionary[i];
        unsigned char targetSymbol = (unsigned char)(128 + i);

        std::string nextText;
        nextText.reserve(currentText.size() * 1.1);

        for (char c : currentText)
        {
            if ((unsigned char)c == targetSymbol)
            {
                nextText += pair;
            }
            else
            {
                nextText += c;
            }
        }

        currentText = std::move(nextText);
        
        if (100 * (tokens - i) / tokens % 10 == 0) {
            std::cout << "\n[+] Completed: " << 100 * (tokens - i) / tokens << "%\n";
        }
    }

    return currentText;
}

std::string BPE::to_string(const CompressedData &data)
{
    std::stringstream output;

    size_t count = data.dictionary.size();
    output.write((char *)&count, sizeof(size_t));

    for (const auto &pair : data.dictionary)
    {
        output.write(pair.data(), 2);
    }

    char sep = 0x00;
    output.write(&sep, 1);

    output.write(data.encodedText.data(), data.encodedText.size());
    return output.str();
}

CompressedData BPE::load_from_file(const std::string &path)
{
    CompressedData out;

    std::ifstream input(path, std::ios::binary);

    size_t count = 0;
    input.read((char *)&count, sizeof(size_t));

    for (size_t i = 0; i < count; ++i)
    {
        char buffer[2];
        input.read(buffer, 2);
        out.dictionary.push_back(std::string(buffer, 2));
    }

    char sep;
    input.read(&sep, 1);

    std::stringstream ss;
    ss << input.rdbuf();
    out.encodedText = ss.str();

    return out;
}
