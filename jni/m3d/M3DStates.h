/**
 *@file
 *@brief	标准头文件，包括STL等常用库
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_STATES_H_
#define M3D_STATES_H_

/** @brief	返回状态 */
typedef enum
{	
	M_ERROR						= -1,//!<通用状态失败
	M_SUCCESS					= 0, //!<通用状态成功
	M_SUCCESS_EXIST				= 1001,//!<

	M_LIC_ERROR = 100, //!<授权无效

	M_Read_OK = 200, //!<成功
	M_Read_CANCEL = 201,//!<读取取消
	M_Read_NO_DEFINE_Error = 202,//!<未知错误
	M_Read_IO_ERROR = 203,//!<io 函数错误
	M_Read_OOM = 204,//!<内存分配失败
	M_Read_ANALYSIS_ERROR = 205,//!<SVL 文件解析失败
	M_Read_NOT_DEFINE_FILE = 206, //!<读取的文件格式不是规定类型

}M3D_STATUS;


#endif /*M3D_STATES_H_*/

