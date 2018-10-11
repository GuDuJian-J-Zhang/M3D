// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_MATERIAL_H_
#define _SVLLIB_MATERIAL_H_
#include "Stk_Object.h"
#include <vector>
#include "Stk_Declare_Ptr.h"

namespace HoteamSoft {
	namespace SVLLib {

		struct MaterialData
		{
			/*
			code释义
			diffuse,transparent,shininess,diffuseMap,metalnessFactor,roughnessFactor,metalnessRoughnessMap,ambient,specular,emissive,
			ambientMap,specularMap,emissiveMap,normalMap,normalScale,displacementMap,displacementScale,displacementBias,aoMap,aoMapIntensity
			*/
			//SAMPLE
			float DiffuseColor[3];
			float fTransparency;
			wstring wstrDiffuseTextureImagePath;

			float shininess;
			float ambientColor[3];
			wstring wstrAmbientMapImagePath;
			float specularColor[3];
			wstring wstrSpecularMapImagePath;
			float emissiveColor[3];
			wstring wstrEmissiveMapImagePath;

			float metalnessFactor;
			float roughnessFactor;
			wstring wstrMetalnessRoughnessMapImagePath;

			float normalScale[2];
			wstring wstrNormalMapImagePath;

			float displacementScale;
			float displacementBias;
			wstring wstrDisplacementMapImagePath;

			float aoMapIntensity;
			wstring wstrAoMapImagePath;

			wstring wstrMatcapImagePath;



			//金正金扩展
			wstring name;
			float mixingFactor;
			vector<wstring> frontTexture;
			vector<wstring> backTexture;
			wstring hightLightTexture;

			MaterialData()
			{
				DiffuseColor[0] = DiffuseColor[1] = DiffuseColor[2] = 0.0f;				
				fTransparency = 0.0f;
				wstrDiffuseTextureImagePath = L"";

				shininess = 0.0f;
				ambientColor[0] = ambientColor[1] = ambientColor[2] = 0.0f;
				wstrAmbientMapImagePath = L"";
				specularColor[0] = specularColor[1] = specularColor[2] = 0.0f;
				wstrSpecularMapImagePath = L"";
				emissiveColor[0] = emissiveColor[1] = emissiveColor[2] = 0.0f;
				wstrEmissiveMapImagePath = L"";

				metalnessFactor = 0.0f;
				roughnessFactor = 0.0f;
				wstrMetalnessRoughnessMapImagePath = L"";

				normalScale[0] = normalScale[1] = 0.0f;
				wstrNormalMapImagePath = L"";

				displacementScale = 0.0f;;
				displacementBias = 0.0f;;
				wstrDisplacementMapImagePath = L"";

				aoMapIntensity = 0.0f;
				wstrAoMapImagePath=L"";


				mixingFactor = 1.0f;
				name = L"";
				hightLightTexture = L"";
			}

			wstring GetMeshDataCode();

		};
class Stk_Document;
class STK_TOOLKIT_EXPORT Stk_Material : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Material)
protected:
	friend class Stk_Document;
	enum {	_typeID = _SVLLIB_TYPE_MATERIAL};	//!< 类别
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
	
public:
	Stk_Material(void);
	virtual ~Stk_Material(void);
	void BindingMaterial(void* i_bingdedNode);
	void SetMaterialType(StkMaterialTypeEnum i_enmuMaterialType);
	void SetTransparency(float i_fTransparency);
	void SetDiffuseColor(float* i_diffuseColor);

	//base
	StkMaterialTypeEnum GetMaterialType();
	void GetTransparency(float& i_fTransparency);
	void GetDiffuseColor(float* i_diffuseColor);
	void SetDiffuseTexture(Stk_TexturePtr i_DiffuseTextureP);
	Stk_TexturePtr GetDiffuseTexture();

	//uv相关
	void SetUvTranslate(float* value);
	void GetUvTranslate(float* value);
	void SetUvScale(float* value);
	void GetUvScale(float* value);
	void SetUvRotate(float value);
	void GetUvRotate(float& value);
	//phong
	void GetAmbientColor(float* i_ambientColor);
	void SetAmbientColor(float* i_ambientColor);
	void GetEmissiveColor(float* i_emissiveColor);
	void SetEmissiveColor(float* i_emissiveColor);
	void GetSpecularColor(float* i_color);
	void SetSpecularColor(float* i_color);
	void SetAmbientMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetAmbientMap();
	void SetSpecularMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetSpecularMap();
	void SetMatcapMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetMatcapMap();
	void SetShininess(float value);
	void GetShininess(float& value);


	//physical
	void SetMetalnessRoughnessMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetMetalnessRoughnessMap();
	void SetMetalnessFactor(float value);
	void GetMetalnessFactor(float& value);
	void SetRoughnessFactor(float value);
	void GetRoughnessFactor(float& value);

	void SetUseClearCoat(bool  value);
	void GetUseClearCoat(bool& value);
	void SetClearCoat(float  value);
	void GetClearCoat(float& value);
	void SetClearCoatRoughness(float  value);
	void GetClearCoatRoughness(float& value);

	void SetNormalMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetNormalMap();
	void GetNormalScale(float* value);
	void SetNormalScale(float* value);

	void SetAoMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetAoMap();
	void SetAoMapIntensity(float value);
	void GetAoMapIntensity(float& value);
	void SetDisplacementMap(Stk_TexturePtr i_texture);
	Stk_TexturePtr GetDisplacementMap();
	void SetDisplacementScale(float value);
	void GetDisplacementScale(float& value);
	void SetDisplacementBias(float value);
	void GetDisplacementBias(float& value);

	void SetMaterialCode(wstring code);
	wstring GetMaterialCode();

	void GetReflectiveTexture(vector<Stk_TexturePtr>& o_frontTexture);

	//珠宝用
	void SetMixingFactor(float mixingFactor);
	void GetMixingFactor(float& mixingFactor );
	void SetFrontTexture(Stk_TexturePtr i_frontTexture);
	void GetFrontTexture(vector<Stk_TexturePtr>& o_frontTexture);//!<可能不止一个texture
	void SetBackTexture(Stk_TexturePtr i_backTexture);
	void GetBackTexture(vector<Stk_TexturePtr>& o_backTexture);//!<可能不止一个texture
	void SetSpecularTexture(Stk_TexturePtr i_SpecularTextureP);
	Stk_TexturePtr GetSpecularTexture();
	void SetMaterialName(wstring i_name);
	wstring GetMaterialName();

	void SetMaterialParameters(string key,pair<string, Stk_TexturePtr>& value);
	void SetMaterialParameters(string key, pair<string, vector<Stk_TexturePtr> >& value);
	void SetMaterialParameters(string key, pair<string, vector<float> >& value);
	void SetMaterialParameters(string key, pair<string, vector<int> >& value);
	void SetMaterialParameters(string key, pair<string, bool>& value);
	map<string, pair<string, string> > GetMaterialParameters();
private:
	//wstring floatToString(float fZ, const int alen = 3);
	//wstring GetMaterialCode();
	wstring GenerateMaterialCode();
	wstring m_materialCode;
	bool m_isNeedUpdateCode;
	//顶点
};
	}//svllib
}//hoteamsoft
#endif