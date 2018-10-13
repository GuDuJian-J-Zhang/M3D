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
			//SAMPLE
			float DiffuseColor[3];
			float fTransparency;
			wstring wstrDiffuseTextureImagePath;

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
				mixingFactor = 1.0f;
				name = L"";
				hightLightTexture = L"";
			}
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

	StkMaterialTypeEnum GetMaterialType();
	void GetTransparency(float& i_fTransparency);
	void GetDiffuseColor(float* i_diffuseColor);
	void SetDiffuseTexture(Stk_TexturePtr i_DiffuseTextureP);
	Stk_TexturePtr GetDiffuseTexture();

	void GetAmbientColor(float* i_diffuseColor);

	void GetReflectiveTexture(vector<Stk_TexturePtr>& o_frontTexture);

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
	//顶点
};
	}//svllib
}//hoteamsoft
#endif