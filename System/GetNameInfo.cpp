#include "GetNameInfo.hpp"
#include "Cxx/Assert.hpp"

#include <system_error>

#include <netdb.h>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

using namespace System;

GetNameInfo::GetNameInfo(const sockaddr* addr, socklen_t addrLength)
{
    CXX_VALIDATE_ARG(addr != nullptr && addrLength > 0);
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

#if defined(BUILD_UNIT_TESTS)
TEST(GetNameInfo, InvalidArguments)
{
    EXPECT_THROW(GetNameInfo(nullptr, 0), std::invalid_argument);
}
#endif /* BUILD_UNIT_TESTS */
