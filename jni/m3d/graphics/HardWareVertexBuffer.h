/**@file
*@brief	GL HardWareVertexBuffer
*@author	XL
*@date
*@version	1.0
*@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
*
*/
#ifndef M3D_HARDWARE_VERTEXBUFFER_H_
#define M3D_HARDWARE_VERTEXBUFFER_H_

#include "m3d/graphics/GPUObject.h"

namespace M3D
{
	class ResourceManager;

	/**
	 * @class
	 * @brief 顶点缓冲对象
	 */
	class M3D_API HardWareVertexBuffer: public GPUObject
	{
	public:
		HardWareVertexBuffer();
		virtual ~HardWareVertexBuffer();

		/**
		* 设置在GPU端申请bufferSize所占字节数
		* @param bufferSize
		* @param dynamic
		*/
		void SetSize(unsigned bufferSize, bool dynamic = false);

		long GetBufferSize();

		/**
		* 一次性将数据从内存拷贝到GPU
		* @param data
		*/
		void SetData(const void* data);
 
		bool HasValue();

		//数据写到缓冲区中
		void WriteBuffer();

		/**
		* 分段将数据拷贝到GPU端
		* @param data 数据地址
		* @param start 起始位置
		* @param count 个数
		* @param discard 超过个数是否截断
		* @return
		*/
		bool SetDataRange(const void* data, unsigned start, unsigned count,
			bool discard = false);

		/**
		* 设置法向量的偏移量
		* @param offset
		*/
		void SetNormalOffset(M3D_OFFSET_TYPE offset);

		/**
		* 获取法向量的偏移量
		*/
		M3D_OFFSET_TYPE GetNormalOffset();

		/**
		* 设置顶点的偏移量
		* @param offset
		*/
		void SetVertexOffset(M3D_OFFSET_TYPE offset);

		/**
		* 获取顶点偏移量
		*/
		M3D_OFFSET_TYPE GetVertexOffset();

		/**
		* 设置纹理坐标偏移量
		* @param offset
		*/
		void SetTextureCoordsOffset(M3D_OFFSET_TYPE offset);

		/**
		* 获取纹理坐标偏移量
		*/
		M3D_OFFSET_TYPE GetTextureCoordsOffset();

		//	void SetVertexOffset();
		//	unsigned GetNormalOffset();

		/**
		* 创建：在GPU端申请空间
		* @return false 失败  true 成功
		*/
		bool Create(int cacheType);
		/**
		 * @brief 绑定
		 */
		void* Bind();
		/**
		 * @brief 解绑
		 */
		void UnBind();
		/**
		 * 缓冲区类型
		 * @return
		 */
		int BufferType();
	private:

		/**
		* 初始化
		*/
		void Init();

	private:
		bool m_dynamic;			//!< GPU申请空间是否为动态标识
		M3D_OFFSET_TYPE m_normalOffset;  //!< 法线偏移量
		M3D_OFFSET_TYPE m_vertexOffset;  //!< 顶点坐标偏移量
		M3D_OFFSET_TYPE m_textureCoordsOffset;  //!< 纹理坐标偏移量

		//	//其他数据 offset TODO
		M3D_OFFSET_TYPE m_bufferOffset; //!< 数据起始
		M3D_OFFSET_TYPE m_cacheBufferSize;	//!< disk申请空间大小

		M3D_OFFSET_TYPE m_BufferSize;	//!< GPU申请空间大小

		int m_bufferType;//!<使用内存类型
		unsigned char* pBaseAddress;//!<
	};
}

#endif /*M3D_HARDWARE_VERTEXBUFFER_H_*/
