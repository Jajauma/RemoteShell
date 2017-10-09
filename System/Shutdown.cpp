#include "Shutdown.hpp"
#include "FileDescriptor.hpp"

#include <cerrno>
#include <system_error>

#include <sys/socket.h>

namespace System {
void
shutdown(const FileDescriptor& sk, int how)
{
    auto ret = ::shutdown(sk.toNative(), how);
    if (ret != 0)
        throw std::system_error{errno, std::system_category()};
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
