#include <cstdlib>
#include <iostream>

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

    return EXIT_SUCCESS;
}
