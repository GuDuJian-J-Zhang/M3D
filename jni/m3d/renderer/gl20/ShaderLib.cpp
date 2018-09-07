#include "m3d/renderer/gl20/ShaderLib.h"
#include "m3d/renderer/gl20/ShaderSrcCode.h"
#include "m3d/graphics/BaseMaterial.h"

M3D::ShaderLib::ShaderLib()
{
	
}

pair<const char*, const char*>* M3D::ShaderLib::GetSrcCode(string name)
{
	if (m_needInit)
	{
		Init();
		m_needInit = false;
	}
	// TODO: 在此处插入 return 语句
	pair<const char*, const char*>* ret = NULL;

	map<string, pair<const char*, const char*> >::iterator it = m_shaderLib.find(name);
	if (it != m_shaderLib.end())
	{
		ret = &(it->second);
	}

	return ret;

}

void M3D::ShaderLib::Init()
{
	m_shaderLib[IntToString(MaterialType_Phong)] = std::make_pair(ShaderSrcCode::PhongMaterialVert(), ShaderSrcCode::PhongMaterialFrag());
	m_shaderLib[IntToString(MaterialType_Pbr)] = std::make_pair(ShaderSrcCode::PbrMaterialVert(), ShaderSrcCode::PbrMaterialFrag());
	m_shaderLib[IntToString(MaterialType_MatCap)] = std::make_pair(ShaderSrcCode::MatCapMaterialVert(), ShaderSrcCode::MatCapMaterialFrag());
	m_shaderLib[IntToString(MaterialType_Depth)] = std::make_pair(ShaderSrcCode::DepthMaterialVert(), ShaderSrcCode::DepthMaterialFrag());
}

map<string, pair<const char*, const char*> > M3D::ShaderLib::m_shaderLib;

bool M3D::ShaderLib::m_needInit = true;
