/**@file
 *@brief	
 *@author	XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_IMAGE_HELPER_H_
#define M3D_IMAGE_HELPER_H_
#include <string>
#include <iostream>

using std::string;
using std::iostream;
namespace M3D{
class Image;
/**
 * @class TODO
 * @brief 贴图辅助类。提供加载、保存图片等函数
 */
class M3D_API ImageHelper
{
public:
	/**
	 * TODO
	 * @param path
	 * @return
	 */
	static Image* LoadImage(const string& path);
	/**
	 * TODO
	 * @param image
	 * @param format
	 * @return
	 */
	static bool SaveImage(Image* image,const string& format);
};
}
#endif /*M3D_IMAGE_HELPER_H_*/
