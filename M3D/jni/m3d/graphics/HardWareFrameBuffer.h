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
	GLuint Create();
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
	
	/**
	* 当前一次的FBO大小改变了，需要解除关联
	*/
	bool DeatchFromPriFBO();

	//新函数

	int AllocColorAttachment();
	void Destroy();
	//void SetSize(int width, int height);
	void ReShape();
	void GenerateFramerBuffer();

	void SetParameters(int colorNumber = 1, bool rboColor = false, bool rboDepth = false,bool useDepth  = true,bool useStencil  = true,bool useMultisample = false ,int multisampler = 4);
	void GenerateColorAttatchment();
	void GenerateDepthAttatchment();

	GLuint AttachTextureToColor();

	void OnlyAttachTextureToColor(GLuint texture);

	//void AttachTextureToColor();

	void AttachRenderBufferToColor();
	void AttachRenderBufferToDepth( bool useStencil = true);
	void AttachTextureToDepth(bool useStencil = true);
	GLRenderTarget* GetColorTarget(int key = 0);
	GLRenderTarget* GetDepthTarget();

	//颜色缓冲区数量
	int ColorAttachmentNumber() const { return m_colorAttachmentNumber; }
	void ColorAttachmentNumber(int val) { m_colorAttachmentNumber = val; }
	GLenum ColorInternalFormat() const { return m_colorInternalFormat; }
	void ColorInternalFormat(GLenum val) { m_colorInternalFormat = val; }
	GLenum DepthInternalFormat() const { return m_depthInternalFormat; }
	void DepthInternalFormat(GLenum val) { m_depthInternalFormat = val; }
	bool UseDepthBuffer() const { return m_useDepthBuffer; }
	void UseDepthBuffer(bool val) { m_useDepthBuffer = val; }
	bool UseStencilBuffer() const { return m_useStencilBuffer; }
	void UseStencilBuffer(bool val) { m_useStencilBuffer = val; }
	bool UseMultisample() const { return m_useMultisample; }
	void UseMultisample(bool val) { m_useMultisample = val; }
	GeometryBuffer* OutColorTexture()  { return m_outColorTexture; }
	void OutColorTexture(GeometryBuffer* val) { m_outColorTexture = val; }
	GLenum OutColorTextureLevel()  { return m_outColorTextureLevel; }
	void OutColorTextureLevel(GLenum val) { m_outColorTextureLevel = val; }
	bool ColorTextureMipmap() const { return m_colorTextureMipmap; }
	void ColorTextureMipmap(bool val) { m_colorTextureMipmap = val; }

	void RenderToMe();

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

	//新函数
	int m_usedColorAttachment;
	int m_colorAttachmentNumber;
	bool m_colorUseRenderBuffer;
	bool m_depthUseRenderBuffer;

	bool m_useDepthBuffer;

	bool m_useStencilBuffer;

	bool m_useMultisample;

	int m_multisamplerNumber;

	map<int, GLRenderTarget*> m_colorTargets;
	GLRenderTarget* m_depthTarget;

	GLenum m_colorInternalFormat; // 指定OpenGL是如何管理颜色纹理单元中数据格式
	GLenum m_depthInternalFormat; //指定OpenGL是如何管理深度纹理单元中数据格式

	GeometryBuffer* m_outColorTexture;
	GLenum m_useOutColorTexture;	
	GLenum m_outColorTextureLevel;
	//GLenum m_outColor
	bool m_colorTextureMipmap;
};

} /* namespace M3D */
#endif /* M3D_HARDWARE_FRAMEBUFFER_H_ */
