/**@file
*@brief	GL HardWareIndexBuffer
*@author	XL
*@date
*@version	1.0
*
*/
#ifndef M3D_HARDWARE_INDEXBUFFER_H_
#define M3D_HARDWARE_INDEXBUFFER_H_

#include "m3d/graphics/GPUObject.h"

namespace M3D
{

/**
 * @class
 * @brief 索引VBO
 */
	class M3D_API HardWareIndexBuffer: public GPUObject
	{
	public:
		HardWareIndexBuffer();
		virtual ~HardWareIndexBuffer();

		/**
		* 设置在GPU端申请Indexbuffer所占字节数
		* @param indexCount
		* @param dynamic
		*/
		void SetSize(unsigned bufferSize, bool dynamic = false);

		/**
		* 一次性将数据从内存拷贝到GPU端
		* @param data
		*/
		void SetData(const void* data);

		/**
		* 分段将数据拷贝到GPU端
		* @param data 数据地址
		* @param start 数据起点
		* @param count 数据大小
		* @param discard 数据超出是否自动截断
		* @return
		*/
		bool SetDataRange(const void* data, unsigned start, unsigned count,
			bool discard = false);

		long GetBufferSize();

		/**
		* 设置数据相对于起点的偏移量
		* @param offset
		*/
		void SetOffset(M3D_OFFSET_TYPE offset);

		/**
		* 获取索引的偏移量
		*/
		M3D_OFFSET_TYPE GetOffset();

		/**
		* 创建：在GPU端申请空间 
		* 设置是否创建VBO对象，如果不创建，在绑定时默认绑定到0，不影响使用。
		* @return false 失败  true 成功
		*/
		bool Create(int bufferType = GPUObject::GPU_CACHE);
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

		/**
		 * 是否有值
		 * @return
		 */
		bool HasValue();

		/**
		 * @brief 数据写到缓冲区中
		 */

		void WriteBuffer();
	private:

		/**
		 * 初始化
		 */
		void Init();

	private:
		M3D_OFFSET_TYPE m_offset;	//!<索引的偏移量

		static bool m_dynamic;		//!<索引是否为动态
		M3D_OFFSET_TYPE m_cacheBufferSize;//!<
		M3D_OFFSET_TYPE m_BufferSize;//!<
		
		M3D_OFFSET_TYPE m_bufferOffset;//!<开始位置
		int m_bufferType;//!<类型
		unsigned char* pBaseAddress;//!<地址
	};
}

#endif /*M3D_HARDWARE_INDEXBUFFER_H_*/
