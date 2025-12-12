#ifndef XML_Compress_H
#define XML_Compress_H
#include "BPE.h"
#include "FileUtils.h"
#include <iostream>
#include <chrono>

void BPE_compress(const std::string &inputPath, const std::string &outputPath);

#endif