
#include "m3d/utils/M3DTools.h"

namespace M3D
{
string M3DTOOLS::floatToString(float fZ, const int alen)
{
	char tempstr[256];
	memset(tempstr,'\0',256);
	sprintf(tempstr,"%.*lf",alen,fZ);
	return string(tempstr);
}

string M3DTOOLS::IntToString(int iZ)
{
	char tempstr[64];
	memset(tempstr,'\0',64);
	sprintf(tempstr,"%d",iZ);
	return string(tempstr);
}

bool M3DTOOLS::FloatSlerp(float from, float to, int count, float* midScale)
{
	for (int i = 0; i < count - 1; i++)
	{
		midScale[i] = from + (to - from) * i / count;
	}
	midScale[count - 1] = to;
	return true;
}

bool M3DTOOLS::VecSlerp(const Vector3& from, const Vector3& to, int count, Vector3* midVec)
{
	for (int i = 0; i < count - 1; i++)
	{
		midVec[i] = from + (to - from) * (i * 1.0f / count);
	}
	midVec[count - 1] = to;
	return true;
}

Vector3 M3DTOOLS::VecSlerp(const Vector3& from, const Vector3& to, float interPos)
{
	Vector3 retV = from + (to - from) * interPos;
	return retV;
}

bool M3DTOOLS::RotationSlerp(const Quaternion& from, const Quaternion& to, int count,
		Quaternion* midRotation)
{
	Quaternion qFrom =  from;
	Quaternion qTo =  to;
	for (int i = 0; i < count - 1; i++)
	{
		Quaternion qMid = qFrom.Slerp(qTo, i * 1.0 / count);
		midRotation[i] =qMid;
	}
	midRotation[count - 1] = to;

	return true;
}

bool M3DTOOLS::FloatSlerp(float from, float to, float* midScale, float interPos)
{
	*midScale = from + (to - from) * interPos;
	return true;
}

bool M3DTOOLS::VecSlerp(const Vector3& from, const Vector3& to, Vector3* midVec,
		float interPos)
{
	*midVec = from + (to - from) * interPos;
	return true;
}

}

