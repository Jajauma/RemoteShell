#include "Listen.hpp"
#include "Cxx/Assert.hpp"
#include "FileDescriptor.hpp"

#include <cerrno>
#include <system_error>

#include <sys/socket.h>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

void
System::listen(const FileDescriptor& sk, int backlog)
{
    CXX_VALIDATE_ARG(backlog > 0);

    auto ret = ::listen(sk.toNative(), backlog);
    if (ret != 0)
        throw std::system_error{errno, std::generic_category()};
}

#if defined(BUILD_UNIT_TESTS)
TEST(Listen, InvalidArguments)
{
    EXPECT_THROW(System::listen(System::FileDescriptor{}, 0), std::logic_error);
    EXPECT_THROW(System::listen(System::FileDescriptor{}, -9999),
                 std::logic_error);
}
#endif /* BUILD_UNIT_TESTS */
