//#include "..\..\StdAfx.h"
#include "DesUtil.h"
#include "resource.h"

CDesUtil::CDesUtil(void)
{
}

CDesUtil::~CDesUtil(void)
{
}

//加密文件算法涵数
BOOL CDesUtil::DesEncryptFile(CString filename,const char* password)
{
	BOOL bRetVal = FALSE;
	SetFileAttributes(filename,FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_NORMAL);
	CFile m_file;
	if(m_file.Open(filename,CFile::modeReadWrite))
	{
		int length = (int)m_file.GetLength();
		char * m_buffer,*m_outBuffer;
		int m_outBufferLength;
		if (length%8!=0)
		{
			m_outBufferLength = (length/8 + 1)*8;
		}
		else m_outBufferLength = length;
		m_buffer = new char[length+1]();
		m_outBuffer = new char[m_outBufferLength+1]();
		m_file.Read(m_buffer,(UINT)length);
		BOOL desResult = DesBytesEncrypt(m_outBuffer,m_outBufferLength,m_buffer,length,password);
		if (desResult==FALSE)
		{
			bRetVal = FALSE;
		}
		else
		{
			m_file.SeekToBegin();
			m_file.Write(m_outBuffer,(UINT)m_outBufferLength);
			bRetVal = TRUE;
		}

		m_file.Close();
		delete[] m_buffer;
		delete[] m_outBuffer;
	}
	return bRetVal;
}

//解密文件算法涵数
BOOL CDesUtil::DesDecryptFile(CString filename,const char* password)
{
	BOOL bRetVal = FALSE;

	SetFileAttributes(filename,FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_NORMAL);
	CFile m_file;
	if(m_file.Open(filename,CFile::modeReadWrite))
	{
		int length = (int)m_file.GetLength();
		char * m_buffer,*m_outBuffer;
		int m_outBufferLength;
		if (length%8!=0)
		{
			m_outBufferLength = (length/8 + 1)*8;
		}
		else m_outBufferLength = length;
		m_buffer = new char[length+1]();
		m_outBuffer = new char[m_outBufferLength+1]();
		m_file.Read(m_buffer,(UINT)length);
		BOOL desResult = DesBytesDecrypt(m_outBuffer,m_outBufferLength,m_buffer,length,password);
		if (desResult==FALSE)
		{
			bRetVal = FALSE;
		}
		else
		{
			m_file.SeekToBegin();
			m_file.Write(m_outBuffer,(UINT)m_outBufferLength);
			bRetVal = TRUE;
		}
		m_file.Close();
		delete[] m_buffer;
		delete[] m_outBuffer;
	}
	return bRetVal;
}

BOOL CDesUtil::DesBytesEncrypt(char *Out, long OutSize,char *In,long InSize,const char *Key)
{
	//	long InLength=strlen(In);
	//	long c=InLength/8;	//文件长度中8字节的倍数
	//	long d=InLength%8;	//文件长度中8字节的余数

	long c=InSize/8;	//文件长度中8字节的倍数
	long d=InSize%8;	//文件长度中8字节的余数

	
	if (OutSize%8!=0)
	{
		return FALSE;
	}
	//char *OutStart = Out;
	char inbuff[8],oubuff[8],skey[8];
	memcpy(skey,Key,8);//如果密码不为8，会不会出错，如果出错，就要在前面把skey的数组赋为0.

	for(long i=0;i<c;i++)
	{
		//ff.Read(inbuff,8);
		for (int i = 0;i<8;i++)
		{
			inbuff[i] = *In;
			In++;
		}
		BOOL desResult = Des_Go(oubuff, inbuff, sizeof(inbuff), skey,sizeof(skey), ENCRYPT);
		if (desResult==FALSE)
		{
			return FALSE;
		}
		for (int i = 0; i<8; i ++)
		{
			*Out = oubuff[i];
			Out++;
		}		
	}
	if(d>0)
	{
		char temp[8];
		memset(temp,0,8);
		for (int i = 0;i<d;i++)
		{
			temp[i] = *In;
			In++;
		}
		BOOL desResult = Des_Go(oubuff, temp, sizeof(temp), skey,sizeof(skey), ENCRYPT);
		if (desResult==FALSE)
		{
			return FALSE;
		}
		for (int i = 0; i< 8; i ++)
		{
			*Out = oubuff[i];
			Out++;
		}
	}
	//Out = OutStart;
	Out = Out - OutSize;
	return TRUE;
}

BOOL CDesUtil::DesBytesDecrypt(char *Out, long OutSize,char *In, long InSize,const char *Key)
{
	long c=InSize/8;	//文件长度中8字节的倍数
	long d=InSize%8;	//文件长度中8字节的余数


	if (OutSize%8!=0)
	{
		return FALSE;
	}

	char inbuff[9] = {},oubuff[9]={},skey[9]={};
	memcpy(skey,Key,8);//如果密码不为8，会不会出错，如果出错，就要在前面把skey的数组赋为0.

	for(long j=0;j<c;j++)
	{
		memcpy(inbuff,In,8);
		In+=8;
		BOOL desResult = Des_Go(oubuff, inbuff, 8, skey,8, DECRYPT);
		if (desResult==FALSE)
		{
			return FALSE;
		}
		memcpy(Out,oubuff,8);
		Out+=8;
	}
	if(d>0)
	{
		char temp[9] = {};
		for (int i = 0;i<d;i++)
		{
			temp[i] = *In;
			In++;
		}
		BOOL desResult = Des_Go(oubuff, temp, 8, skey,8, DECRYPT);
		if (desResult==FALSE)
		{
			return FALSE;
		}
		for (int i = 0; i< 8; i ++)
		{
			*Out = oubuff[i];
			Out++;
		}
	}
	Out = Out - OutSize;
	return TRUE;
}  

#if 0
void CDesUtil::DesEncryptString(CString &inputString ,CString &outputString, const char key[9])
{
	//上面生成了一个字符串，下面进行加密
	int length = inputString.GetLength();
	char *input = new char[length+1];
	memset(input,0,length+1);
	strcpy_s(input,length+1,inputString);

	int outLength = 0; 
	char* output = NULL;
	if((length%8)!= 0)
	{
		outLength = (length/8 +1) * 8;
	}
	else
	{
		outLength = length;
	}
	output = new char[outLength +1];
	memset(output,0,outLength +1);
	DesBytesEncrypt(output,outLength,input,length,key);

	//把加密后的数据以16进制显示
	CString str16bit =CStringToHex(output,outLength);
	delete[] output;
	delete[] input;
	outputString = str16bit;
}
#endif

VOID CDesUtil::DesDecryptString(string &input, string &output,const char key[9])
{
	int inputLength = input.length();
    int afterDesLen = inputLength/2 ;
	char* afterDes = new char[afterDesLen+1];
	memset(afterDes,0,afterDesLen+1);
	HexToCString(afterDes,input);
	char* desOut = new  char[afterDesLen +1];
	memset(desOut,0,afterDesLen+1);
    DesBytesDecrypt(desOut,afterDesLen,afterDes,afterDesLen,key);
	output = desOut;
	delete[] desOut;
	delete[] afterDes;
}

CString CDesUtil::CStringToHex(char input[],int length)
{
 	char* charStr = new char[length+1];
	memset(charStr,0,length+1);
	memcpy(charStr,input,length);
	char szChar[3];
	CString result;
	for(int i=0;i<length; i++)
	{
		_itoa_s((unsigned char)charStr[i],szChar,16);
		if(strlen(szChar) == 1)
			result+="0";
		result += szChar;
	}
	result.MakeUpper();
	delete[] charStr;
	return result;
}

VOID CDesUtil::HexToCString(char output[],string& hexStr)
{
		int length = hexStr.length();
		char* changeChar = new char[length+1];
		char* outChar = new char[length/2+1];
		memset(changeChar,0,length+1);
		strcpy_s(changeChar,length+1,hexStr.c_str());
		memset(outChar,0,length/2+1);
		unsigned short first,second;
		unsigned char resultChar,tempChar;
		for (int i=0;i<length;i = i+2)
		{
			if(changeChar[i]>='0'&&changeChar[i]<='9')
				first = changeChar[i]-'0';	
			else
				first = changeChar[i] -'A'+10;
			if(changeChar[i+1]>='0'&&changeChar[i+1]<='9')
				second = changeChar[i+1]-'0';
			else
				second = changeChar[i+1]-'A'+10;
			tempChar = (unsigned char)first;
			tempChar = tempChar<<4;
			resultChar = (unsigned char)second;
			resultChar = resultChar^tempChar;
			outChar[i/2] = resultChar;
		}
		memcpy(output,outChar,length/2);
		delete [] changeChar;
		delete [] outChar;		
}


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Code starts from Line 130
//////////////////////////////////////////////////////////////////////////

void CDesUtil::SetKey(const char* Key, int len)
{
	memset(deskey, 0, 16);
	memcpy(deskey, Key, len>16?16:len);
	SetSubKey(&SubKey[0], &deskey[0]);
	Is3DES = len>8 ? (SetSubKey(&SubKey[1], &deskey[8]), true) : false;
}
void CDesUtil::SDES(char Out[8], char In[8], const PSubKey pSubKey, bool Type)
{
	//static bool M[64], tmp[32], *Li=&M[0], *Ri=&M[32];
	bool M[64], tmp[32], *Li=&M[0], *Ri=&M[32];
	ByteToBit(M, In, 64);
	Transform(M, M, IP_Table, 64);
	if( Type == ENCRYPT ){
		for(int i=0; i<16; ++i) {
			memcpy(tmp, Ri, 32);
			F_func(Ri, (*pSubKey)[i]);
			Xor(Ri, Li, 32);
			memcpy(Li, tmp, 32);
		}
	}else{
		for(int i=15; i>=0; --i) {
			memcpy(tmp, Li, 32);
			F_func(Li, (*pSubKey)[i]);
			Xor(Li, Ri, 32);
			memcpy(Ri, tmp, 32);
		}
	}
	Transform(M, M, IPR_Table, 64);
	BitToByte(Out, M, 64);
}
void CDesUtil::SetSubKey(PSubKey pSubKey, const char Key[8])
{
	/*static bool K[64], *KL=&K[0], *KR=&K[28];*/
	bool K[64], *KL=&K[0], *KR=&K[28];
	ByteToBit(K, Key, 64);
	Transform(K, K, PC1_Table, 56);
	for(int i=0; i<16; ++i) {
		RotateL(KL, 28, LOOP_Table[i]);
		RotateL(KR, 28, LOOP_Table[i]);
		Transform((*pSubKey)[i], K, PC2_Table, 48);
	}
}
void CDesUtil::F_func(bool In[32], const bool Ki[48])
{
	/*static bool MR[48];*/
	bool MR[48];
	Transform(MR, In, E_Table, 48);
	Xor(MR, Ki, 48);
	S_func(In, MR);
	Transform(In, In, P_Table, 32);
}
void CDesUtil::S_func(bool Out[32], const bool In[48])
{
	for(char i=0,j,k; i<8; ++i,In+=6,Out+=4) {
		j = (In[0]<<1) + In[5];
		k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
		ByteToBit(Out, &S_Box[i][j][k], 4);
	}
}
void CDesUtil::Transform(bool *Out, bool *In, const char *Table, int len)
{
	for(int i=0; i<len; ++i)
		Tmp[i] = In[ Table[i]-1 ];
	memcpy(Out, Tmp, len);
}
void CDesUtil::Xor(bool *InA, const bool *InB, int len)
{
	for(int i=0; i<len; ++i)
		InA[i] ^= InB[i];
}
void CDesUtil::RotateL(bool *In, int len, int loop)
{
	memcpy(Tmp, In, loop);
	memcpy(In, In+loop, len-loop);
	memcpy(In+len-loop, Tmp, loop);
}
void CDesUtil::ByteToBit(bool *Out, const char *In, int bits)
{
	for(int i=0; i<bits; ++i)
		Out[i] = (In[i>>3]>>(i&7)) & 1;
}
void CDesUtil::BitToByte(char *Out, const bool *In, int bits)
{
	memset(Out, 0, bits>>3);
	for(int i=0; i<bits; ++i)
		Out[i>>3] |= In[i]<<(i&7);
}

bool CDesUtil::Des_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type)
{
	if( !( Out && In && Key && (datalen=(datalen+7)&0xfffffff8) ) ) 
		return false;
	SetKey(Key, keylen);
	if( !Is3DES ) {   // 1次DES
		for(long i=0,j=datalen>>3; i<j; ++i,Out+=8,In+=8)
			SDES(Out, In, &SubKey[0], Type);
	} else{   // 3次DES 加密:加(key0)-解(key1)-加(key0) 解密::解(key0)-加(key1)-解(key0)
		for(long i=0,j=datalen>>3; i<j; ++i,Out+=8,In+=8) {
			SDES(Out, In,  &SubKey[0], Type);
			SDES(Out, Out, &SubKey[1], !Type);
			SDES(Out, Out, &SubKey[0], Type);
		}
	}
	return true;
}
