// --- BPE_Bridge.h ---
#ifndef BPE_BRIDGE_H
#define BPE_BRIDGE_H

#include <string>
#include <QByteArray>

// Compression utility (used by CompressPage.cpp)
std::string compressxmlString(const std::string& xmlContent);

// Decompression utility (used by DecompressPage.cpp)
std::string bpe_decompress_in_memory(const QByteArray& compressedBytes);

#endif // BPE_BRIDGE_H