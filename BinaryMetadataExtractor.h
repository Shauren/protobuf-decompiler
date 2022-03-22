#ifndef BINARY_METADATA_EXTRACTOR_H
#define BINARY_METADATA_EXTRACTOR_H

#include "MetadataExtractor.h"

class BinaryMetadataExtractor : public MetadataExtractor
{
public:
    void Parse(boost::filesystem::path const& binaryPath) override;

private:
    void ReadFile(boost::filesystem::path const& binaryPath);
    void FindMetadata();

    std::string _binary;
};

#endif // BINARY_METADATA_EXTRACTOR_H
