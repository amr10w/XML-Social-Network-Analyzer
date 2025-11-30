#include <XML_Compress.h>

void BPE_compress(const std::string &inputPath, const std::string &outputPath)
{
  std::string xml = readFileToString(inputPath);

  BPE compressor;

  auto start = std::chrono::high_resolution_clock::now();

  std::cout << "\n[+] Started compressing \"" << inputPath << "\"\n";

  auto compressed = compressor.compress(xml);

  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  int original_size = xml.size();
  int compression_size = compressed.encodedText.size() + compressed.dictionary.size() * 2 + sizeof(int);
  double ratio = (1 - (double)compression_size / original_size) * 100;
  std::cout << "\n[+] Compression complete!\n";
  std::cout << "\n[+] Compression ratio: " << ratio << "%\n";
  std::cout << "\n[+] Compression time: " << duration.count() << " ms\n";
  std::cout << "\n[+] Compression size(encoding): " << compression_size << " bytes\n";
  compressor.write_to_file(outputPath, compressed);
  std::cout << "\n[+] Saved output to \"" << outputPath << "\"" << std::endl;
}