#ifndef SHADER_LIB_H_
#define  SHADER_LIB_H_
#include "m3d/M3D.h"
namespace M3D 
{
	class ShaderLib
	{
	public:
		ShaderLib();
		static pair<const char*, const char*>* GetSrcCode(string name);
	private:
		static void Init();
		static map<string, pair<const char*, const char*> > m_shaderLib;
		static bool m_needInit;
	};
}

#endif
