#ifndef HEADER_A55841F6_ACC5_11E7_AFC3_74D435E5BEA6_INCLUDED
#define HEADER_A55841F6_ACC5_11E7_AFC3_74D435E5BEA6_INCLUDED

#include <system_error>

namespace System {
enum class GetAddrInfoErrc {};
} /* namespace System */

namespace std {
template <>
struct is_error_code_enum<System::GetAddrInfoErrc> : true_type
{
};
} /* namespace std */

namespace System {
std::error_code make_error_code(GetAddrInfoErrc e);
} /* namespace System */

#endif /* end of the header guard */
