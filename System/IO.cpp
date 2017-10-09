#include "IO.hpp"
#include "Cxx/Assert.hpp"

#include <cerrno>
#include <system_error>

#include <unistd.h>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

using namespace System;

IOResult::IOResult()
    : mCount{0}
    , mInterrupted{true}
{
}

IOResult::IOResult(BytesCount count)
    : mCount{count}
    , mInterrupted{false}
{
    CXX_VALIDATE_ARG(count >= 0);
}

bool
IOResult::isInterrupted() const
{
    return mInterrupted;
}

IOResult::BytesCount
IOResult::count() const
{
    CXX_ASSERT(!mInterrupted);
    return mCount;
}

IOResult
System::read(FileDescriptor::Handle fd, IOBuffer& buffer)
{
    auto ret = ::read(fd, buffer.begin(), buffer.size());
    if (ret == -1)
        switch (errno)
        {
        case EINTR:
            return IOResult{};
        default:
            throw std::system_error{errno, std::system_category()};
        }

    return IOResult{ret};
}

IOResult
System::write(FileDescriptor::Handle fd, const IOBuffer& buffer,
              IOBuffer::size_type count)
{
    CXX_VALIDATE_ARG(!(count > buffer.size()));

    auto ret = ::write(fd, buffer.cbegin(), count);
    if (ret == -1)
        switch (errno)
        {
        case EINTR:
            return IOResult{};
        default:
            throw std::system_error{errno, std::system_category()};
        }

    return IOResult{ret};
}

IOResult
System::read(const FileDescriptor& fd, IOBuffer& buffer)
{
    return read(fd.toNative(), buffer);
}

IOResult
System::write(const FileDescriptor& fd, const IOBuffer& buffer,
              IOBuffer::size_type count)
{
    return write(fd.toNative(), buffer, count);
}

#if defined(BUILD_UNIT_TESTS)
TEST(IO, IOResultConstructorWithoutArguments)
{
    IOResult ret;
    EXPECT_TRUE(ret.isInterrupted());
    EXPECT_THROW(ret.count(), std::logic_error);
}

TEST(IO, IOResultInvalidArguments)
{
    EXPECT_THROW(IOResult{-1}, std::logic_error);
}

TEST(IO, IOResultZero)
{
    IOResult ret{0};
    EXPECT_FALSE(ret.isInterrupted());
    EXPECT_EQ(ret.count(), 0);
}

TEST(IO, IOResultPositive)
{
    IOResult ret{4096};
    EXPECT_FALSE(ret.isInterrupted());
    EXPECT_EQ(ret.count(), 4096);
}
#endif /* BUILD_UNIT_TESTS */
