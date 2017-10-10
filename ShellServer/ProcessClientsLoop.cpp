#include "ProcessClientsLoop.hpp"
#include "Cxx/Unused.hpp"
#include "System/FileDescriptor.hpp"
#include "System/GetNameInfo.hpp"

#include <signal.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
#include <system_error>

void
ReapChildProcess(int signalCode)
{
    Cxx::Unused(signalCode);
}

void
ShellServer::processClientsLoop(System::FileDescriptor& serverSocket)
{
    struct sigaction act;
    std::memset(&act, 0, sizeof(act));
    act.sa_handler = ReapChildProcess;

    auto ret = ::sigaction(SIGCHLD, &act, nullptr);
    if (ret != 0)
        throw std::system_error{errno, std::generic_category()};

    while (true)
    {
        sockaddr clientAddress;
        socklen_t clientAddressLength{sizeof(clientAddress)};

        auto ret = ::accept(
            serverSocket.toNative(), &clientAddress, &clientAddressLength);
        if (ret == -1)
            switch (errno)
            {
            case EINTR:
            case ECONNABORTED:
                continue;
            case EMFILE:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
                std::cerr << "System running low on resources, can't accept "
                             "incoming connections"
                          << std::endl;
                continue;
            default:
                throw std::system_error{errno, std::generic_category()};
            }

        System::FileDescriptor clientSocket{ret};
        System::GetNameInfo peerName(&clientAddress, clientAddressLength);
        std::cerr << "Accepted connection from " << peerName.host() << ":"
                  << peerName.service() << std::endl;
    }
}
