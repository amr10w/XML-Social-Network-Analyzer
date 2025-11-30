#ifndef XML_Decompress_H
#define XML_Decompress_H
#include "BPE.h"
#include "FileUtils.h"
#include <iostream>
#include <chrono>


void BPE_decompress(const std::string &inputPath, const std::string &outputPath);

#endif