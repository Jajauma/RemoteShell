#ifndef HEADER_04A4F32C_A8E1_11E7_8144_74D435E5BEA6_INCLUDED
#define HEADER_04A4F32C_A8E1_11E7_8144_74D435E5BEA6_INCLUDED

#include <sys/socket.h>

namespace System {
class GetNameInfo
{
public:
    GetNameInfo(const sockaddr* addr, socklen_t addrLength);
    const char* host() const;
    const char* service() const;
    int port() const;

private:
    static constexpr auto MaxHost    = 1025;
    static constexpr auto MaxService = 32;
    char mHost[MaxHost];
    char mService[MaxService];
};
} /* namespace System  */

#endif /* end of the header guard */
