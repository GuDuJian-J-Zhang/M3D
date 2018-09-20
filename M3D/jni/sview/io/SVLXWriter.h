//
//  StlWriter.h
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#ifndef __M3D__SVLXWriter__
#define __M3D__SVLXWriter__
#include "M3D/M3DExport.h"
#include <set>
#include "Writer.h"
#include "m3d/base/StringHash.h"

namespace M3D
{
	class Model;
	class Body;
	class Face;
	class Edge;
	class Material;
	class PbrMaterial;
	class ShaderMaterial;
	class MatCapMaterial;
	class Texture2D;
	class Image;
	class BaseMaterial;
}


using M3D::Model;
using M3D::Body;
using M3D::Face;
using M3D::Edge;
using M3D::Material;
using M3D::PbrMaterial;
using M3D::Texture2D;
using M3D::ShaderMaterial;
using M3D::Image;
using M3D::BaseMaterial;
using M3D::MatCapMaterial;

namespace SVIEW
{
    class M3D_API SVLXWriter: public Writer {
    public:
    	SVLXWriter(void);
        virtual ~SVLXWriter();


		int SaveAll(View * view, const string& srcFilePath, const string & targetPath);

		//整体保存
		int  WriteSavedModels(View* view,const string& srcFilePath, const string &targetPath);

        virtual int SaveTo(View* view, vector<Model*>& model,const string &targetPath);

        virtual int SaveTo(View* view,const string& srcPath,const string &targetPath);

		int SaveLights(const string& destFilePath);
        
		int WriteMaterials(const string& destFilePath);

		virtual int SaveToByIncrease(View* view, const string& tempPath, const string &strsrcPath, const string &strsrcName);
       
		void WriteTopInstance(M3D::Model* model, void* svlInstance);

		void FillInstance(M3D::Model* model, M3D::Model* topParentmodel, void* svlInstance);

		void WriteInstance(M3D::Model* model, M3D::Model* topParentmodel, void* svlInstance);

		void WriteProtoType(M3D::Model* model, void* svlProtoType);

		//得到这个模型原型对应的键值
		M3D::StringHash GetModelProtypeCode(M3D::Model* model);

		void WriteAllProtoTypes(M3D::Model* model);

		void SaveView();
        
//        /**
//         保存视图和批注
//
//         @param view
//         @param srcFilePath svlx文件地址
//         */
//        string SaveViewAndAnnotation(View* view,const string& srcFilePath);
		void writeMeshPrim(void* meshPrim, M3D::Body* body);
		void writeLineSet(void* meshPrim, M3D::Body* body);
		void writeMeshFace(void* meshPrim, M3D::Face* face);
		void writeMeshEdge(void* meshPrim, M3D::Edge* edge);

        /**
         添加文件到SVLX文件中

         @param view
         @param strsrcPath 需添加文件的路径
         @param strsvlxPath svlx文件路径
         @param strsrcName 需添加文件的名称（完整-带后缀）
         @return 
         */
        int AddFileToSVLX(View* view , const string &strsrcPath,const string &strsvlxPath,const string &strsrcName);
		
		vector<Model*>& GetSavedModels();

		void SetSavedModel(vector<Model*>& val);
        
	private:
		/************************************************************************/
		/* 保存材质                                                                     */
		/************************************************************************/
		void SaveMaterial(Model* model, void* instancePtr);
		void SaveMaterial();
        //保存测量
        int SaveMeasures(const string& destFilePath);
        //保存批注
        int SaveAnnotations(const string& destFilePath);
		bool SaveTexture2D(Texture2D* texture2d, void* stktexturePtr);

		bool SaveImage(Image* image, void* stktexturePtr);

		bool SavePhoneTexture(Material* phoneMaterial,void* stkMaterial);
		bool SavePBRTexture(PbrMaterial* pbrMaterial,void* stkMaterial);
		bool SaveMatCapTexture(MatCapMaterial* matcapMaterial, void* stkMaterial);
		bool SaveShaderTexture(ShaderMaterial* pbrMaterial, void* stkMaterial);

		/************************************************************************/
		/* 保存自定义视图                                                                     */
		/************************************************************************/
		void SaveUserDefineViews(Model* pModel);
		/************************************************************************/
		/* 更新SVLX文件的Info数据                                                                     */
		/************************************************************************/
		void SaveUserInfo();

		//判断装配结构是否进行过调整
		bool UpdateSVLXFile();

		bool UpdateInstance(Model* model, void* binInstance);

		///获取需要保持模型所有的材质
		void UpdateMaterialsSet();

		void SaveAttribute();

		unsigned int UpdateSVLXID(unsigned int origSVLID);
	private:
		View* m_View;	
		string m_tempPath;
		void* m_svl2Doc;
		void* m_pDoc;
		vector<Model*> m_savedModels; //将要保存的顶级模型，如果保存的单个模型

		std::set<BaseMaterial*> m_baseMaterials;

		int m_updateSVLID;
    };
}

#endif /* defined(__M3D__StlWriter__) */
