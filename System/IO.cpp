#include "IO.hpp"
#include "Cxx/Assert.hpp"
#include "FileDescriptor.hpp"

#include <unistd.h>

#include <cerrno>
#include <system_error>

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

using namespace System;

IOResult::IOResult(IOBuffer::size_type count)
    : mCount{count}
{
}

IOResult::IOResult(const std::error_code& errorCode)
    : mCount{0}
    , mErrorCode{errorCode}
{
}

IOBuffer::size_type
IOResult::count() const
{
    CXX_ASSERT(!mErrorCode);
    return mCount;
}

const std::error_code&
IOResult::errorCode() const
{
    return mErrorCode;
}

IOResult
System::read(int fd, IOBuffer& buffer)
{
    auto ret = ::read(fd, buffer.begin(), buffer.size());
    if (ret == -1)
        return IOResult{std::error_code{errno, std::generic_category()}};
    return IOResult{static_cast<IOBuffer::size_type>(ret)};
}

IOResult
System::write(int fd, const IOBuffer& buffer, IOBuffer::size_type count)
{
    CXX_VALIDATE_ARG(!(count > buffer.size()));

    auto ret = ::write(fd, buffer.cbegin(), count);
    if (ret == -1)
        return IOResult{std::error_code{errno, std::generic_category()}};
    return IOResult{static_cast<IOBuffer::size_type>(ret)};
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
TEST(IO, IOResultZeroCount)
{
    IOResult ret{0};
    EXPECT_NO_THROW(ret.count());
    EXPECT_EQ(ret.count(), 0);
}

TEST(IO, IOResultNonZeroCount)
{
    IOResult ret{4096};
    EXPECT_NO_THROW(ret.count());
    EXPECT_EQ(ret.count(), 4096);
}

TEST(IO, IOResultErrorCode)
{
    IOResult ret{std::error_code{EINTR, std::generic_category()}};
    EXPECT_TRUE(ret.errorCode() == std::errc::interrupted);
    EXPECT_THROW(ret.count(), std::logic_error);
}
#endif /* BUILD_UNIT_TESTS */
