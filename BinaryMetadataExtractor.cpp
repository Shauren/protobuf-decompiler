#include "BinaryMetadataExtractor.h"
#include <boost/filesystem/path.hpp>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/coded_stream.h>
#include <fstream>

namespace
{
constexpr int kMaxVarintBytes = 10;

uint64_t DecodeVarint64(char* buffer, size_t* count)
{
    uint8_t* buffer_ = reinterpret_cast<uint8_t*>(buffer);
    uint64_t result = 0;
    uint32_t b;

    do
    {
        if (*count == kMaxVarintBytes)
            break;

        b = *buffer_;
        result |= static_cast<uint64_t>(b & 0x7F) << (7 * *count);
        ++buffer_;
        ++* count;
    }
    while (b & 0x80);

    return result;
}

class BinaryMetadata : public MetadataExtractor::Metadata
{
public:
    explicit BinaryMetadata(std::string id, uint8_t const* data, int32_t length) : Metadata(std::move(id)), Data(data), Length(length)
    {
    }

    std::shared_ptr<google::protobuf::io::CodedInputStream> CreateCodedInputStream() const override
    {
        return std::make_shared<google::protobuf::io::CodedInputStream>(Data, Length);
    }

private:
    uint8_t const* Data;
    int32_t Length;
};
}

void BinaryMetadataExtractor::Parse(boost::filesystem::path const& binaryPath)
{
    ReadFile(binaryPath);
    FindMetadata();
}

void BinaryMetadataExtractor::ReadFile(boost::filesystem::path const& binaryPath)
{
    std::ifstream ifs(binaryPath.string(), std::ifstream::binary);
    if (!ifs)
        throw std::runtime_error("could not open " + binaryPath.string());

    ifs >> std::noskipws;
    ifs.seekg(0, std::ios_base::end);
    _binary.reserve(ifs.tellg());
    ifs.seekg(0, std::ios_base::beg);

    std::copy(std::istream_iterator<char>(ifs), std::istream_iterator<char>(), std::back_inserter(_binary));
}

void BinaryMetadataExtractor::FindMetadata()
{
    size_t itr = 0;
    while (itr < _binary.size())
    {
        itr = _binary.find(".proto", itr);
        if (itr == std::string::npos)
            break;

        itr += 6;

        size_t start = _binary.rfind('\xA', itr);
        if (start == std::string::npos || _binary[start - 1] != '\0')
            continue;

        size_t decodedBytes = 0;
        uint64_t length = DecodeVarint64(&_binary[start + 1], &decodedBytes);
        size_t end = start + 1 + decodedBytes;
        if (itr - end != length)
            continue;

        google::protobuf::io::CodedInputStream stream(reinterpret_cast<uint8_t*>(&_binary[start]), _binary.size() - start);

        google::protobuf::FileDescriptorProto descriptor;
        if (!descriptor.MergeFromCodedStream(&stream))
            continue;

        _metadatas.emplace_back(new BinaryMetadata(std::to_string(_metadatas.size()), reinterpret_cast<uint8_t*>(&_binary[start]), stream.CurrentPosition() - 1));
    }
}
