/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
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
