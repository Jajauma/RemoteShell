#ifndef HEADER_920B97B8_AAAB_11E7_A376_74D435E5BEA6_INCLUDED
#define HEADER_920B97B8_AAAB_11E7_A376_74D435E5BEA6_INCLUDED

#include "FileDescriptor.hpp"

#include <array>
#include <cstddef>

namespace System {
class IOResult
{
public:
    using BytesCount = long long int;

    IOResult();
    explicit IOResult(BytesCount count);
    BytesCount count() const;
    operator bool() const;

private:
    BytesCount mCount;
    bool mInterrupted;
};

using IOBuffer = std::array<char, 4096>;

IOResult read(FileDescriptor::Handle fd, IOBuffer& buffer);
IOResult write(FileDescriptor::Handle fd, const IOBuffer& buffer,
               IOBuffer::size_type count);

IOResult read(const FileDescriptor& fd, IOBuffer& buffer);
IOResult write(const FileDescriptor& fd, const IOBuffer& buffer,
               IOBuffer::size_type count);
} /* namespace System  */

#endif /* end of the header guard */
