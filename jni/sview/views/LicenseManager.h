/**@file
 *@brief	授权管理
 *@author   XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 * 提供授权控制
 *
 */

#ifndef M3D_LICENSEMANAGER_H_
#define M3D_LICENSEMANAGER_H_
#include "m3d/M3D.h"

#include<time.h>
#include<stdio.h>

namespace SVIEW
{
/**
 * @class
 * @brief 授权管理类
 */
class M3D_API LicenseManager
{
public:
	/**
	 * 获取授权管理器
	 * @return
	 */
	static LicenseManager* GetLicenseManager();
	/**
	 * 获取授权
	 * @return
	 */
	bool ObtainAuthorization();
	/**
	 * 设置license
	 * @param license
	 */
	void SetLicense(const string& license);

	void SetMacAddress(const vector<string>& macAddress);

	string GetEndDateTime(const string& license);

private:
	LicenseManager();

	/**
	 * 检查licensee
	 * @return
	 */
	int CheckLicenese();

private:
	time_t now;//!<当前时间
	int m_year;//!<年
	int m_month;//!<月
	int m_day;//!<日

	vector<string> m_mac;

	struct tm * timenow;

	static string licenseKey;//!<

	static LicenseManager* manager;//!<
	static int keyState;//!<
	const static int KEYSTATEOK = 1;//!<
	const static int KEYSTATEUNVALUE = 2;//!<
};

}

#endif /*M3D_LICENSEMANAGER_H_*/
