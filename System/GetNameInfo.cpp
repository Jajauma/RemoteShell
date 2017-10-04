#include "GetNameInfo.hpp"
#include "Cxx/Assert.hpp"

#include <system_error>

#include <netdb.h>

using namespace System;

GetNameInfo::GetNameInfo(const sockaddr* addr, socklen_t addrLength)
{
    CXX_VALIDATE_ARG(addr != nullptr);
    mHost[0] = mPort[0] = '\0';

    int ret = ::getnameinfo(addr,
                            addrLength,
                            mHost,
                            MaxHost,
                            mPort,
                            MaxPort,
                            NI_NUMERICHOST | NI_NUMERICSERV);
    if (ret != 0)
        throw std::system_error{
            errno, std::generic_category(), ::gai_strerror(ret)};
}

const char*
GetNameInfo::host() const
{
    return mHost;
}

const char*
GetNameInfo::port() const
{
    return mPort;
}
