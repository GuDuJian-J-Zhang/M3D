/**@file
 *@brief	MovedMatrixInfo类头文件
 *@author	bing
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 */

#ifndef M3D_MOVEDMATRIXINFO_H_
#define M3D_MOVEDMATRIXINFO_H_
#include "m3d/M3D.h"
#include "m3d/base/Matrix4.h"

namespace M3D
{

/**@class MovedMatrixInfo
 * @brief MovedMatrixInfo类
 *
 * 移动后的矩阵信息类，包括原始矩阵、移动后的矩阵。
 * 用于存储和恢复当前模型位置、视图状态。
 */
class M3D_API MovedMatrixInfo
{
public:
	MovedMatrixInfo();
	virtual ~MovedMatrixInfo();

	int m_Id; //!< model id
	string m_Path; //!< model 路径
	Matrix4 m_InitMatrix; //!< 初始矩阵
	Matrix4 m_MovedMatrix; //!< 移动后矩阵
	Matrix4 m_PlcMatrx; //!<	配置矩阵

};

}
#endif /* M3D_MOVEDMATRIXINFO_H_ */
