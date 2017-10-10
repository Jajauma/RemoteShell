#include "ProcessClientAsync.hpp"
#include "Cxx/Unused.hpp"

#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>

void
ShellServer::processClientAsync(System::FileDescriptor& serverSocket,
                                System::FileDescriptor& clientSocket)
{
    Cxx::Unused(serverSocket);
    Cxx::Unused(clientSocket);

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
    {
        std::exit(0);
    }
}
