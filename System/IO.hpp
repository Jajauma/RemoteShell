#ifndef HEADER_920B97B8_AAAB_11E7_A376_74D435E5BEA6_INCLUDED
#define HEADER_920B97B8_AAAB_11E7_A376_74D435E5BEA6_INCLUDED

#include <array>
#include <system_error>

namespace System {
class FileDescriptor;
} /* namespace System */

namespace System {
using IOBuffer = std::array<char, 4096>;
class IOResult
{
public:
    explicit IOResult(IOBuffer::size_type count);
    explicit IOResult(const std::error_code& errorCode);
    IOBuffer::size_type count() const;
    const std::error_code& errorCode() const;

private:
    IOBuffer::size_type mCount;
    std::error_code mErrorCode;
};

IOResult read(int fd, IOBuffer& buffer);
IOResult write(int fd, const IOBuffer& buffer, IOBuffer::size_type count);

IOResult read(const FileDescriptor& fd, IOBuffer& buffer);
IOResult write(const FileDescriptor& fd, const IOBuffer& buffer,
               IOBuffer::size_type count);
} /* namespace System  */

#endif /* end of the header guard */
