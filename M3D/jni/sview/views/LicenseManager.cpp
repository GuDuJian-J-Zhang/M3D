/**@file
 *@brief	授权管理类cpp文件
 *@author   XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#include "sview/views/LicenseManager.h"

#include "m3d/base/json/json.h"
#include "sview/utils/SAESHelper.h"
namespace SVIEW
{
 LicenseManager* LicenseManager:: manager = NULL;
 int LicenseManager::keyState = -1;
 string LicenseManager::licenseKey ="";
LicenseManager::LicenseManager()
{
	 m_year = 1900;
	 m_month = 1;
	 m_day = 31;
}

LicenseManager* LicenseManager::GetLicenseManager()
{
	if(manager == NULL )
	{
		manager = new LicenseManager();
	}
	return manager;
}

void LicenseManager::SetLicense(const string& license)
{
	keyState = -1;
	licenseKey = license;
}

void LicenseManager::SetMacAddress(const vector<string>& macAddress)
{
	this->m_mac = macAddress;
}

string LicenseManager::GetEndDateTime(const string& license)
{
	licenseKey = license;
	if (CheckLicenese() == 0)
	{
		char info[255] = { 0 };
		sprintf(info, "%d/%d/%d", m_year, m_month, m_day);
		string strInfo = info;
		return strInfo;
	}
	else
	{
		return nullptr;
	}
}

int LicenseManager::CheckLicenese()
{
	if(licenseKey.length() > 5)
	{
		string inlicenseKey = licenseKey;
		string outlicenseKey;

		SVIEW::SAESHelper aesHelper = SVIEW::SAESHelper();
		aesHelper.aesUncrypt(inlicenseKey,outlicenseKey);

		Json::Reader reader;
		Json::Value readValue;

		if (reader.parse(outlicenseKey, readValue))
		{
			m_year = readValue["Y"].asInt();
			m_month = readValue["M"].asInt();
			m_day =  readValue["D"].asInt();

			string macAddress = readValue["Mac"].asString();

			//mac地址无效
			if (macAddress.length()>5 )
			{
				bool matchMac = false;
				for (int i =0;i<this->m_mac.size();i++)
				{
					if (macAddress == this->m_mac.at(i)) {
						matchMac = true;
					}
				}
				//如果没有匹配mac
				if (!matchMac) {
					m_year = 1901;
					m_month = 1;
					m_day = 31;
				}
			}

			LOGI("License year %d,month %d day %d",m_year,m_month,m_day);
		}
		else
		{
			m_year = 1901;
			m_month = 1;
			m_day = 31;
			//格式出错
			return -1;
		}
	}

	return 0;
}
    bool LicenseManager::ObtainAuthorization()
    {
        if(keyState == -1)
        {
            CheckLicenese();
            
            keyState = 0;
            bool licenseState = false;
            time(&now);
            timenow = localtime(&now);
            
            int tm_mday = timenow->tm_mday; /* day of the month */
            int tm_mon = timenow->tm_mon; /* month */
            int tm_year = timenow->tm_year; /* year */
            
            LOGI("now year %d mon %d day %d",tm_year,tm_mon,tm_mday);
            
            tm timeTarget;
            timeTarget.tm_year =  this->m_year -1900;
            timeTarget.tm_mon = this->m_month-1;
            timeTarget.tm_mday = this->m_day -0;
            timeTarget.tm_hour = 0;
            timeTarget.tm_min = 0;
            timeTarget.tm_sec = 0;
            
            time_t target = mktime(&timeTarget);
            int diff = difftime(target,now);
            
            keyState = diff>0 ?1:-1;
        }
        
        return keyState == 1;
        //return true;
    }

}

