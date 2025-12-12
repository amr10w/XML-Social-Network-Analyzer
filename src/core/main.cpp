// --- main.cpp (Simple Decompression Test) ---

#include "BPE.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// ==========================================================
// PLACEHOLDER UTILITIES (REQUIRED FOR BPE::write_to_file/load_from_file)
// NOTE: Ensure your actual FileUtils or global utilities are linked
// ==========================================================

// Simple utility to read file contents into a string (used implicitly by your framework)
std::string readFileToString(const std::string& path) {
    std::ifstream t(path);
    if (!t.is_open()) return "";
    return std::string((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());
}

// Simple utility to write a string to a file (used implicitly by your framework)
void writeToFile(const std::string& path, const std::string& content) {
    std::ofstream outfile(path);
    if (!outfile.is_open()) return;
    outfile << content;
}

// ==========================================================
// MAIN TEST FUNCTION
// ==========================================================

int main() {
    // 1. Setup: Define the test data and file names
    const std::string originalXml = R"(
<data><item>value1</item><item>value2</item><item>value3</item></data>
)";
    const std::string compressedFilePath = "temp_test.comp";
    const std::string decompressedFilePath = "decompressed_result.xml";

    BPE processor;

    std::cout << "--- Decompression Isolation Test ---" << std::endl;
    std::cout << "Original Size: " << originalXml.size() << " bytes" << std::endl;

    // --- STEP 1: PREP (Compression & Saving to File) ---
    // We MUST compress first to create the valid binary file structure (.comp)
    std::cout << "[1/4] Performing compression to create test file..." << std::endl;
    CompressedData compressedData = processor.compress(originalXml);
    processor.write_to_file(compressedFilePath, compressedData);
    
    // --- STEP 2: LOAD (Testing BPE::load_from_file) ---
    std::cout << "[2/4] Loading compressed data from file: " << compressedFilePath << std::endl;
    auto start_load = std::chrono::high_resolution_clock::now();
    CompressedData loadedData = processor.load_from_file(compressedFilePath);
    auto end_load = std::chrono::high_resolution_clock::now();
    std::cout << "      Load Time: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(end_load - start_load).count() 
              << " us" << std::endl;

    // --- STEP 3: DECOMPRESS (Testing BPE::decompress) ---
    std::cout << "[3/4] Starting decompression..." << std::endl;
    auto start_decomp = std::chrono::high_resolution_clock::now();
    std::string decompressedXml = processor.decompress(loadedData);
    auto end_decomp = std::chrono::high_resolution_clock::now();
    auto duration_decomp = std::chrono::duration_cast<std::chrono::milliseconds>(end_decomp - start_decomp);
    
    std::cout << "      Decompression Time: " << duration_decomp.count() << " ms" << std::endl;
    writeToFile(decompressedFilePath, decompressedXml);

    // --- STEP 4: VERIFICATION ---
    std::cout << "[4/4] Verifying result..." << std::endl;
    bool success = (originalXml == decompressedXml);

    if (success) {
        std::cout << "✅ SUCCESS: Decompressed XML matches original." << std::endl;
        // Clean up the temporary file
        std::remove(compressedFilePath.c_str());
        std::remove(decompressedFilePath.c_str());
    } else {
        std::cout << "❌ FAILURE: Decompressed XML DOES NOT match original." << std::endl;
        std::cout << "Original: |" << originalXml << "|" << std::endl;
        std::cout << "Decompressed: |" << decompressedXml << "|" << std::endl;
    }

    return success ? 0 : 1;
}