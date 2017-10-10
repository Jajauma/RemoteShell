#include "Dup2.hpp"

#include <unistd.h>

#include <cerrno>
#include <system_error>

void
System::dup2(int oldfd, int newfd)
{
    while (true)
    {
        auto ret = ::dup2(oldfd, newfd);
        if (ret == -1)
            switch (errno)
            {
            case EINTR:
                continue;
            default:
                throw std::system_error{errno, std::generic_category()};
            }
        return;
    }
}
