#ifndef M3D_SHADER_CHUNK_H_
#define M3D_SHADER_CHUNK_H_
#include "m3d/M3D.h"
namespace M3D
{
	class ShaderChunk
	{
	public:
		ShaderChunk();
		~ShaderChunk();
		static map<string, string>& GetShaderChunks() {
			if (m_needInit)
			{
				Init();
				m_needInit = false;
			}return m_shaderChunks;
		}
		static void Init();
		static void Clear();
		//void ShaderChunks(map<string, string> val) { m_shaderChunks = val; }
	private:
		static bool m_needInit;
		static map<string, string> m_shaderChunks;
	};
}
#endif




