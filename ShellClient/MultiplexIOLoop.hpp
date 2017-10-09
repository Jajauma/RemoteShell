#ifndef HEADER_97E8393C_ACBA_11E7_AFC3_74D435E5BEA6_INCLUDED
#define HEADER_97E8393C_ACBA_11E7_AFC3_74D435E5BEA6_INCLUDED

namespace System {
class FileDescriptor;
} /* namespace System */

namespace ShellClient {
void multiplexIOLoop(const System::FileDescriptor& conn);
} /* namespace ShellClient */

#endif /* end of the header guard */
