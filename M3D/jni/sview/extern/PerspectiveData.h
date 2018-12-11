/**@file
 *@brief	几种基本视图相关常量的定义
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef PERSPECTIVE_H_
#define PERSPECTIVE_H_
#include "m3d/M3D.h"
namespace M3D
{
class ControlInfo;
/**
 * @class
 * @brief 主要定义基本视图用到的相关常量
 *
 */
class M3D_API PerspectiveData
{
public:
	const static int FRONT = 0;
	const static int BACK = 1;

	const static int LEFT = 2;
	const static int RIGHT = 3;

	const static int TOP = 4;
	const static int BOTTOM = 5;

	const static int ISOMETRIC = 6;

	const static int DEFAULT = 7;

	const static int BACKISOME = 8;

	const static int CUSTOM = 9;

	const static int UPFRONTLEFT = 10;   //上前左轴测图
	const static int UPBEHINDRIGHT = 11;  //上后右轴测图
	const static int UPBEHINDLEFT = 12; //上后左轴测图

	const static int DOWNFRONTRIGHT = 13; //下前右轴测图
	const static int DOWNFRONTLEFT = 14; //下前左轴测图
	const static int DOWNBEHINDRIGHT = 15; //下后右轴测图

	const static int  UPBEHIND = 16;   //上后测图
	const static int  UPFRONT = 17;   //上前测图
	const static int  UPLEFT = 18;   //上左测图
	const static int  UPRIGHT = 19;   //上右测图
	const static int  DOWNBEHIND = 20;   //下后测图
	const static int  DOWNFRONT = 21;   //下前测图
	const static int  DOWNLEFT = 22;   //下左测图
	const static int  DOWNRIGHT = 23;   //下右测图
	const static int  FRONTLEFT = 24;   //前左测图
	const static int  RONTRIGHT = 25;   //前右测图
	const static int  BEHINDLEFT = 26;   //后左测图
	const static int  BEHINDRIGHT = 27;   //后右测图

	const static ControlInfo FRONT_MVMATRIX;
	const static ControlInfo BACK_MVMATRIX;

	const static ControlInfo LEFT_MVMATRIX;
	const static ControlInfo RIGHT_MVMATRIX;

	const static ControlInfo TOP_MVMATRIX;
	const static ControlInfo BOTTOM_MVMATRIX;

	const static ControlInfo ISOMETRIC_MVMATRIX;

	const static ControlInfo DEFAULT_MVMATRIX;

	const static ControlInfo BACKISOME_MVMATRIX;  //后轴测图
	const static ControlInfo UPFRONTLEFT_MVMATRIX;   //上前左轴测图
	const static ControlInfo UPBEHINDRIGHT_MVMATRIX;   //上后右轴测图
	const static ControlInfo UPBEHINDLEFT_MVMATRIX;   //上后左轴测图

	const static ControlInfo DOWNFRONTRIGHT_MVMATRIX;   //下前右轴测图
	const static ControlInfo DOWNFRONTLEFT_MVMATRIX;   //下前左轴测图
	const static ControlInfo DOWNBEHINDRIGHT_MVMATRIX;   //下后右轴测图

	const static ControlInfo UPBEHIND_MVMATRIX;   //上后测图
	const static ControlInfo UPFRONT_MVMATRIX;   //上前测图
	const static ControlInfo UPLEFT_MVMATRIX;   //上左测图
	const static ControlInfo UPRIGHT_MVMATRIX;   //上右测图
	const static ControlInfo DOWNBEHIND_MVMATRIX;   //下后测图
	const static ControlInfo DOWNFRONT_MVMATRIX;   //下前右轴测图
	const static ControlInfo DOWNLEFT_MVMATRIX;   //下左测图
	const static ControlInfo DOWNRIGHT_MVMATRIX;   //下右测图
	const static ControlInfo FRONTLEFT_MVMATRIX;   //前左测图
	const static ControlInfo RONTRIGHT_MVMATRIX;   //前右测图
	const static ControlInfo BEHINDLEFT_MVMATRIX;   //后左测图
	const static ControlInfo BEHINDRIGHT_MVMATRIX;   //后右测图

private:
	const static float DISTRI; //扰动,防止矩阵不可逆

};
}

#endif //PERSPECTIVE_H_
