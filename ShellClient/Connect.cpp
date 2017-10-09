#include "Connect.hpp"

#include "System/FileDescriptor.hpp"
#include "System/GetAddrInfo.hpp"
#include "System/GetNameInfo.hpp"
#include "System/Socket.hpp"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <system_error>

System::FileDescriptor
ShellClient::connect(const System::GetAddrInfo& addrList)
{
    for (auto addr : addrList)
    {
        System::GetNameInfo name{addr.ai_addr, addr.ai_addrlen};
        std::cerr << "Trying " << name.host() << ":" << name.service()
                  << " ... " << std::flush;

        System::FileDescriptor sk = System::socket(
            addr.ai_family, addr.ai_socktype, addr.ai_protocol);

        if (::connect(sk.toNative(), addr.ai_addr, addr.ai_addrlen) != 0)
        {
            std::cerr << "[ERROR]" << std::endl;
            throw std::system_error{errno, std::generic_category()};
        }
        else
        {
            std::cerr << "[OK]" << std::endl;
            return sk;
        }
    }

    throw std::runtime_error{"No more addresses left"};
}

System::FileDescriptor
ShellClient::connect(const char* host, const char* port)
{
    return connect(System::GetAddrInfo{System::Protocol::TCP, host, port});
}

System::FileDescriptor
ShellClient::connect(const char* port)
{
    return connect(System::GetAddrInfo{
        System::Protocol::TCP, System::SpecialAddress::Loopback, port});
}
