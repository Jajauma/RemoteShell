#include "GetAddrInfoErrors.hpp"

#include <netdb.h>

namespace {
class GetAddrInfoErrorCategory : public std::error_category
{
public:
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char*
GetAddrInfoErrorCategory::name() const noexcept
{
    return "getaddrinfo";
}

std::string
GetAddrInfoErrorCategory::message(int ev) const
{
    return std::string{::gai_strerror(ev)};
}

const GetAddrInfoErrorCategory GetAddrInfoErrorCategoryInstance{};
} /* anonymous namespace */

std::error_code
System::make_error_code(GetAddrInfoErrc e)
{
    return {static_cast<int>(e), GetAddrInfoErrorCategoryInstance};
}
