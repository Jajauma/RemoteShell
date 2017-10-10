#include "Socket.hpp"
#include "FileDescriptor.hpp"

#include <sys/socket.h>
#include <sys/types.h>

#include <cerrno>
#include <system_error>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#include <netinet/in.h>
#endif /* BUILD_UNIT_TESTS */

System::FileDescriptor
System::socket(int domain, int type, int protocol)
{
    auto ret = ::socket(domain, type, protocol);
    if (ret == -1)
        throw std::system_error{errno, std::generic_category()};

    return System::FileDescriptor{ret};
}

#if defined(BUILD_UNIT_TESTS)
TEST(Socket, InvalidArguments)
{
    EXPECT_THROW(System::FileDescriptor sk = System::socket(1234, 5678, -90),
                 std::system_error);
}

TEST(Socket, UDP)
{
    EXPECT_NO_THROW(System::FileDescriptor sk4
                    = System::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
    EXPECT_NO_THROW(System::FileDescriptor sk6
                    = System::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP));
}

TEST(Socket, TCP)
{
    EXPECT_NO_THROW(System::FileDescriptor sk4
                    = System::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    EXPECT_NO_THROW(System::FileDescriptor sk6
                    = System::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP));
}
#endif /* BUILD_UNIT_TESTS */
