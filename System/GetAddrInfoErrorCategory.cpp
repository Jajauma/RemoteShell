#include "GetAddrInfoErrorCategory.hpp"

#include <netdb.h>

namespace {
struct GetAddrInfoErrorCategoryImplementation : std::error_category
{
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char*
GetAddrInfoErrorCategoryImplementation::name() const noexcept
{
    return "getaddrinfo";
}

std::string
GetAddrInfoErrorCategoryImplementation::message(int ev) const
{
    return std::string{::gai_strerror(ev)};
}
} /* anonymous namespace */

const std::error_category&
System::GetAddrInfoErrorCategory()
{
    static GetAddrInfoErrorCategoryImplementation instance;
    return instance;
}
