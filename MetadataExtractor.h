#ifndef METADATA_EXTRACTOR_H
#define METADATA_EXTRACTOR_H

#include <memory>
#include <string>
#include <vector>

namespace boost
{
namespace filesystem
{
    class path;
}
}

namespace google
{
namespace protobuf
{
    namespace io
    {
        class CodedInputStream;
    }
}
}

class MetadataExtractor
{
public:
    class Metadata
    {
    public:
        explicit Metadata(std::string id) : _id(std::move(id)) { }

        virtual ~Metadata() = default;

        std::string const& GetId() const { return _id; }

        virtual std::shared_ptr<google::protobuf::io::CodedInputStream> CreateCodedInputStream() const = 0;

    protected:
        std::string const _id;
    };

    virtual ~MetadataExtractor() = default;

    virtual void Parse(boost::filesystem::path const& path) = 0;

    std::vector<std::unique_ptr<Metadata>> const& GetMetadata() const { return _metadatas; }

    Metadata const* GetById(std::string const& id) const;

protected:
    std::vector<std::unique_ptr<Metadata>> _metadatas;
};

#endif // METADATA_EXTRACTOR_H
