#include "Bind.hpp"
#include "System/FileDescriptor.hpp"
#include "System/GetAddrInfo.hpp"
#include "System/GetNameInfo.hpp"
#include "System/Socket.hpp"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <system_error>

System::FileDescriptor
ShellServer::bind(const System::GetAddrInfo& addrList)
{
    for (auto addr : addrList)
    {
        System::GetNameInfo name{addr.ai_addr, addr.ai_addrlen};
        std::cerr << "Starting server on " << name.host() << ":"
                  << name.service() << " ... " << std::flush;

        System::FileDescriptor sk = System::socket(
            addr.ai_family, addr.ai_socktype, addr.ai_protocol);

        try
        {
            if (::bind(sk.toNative(), addr.ai_addr, addr.ai_addrlen) == 0)
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

    throw std::runtime_error{"Can't start network server."};
}

System::FileDescriptor
ShellServer::bind(const char* host, const char* port)
{
    return bind(System::GetAddrInfo{System::Protocol::TCP, host, port});
}

System::FileDescriptor
ShellServer::bind(const char* port)
{
    return bind(System::GetAddrInfo{
        System::Protocol::TCP, System::SpecialAddress::Wildcard, port});
}
