#include "Connect.hpp"
#include "MultiplexIOLoop.hpp"
#include "System/FileDescriptor.hpp"

#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    System::FileDescriptor clientSocket;
    try
    {
        switch (argc)
        {
        case 2:
            clientSocket = ShellClient::connect(argv[1]);
            break;
        case 3:
            clientSocket = ShellClient::connect(argv[1], argv[2]);
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [<addr>] <port>"
                      << std::endl;
            return EXIT_FAILURE;
        }
        ShellClient::multiplexIOLoop(clientSocket);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
