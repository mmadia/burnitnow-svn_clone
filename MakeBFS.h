#include <Be.h>
#ifndef __MAKE_BFS__
#define __MAKE_BFS__

#define BUFFERLEN 1024*512

status_t CopyFile(const entry_ref &source, const entry_ref &dest);
int CopyFiles();
int MountBFS();
int UnmountBFS();
int MakeBFS();
uint64 GetBFSSize();

#endif