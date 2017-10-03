#ifndef HEADER_E342DDC2_A78F_11E7_9844_74D4359F3068_INCLUDED
#define HEADER_E342DDC2_A78F_11E7_9844_74D4359F3068_INCLUDED

#include <cerrno>
#include <system_error>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace System {
template <typename T1, typename T2, typename T3>
auto
socket(T1 arg1, T2 arg2, T3 arg3) -> decltype(::socket(arg1, arg2, arg3))
{
    auto fd = ::socket(arg1, arg2, arg3);
    if (fd == -1)
        throw std::system_error{errno, std::generic_category()};
    return fd;
}

template <typename T1, typename T2, typename T3>
void
bind(T1 arg1, T2 arg2, T3 arg3)
{
    auto ret = ::bind(arg1, arg2, arg3);
    if (ret == -1)
        throw std::system_error{errno, std::generic_category()};
}

template <typename T1>
void
close(T1 arg1)
{
    auto ret = ::close(arg1);
    if (ret == -1)
        throw std::system_error{errno, std::generic_category()};
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void
setsockopt(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
{
    auto ret = ::setsockopt(arg1, arg2, arg3, arg4, arg5);
    if (ret == -1)
        throw std::system_error{errno, std::generic_category()};
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void
getsockopt(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
{
    auto ret = ::getsockopt(arg1, arg2, arg3, arg4, arg5);
    if (ret == -1)
        throw std::system_error{errno, std::generic_category()};
}

template <typename T1, typename T2, typename T3, typename T4>
void
getaddrinfo(T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
    auto ret = ::getaddrinfo(arg1, arg2, arg3, arg4);
    if (ret != 0)
        throw std::system_error{
            errno, std::generic_category(), gai_strerror(ret)};
}

template<typename T1>
void
freeaddrinfo(T1 arg1)
{
    ::freeaddrinfo(arg1);
}
} /* namespace System  */

#endif /* end of the header guard */
