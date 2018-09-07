/**@file
 *@brief
 *@author	CDJ
 *@date		2015-12-14
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef SEQUENCENUMBERNOTE_H_
#define SEQUENCENUMBERNOTE_H_
#include "m3d/model/Note.h"
namespace M3D
{

/**
 * @class 序号批注类
 */
class M3D_API SequenceNumberNote:public Note
{
public:
	SequenceNumberNote();
	virtual ~SequenceNumberNote();
	/**
	 * @brief 获取批注位置
	 * @return 返回位置
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
    /**
     * @brief 将对象值转成JSON数据
     * @param pos 位置
     */
    string toJson();
    SequenceNumberNote* fromJson(string value);
private:
	/**
	 * @brief 初始化
	 */
	void Init();
	Vector3 m_notePos;//!<批注位置

	Vector3 m_textPos;//!<文字显示位置

	string m_textValue;//!<文本内容
};

} /* namespace M3D */
#endif /* SEQUENCENUMBERNOTE_H_ */
