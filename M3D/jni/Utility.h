#ifndef M3D_Uitility_H_
#define M3D_Uitility_H_

#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#elif __IOS__
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
using std::string;
using std::wstring;
using std::vector;

#define  SHOW_LOG true
#define  LOG_TAG    "M3D"
#ifdef __ANDROID__
#define  LOGI(...)  if(SHOW_LOG) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  if(SHOW_LOG) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define UTF_8  "utf8"
#define UTF_16 "utf16"
#define UTF_32 "utf32"

#define UTF_16BE "utf16_BigEndian"
#define UTF_32BE "utf32_BigEndian"

#define UTF_16LE "utf16_LittleEndian"
#define UTF_32LE "utf32_LittleEndian"
#endif

#ifdef WIN32

#define UTF_8  "utf8"
#define UTF_16 "utf16"
#define UTF_32 "utf32"

#define UTF_16BE "utf16_BigEndian"
#define UTF_32BE "utf32_BigEndian"

#define UTF_16LE "utf16_LittleEndian"
#define UTF_32LE "utf32_LittleEndian"

// #define WIN32_LEAN_AND_MEAN
	//#include <windows.h>
	//#include <wingdi.h>	

/** This file is part of the Mingw32 package.
 *  unistd.h maps     (roughly) to io.h
 */
#ifndef _UNISTD_H
#define _UNISTD_H
#include <io.h>
#include <process.h>
#endif /* _UNISTD_H */
 void printLog(const char* tag,const char* format,...);
	#define  LOGI(...)  if(SHOW_LOG) printLog ("Info",__VA_ARGS__);
	#define  LOGE(...)  if(SHOW_LOG) printLog ("Error",__VA_ARGS__);
#endif

#ifdef __IOS__
#define  LOGI(...) printLog("Info",__VA_ARGS__);
#define  LOGE(...) printLog("Error",__VA_ARGS__);

#define UTF_8  "UTF-8"
#define UTF_16 "UTF-16"
#define UTF_32 "UTF-32"

#define UTF_16BE "UTF-16BE"
#define UTF_32BE "UTF-32BE"

#define UTF_16LE "UTF-16LE"
#define UTF_32LE "UTF-32LE"

void printLog(const char* tag,const char* format,...);
int code_convert( const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen );

#ifndef _CLASS_SLOPPY_
#define _CLASS_SLOPPY_
template<class T>
class sloppy {};

// convert between T** and const T**
template<class T>
class sloppy<T**>
{
    T** t;
public:
    sloppy(T** mt) : t(mt) {}
    sloppy(const T** mt) : t(const_cast<T**>(mt)) {}
    
    operator T** () const { return t; }
    operator const T** () const { return const_cast<const T**>(t); }
};
#endif

#endif


//typedef unsigned short WCHAR;

/**
 * 获取系统时间（timeOfDay），单位为毫秒
 */
long GetTimeLong();

/// 打印二进制字符

/**
 * 将文本字符串打印成二进制形式
 * @param str 字符串
 * @param len 长度
 */
void LogCharHEX(char* str,int len);


/**split方法，使用传入的vector
 *
 * @param s
 * @param delim
 * @param elems
 * @return
 */
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

/**split方法，返回一个新的vector
 *
 * @param s
 * @param delim
 * @return
 */
std::vector<std::string> split(const std::string &s, char delim);



/**
 * 转换string到wstring 使用ucnvConvert
 * @param srcStr 原字符串string
 * @param srcEncode 原编码
 * @return 返回转换完成的wstring
 * @note android 上wchar_t为4字节，因此转换为utf32_LittleEndian编码格式
 */
std::wstring StringToWString(const std::string& srcStr,const char* srcEncode="utf8");

/**
 * 转换wstring到string 使用ucnvConvert
 * @param srcStr 原字符串wstring
 * @param outStr 输出字符串
 * @return 返回outStr
 */
std::string WStringToString(const std::wstring& srcStr);

/**
 * 将WCHAR（unsigned short）2字节 转换为安卓wstring
 * @param srcStr
 * @return
 */
std::wstring WCHARStrToWString(unsigned short * srcStr);

/**
 *	将安卓wstring(32位wchar_t)转为16位ushort(自定义的WCHAR)
 * @param srcStr
 * @return
 */
//WCHAR* WStringToWCHARStr(const std::wstring& srcStr);


/**
 * 调用系统icu库的编码转换函数
 * @param tarEncode 	目标编码
 * @param srcEncode 	源编码
 * @param tarBuf		目标buf
 * @param tarBuffLen	目标容量（byte）
 * @param srcBuf		原buf
 * @param srcBufLen		原容量（byte）
 * @param pErrorCode	返回的错误码
 * @return 返回完全转换需要的字节长度（byte）
 */
int UCNVConvert(const char* tarEncode,
		const char* srcEncode,
		char* tarBuf,
		int tarBuffLen,
		const char* srcBuf,
		int srcBufLen,
		int* pErrorCode) ;
#ifdef __ANDROID__
/**
 * jstring转char*
 * @param env		java环境
 * @param jstr		要转换的jstring
 * @param encoding 	编码
 * @return			返回char*
 */
extern string JStrToStr(JNIEnv* env, jstring jstr, const char* encoding="utf8");

/**
 * char* 转 jstring
 * @param env		java环境
 * @param str		要转换的str
 * @param encoding	编码
 * @return			返回jstring
 */
extern jstring	StrToJStr(JNIEnv* env, const char* str, const char* encoding="utf8");

/**
 * 根据类全名获取java 对象
 * @param env	java环境
 * @param path	类全名
 * @return		jobject指针
 */
extern jobject* GetJObject(JNIEnv *env, const char *path);
#endif
/**
 * int转为string
 * @param value
 * @return
 */
std::string IntToString(const int& value);

/**
 * color转为string
 * @param red
 * @param green
 * @param blue
 * @param alpha
 * @return
 */
std::string ColorToString(const int& red, const int& green, const int& blue, const int& alpha);

/**
 * 创建多级目录
 * @param mulDir 多级目录
 * @return 成功返回true，失败返回false
 */
bool CreateMulityPath(const char* mulDir);

/**
 * 将顶点索引转换为顶点集合
 * @param outVertexBuf 	传出的顶点buf
 * @param outNormalBuf 	传出的法线buf
 * @param vertexArray  	传入的element数组
 * @param indexArray	传入的索引数组
 */
void GetVertexBufByVertexAndIndex(vector<float>* outVertexBuf,vector<float>* outNormalBuf,
		vector<float> vertexArray,vector<unsigned short> indexArray);

/**
 * 打印日志输出数组
 * @param pArray	数组
 * @param len		数组长度
 * @param typeSize	数组类型
 * @param flag		sprintf时的参数字符如"%f"
 */
void LogArray(void* pArray,int len,int typeSize,char* flag);

/**
 * 打印日志输出ushort 数组
 * @param pArray	数组
 * @param len		数组长度
 */
void LogUShortArray(unsigned short* pArray, int len);

/**
 * 打印日志输出float数组
 * @param pArray	数组
 * @param len		数组长度
 */
void LogFloatArray(float* pArray, int len);

/**
 * 打印日志输出int数组
 * @param pArray	数组
 * @param len		数组长度
 */
void LogIntArray(unsigned int* pArray, int len);

#endif /* Util_H_ */
