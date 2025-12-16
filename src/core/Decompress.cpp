#include <Decompress.h>

int BPE_decompress(const std::string &inputPath, const std::string &outputPath)
{

  std::ifstream i(inputPath);
  if (!i.is_open())
  {
    std::cerr << "\nCouldn't open \"" << inputPath << "\"\n";
    return 4;
  }
  i.close();
  std::ofstream o(outputPath);
  if (!o.is_open())
  {
    std::cerr << "\nCouldn't open \"" << outputPath << "\"\n";
    return 4;
  }
  o.close();
  
  BPE decompressor;

  std::string input = readFileToString(inputPath);
  auto loaded = decompressor.from_string(input);
  auto start = std::chrono::high_resolution_clock::now();
  std::cout << "\n[+] Started decompressing \"" << inputPath << "\"\n";
  std::string decompressed = decompressor.decompress(loaded);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  writeToFile(outputPath, decompressed);
  std::cout << "\n[+] Decompression complete!\n";
  std::cout << "\n[+] Decompression time: " << duration.count() << " ms\n";
  std::cout << "\n[+] Saved output to \"" << outputPath << "\"\n";
  return 0;
}