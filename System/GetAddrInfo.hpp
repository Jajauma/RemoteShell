#ifndef HEADER_DDD50F64_A85B_11E7_BC31_74D4359F3068_INCLUDED
#define HEADER_DDD50F64_A85B_11E7_BC31_74D4359F3068_INCLUDED

struct addrinfo;

namespace System {
enum class Protocol { TCP, TCP4, TCP6, UDP, UDP4, UDP6 };
enum class SpecialAddress { Loopback, Wildcard };

class GetAddrInfo
{
public:
    GetAddrInfo(Protocol protocol, const char* host, const char* port);
    GetAddrInfo(Protocol protocol, SpecialAddress specialAddress,
                const char* port);
    GetAddrInfo(GetAddrInfo&& other);
    GetAddrInfo& operator=(GetAddrInfo&& other);
    ~GetAddrInfo();

private:
    addrinfo* mResult;
};
} /* namespace System  */

#endif /* end of the header guard */
