#ifndef DATA_PROCESS_HEAD_FILE
#define DATA_PROCESS_HEAD_FILE

#include <string>
using namespace std;

#define	MAC_LEN		3
#define	DATE_LEN	5
#define	VER_LEN		16

// ע����
struct RegisterCode
{
	int iProFlag;
	int iProEdition;
	char szProVer[VER_LEN];
	char szSpaceDate[DATE_LEN];
	char szMac1[MAC_LEN];
	char szMac2[MAC_LEN];
};

//////////////////////////////////////////////////////////////////////////

//���ݴ�����
class CDataProcess
{
public:
	CDataProcess();
	~CDataProcess();

	//��Ա����
private:

	// ��Ա����
public:
	bool IsDigitStr(string strContent);
	bool RestoreCode(string strRegCode, RegisterCode& RegCode);
	string DigitDecrypt(string strIn);
	string GetProperTime(string strSpacetime);
	int GetSpaceDate(string strTime);
};

#endif