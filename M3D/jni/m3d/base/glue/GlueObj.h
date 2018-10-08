/**
 *@file
 *@brief 粘合层，通过此类来简介调用java层和ios层相关方法
 *@author	XL
 *@date		2015-8-24
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_GLUEOBJ_H
#define M3D_GLUEOBJ_H

#include "m3d/M3D.h"

#ifdef __ANDROID__
#include <jni.h>
	typedef JNIEnv GlueEnv ;
	typedef jobject Glueobject ;
#endif

#ifdef __IOS__
	typedef int GlueEnv ;
	typedef int Glueobject ;
#endif

#ifdef _WIN32
	typedef int GlueEnv ;
	typedef int Glueobject ;
#endif
	/**
	 * 构造图片回调函数
	 * @param
	 * @param
	 * @return
	 */
#ifdef _WIN32
	typedef bool(__stdcall*CREATEIMAGE)(void*, const char*);
#else
    typedef bool (*CREATEIMAGE)(void*,const char*);
#endif
namespace M3D
{
class SceneManager;
class Shape2DSet;
class M3D_API GlueObj
{
public:
	/**
	 *构造函数
	 */
	GlueObj();
	/**
	 *析构函数
	 */
	GlueEnv* GetGlueEnv();

	/**
	 * 设置粘合层环境变量
	 * @param env
	 */
	void SetGlueEnv(GlueEnv * env);
	/**
	 * 返回粘合层对象
	 * @return 粘合层对象
	 */
	Glueobject GetGluObject();
	/**
	 * 设置粘合层对象
	 * @param obj 粘合层对象
	 */
	void SetGluObject(Glueobject obj);
	/**
	 * 通过2d对象构建二维图片
	 * @param shapeSet
	 * @param scene
	 * @return
	 */
	string createImage(Shape2DSet* shapeSet,SceneManager* scene);
	/**
	 * 创建二维图片回调函数
	 * @param fun
	 */
    void SetCreateImageFun(CREATEIMAGE fun);
    /**
     * 获取创建二维图片的回掉函数
     * @return 创建二维图片的回掉函数 有JNI层 或者iOS OC端等上层实现
     */
    CREATEIMAGE GetCreateImageFun();

private:
    /**
     * 初始化
     */
	void Init();

private:
    CREATEIMAGE createImageFun; //!<粘合层回调函数
	GlueEnv * m_env;//!< 粘合层环境上下文
	Glueobject m_obj;//!<粘合层对象

};

}
#endif //M3D_GLUEOBJ_H
