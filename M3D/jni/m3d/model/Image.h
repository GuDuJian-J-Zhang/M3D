/**@file
 *@brief	自定义贴图文件格式，从APK压缩包中读取PNG格式图片
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_MIMAGE_H
#define M3D_MIMAGE_H
#include "m3d/M3D.h"

namespace M3D
{
/**
 * @class
 * @brief 从APK压缩包中，提取PNG格式文件，解码成GL使用格式
 */
class M3D_API Image
{
public:
	Image();
	virtual ~Image();

	/**
	 * 读取指定路径的图片
	 * @param fileName
	 */
	void Read(const string& fileName);

	/**
	 * 通过数据流初始化图片
	 * @param data
	 */
	void SetData(const void* data);
	/**
	 * 纪录下数据的开始位置和对应长度
	 * @param data
	 * @param dataLength
	 */
	void SetData(unsigned char* data, int dataLength);

public:
	bool m_bAlpha;//!< 图片是否有透明度
	int m_iWidth;//!< 宽度
	int m_iHeight;//!<高度
	int m_dataLength;//!<数据长度
	unsigned char* m_pPixelData;//!< 数据地址
};
}

#endif /*MIMAGE_H*/
