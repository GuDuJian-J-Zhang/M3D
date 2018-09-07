/**@file
 *@brief
 *@author	bing
 *@date		2014-2-20
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_COMTEXT_H_
#define M3D_COMTEXT_H_

#include "Shape.h"
#include "CText.h"

namespace M3D
{

class Mesh;
class VertexSet;

/**@class ComText
 * @brief 复合文本类（对应Stk_ComText）
 *
 * 由于DataManager中的Stk_ComText类存在诸多限制：
 * 1.必须从M3d内核层引用文件IO读取接口层的类定义。
 * 2.无法针对显示优化：修改类以增加顶点、索引缓存等。
 * 3.无法扩展文本类增加更多其他属性。
 */
class M3D_API ComText :public Shape{
public:
	ComText();
	virtual ~ComText();

	void AddCText(CText* ct);
	CText* GetCText(int pos);
	vector<CText*>& GetCTextList(){return m_CTextList;};

	/**
	 * 得到文本组合后的网格数据
	 * @return Mesh
	 */
	Mesh* GetMesh();

	/**
	 * 计算包围盒，派生类需重写
	 */
	virtual void ComputeBox();

	/**
	* 得到渲染颜色
	* @return
	*/
	virtual const  Color& GetShapeColor();

private:
	void Init();
	void MarkDirty();
	void UpDataMesh();

private :
	vector<CText*> m_CTextList; //!<存储所有的文本数据

	//存储所有的顶点
	VertexSet* m_VertexSet; //!< 存储文本分元后的Mesh顶点数据
	//存储Mesh
	Mesh* m_Meshdata; //!< 存储分元后的Mesh数据

	bool m_isDirty; //!< 脏标识
	mutable Mutex m_mutex; //!<线程锁对象
};

}

#endif /* COMTEXT_H_ */
