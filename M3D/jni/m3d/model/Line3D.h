/**@file
 *@brief	Line3D类头文件
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef  M3D_LINE3D_H_
#define M3D_LINE3D_H_
#include "m3d/base/Vector3.h"
#include "m3d/model/Shape.h"

#define ARRAW_SIZE 2
namespace M3D {

/**@class Line3D
 * @brief Line3D类
 *
 * 表示三维中的直线线段，在两端点可包含末端符号。
 */
class M3D_API Line3D: public Shape {
public:
	Line3D();
	Line3D(Vector3* p1, Vector3* p2);
	Line3D(const Vector3& inStart, const Vector3& inEnd);
	virtual ~Line3D();

	void Init();

	/**
	 * 设置值
	 * @param p0 开始点
	 * @param p1 结束点
	 */
	void SetValue(const Vector3& p0, const Vector3& p1);

	/**
	 * 获取表示直线1与直线2最近距离的线段的端点
	 * @param line2
	 * @param ptOnThis
	 * @param ptOnLine2
	 * @return 成功返回true，否则为false
	 */
	bool GetClosestPoints(const Line3D& line2, Vector3& ptOnThis,
			Vector3& ptOnLine2) const;

	/**
	 *获取直线上离直线外的点最近的点（点在直线上的投影）
	 * @param point
	 * @return 返回获取的点
	 */
	Vector3 GetClosestPoint(const Vector3& point) const;

	/**
	 * 获取位置
	 * @return
	 */
	const Vector3& GetPosition(void) const;
    /**
     * 获取结束位置
     * @return
     */
    const Vector3& GetEndPoint(void) const;
	/**
	 * 获取方向向量
	 * @return
	 */
	const Vector3 GetDirection(void) const;

	/**
	 * 计算包围盒
	 */
	void ComputeBox();

	/**
	 * 获取起点末端符号类型
	 * @return
	 */
	int GetStartArrow() {
		return m_StartArrowType;
	}

	/**
	 * 获取终点末端符号类型
	 * @return
	 */
	int GetEndArrow() {
		return m_EndArrowType;
	}

	/**
	 * 设置起点末端符号类型
	 * @param startArrow
	 */
	void SetStartArrow(int startArrow);

	/**
	 * 设置终点末端符号类型
	 * @param endArrow
	 */
	void SetEndArrow(int endArrow);

	/**
	 * 根据末端符号类型生成末端符号buffer方法
	 * @param arrowType 末端符号类型
	 * @param point		端点
	 * @param direct	方向向量
	 * @param width		宽度
	 * @param height	高度
	 * @param outBuf	输出buf
	 * @note 调用了/svl2/PMIESymbol.h 中的相关方法来创建末端符号几何信息
	 */
	void CreateArrowBuffer(int arrowType, Vector3 point, Vector3 direct,
			float width, float height, vector<vector<float> > &outBuf);

	/**
	 *  获取起点末端符号buf
	 * @return
	 */
	vector<vector<float> > GetStartArrowBuffer() {
		return m_StartArrowBufferArray;
	}

	/**
	 * 获取终点末端符号buf
	 * @return
	 */
	vector<vector<float> > GetEndArrowBuffer() {
		return m_EndArrowBufferArray;
	}

	/**
	* 得到渲染颜色
	* @return
	*/
	virtual const  Color& GetShapeColor();

	void SetLineWidth(int width);
	int GetLineWidth();

	void SetCanDelete(bool bValue);
	bool GetCanDelete();
public:
	Vector3 m_StartPoint; //!< 起点
	Vector3 m_EndPoint; //!< 终点

private:
	bool m_CanDelete; //圆弧形线在协同消息中是否需要删除
	int m_lineWidth;
	int m_StartArrowType; ///起点末端符号类型
	int m_EndArrowType; ///终点末端符号类型
	vector<vector<float> > m_StartArrowBufferArray; ///起点末端符号buff
	vector<vector<float> > m_EndArrowBufferArray; ///终点末端符号buff
};
}
#endif // LINE3D_H_
