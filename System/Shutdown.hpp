#ifndef HEADER_4289F7A4_ACB6_11E7_AFC3_74D435E5BEA6_INCLUDED
#define HEADER_4289F7A4_ACB6_11E7_AFC3_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
} /* namespace System */

namespace System {
void shutdownRead(const FileDescriptor& sk);
void shutdownWrite(const FileDescriptor& sk);
void shutdownReadWrite(const FileDescriptor& sk);
} /* namespace System */

#endif /* end of the header guard */
