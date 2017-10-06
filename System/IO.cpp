#include "IO.hpp"
#include "Cxx/Assert.hpp"
#include "FileDescriptor.hpp"

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

IOResult::BytesCount
IOResult::count() const
{
    CXX_ASSERT(!mInterrupted);
    return mCount;
}

IOResult::operator bool() const
{
    return !mInterrupted;
}

IOResult
System::read(const FileDescriptor& fd, IOBuffer& buffer)
{
    auto ret = ::read(fd.toNative(), buffer.begin(), buffer.size());
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
System::write(const FileDescriptor& fd, const IOBuffer& buffer,
              std::size_t count)
{
    CXX_VALIDATE_ARG(!(count > buffer.size()));

    auto ret = ::write(fd.toNative(), buffer.cbegin(), count);
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

#if defined(BUILD_UNIT_TESTS)
TEST(IO, IOResultConstructorWithoutArguments)
{
    IOResult ret;
    EXPECT_FALSE(ret);
    EXPECT_THROW(ret.count(), std::logic_error);
}

TEST(IO, IOResultInvalidArguments)
{
    EXPECT_THROW(IOResult{-1}, std::logic_error);
}

TEST(IO, IOResultZero)
{
    IOResult ret{0};
    EXPECT_TRUE(ret);
    EXPECT_EQ(ret.count(), 0);
}

TEST(IO, IOResultPositive)
{
    IOResult ret{4096};
    EXPECT_TRUE(ret);
    EXPECT_EQ(ret.count(), 4096);
}
#endif /* BUILD_UNIT_TESTS */
