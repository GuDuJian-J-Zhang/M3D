/**@file
 *@brief	VoiceAnnovation类头文件
 *@author
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *VoiceAnnovation类头文件
 *
 */

#ifndef  M3D_VOICE_NOTE_H
#define M3D_VOICE_NOTE_H
#include "m3d/model/Note.h"

namespace M3D
{

class ImageBoard;
class Image;
class Texture;
class Billboard;

/**@class VoiceAnnovation
 * @brief VoiceAnnovation类
 *
 * 语音批注标识图形类
 *
 */
class M3D_API VoiceNote: public Note
{
//public:
//	static  const string VoiceAnnotationImage;
//	static Texture* defaultImage;
//
//	static Texture* GetDefaultVoiceImage();
public:
	VoiceNote(SceneManager* scene);
	VoiceNote(const Vector3& vec , SceneManager* scene);

	VoiceNote(const Vector3& pos, const Vector2& size ,SceneManager* scene);

	virtual ~VoiceNote();

	/**
	 * @brief 设置批注位置大小
	 * @param pos 位置
	 * @param size 大小
	 */
	void Set(const Vector3& pos, const Vector2& size);

	/**
	 * @brief 设置选择状态
	 * @param select 选择状态
	 */
	virtual void SetSelected(bool select);

	/**
	 * @brief 计算包围盒
	 */
	virtual void ComputeBox();

	/**
	 * @brief 拾取动
	 * @param action 拾取动作参数
	 */
	virtual void RayPick(RayPickAction* action);

	/**
	 * @brief 设置可见性相关参数，虚函数 @see Renderable定义
	 * @param renderAction 渲染动作
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * @brief 设置位置
	 * @param position 位置参数
	 */
	void SetPosition(const Vector3& position);

	/**
	 * @brief 获取位置
	 * @return 返回位置
	 */
	Vector3& GetPosition();

	/**
	 * @brief 获取批注图片
	 * @return 返回图片
	 */
	ImageBoard* GetImageboard();

	/**
	 * @brief 设置uri
	 * @param uri
	 */
	void SetUri(string &uri){m_uri = uri;}
	/**
	 * @brief 获取uri
	 * @return 返回URI
	 */

	string& GetUri(){return m_uri;}

	/**
	 * @brief 设置格式
	 * @param format 格式参数
	 */
	void SetFormat(string &format){m_format = format;}
	/**
	 * @brief 获取参数
	 * @return 返回格式
	 */
	string& GetFormat(){return m_format;}

	/**
	 * @brief 设置语音数据
	 * @param voiceData 语音数据
	 */

	void SetVoiceData(string &voiceData){m_voiceData = voiceData;}
	/**
	 * @brief 获取语音数据
	 * @return 返回语音
	 */
	string& GetVoiceData(){return m_voiceData;}

private:
	/**
	 * @brief 初始化
	 */
	void Init(SceneManager* scene);

public:
	ImageBoard* m_imageboard;//!<生成的批注图片

	Texture* m_TexImage;//!<纹理图片
private:
	Vector3 m_position;//!<位置
	string m_uri;//!<uri
	string m_format;//!<格式
	string m_voiceData;//!<语音数据

};
}
#endif /*M3D_VOICE_NOTE_H*/
