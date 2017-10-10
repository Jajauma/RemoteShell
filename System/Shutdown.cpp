#include "Shutdown.hpp"
#include "FileDescriptor.hpp"

#include <sys/socket.h>

#include <cerrno>
#include <system_error>

namespace System {
void
shutdown(const FileDescriptor& sk, int how)
{
    auto ret = ::shutdown(sk.toNative(), how);
    if (ret != 0)
        throw std::system_error{errno, std::generic_category()};
}
} /* namespace System */

void
System::shutdownRead(const FileDescriptor& sk)
{
    shutdown(sk, SHUT_RD);
}

void
System::shutdownWrite(const FileDescriptor& sk)
{
    shutdown(sk, SHUT_WR);
}

void
System::shutdownReadWrite(const FileDescriptor& sk)
{
    shutdown(sk, SHUT_RDWR);
}
