#include "GetAddrInfo.hpp"
#include "Cxx/Assert.hpp"

#include <cerrno>
#include <cstring>
#include <system_error>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

using namespace System;

GetAddrInfo::GetAddrInfo(Protocol protocol, const char* host, const char* port)
{
    CXX_VALIDATE_ARG(!(host == nullptr && port == nullptr));

    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));

    switch (protocol)
    {
    case Protocol::TCP4:
    case Protocol::UDP4:
        hints.ai_family = AF_INET;
        break;
    case Protocol::TCP6:
    case Protocol::UDP6:
        hints.ai_family = AF_INET6;
        break;
    case Protocol::UDP:
    case Protocol::TCP:
        hints.ai_family = AF_UNSPEC;
        break;
    }

    switch (protocol)
    {
    case Protocol::TCP4:
    case Protocol::TCP6:
    case Protocol::TCP:
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        break;
    case Protocol::UDP4:
    case Protocol::UDP6:
    case Protocol::UDP:
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        break;
    }

    hints.ai_flags = AI_NUMERICSERV | AI_V4MAPPED | AI_ADDRCONFIG;

    int ret = ::getaddrinfo(host, port, &hints, &mResult);
    if (ret != 0)
        throw std::system_error{
            errno, std::generic_category(), ::gai_strerror(ret)};
}

GetAddrInfo::GetAddrInfo(Protocol protocol, Special special, const char* port)
{
    CXX_VALIDATE_ARG(port != nullptr);

    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));

    switch (protocol)
    {
    case Protocol::TCP4:
    case Protocol::UDP4:
        hints.ai_family = AF_INET;
        break;
    case Protocol::TCP6:
    case Protocol::UDP6:
        hints.ai_family = AF_INET6;
        break;
    case Protocol::UDP:
    case Protocol::TCP:
        hints.ai_family = AF_UNSPEC;
        break;
    }

    switch (protocol)
    {
    case Protocol::TCP4:
    case Protocol::TCP6:
    case Protocol::TCP:
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        break;
    case Protocol::UDP4:
    case Protocol::UDP6:
    case Protocol::UDP:
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        break;
    }

    hints.ai_flags = AI_NUMERICSERV | AI_V4MAPPED | AI_ADDRCONFIG;
    if (special == Special::Wildcard)
        hints.ai_flags |= AI_PASSIVE;

    int ret = ::getaddrinfo(nullptr, port, &hints, &mResult);
    if (ret != 0)
        throw std::system_error{
            errno, std::generic_category(), ::gai_strerror(ret)};
}

GetAddrInfo::GetAddrInfo(GetAddrInfo&& other)
    : mResult{other.mResult}
{
    other.mResult = nullptr;
}

GetAddrInfo&
GetAddrInfo::operator=(GetAddrInfo&& other)
{
    if (mResult)
        ::freeaddrinfo(mResult);

    mResult       = other.mResult;
    other.mResult = nullptr;

    return *this;
}

GetAddrInfo::~GetAddrInfo()
{
    if (mResult)
        ::freeaddrinfo(mResult);
}

#if defined(BUILD_UNIT_TESTS)
TEST(GetAddrInfo, Test1)
{
    // GetAddrInfo gai1{"test", "1", nullptr};
    // GetAddrInfo gai2{std::move(gai1)};
    // GetAddrInfo gai3 = std::move(gai1);
}
#endif /* BUILD_UNIT_TESTS */
