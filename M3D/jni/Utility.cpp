
#ifdef __ANDROID__
#include <jni.h>
#include "sys/system_properties.h"
#include "dlfcn.h"
#endif


#ifdef __IOS__
#include <iconv.h>
#endif

#ifdef _WIN32
#include "Log4Native.h"
#define SPRINTF_TRACE_BUFSIZE 409600
#endif

#include "Utility.h"
#include <stdarg.h>

#include "m3d/utils/Platform.h"
//#include "m3d.h"
#include <sys/stat.h>
#include <sstream>
#ifdef _WIN32
void printInfo(const char* format, ...)
{
	/* 初始时假设我们只需要不超过100字节大小的空间 */
	int n, size = 100;
	char *p = NULL;
	va_list ap;
	if ((p = (char *)malloc(size * sizeof(char))) == NULL)
		return;
	int i = 0;
	while (i++<20)
	{
		/* 尝试在申请的空间中进行打印操作 */
		va_start(ap, format);
		n = vsnprintf(p, size, format, ap);
		va_end(ap);
		/* 如果vsnprintf调用成功，返回该字符串 */
		if (n > -1 && n < size)
			break;
		/* vsnprintf调用失败(n<0)，或者p的空间不足够容纳size大小的字符串(n>=size)，尝试申请更大的空间*/
		size *= 2; /* 两倍原来大小的空间 */
		if ((p = (char *)realloc(p, size * sizeof(char))) == NULL)
			break;
	}
	if (p != NULL)
	{
		Log4Native::Logger::ReportMessageInfo(p);
		free(p);
	}
}

void printError(const char* format, ...)
{	
	/* 初始时假设我们只需要不超过100字节大小的空间 */
	int n, size = 100;
	char *p = NULL;
	va_list ap;
	if ((p = (char *)malloc(size * sizeof(char))) == NULL)
		return;
	int i = 0;
	while (i++ < 20)
	{
		/* 尝试在申请的空间中进行打印操作 */
		va_start(ap, format);
		n = vsnprintf(p, size, format, ap);
		va_end(ap);
		/* 如果vsnprintf调用成功，返回该字符串 */
		if (n > -1 && n < size)
			break;
		/* vsnprintf调用失败(n<0)，或者p的空间不足够容纳size大小的字符串(n>=size)，尝试申请更大的空间*/
		size *= 2; /* 两倍原来大小的空间 */
		if ((p = (char *)realloc(p, size * sizeof(char))) == NULL)
			break;
	}
	if (p != NULL)
	{
		Log4Native::Logger::ReportMessageError(p);
		free(p);
	}
}

void printDebug(const char* format, ...)
{	
	/* 初始时假设我们只需要不超过100字节大小的空间 */
	int n, size = 100;
	char *p = NULL;
	va_list ap;
	if ((p = (char *)malloc(size * sizeof(char))) == NULL)
		return;
	int i = 0;
	while (i++ < 20)
	{
		/* 尝试在申请的空间中进行打印操作 */
		va_start(ap, format);
		n = vsnprintf(p, size, format, ap);
		va_end(ap);
		/* 如果vsnprintf调用成功，返回该字符串 */
		if (n > -1 && n < size)
			break;
		/* vsnprintf调用失败(n<0)，或者p的空间不足够容纳size大小的字符串(n>=size)，尝试申请更大的空间*/
		size *= 2; /* 两倍原来大小的空间 */
		if ((p = (char *)realloc(p, size * sizeof(char))) == NULL)
			break;
	}
	if (p != NULL)
	{
		Log4Native::Logger::ReportMessageDebug(p);
		free(p);
	}
}
#endif

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


//取得ushort型（2字节）字符串的长度
int wlen(unsigned short * strString) {
	int i = 0;
	while (1) {
		if (strString[i] == 0) {
			break;
		} else {
			i++;
		}
	}
	return i;
}

//short 小头转大头
short l2b(const short* pSrc) {
	return ((*pSrc >> 8) & 0xff) | ((*pSrc << 8) & 0xff00);
}

//将WCHAR字符串的制定长度转为大头
void makePWCHARBig(short* pSrcStr, int len) {
	for (int i = 0; i < len; i++) {
		short* pTmp = pSrcStr + i;
		*pTmp = l2b(pTmp);
	}
}

//
std::wstring StringToWString(const std::string& s, const char* local) {

	return M3D::Platform::StringToWString(s);
}

//
std::string WStringToString(const std::wstring& ws)
{
	std::string outStr;
	if(ws.length() == 0)
	{
		outStr = "";///若ws的长度为0，则直接返回一个空的string
		return outStr;
	}
	const char* srcStr = (const char*)ws.c_str();
	int srcLen = (ws.size())*4;
	int tarBufLen = srcLen;
	char* tarBuf = new char[tarBufLen];
	memset(tarBuf,'\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_8, UTF_32LE, tarBuf,
			tarBufLen,srcStr, srcLen, &errCode);
	outStr = tarBuf;
//	LOGI("wstring2string: convert errcode:%d srclen:%d str:%s ret:%d"
//			, errCode, srcLen, tmpwstr.c_str(),ret);
	delete[] tarBuf;
	return outStr;
}

std::string IntToString(const int& value){
	std::stringstream ss;
	ss << value;
	std::string ret;
	ss >> ret;
	return ret;
}

std::string ColorToString(const int& red, const int& green, const int& blue, const int& alpha){
	std::stringstream ss;
	ss << red << ", ";
	ss << green << ", ";
	ss << blue << ", ";
	ss << alpha;
	std::string ret;
	ss >> ret;
	return ret;
}

void LogCharHEX(char* str, int len) {
	char tmpStr[1000];
	memset(tmpStr, 0x0, 1000);
	for (int i = 0; i < len; i++) {
		sprintf(tmpStr, "%s %x", tmpStr, str[i]);
	}
	LOGI(tmpStr);
}

//将2字节WCHAR*转换为4字节wchar_t
//53 0 56 0 4c 0 46 0 49 0 4c 0 45 0 0 0 =>
//53 0 0 0 56 0 0 0 4c 0 0 0 46 0 0 0 49 0 0 0 4c 0 0 0 45 0 0 0 0 0 0 0
wstring WCHARStrToWString(unsigned short * srcStr) {
	int srcLen = wlen(srcStr);
	int tarBufLen = (srcLen + 2) * 4;
	char* tarBuf  = new char[tarBufLen];
	memset(tarBuf, 0x0, tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_32LE, UTF_16LE, tarBuf,
			tarBufLen, (const char*) srcStr, srcLen * 2, &errCode);
	wstring tmpwstr = (wchar_t*) tarBuf;

	//log
//	LOGI("WCHARStr2wchat_tStr begin");
//	logCharHEX((char*) srcStr, (srcLen + 1) * 2); //src
//	logCharHEX((char*) tmpwstr.c_str(), (srcLen + 1) * 4);

//	string tmpstr = wstring2string(tmpwstr);
//	LOGI("WCHARStr2wchat_tStr: convert errcode:%d srclen:%d str:%s ", errCode, srcLen, tmpstr.c_str());
//
//	LOGI("WCHARStr2wchat_tStr end");
	delete tarBuf;
	return tmpwstr;
}

#ifdef __IOS__
int code_convert( const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen )
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf,0,outlen);
	cd = iconv_open(to_charset,from_charset);
	if(cd==0) return -1;
	if(iconv(cd,sloppy<char**>(pin),&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}
#endif

//使用icu库转换编码
int //返回完全转换需要的字节长度（byte）
UCNVConvert(const char* tarEncode, //目标编码
		const char* srcEncode, //原编码
		char* tarBuf, //目标buf
		int tarBufLen, //目标容量（byte）
		const char* srcBuf, //原buf
		int srcBufLen, //原容量（byte）
		int* pErrorCode //返回的错误码
		) {
	int ret = -1;

#ifdef WIN32

#endif

#ifdef __IOS__
	ret = code_convert(srcEncode,tarEncode,srcBuf,srcBufLen,tarBuf,tarBufLen);
#endif

#ifdef __ANDROID__
	//声明函数指针
	int (*ucnv_convert)(const char*, const char*, char*, int32_t, const char*,
			int32_t, int32_t*)=0;

	//加载动态库
	void* pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);

	//根据版本不同生成不同后缀的ucnv_convert函数
	//android2.2为ucnv_convert_4_2
	//2.3=44
	//4.0=46
	//
	char ver[50];
	__system_property_get("ro.build.version.sdk", ver);
	int verNo = atoi(ver);
	//LOGE("sys ver:%s",ver);
	char* convertFunName;
	if (verNo >= 28) //android 9.0 p =28;
				{
		convertFunName = "ucnv_convert_60";
	} else
	if (verNo >= 26) //android 8.0 N =26;
			{
		convertFunName = "ucnv_convert_58";
	} else if (verNo >= 24) //android 7.0 M =23;
			{
		convertFunName = "ucnv_convert_56";
	} else if (verNo >= 23) //android 6.0 M =23;
			{
		convertFunName = "ucnv_convert_55";
	} else if (verNo >= 21) //android 5.0 lollipop =21;android5.1 lollipop mr1 = 22
			{
		convertFunName = "ucnv_convert_53";
	} else if (verNo >= 19) //android 4.4 kitkat and 4.4w kitkat watch
			{
		convertFunName = "ucnv_convert_51";
	} else if (verNo >= 18) //jelly bean mr2 android 4.3
			{
		convertFunName = "ucnv_convert_50";
	} else if (verNo >= 16) //jelly bean android4.2
			{
		convertFunName = "ucnv_convert_48";
	} else if (verNo >= 14) //ice cream sandwich 4.0
			{
		convertFunName = "ucnv_convert_46";
	} else if (verNo >= 9) {
		convertFunName = "ucnv_convert_44";
	} else if (verNo >= 8) {
		convertFunName = "ucnv_convert_4_2";
	}

	ucnv_convert = (int (*)(const char*, const char*, char*, int32_t,
			const char*, int32_t, int32_t*))dlsym(pDL,convertFunName);

	//使用ucnv_convert函数进行转换字符串
if(	ucnv_convert) {
		ret = ucnv_convert(tarEncode,srcEncode,tarBuf,tarBufLen,srcBuf,srcBufLen,pErrorCode);
	}
	else
	{
		LOGE("Utility : ucnvConvert err: fun %s not found! verNo:%d",convertFunName,verNo);
	}
#endif
	return ret;
}

#ifdef __ANDROID__

string JStrToStr(JNIEnv* env, jstring jstr, const char* encoding) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF(encoding);
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	std::string stemp(rtn);
	free(rtn);
	return stemp;
}

////char* to jstring
//jstring strToJstr(JNIEnv* env, const char* pat)
//{
//	return strToJstr(env, pat, "utf-8");
//}

jstring StrToJStr(JNIEnv* env, const char* pat, const char* encoding) {
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray((jsize) strlen(pat));
	env->SetByteArrayRegion(bytes, 0, (jsize) strlen(pat), (jbyte*) pat);
	jstring encodingStr = env->NewStringUTF(encoding);

	jstring ret = (jstring) env->NewObject(strClass, ctorID, bytes,
			encodingStr);

	env->DeleteLocalRef(strClass);
	env->DeleteLocalRef(bytes);
	env->DeleteLocalRef(encodingStr);
	return ret;
}

jobject* GetJObject(JNIEnv *env, const char *path) {
	LOGI("Get JAVA Object :%s", path);
	jobject obj;
	jclass cls = env->FindClass(path);
	if (!cls) {
		LOGE("Can't find class");
		return &obj;
	}
	jmethodID constr = env->GetMethodID(cls, "<init>", "()V");
	if (!constr) {
		LOGE("init error");
		return &obj;
	}
	obj = env->NewObject(cls, constr);
	return &obj;
}

#endif

long GetTimeLong() {
#ifdef __MOBILE__
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long curTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return curTime;
#else
	return 0;
#endif
}

//创建多级目录
bool CreateMulityPath(const char* muldir) {
#ifdef MOBILE
	int i,len;
	char str[512];

	strncpy(str,muldir,512);
	len = strlen(str);
	for(int i=0;i<len;i++)
	{
		if(str[i]=='/'||str[i]=='\\')
		{
			char oldChar = str[i];
			str[i] = '\0';
			if(access(str,F_OK)!=0)
			{
				mkdir(str,777);
			}
			str[i] = oldChar;
		}
	}

	if(len>0 && access(str,F_OK)!=0)
	{
		mkdir(str,777);
	}
	return access(str,F_OK)==0;

#else
	return false;
#endif
}

//将顶点索引转换为顶点集合
void GetVertexBufByVertexAndIndex(vector<float>* outVertexBuf,
		vector<float>* outNormalBuf, vector<float> vertexArray,
		vector<unsigned short> indexArray) {
	for (int i = 0; i < indexArray.size(); i++) {
		outVertexBuf->push_back(vertexArray[indexArray[i] * 6]);
		outVertexBuf->push_back(vertexArray[indexArray[i] * 6 + 1]);
		outVertexBuf->push_back(vertexArray[indexArray[i] * 6 + 2]);
		outNormalBuf->push_back(vertexArray[indexArray[i] * 6 + 3]);
		outNormalBuf->push_back(vertexArray[indexArray[i] * 6 + 4]);
		outNormalBuf->push_back(vertexArray[indexArray[i] * 6 + 5]);
	}
}

//输出数组日志
void LogArray(void* pArray, int len, int typeSize, char* flag) {
//	void *p = pArray;
//	int tmpStrLen = len + 10;
//	string flagStr = "%s ";
//	flagStr += flag;
//
//	if(len>1000) len = 1000;
//
//	char tmpStr[tmpStrLen];
//	memset(tmpStr, 0x0, tmpStrLen);
//	for (int i = 0; i < len; i++) {
//		sprintf(tmpStr, flagStr.c_str(), tmpStr, p);
//		p= p + typeSize;
//	}
//	LOGI("%s",tmpStr);
}

void LogFloatArray(float* pArray, int len) {
	int strLen = 20 * len;
	char* tmpStr = new char[strLen];
	memset(tmpStr, 0x0, strLen);
	for (int i = 0; i < len; i++) {
		sprintf(tmpStr, "%s %f", tmpStr, pArray[i]);
	}
	LOGI("%s", tmpStr);

	delete tmpStr;
}

void LogUShortArray(unsigned short* pArray, int len) {
	int strLen = 20 * len;
	char* tmpStr = new char[strLen];
	memset(tmpStr, 0x0, strLen);
	for (int i = 0; i < len; i++) {
		sprintf(tmpStr, "%s %d", tmpStr, pArray[i]);
	}

	LOGI("%s", tmpStr);
	delete tmpStr;
}

void LogIntArray(unsigned int* pArray, int len) {
	char tmpStr[1000];
	memset(tmpStr, 0x0, 1000);
	for (int i = 0; i < len; i++) {
		sprintf(tmpStr, "%s %d", tmpStr, pArray[i]);
	}
	LOGI("%s", tmpStr);
}

