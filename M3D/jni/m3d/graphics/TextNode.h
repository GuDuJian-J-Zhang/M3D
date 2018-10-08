/**@file
 *@brief
 *@author	bing
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_TEXTNODE_H_
#define M3D_TEXTNODE_H_

#ifdef __MOBILE__
#include <FTGL/ftgles.h>
#endif

#include "m3d/scene/SceneNode.h"
#include "m3d/base/Color.h"

namespace M3D
{
/**
 * @class
 * @brief 文本节点
 */
class M3D_API TextNode: public SceneNode
{
public:
	TextNode();
	virtual ~TextNode();
	/**
	 *
	 * @return
	 */
	virtual int GetType(void);
	/**
	 *
	 * @param pText
	 */
	void SetText(char* pText);

protected:
//	virtual void FinalRelease(void); // Container.h

public:

	Vector3 m_Pos; //!<绘制位置
	char *m_pText;//!<文本内容
	Color m_color;//!<
	bool m_IsFixedSize; //!<固定大小
	bool m_IsFrontDisplay; //!<前端显示
	bool m_IsFaceToCamera; //!<面向相机
	bool m_IsFixedLocation; //!<固定位置

};

}

#endif /* M3D_TEXTNODE_H_ */
