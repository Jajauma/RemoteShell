#include "FileDescriptor.hpp"
#include "Cxx/Assert.hpp"

#include <unistd.h>

#include <cerrno>
#include <iostream>
#include <system_error>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

using namespace System;

static constexpr FileDescriptor::Handle None = -1;

FileDescriptor::FileDescriptor()
    : mHandle{None}
{
}

FileDescriptor::FileDescriptor(Handle fd)
    : mHandle{fd}
{
    CXX_VALIDATE_ARG(!(fd < 0));
}

FileDescriptor::FileDescriptor(FileDescriptor&& other)
    : mHandle{other.mHandle}
{
    other.mHandle = None;
}

FileDescriptor&
FileDescriptor::operator=(FileDescriptor&& other)
{
    close();

    mHandle       = other.mHandle;
    other.mHandle = None;

    return *this;
}

FileDescriptor::~FileDescriptor()
{
    try
    {
        close();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

FileDescriptor::Handle
FileDescriptor::toNative() const
{
    CXX_ASSERT(mHandle != None);
    return mHandle;
}

void
FileDescriptor::close()
{
    if (mHandle != None)
    {
        while (::close(mHandle) != 0)
            if (errno == EINTR)
                continue;
            else
                throw std::system_error{errno, std::generic_category()};
        mHandle = None;
    }
}

#if defined(BUILD_UNIT_TESTS)
TEST(FileDescriptor, InvalidArguments)
{
    EXPECT_THROW(FileDescriptor{-1}, std::invalid_argument);
}

TEST(FileDescriptor, ConstructorWithoutArguments)
{
    EXPECT_NO_THROW(FileDescriptor{});
}

TEST(FileDescriptor, ToNative)
{
    FileDescriptor x{};
    EXPECT_THROW(x.toNative(), std::logic_error);
}
#endif /* BUILD_UNIT_TESTS */
