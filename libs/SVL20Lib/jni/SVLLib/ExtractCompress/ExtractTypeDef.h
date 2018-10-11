#ifndef _SVLLIB_EXTTRACTTYPEDEF_H_
#define _SVLLIB_EXTTRACTTYPEDEF_H_

#ifdef __MOBILE__
typedef void*	HANDLE;
typedef long long	__int64;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;

typedef struct _FILETIME {
	unsigned long dwLowDateTime;
	unsigned long dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
#define _FILETIME_

#define MAX_PATH 256
#endif

#endif
