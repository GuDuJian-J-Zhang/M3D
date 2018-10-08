#include "m3d/renderer/gl10/GL10Context.h"

namespace M3D
{

GL10Context::GL10Context()
{

}

GL10Context::~GL10Context()
{

}
int GL10Context::GetContextType()
{
	return 1;
}

void GL10Context::GetDoublev(GLenum pname, double *params)
{
	float tM[16];
	glGetFloatv(pname, tM);
	for (int i = 0; i < 16; i++)
	{
		params[i] = tM[i];
	}
}

void GL10Context::GetFloatv(GLenum pname, float *params)
{
	glGetFloatv(pname, params);
}

void GL10Context::GetDoublev(GLenum pname, Matrix4& m)
{
	// this->GetDoublev(pname,*m.m);
}
void GL10Context::GetFloatv(GLenum pname, Matrix4& m)
{
	this->GetFloatv(pname, (float*) m.Data());
}

}
