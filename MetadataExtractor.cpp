#include "MetadataExtractor.h"

MetadataExtractor::Metadata const* MetadataExtractor::GetById(std::string const& id) const
{
    auto itr = std::find_if(_metadatas.begin(), _metadatas.end(), [&](std::unique_ptr<Metadata> const& ptr) { return ptr->GetId() == id; });
    if (itr != _metadatas.end())
        return itr->get();

    return nullptr;
}
