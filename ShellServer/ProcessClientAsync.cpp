#include "ProcessClientAsync.hpp"
#include "System/Dup2.hpp"
#include "System/FileDescriptor.hpp"

#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>

void
ShellServer::processClientAsync(System::FileDescriptor& serverSocket,
                                System::FileDescriptor& clientSocket)
{
    auto ret = ::fork();
    if (ret == -1)
        switch (errno)
        {
        case EAGAIN:
        case ENOMEM:
            std::cerr << "System running low on resources, can't allocate"
                         " client task"
                      << std::endl;
            return;
        default:
            throw std::system_error{errno, std::generic_category()};
        }
    else if (ret == 0)
        try
        {
            serverSocket.close();
            System::dup2(clientSocket.toNative(), STDIN_FILENO);
            System::dup2(clientSocket.toNative(), STDOUT_FILENO);
            System::dup2(clientSocket.toNative(), STDERR_FILENO);
            clientSocket.close();

            auto ret = ::execlp("/bin/sh", "sh", nullptr);
            if (ret == -1)
                throw std::system_error{errno, std::generic_category()};

            std::exit(EXIT_SUCCESS);
        }
        catch (std::exception& e)
        {
            std::exit(EXIT_FAILURE);
        }
}
