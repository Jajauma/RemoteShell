#include "Assert.hpp"

#if defined(BUILD_UNIT_TESTS)
#include <gtest/gtest.h>
#endif /* BUILD_UNIT_TESTS */

#if defined(BUILD_UNIT_TESTS)
void
func(int arg)
{
    CXX_VALIDATE_ARG(arg > 0);
}

TEST(Assert, CxxValidateArg)
{
    EXPECT_NO_THROW(func(1));
    EXPECT_THROW(func(-1), std::invalid_argument);
}

TEST(Assert, CxxAssert)
{
    EXPECT_NO_THROW(CXX_ASSERT(2 + 2 == 4));
    EXPECT_THROW(CXX_ASSERT(-1 > 1), std::runtime_error);
}
#endif /* BUILD_UNIT_TESTS */
