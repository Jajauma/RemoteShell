#ifndef HEADER_920B97B8_AAAB_11E7_A376_74D435E5BEA6_INCLUDED
#define HEADER_920B97B8_AAAB_11E7_A376_74D435E5BEA6_INCLUDED

#include <cstddef>

namespace System {
class IOResult
{
public:
    using BytesCount = long long int;

    IOResult();
    explicit IOResult(BytesCount count);
    BytesCount count() const;
    operator bool() const;

private:
    BytesCount mCount;
    bool mInterrupted;
};
} /* namespace System  */

#endif /* end of the header guard */
