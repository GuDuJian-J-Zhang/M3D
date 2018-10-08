/**@file
 *@brief	ID产生类，能够产生唯一的ID
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_IDCREATER_H
#define M3D_IDCREATER_H
#include "m3d/M3D.h"
namespace M3D
{
/**@class
 * @brief 产生唯一ID使用，提供UUID，时间类型ID操作
 *
 *
 *
 */
class M3D_API IDCreator
{
public:
	IDCreator();
	virtual ~IDCreator();

	/**
	 * 获得默认类型的ID
	 * @return 默认类型ID
	 */
	static int GetDefaultID();

	/**
	 * 让type类型的ID初始化为从id开始自增
	 * @param type
	 * @param id
	 */
	void Initialize(int type, int id);
	/**
	 * 得到type类型的ID
	 * @param type
	 * @return
	 */
	int GetID(int type);

	/**
	 * 创建UUID
	 * @return
	 */
	static string GetUUID();
	/**
	 * 根据时间创建hash字符串
	 * @return
	 */
	static string GetIDFromTime();

public:
	/**
	 * 创建ID的类型
	 */
	const static int DEFAULT;
	const static int MODEL;
	const static int NODE;
	const static int SHAPE;
	const static int HANDLE;

private:
	map<int, int>::iterator m_currentType;
	map<int, int> m_IDMap;

	static IDCreator* m_pcreator;
};
}

#endif  /*M3D_IDCREATER_H*/
