#ifndef Util_h__
#define Util_h__
#include <vector>
#include <string>


class CUtil
{
public:
	CUtil(void);
	~CUtil(void);

public:
	//获取本机所有Mac
	static void GetAllMac(std::vector<std::string>& sa);
};

#endif
