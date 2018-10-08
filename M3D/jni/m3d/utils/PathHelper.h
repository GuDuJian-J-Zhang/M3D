/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_PATH_HELPER_H_
#define  M3D_PATH_HELPER_H_
#include <string>
#include <iostream>
#include "m3d/m3d.h"
using std::string;
using std::iostream;
namespace M3D{
class Model;
/**
 * @class
 * @brief 路径辅助类。提供路径转换工具函数
 */
class M3D_API PathHelper
{
public:
	/**
	 * 得到SVL格式的路径
	 * @param model
	 * @return
	 */
	static string GetSVLPath(Model* model);

	/**
	 * 得到M3D内部路径
	 * @param model
	 * @return
	 */
	static string GetM3DPath(Model* model);

	/**
	 * 将M3D的十六进制路径，转换为SVL的10进制路径
	 *
	 * @param m3dHexPath
	 * @return 转换成功的路径
	 */
	static string M3DHexPathToSVLDec(const string& m3dHexPath);

	/**
	 * 将SVL的十进制路径转换为M3D的16进制路径
	 * @param svlDecPath
	 * @return 转换成功的路径
	 */
	static string SVLDecToM3DHexPath(const string& svlDecPath);

	/**
	 * svl 格式 0|1|A|FF
	 * svl格式配置路径 16进制至10进制
	 * @param hexPath
	 * @param decPath
	 */
	static string SVLPathHexToDec(const string& hexPath);

	/**
	 * svl格式配置路径 10进制至16进制
	 * @param decPPath
	 * @param hecPath
	 */
	static string SVLPathDecToHex(const string& decPPath);

	/**
	 * svl 10进制格式的路径至M3D格式的路径
	 * svl格式  格式 0|1|2|3
	 * M3D格式  格式 0|1|2|3
	 * @param svlPath
	 * @param m3dPath
	 */
	static string SVLPathToM3D(const string& svlPath);

	/**
	 * 将M3D路径转换成SVL格式路径
	 * @param m3dPath
	 * @param svlPath
	 */
	static string M3DPathToSVLPath(const string& m3dPath);

	/**
	 * 将M3D路径转换成动画格式路径
	 * @param m3dPath
	 * @param animationPath
	 */
	static string M3DPathToAnimation(const string& m3dPath);

	/**
	 * 将动画格式路径转换成M3D路径
	 * @param animationPath
	 * @param m3dPath
	 */
	static string AnimationPathToM3D(const string& animationPath);

	/**
	 * 得到当前Path所代表的Node 所在装配的级别
	 * @param m3dPath
	 * @return 装配级别
	 */
	static int GetPathLevel(const string& m3dPath);

public:
	const static  string M3D_PATH_PRE;
};
}
#endif /*M3D_PATH_HELPER_H_*/
