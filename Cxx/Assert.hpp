#ifndef HEADER_B4F7204E_9A1B_11E7_A0D4_74D435E5BEA6_INCLUDED
#define HEADER_B4F7204E_9A1B_11E7_A0D4_74D435E5BEA6_INCLUDED

#include <sstream>
#include <stdexcept>

#define CXX_VALIDATE_ARG(condition)                                           \
    do                                                                        \
    {                                                                         \
        if (!(condition))                                                     \
        {                                                                     \
            std::ostringstream tmp;                                           \
            tmp << "Assertion `" #condition "` failed in " << __FILE__ << ":" \
                << __LINE__;                                                  \
            throw std::invalid_argument{tmp.str()};                           \
        }                                                                     \
    } while (false)

#define CXX_ASSERT(condition)                                                 \
    do                                                                        \
    {                                                                         \
        if (!(condition))                                                     \
        {                                                                     \
            std::ostringstream tmp;                                           \
            tmp << "Assertion `" #condition "` failed in " << __FILE__ << ":" \
                << __LINE__;                                                  \
            throw std::logic_error{tmp.str()};                                \
        }                                                                     \
    } while (false)

#endif /* end of the header guard */
