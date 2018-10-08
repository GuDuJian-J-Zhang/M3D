/**@file
 *@brief
 *@author	CDJ
 *@date		2016-3-24
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_HARDWARE_FRAMEBUFFER_H_
#define M3D_HARDWARE_FRAMEBUFFER_H_
#include"m3d/M3D.h"
#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/gl20/GLRenderTarget.h"
#include "m3d/graphics/GPUObject.h"
#include "m3d/graphics/GeometryBuffer.h"
namespace M3D
{

/**
 * @class
 * @brief 帧缓存对象类。用来实现高级效果
 */
class M3D_API HardWareFrameBuffer: public GPUObject
{
public:
	HardWareFrameBuffer();
	HardWareFrameBuffer(int width ,int height);
	/**
	 * @brief 初始化
	 */
	void Init();
	/**
	 * 获取FBO
	 * @return
	 */
	GLuint GetFBO();
	virtual ~HardWareFrameBuffer();
	/**
	 * @brief 创建深度挂载
	 * @param useRBO
	 * @return
	 */
	bool CreateDepthAttachment(bool useRBO);
	/**
	 * 绑定颜色挂载点
	 * @param index
	 * @return
	 */
	bool LinkTextureColorAttachment(int index);

	void SetColorAttachmentAttribute(int index,bool useMutiSample,bool useRBO);
	/**
	 * 设置大小
	 * @param width
	 * @param height
	 */
	void SetSize(int width,int height);
	/**
	 * 获取宽度
	 * @return
	 */
	int GetWidth();
	/**
	 * 获取高度
	 * @return
	 */
	int GetHeight();
	/**
	 * 设置最大颜色挂载数量
	 * @param maxNum
	 */
	void SetMaxColorAttachmentNumber(int maxNum);
	/**
	 * @brief 绑定FBO
	 */
	void Bind();
	/**
	 * @brief 解绑FBO
	 */
	void UnBind();
	/**
	 * @brief 检查FBO状态
	 * @return
	 */
	bool CheckStatus();

	/**
	 * @brief 保存当前的FBO
	 */
	void KeepOriginalFBO();

	/**
	 * @brief 在解绑当前的FBO使用后，恢复到原来的（主要用于IOS系统）
	 */
	void RestoreOriginalFBO();

	/**
	 * @brief 获取深度挂载
	 * @return
	 */
	GLRenderTarget* GetDepthAttachment()
	{
		return m_depthAttachment;
	}

	/**
	 * @brief 设置深度挂载
	 * @param depthAttachment
	 */
	void SetDepthAttachment(GLRenderTarget* depthAttachment)
	{
		m_depthAttachment = depthAttachment;
	}

	/**
	 * @brief 获取颜色挂载
	 * @param index
	 * @return
	 */
	GLRenderTarget* GetColorAttachment(int index);

	/**
	 * @brief 清除资源
	 */
	void ClearResource();
	/**
	 * @brief 重置大小
	 */
	void Resize();

	/**
	 * 有有效内容，比如Depth或者ColorAttachment
	 */
	bool HasValue();

	GLint GetOriginalFBO();
	GLint GetFirstFBO();
private:
	/**
	 * @brief 进行标记
	 */
	void MakeDirty();
private:
	GLint m_originalFBO;//!<在使用该缓冲区之前，opengl绑定的FBO对象
	GLRenderTarget *m_depthAttachment;//!<深度挂载
	vector<GLRenderTarget*> m_colorAttachments;//!<颜色挂载数组
	int m_maxColorAttachNum;//!<颜色最大挂载数量
	int m_width;//!<宽度
	int m_height;//!<高度
	bool m_dirty;//!<标记
	bool m_depthMS;
	bool m_depthRBO;
	vector<bool> m_colorAttaMSS;
	vector<bool> m_colorAttaRBOS;
	GLint m_firstFBO;//应用层的那个FBO
};

} /* namespace M3D */
#endif /* M3D_HARDWARE_FRAMEBUFFER_H_ */
