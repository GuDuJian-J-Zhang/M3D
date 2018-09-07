#ifndef M3D_SHADER_MATERIAL_H_
#define M3D_SHADER_MATERIAL_H_
#include "m3d/graphics/BaseMaterial.h"
namespace M3D
{
	/************************************************************************/
	/* shader²ÄÖÊ                                                                     */
	/************************************************************************/
	class ShaderMaterial:
		public BaseMaterial
	{
	public:
		ShaderMaterial();
		~ShaderMaterial();
		string FragmentShader() const { return m_fragmentShader; }
		void FragmentShader(string val) { m_fragmentShader = val; }
		string VertexShader() const { return m_vertexShader; }
		void VertexShader(string val) { m_vertexShader = val; }
	private:
		void RealeseParameters();
		string m_fragmentShader;
		string m_vertexShader;
	};
}
#endif // !1



