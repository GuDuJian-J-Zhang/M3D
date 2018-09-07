/**
 *@file
 *@brief    BaseEvent TODO
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef    M3D_BASE_EVENT_H_
#define    M3D_BASE_EVENT_H_
#include "m3d/M3D.h"

namespace M3D
{

class M3D_API BaseEvent
{
public:
	const static string BASE_EVENT;
	const static string CAMERA_EVENT;
	const static string SHAPE_EVENT;

public:
	BaseEvent();
	virtual ~BaseEvent();

	string& GetProperty();

	void SetProperty(const string& property);

	virtual const string& GetType();

	void MarkDirty();
	bool IsDirty();

protected:
	virtual void CreateProperty();
	virtual void ApplyProperty();

private:
	string m_Content;
	bool m_isDirty;
};

}
#endif /*M3D_BASE_EVENT_H_*/

