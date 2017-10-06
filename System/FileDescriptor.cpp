#include "FileDescriptor.hpp"
#include "Cxx/Assert.hpp"

#include <cerrno>
#include <iostream>
#include <system_error>

#include <unistd.h>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

using namespace System;

static constexpr FileDescriptor::Handle None = -1;

FileDescriptor::FileDescriptor()
    : mHandle(None)
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
    if (mHandle != None)
        closeUninterrubtible();

    mHandle       = other.mHandle;
    other.mHandle = None;

    return *this;
}

FileDescriptor::~FileDescriptor()
{
    if (mHandle != None)
        try
        {
            closeUninterrubtible();
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
}

FileDescriptor::Handle
FileDescriptor::toNative() const
{
    return mHandle;
}

void
FileDescriptor::closeUninterrubtible()
{
    while (::close(mHandle) != 0)
        if (errno == EINTR)
            continue;
        else
            throw std::system_error{errno, std::system_category()};
}

#if defined(BUILD_UNIT_TESTS)
TEST(FileDescriptor, InvalidArguments)
{
    EXPECT_THROW(FileDescriptor(-1), std::invalid_argument);
}

TEST(FileDescriptor, ConstructorWithoutArguments)
{
    EXPECT_NO_THROW(FileDescriptor());
}
#endif /* BUILD_UNIT_TESTS */
