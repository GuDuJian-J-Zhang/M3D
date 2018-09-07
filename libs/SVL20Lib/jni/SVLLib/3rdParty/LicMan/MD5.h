#pragma once

#ifndef _MD5_H
#define _MD5_H

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

#include <string>
using namespace std;

class CMD5
{
public:
	typedef struct
	{
		uint32 total[2];
		uint32 state[4];
		uint8 buffer[64];
	}
	md5_context;

	static void md5_process( md5_context *ctx, uint8 data[64] );
	static void md5_starts( md5_context *ctx );
	static void md5_update( md5_context *ctx, uint8 *input, uint32 length );
	static void md5_finish( md5_context *ctx, uint8 digest[16] );

public:
	CMD5(void);
	~CMD5(void);
	static string CMD5::GetMD5(char* lpString);
};

#endif /* md5.h */