#ifndef FILESYSTEM_METADATA_EXTRACTOR_H
#define FILESYSTEM_METADATA_EXTRACTOR_H

#include "MetadataExtractor.h"

class FilesystemMetadataExtractor : public MetadataExtractor
{
public:
    void Parse(boost::filesystem::path const& path) override;
};

#endif // FILESYSTEM_METADATA_EXTRACTOR_H
