/**@file
 *@brief    存储交互层的所有Handler，M3D为编程简便
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef	M3D_NOTE_GROUP_H_
#define M3D_NOTE_GROUP_H_

#include "m3d/scene/GroupNode.h"
#include "m3d/M3D.h"
namespace M3D
{
class CameraNode;
class HandlerNode;
class SceneManager;

/**
 * @class 批注组类，用来临时存放批注对象
 */
class M3D_API NoteGroup: public GroupNode
{
public:
    const static int ANNOTATION_TYPE_BASE ; //!<文本批注
    const static int ANNOTATION_TYPE_COMPONENTNAME; //!<零组件批注
    const static int ANNOTATION_TYPE_VOICE; //!<语音批注
    const static int ANNOTATION_TYPE_SEQUENCE;//!<序号批注
    const static int ANNOTATION_TYPE_GESTURE;//3D手势批注
    
	NoteGroup();
	virtual ~NoteGroup();
	/**
	 * @brief 获取类型
	 * @return 批注类型
	 */
	virtual int GetType(void);
	/**
	 * @brief 初始化
	 */
	void Initialize();
	/**
	 * @brief 清空
	 */
	void Clear();

};
}
#endif /*M3D_NOTE_GROUP_H_*/
