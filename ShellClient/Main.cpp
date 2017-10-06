#include <cstdlib>
#include <iostream>

int
main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <addr> <port>" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
