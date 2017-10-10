#include "Connect.hpp"
#include "System/FileDescriptor.hpp"
#include "System/GetAddrInfo.hpp"
#include "System/GetNameInfo.hpp"
#include "System/Socket.hpp"

#include <sys/socket.h>
#include <sys/types.h>

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

        System::FileDescriptor sk{
            System::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol)};

        try
        {
            if (::connect(sk.toNative(), addr.ai_addr, addr.ai_addrlen) == 0)
            {
                std::cerr << "[OK]" << std::endl;
                return sk;
            }
            else
                throw std::system_error{errno, std::generic_category()};
        }
        catch (std::exception& e)
        {
            std::cerr << "[" << e.what() << "]" << std::endl;
            continue;
        }
    }

    throw std::runtime_error{"Can't establish network connection."};
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
