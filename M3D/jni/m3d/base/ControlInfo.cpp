#include "m3d/base/ControlInfo.h"

namespace M3D
{
ControlInfo::ControlInfo()
{
	ReSet();
}
ControlInfo::ControlInfo(float w, float x, float y, float z)
{
	rotation = Quaternion(w,x,y,z);
	moveVector.ToZero();
	scaleVector.ToOne();
	scaleFactor = 1;
	moveVector.ToZero();
}

void ControlInfo::ReSet()
{
	rotation = Quaternion(0,Vector3(0,0,1));
	moveVector.ToZero();
	scaleVector.ToOne();
	scaleFactor = 1;
	fitFactor = 1;
	aspective = 1;
}

void ControlInfo::SetScale(float scale)
{
	this->scaleFactor = scale;
}
float ControlInfo::GetFitFactor()
{
	return this->scaleFactor;
}

float ControlInfo::GetScale()
{
	return this->scaleFactor * fitFactor;
}
void ControlInfo::SetFitFactor(float facetor)
{
	this->fitFactor = facetor;
}

void ControlInfo::SetAspective(float aspective)
{
	this->aspective = aspective;
}

float ControlInfo::GetAspective()
{
	return this->aspective;
}


void ControlInfo::SetRotate(const Quaternion& rot)
{
	this->rotation = rot;
}

void ControlInfo::SetMoveVec(const Vector3& movVec)
{
	this->moveVector = movVec;
}

}
