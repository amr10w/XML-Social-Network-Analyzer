// --- BPE_Bridge.cpp ---
#include "BPE_Bridge.h"

// Forward declaration of internal memory loader
CompressedData load_compressed_data_from_memory(const std::string& compressedString);

// =======================================================
// Core Decompression Bridge Function
// =======================================================

std::string bpe_decompress_in_memory(const QByteArray& compressedBytes)
{
    // 1. Convert QByteArray (Qt's binary buffer) to std::string for BPE logic
    std::string compressedStdString(compressedBytes.constData(), compressedBytes.size());
    
    BPE decompressor;
    
    // 2. Load the dictionary and encoded text from the memory buffer
    CompressedData loaded = load_compressed_data_from_memory(compressedStdString);
    
    // 3. Perform decompression
    return decompressor.decompress(loaded);
}

// =======================================================
// Core Compression Bridge Function (For CompressPage)
// =======================================================

// This function assumes your original compression utility was declared as:
// extern std::string compressXMLString(const std::string& xmlContent);
// We implement it here by performing compression and then writing the result to memory.

QByteArray compressXMLString(const std::string& xmlContent)
{
    BPE compressor;
    
    // 1. Perform compression
    CompressedData result = compressor.compress(xmlContent);
    
    // 2. Write the result (dictionary + encoded text) to an output string stream
    QByteArray output;
    
    // Mimic the logic of BPE::write_to_file but write to ss_out
    size_t count = result.dictionary.size();
    output.append((char *)&count, sizeof(size_t));

    for (const auto &pair : result.dictionary)
    {
        // Must ensure the pair length is exactly 2 bytes as per BPE logic
        output.append(pair.data(), 2); 
    }

    char sep = 0x00;
    output.append(&sep, 1);

    output.append(result.encodedText.data(), result.encodedText.size());
    
    // 3. Return the complete compressed data as a single string
    return output;
}


// =======================================================
// Internal Helper: Load CompressedData from Memory Buffer
// =======================================================

CompressedData load_compressed_data_from_memory(const std::string& compressedString)
{
    CompressedData out;
    std::stringstream input(compressedString);
    
    size_t count = 0;
    
    // Read dictionary size (size_t)
    input.read((char *)&count, sizeof(size_t));

    if (input.fail()) throw std::runtime_error("Failed to read dictionary size header.");

    // Read dictionary pairs (2 bytes each)
    for (size_t i = 0; i < count; ++i)
    {
        char buffer[2];
        input.read(buffer, 2);
        if (input.fail()) throw std::runtime_error("Corrupted dictionary data.");
        out.dictionary.push_back(std::string(buffer, 2));
    }

    // Read separator (0x00)
    char sep;
    input.read(&sep, 1);
    if (input.fail() || sep != 0x00) throw std::runtime_error("Missing or invalid data separator.");

    // Read the encoded text (the rest of the stream)
    std::stringstream ss_encoded;
    ss_encoded << input.rdbuf();
    out.encodedText = ss_encoded.str();
    
    return out;
}