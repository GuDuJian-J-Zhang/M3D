/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_COMMAND_H
#define     M3D_COMMAND_H
#include 	"m3d/M3D.h"
#include "Sview/views/view.h"
#include "m3d/utils/PathHelper.h"

using SVIEW::View;


namespace M3D
{
/**
 *@class
 *@brief 操作类
 */
class M3D_API Operation:public Object
{
public:
	static const int TYPE;//!<类型
public:
	Operation();
	virtual ~Operation();

	/**
	 * @brief 获取类型
	 * @return 返回类型
	 */
	int GetType();
	/**
	 * @brief 执行操作
	 * @return 成功true 失败false
	 */
    bool execute();

    /**
     * @brief 执行
     * @return 成功true 失败 false
     */
	virtual bool OnExecute();

	//TODO
	virtual bool Undo();
	//TODO
	virtual bool Redo();

	/**
	 * @brief 转化为Json字符串
	 * @return Json字符串
	 */
	virtual string ToJsonString();

	/**
	 * @brief 获取当前视图
	 * @return 返回当前视图
	 */
	virtual View * GetView();

	/**
	 * @brief 设置当前视图
	 * @param view 视图
	 */
	virtual void SetView(View * view);
protected:
	View * m_view;//!<当前视图
    int m_commandType; //!<命令类型
};

}
#endif

