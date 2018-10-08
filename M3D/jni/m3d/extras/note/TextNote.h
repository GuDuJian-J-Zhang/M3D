/**@file
 *@brief	测量note类头文件
 *@author   xuli
 *@date		2013-7-10
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Face类头文件
 *
 */

#ifndef M3D_ANNOTATION_H_
#define M3D_ANNOTATION_H_

#include "m3d/model/Note.h"

namespace M3D
{

/**
 * @class 文本批注类
 */
class M3D_API TextNote: public Note
{
public:
	TextNote();
	virtual ~TextNote();

	/**
	 * @brief 获取批注位置
	 * @return
	 */
	Vector3& GetNotePos();
	/**
	 * @brief 设置批注位置
	 * @param pos 要设置的位置
	 */
	void SetNotePos(const Vector3& pos);
	/**
	 * @brief 获取文本位置
	 * @return 位置
	 */
	Vector3& GetTextsPos();
	/**
	 * @brief 设置文本位置
	 * @param pos 位置
	 */
	void SetTextsPos(const Vector3& pos);
private:
	/**
	 * @brief 初始化
	 */
	void Init();

	Vector3 m_notePos;//!<批注位置

	Vector3 m_textPos;//!<文字显示位置

	string m_textValue;//!<文本内容
};

}

#endif /*M3D_ANNOTATION_H_*/

