#include "FilesystemMetadataExtractor.h"
#include <boost/filesystem/operations.hpp>
#include <boost/optional.hpp>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>

namespace
{
class FilesystemMetadata : public MetadataExtractor::Metadata
{
public:
    explicit FilesystemMetadata(std::string id) : Metadata(std::move(id))
    {
    }

    std::shared_ptr<google::protobuf::io::CodedInputStream> CreateCodedInputStream() const override
    {
        struct Data
        {
            std::ifstream in;
            boost::optional<google::protobuf::io::IstreamInputStream> zeroCopyInput;
            boost::optional<google::protobuf::io::CodedInputStream> decoder;
        };

        std::shared_ptr<Data> data = std::make_shared<Data>();

        data->in.open(_id.c_str(), std::ios::binary | std::ios::in);
        if (!data->in.good())
            return nullptr;

        data->zeroCopyInput.emplace(&data->in);
        data->decoder.emplace(&data->zeroCopyInput.get());
        return { data, data->decoder.get_ptr() };
    }
};
}

void FilesystemMetadataExtractor::Parse(boost::filesystem::path const& path)
{
    boost::filesystem::directory_iterator end;
    for (boost::filesystem::directory_iterator itr(path); itr != end; ++itr)
        if (is_regular_file(itr->status()))
            if (itr->path().extension().string().find(".protoc") != std::string::npos)
                _metadatas.emplace_back(new FilesystemMetadata(itr->path().string()));
}
