#ifndef HEADER_50240AB0_ACEF_11E7_BB02_74D435E5BEA6_INCLUDED
#define HEADER_50240AB0_ACEF_11E7_BB02_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
} /* namespace System */

namespace System {
void listen(const FileDescriptor& sk, int backlog = 32);
} /* namespace System */

#endif /* end of the header guard */
