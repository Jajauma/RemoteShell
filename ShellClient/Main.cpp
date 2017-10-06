#include <array>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <system_error>

#include <sys/select.h>
#include <unistd.h>

#include "Connect.hpp"
#include "System/FileDescriptor.hpp"

int
main(int argc, char* argv[])
{
    System::FileDescriptor conn;

    try
    {
        switch (argc)
        {
        case 2:
            conn = ShellClient::connect(argv[1]);
            break;

        case 3:
            conn = ShellClient::connect(argv[1], argv[2]);
            break;

        default:
            std::cerr << "Usage: " << argv[0] << " [<addr>] <port>"
                      << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    fd_set rset;
    FD_ZERO(&rset);
    while (true)
    {
        FD_SET(STDIN_FILENO, &rset);
        FD_SET(conn.toNative(), &rset);
        auto maxFD1 = std::max(static_cast<int>(STDIN_FILENO),
                               static_cast<int>(conn.toNative()))
                      + 1;
        auto ret = ::select(maxFD1, &rset, nullptr, nullptr, nullptr);
        if (ret == -1)
            switch (errno)
            {
            case EINTR:
                continue;
            default:
                throw std::system_error{errno, std::system_category()};
            }
        else if (ret > 0)
        {
            if (FD_ISSET(conn.toNative(), &rset))
            {
                std::cerr << "Socket is ready (+";
                std::array<char, 4096> buf;
                auto socketRead
                    = ::read(conn.toNative(), buf.begin(), buf.size());
                std::cerr << socketRead << " bytes)" << std::endl;
                auto stdoutWrtite
                    = ::write(STDOUT_FILENO, buf.cbegin(), socketRead);
            }
            if (FD_ISSET(STDIN_FILENO, &rset))
            {
                std::cerr << "Standard input is ready (+";
                std::array<char, 4096> buf;
                auto stdinRead = ::read(STDIN_FILENO, buf.begin(), buf.size());
                std::cerr << stdinRead << " bytes)" << std::endl;
                auto socketWrite
                    = ::write(conn.toNative(), buf.cbegin(), stdinRead);
            }
        }
    }

    return EXIT_SUCCESS;
}
