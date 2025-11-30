#include <XML_Decompress.h>

void BPE_decompress(const std::string &inputPath, const std::string &outputPath)
{
  readFileToString(inputPath);

  writeToFile(outputPath, "");

  BPE decompressor;

  auto loaded = decompressor.load_from_file(inputPath);
  auto start = std::chrono::high_resolution_clock::now();
  std::cout << "\n[+] Started decompressing \"" << inputPath << "\"\n";
  std::string decompressed = decompressor.decompress(loaded);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  writeToFile(outputPath, decompressed);
  std::cout << "\n[+] Decompression complete!\n";
  std::cout << "\n[+] Decompression time: " << duration.count() << " ms\n";
  std::cout << "\n[+] Saved output to \"" << outputPath << "\"\n";
}