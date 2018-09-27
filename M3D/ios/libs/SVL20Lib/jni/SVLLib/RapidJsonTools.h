// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	RapidJsonTools
*	@brief	封装PapidJson库提供的接口类
*	@par 历史记录
*				2017/07/14 创建。likun(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_RAPIDJSONTOOLS_H_
#define _SVLLIB_RAPIDJSONTOOLS_H_

#include "document.h"
#include "reader.h"
#include "stringbuffer.h"
#include "writer.h"
#include "prettywriter.h"
#include "allocators.h"
#include "Stk_API.h"
#define RAPIDJSON_HAS_STDSTRING 1

using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::PrettyWriter;
using rapidjson::Writer;
using rapidjson::StringRef;
using rapidjson::MemoryPoolAllocator;
using namespace rapidjson;

using namespace std;

namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT CRapidJsonTools
		{
		public:
			/****************************************************************************
			*	@brief		获取文件的扩展名
			*	@param[in]	strFilePath 文件名称或路径
			*	@param[out]	无
			*	@retval		文件的扩展名
			*	@note		无
			*	@attention
			****************************************************************************/
			static std::wstring GetFileExt(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		读取Json文件
			*	@param[in]	strFilePath	文件路径
			*	@param[out]	jsonDoc rapidJson文档对象
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		其他				读取失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS ReadJsonFile(const std::wstring& strFilePath, Document& jsonDoc);
			static STK_STATUS ReadJsonData(char *i_bufP, const unsigned int i_bufSize, Document& jsonDoc);
			static STK_STATUS ReadJsonData(char ***i_bufP, const std::vector<int> &i_vcBufNum, Document& jsonDoc);
			/****************************************************************************
			*	@brief		保存Json文件
			*	@param[in]	strFilePath	文件路径
			*	@param[in]	jsonDoc	rapidJson文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS WriteJsonFile(const std::wstring& strFilePath, Document& jsonDoc);

			/****************************************************************************
			*	@brief		读取线集文件
			*	@param[in]	strFilePath	线集文件路径
			*	@param[in]	pStkDocPtr	StkDoc对象，将从文件中的数据填充到对应的Model中
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS ReadLineDoc(const std::wstring& strFilePath, void* pStkDocPtr);

			static STK_STATUS ParseLine(Document& jsonDoc, Stk_Document* pStkDocPtr);
			/****************************************************************************
			*	@brief		保存线集文件
			*	@param[in]	strFilePath	线集文件路径
			*	@param[in]	pStkDocPtr	StkDoc对象，将所有Model中的线集数据保存到线集文件中
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS WriteLineDoc(const std::wstring& strFilePath, Stk_DocumentPtr pStkDocPtr);

			static STK_STATUS LineSetToJsonValue(Stk_LineSetPtr curLineSetPtr, Value& valLineSet, Document& jsonDoc);
			static STK_STATUS LineSetFromJsonValue(Stk_LineSetPtr lineSetPtr, Value& valLineSet, Document& jsonDoc);
			static STK_STATUS CurveToJsonValue(Stk_CurveEntityPtr curvePtr, Value& valLineSet, Document& jsonDoc);
			static Stk_CurveEntityPtr CurveFromJsonValue(Value& valCurve, Document& jsonDoc);
			static STK_STATUS SpacialPointToJsonValue(Stk_SpacialPointEntityPtr spacialPointPtr, Value& valSpacialPoint, Document& jsonDoc);
			static STK_STATUS SpacialPointFromJsonValue(Stk_SpacialPointEntityPtr pSpacialPointEntityPtr, Value& valSpacialPoint, Document& jsonDoc);
			/****************************************************************************
			*	@brief		读取文件加载信息
			*	@param[in]	strFilePath	文件路径
			*	@param[in]	pLoadInfPtr	文件加载信息对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS ReadLoadInf(const std::wstring& strFilePath, Stk_LoadInf* pLoadInfPtr);
			static STK_STATUS ReadLoadInf(char ***i_bufP, const std::vector<int> &i_vcBufNum, Stk_LoadInf* pLoadInfPtr);
			static STK_STATUS ReadLoadAttr(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_AttributeCollectionPtr pAttrCollection);
			static STK_STATUS ReadLoadGeoAttr(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_GeometryAttributeCollectionPtr pGeoAttrCollection);
			static STK_STATUS ReadLoadPMI(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_PMIDocumentPtr pPMIDoc);
			static STK_STATUS ReadLoadView(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_ViewDocumentPtr pViewDoc);
			static STK_STATUS ReadLoadInf(char *i_bufP, const int i_bufSize, Stk_LoadInf* pLoadInfPtr);
			static STK_STATUS ParseLoadInf(Document& jsonDoc, Stk_LoadInf* pLoadInfPtr);
			static STK_STATUS ParseLoadAttr(Document& jsonDoc, Stk_AttributeCollectionPtr pAttrCollection);
			static STK_STATUS ParseLoadGeoAttr(Document& jsonDoc, Stk_GeometryAttributeCollectionPtr pGeoAttrCollection);
			static STK_STATUS ParseLoadPMI(Document& jsonDoc, Stk_PMIDocumentPtr pPMIDoc);
			static STK_STATUS ParseLoadView(Document& jsonDoc, Stk_ViewDocumentPtr pPMIDoc);
			/****************************************************************************
			*	@brief		保存文件加载信息
			*	@param[in]	strFilePath	线集文件路径
			*	@param[in]	pLoadInfPtr	文件加载信息对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS WriteLoadInf(const std::wstring& strFilePath, Stk_LoadInf* pLoadInfPtr);

			static STK_STATUS ModelGroupToJsonValue(Stk_ModelGroup* modelGroupPtr, Value& valModelGroup, Document& jsonDoc);
			static STK_STATUS ModelGroupFromJsonValue(Stk_ModelGroup* modelGroupPtr, Value& valModelGroup, Document& jsonDoc);
			static STK_STATUS CameraToJsonValue(Stk_CameraPtr cameraPtr, Value& valCamera, Document& jsonDoc);
			static STK_STATUS CameraFromJsonValue(Stk_CameraPtr cameraPtr, Value& valCamera, Document& jsonDoc);


			/****************************************************************************
			*	@brief		读取图层文件
			*	@param[in]	strFilePath	图层文件路径
			*	@param[in]	pStkDocPtr	StkDoc对象，将从文件中的数据填充到对应的Model中
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS ReadLayerDoc(const std::wstring& strFilePath, void* pStkDocPtr);
			static HoteamSoft::SVLLib::STK_STATUS ReadLayer(char ***i_bufP, const std::vector<int> &i_vcBufNum, void* pStkDocPtr);
			static HoteamSoft::SVLLib::STK_STATUS ParseLayer(Document& jsonDoc, Stk_Document* pStkDocPtr);
			/****************************************************************************
			*	@brief		保存图层文件
			*	@param[in]	strFilePath	图层文件路径
			*	@param[in]	pStkDocPtr	StkDoc对象，将所有Model中的图层数据保存到图层文件中
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS WriteLayerDoc(const std::wstring& strFilePath, Stk_DocumentPtr pStkDocPtr);

			/****************************************************************************
			*	@brief		读取几何特征文件
			*	@param[in]	strFilePath	几何特征文件路径
			*	@param[in]	pStkDocPtr	StkDoc对象，将从文件中的数据填充到对应的Model中
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS ReadFeatDoc(const std::wstring& strFilePath, void* pStkDocPtr);
			static HoteamSoft::SVLLib::STK_STATUS ReadFeat(char ***i_bufP, const std::vector<int> &i_vcBufNum, void* pStkDocPtr);
			static HoteamSoft::SVLLib::STK_STATUS ParseFeat(Document& jsonDoc, Stk_Document* pStkDocPtr);
			

			/****************************************************************************
			*	@brief		保存几何特征文件
			*	@param[in]	strFilePath	几何特征文件路径
			*	@param[in]	pStkDocPtr	StkDoc对象，将所有Model中的几何特征数据保存到几何特征文件中
			*	@param[out]	无
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			static STK_STATUS WriteFeatDoc(const std::wstring& strFilePath, Stk_DocumentPtr pStkDocPtr);
			static STK_STATUS MagneticLineToJsonValue(Stk_Feat_MagneticLinePtr magneticLinePtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS MagneticLineFromJsonValue(Stk_Feat_MagneticLinePtr magneticLinePtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS HoleToJsonValue(Stk_Feat_HolePtr holePtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS HoleFromJsonValue(Stk_Feat_HolePtr holePtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS PipeToJsonValue(Stk_Feat_PipePtr pipePtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS PipeFromJsonValue(Stk_Feat_PipePtr pipePtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS MarkToJsonValue(Stk_Feat_MarkPtr markPtr, Value& valFeatData, Document& jsonDoc);
			static STK_STATUS MarkFromJsonValue(Stk_Feat_MarkPtr markPtr, Value& valFeatData, Document& jsonDoc);
			/************************************************************************/
			/* 获取属性文件中的所有文件名称                                                            */
			/************************************************************************/
			static STK_STATUS getInstanceNames(string& i_AttributeBuf, map<STK_UINT32, std::wstring>& i_mapInstanceIDtoName);
			/************************************************************************/
			/* 获取指定ID的属性信息                                                                     */
			/************************************************************************/
			static STK_STATUS getAttributesByIds(string& i_AttributeBuf, vector<STK_UINT32> i_vecId, map<STK_UINT32, map<std::wstring, std::wstring>>& i_mapKeyToValue);
            static string RemoveExcess(string& strStream);
		};
	}//svllib
}//hoteamsoft

#endif
