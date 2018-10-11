// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	RapidJsonTools
*	@brief	封装PapidJson库提供的接口�?
*	@par 历史记录
*				2017/07/14 创建。likun(Hoteamsoft)
**************************************************************************/
#include <fstream>
#include "RapidJsonTools.h"
#include "ChineseCode.h"
//#include <tchar.h>
#include "Stk_Model.h"
#include "Stk_LineSet.h"
#include "Stk_SpacialPoint.h"
#include "Stk_LoadInf.h"
#include "Stk_AttributeCollection.h"
#include "Stk_GeometryAttributeCollection.h"
#include "Stk_ModelGroup.h"
#include "Stk_Camera.h"
#include "Stk_LayerInfosSet.h"
#include "Stk_Feat.h"
#include "StringTools.h"
#include "Alloc.h"
#include "Stk_PMIDocument.h"
#include "Stk_PMI.h"
#include "Stk_ViewDocument.h"
#include "Stk_View.h"

#define LINEDOC_VERSION			"2.0.1"
#define LINEDOC_NODE_NAME_VERSION			"version"
#define LINEDOC_NODE_NAME_MODELS                "models"
#define LINEDOC_NODE_NAME_ID                "id"
#define LINEDOC_NODE_NAME_LINESETS	"lineSets"
#define LINEDOC_NODE_NAME_SUBLINES	"subLines"
#define LINEDOC_NODE_NAME_POINTS   "points"
#define LINEDOC_NODE_NAME_LINESET_ID	"id"
#define LINEDOC_NODE_NAME_LINESET_TYPE	"type"
#define LINEDOC_NODE_NAME_LINESET_NAME	"name"
#define LINEDOC_NODE_NAME_LINESET_VISIBLE	"visible"
#define LINEDOC_NODE_NAME_LINESET_COLOR	"color"
#define LINEDOC_NODE_NAME_CURVES	"curves"
#define LINEDOC_NODE_NAME_CURVE_TYPE	"type"
#define LINEDOC_NODE_NAME_CURVE_VISIBLE	"visible"
#define LINEDOC_NODE_NAME_CURVE_COLOR	"color"
#define LINEDOC_NODE_NAME_CURVE_DISPLAYTYPE	"displayType"
#define LINEDOC_NODE_NAME_CURVE_USAGETYPE	"usageType"
#define LINEDOC_NODE_NAME_CURVE_MIN	"min"
#define LINEDOC_NODE_NAME_CURVE_MAX	"max"
#define LINEDOC_NODE_NAME_CURVE_POINTS	"points"
#define LINEDOC_NODE_NAME_CURVE_CENTERPOINT	"centerPoint"
#define LINEDOC_NODE_NAME_CURVE_DIR_X	"dirX"
#define LINEDOC_NODE_NAME_CURVE_DIR_Y	"dirY"
#define LINEDOC_NODE_NAME_CURVE_DIR_Z	"dirZ"
#define LINEDOC_NODE_NAME_CURVE_MAJORRADIUS	"majorRadius"
#define LINEDOC_NODE_NAME_CURVE_MINORRADIUS	"minorRadius"
#define LINEDOC_NODE_NAME_CURVE_STARTPNT	"startPoint"
#define LINEDOC_NODE_NAME_CURVE_ENDPNT	"endPoint"
#define LINEDOC_NODE_NAME_CURVE_STARTPAR	"startPar"
#define LINEDOC_NODE_NAME_CURVE_ENDPAR	"endPar"
#define LINEDOC_NODE_NAME_CURVE_DIR_NORMAL	"dirNormal"
#define LINEDOC_NODE_NAME_CURVE_DIR_ORIGIN	"dirOrigin"
#define LINEDOC_NODE_NAME_CURVE_DEGREE	"degree"
#define LINEDOC_NODE_NAME_CURVE_KNOTPLICITIES	"knotPlicities"
#define LINEDOC_NODE_NAME_CURVE_KNOTS	"knots"
#define LINEDOC_NODE_NAME_CURVE_WEIGHTS	"weights"
#define LINEDOC_NODE_NAME_CURVE_SEMIAXIS	"semiAxis"
#define LINEDOC_NODE_NAME_CURVE_SEMIIMGAXIS	"semiImgAxis"
#define LINEDOC_NODE_NAME_CURVE_FOCALDIS	"focalDis"
#define LINEDOC_NODE_NAME_POINTS_TYPE	"type"
#define LINEDOC_NODE_NAME_POINTS_NAME	"name"
#define LINEDOC_NODE_NAME_POINTS_VISIBLE	"visible"
#define LINEDOC_NODE_NAME_POINTS_COLOR	"color"
#define LINEDOC_NODE_NAME_POINTS_POS	"pos"
//版本记录
//2.0.1 中广核现场验证版�?
//2.0.2 修改了bom文件的结构将配置矩阵改成4X4
#define LOADINFDOC_CURRENT_VERSION					"2.0.2"
#define LOADINFDOC_NODE_NAME_VERSION				"version"
#define LOADINFDOC_NODE_NAME_INFO					"info"
#define LOADINFDOC_NODE_NAME_LOADINF				"loader"
#define LOADINFDOC_NODE_NAME_CAMERA					"camera"
#define LOADINFDOC_NODE_NAME_LOADHIDDEN			"loadHidden"
#define LOADINFDOC_NODE_NAME_GROUPLIST				"groupList"
#define LOADINFDOC_NODE_NAME_INITSTATE				"initState"
#define LOADINFDOC_NODE_NAME_DEFAULTGROUP			"defaultGroup"

#define LOADINFDOC_NODE_NAME_GROUP					"group"
#define LOADINFDOC_NODE_NAME_GROUP_ID				"id"
#define LOADINFDOC_NODE_NAME_GROUP_TYPE			"type"
#define LOADINFDOC_NODE_NAME_GROUP_NAME			"name"
#define LOADINFDOC_NODE_NAME_GROUP_PRIORITY		"priority"
#define LOADINFDOC_NODE_NAME_GROUP_PRECISE			"precise"
#define LOADINFDOC_NODE_NAME_GROUP_ITEMS			"groupItems"


#define LOADINFDOC_NODE_NAME_ATTRIBUTES				"attributes"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_BOUNDINGBOX		"boundingBox"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_MODELCOUNT		"modelCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_INSCOUNT		"instanceCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHCOUNT		"meshCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_FACECOUNT		"faceCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHEDGECOUNT		"meshEdgeCount"
//#define LOADINFDOC_NODE_NAME_ATTRIBUTE_INSCOUNT		"instanceCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_PMICOUNT		"pmiCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_VIEWCOUNT		"viewCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_UNIT			"unit"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_TRIANGLECOUNT	"triangleCount"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_PREVIEWIMAGE	"preview"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_FILESIZE		"fileSize"
#define LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHDATASIZE		"meshDataSize"

/************************************************************************/
/* 转换器信息宏定义                                                                     */
/************************************************************************/
#define LOADINFDOC_NODE_NAME_CONVERTER				"converter"
#define LOADINFDOC_NODE_NAME_CONVERTER_VERSION				"version"
#define LOADINFDOC_NODE_NAME_CONVERTER_COMPUTERNAME	"computerName"
#define LOADINFDOC_NODE_NAME_CONVERTER_USERNAME					"userName"
#define LOADINFDOC_NODE_NAME_CONVERTER_NAME					"name"
#define LOADINFDOC_NODE_NAME_CONVERTER_SOURCEFORMAT				"sourceFormat"
#define LOADINFDOC_NODE_NAME_CONVERTER_SOURCEVERSION				"sourceVersion"
#define LOADINFDOC_NODE_NAME_CONVERTER_CONVERTERTIME				"convertTime"
#define LOADINFDOC_NODE_NAME_CONVERTER_LINEARTOL				"linearTol"
#define LOADINFDOC_NODE_NAME_CONVERTER_ANGULARTOL				"angularTol"
#define LOADINFDOC_NODE_NAME_CONVERTER_CONFIG				"config"
#define LOADINFDOC_NODE_NAME_CONVERTER_BEGINTIME				"beginTime"
#define LOADINFDOC_NODE_NAME_CONVERTER_ENDTIME				"endTime"

#define CAMERA_NODE_NAME_PROJECTTYPE				"projectType"
#define CAMERA_NODE_NAME_FOV						"fov"
#define CAMERA_NODE_NAME_ORIGIN						"origin"
#define CAMERA_NODE_NAME_TARGETVEC					"targetVector"
#define CAMERA_NODE_NAME_UPVEC						"upVector"
#define CAMERA_NODE_NAME_MATIX						"matix"
#define CAMERA_NODE_NAME_ASPECTRATIO				"aspectRatio"
#define CAMERA_NODE_NAME_NEARDISTANCE				"nearDistance"
#define CAMERA_NODE_NAME_FARDISTANCE				"farDistance"
#define CAMERA_NODE_NAME_FOCALDISTANCE				"focalDistance"
#define CAMERA_NODE_NAME_HEIGHT						"height"

#define LOADINFDOC_NODE_NAME_SCENE					"scene"
#define LOADINFDOC_NODE_NAME_BACKGROUND				"background"
#define LOADINFDOC_NODE_NAME_BACKGROUND_TYPE		"type"
#define LOADINFDOC_NODE_NAME_BACKGROUND_STYLE		"style"
#define LOADINFDOC_NODE_NAME_BACKGROUND_ITEMS		"items"

#define LOADINFDOC_NODE_NAME_LIGHTS					"lights"
#define LOADINFDOC_NODE_NAME_LIGHTS_TYPE			"type"			
#define LOADINFDOC_NODE_NAME_LIGHTS_POSITION		"position"
#define LOADINFDOC_NODE_NAME_LIGHTS_DIRECTION		"direction"
#define LOADINFDOC_NODE_NAME_LIGHTS_AMBIENT			"ambient"
#define LOADINFDOC_NODE_NAME_LIGHTS_DIFFUSE			"diffuse"
#define LOADINFDOC_NODE_NAME_LIGHTS_SPECULAR		"specular"
#define LOADINFDOC_NODE_NAME_LIGHTS_CONSTANT		"constant"
#define LOADINFDOC_NODE_NAME_LIGHTS_LINEAR			"linear"
#define LOADINFDOC_NODE_NAME_LIGHTS_QUADRATIC		"quadratic"
#define LOADINFDOC_NODE_NAME_LIGHTS_CUTOFF			"cutoff"
#define LOADINFDOC_NODE_NAME_LIGHTS_OUTERCUTOFF		"outercutoff"


#define  ATTRIBUTEDOC_NODE_NAME_VERSION  "version"
#define  ATTRIBUTEDOC_NODE_NAME_INSTANCES "instances"
#define  ATTRIBUTEDOC_NODE_NAME_MODELS    "models"
#define  ATTRIBUTEDOC_NODE_NAME_MESHES    "meshes"
#define  ATTRIBUTEDOC_NODE_NAME_MESHFACES "meshFaces"
#define  ATTRIBUTEDOC_NODE_NAME_MESHEDGES "meshEdges"
#define  ATTRIBUTEDOC_NODE_NAME_GEOMETRIES   "geometries"

#define  ATTRIBUTE_NODE_NAME_NAME "name"
#define  ATTRIBUTE_NODE_NAME_ID   "id"
#define  ATTRIBUTE_NODE_NAME_ATTRIBUTES "attributes"


#define LAYERDOC_VERSION							"2.0.1"
#define LAYERDOC_NODE_NAME_VERSION					"version"
#define LAYERDOC_NODE_NAME_MODELS					"models"
#define LAYERDOC_NODE_NAME_MODEL_ID					"id"
#define LAYERDOC_NODE_NAME_INFOSET					"layerInfoSets"
#define LAYERDOC_NODE_NAME_INFOSET_ID				"id"
#define LAYERDOC_NODE_NAME_INFOSET_DEFLAYERFILTER	"defaultLayer"
#define LAYERDOC_NODE_NAME_LAYERFILTERS				"layerFilters"
#define LAYERDOC_NODE_NAME_LAYERFILTERS_ID			"id"
#define LAYERDOC_NODE_NAME_LAYERFILTERS_NAME		"name"
#define LAYERDOC_NODE_NAME_LAYERFILTERS_DESC			"description"
#define LAYERDOC_NODE_NAME_LAYERFILTERS_SELECTED		"selectedLayers"
#define LAYERDOC_NODE_NAME_LAYERFILTER_ISSELECTED		"isSelected"
#define LAYERDOC_NODE_NAME_INFOSET_DEFLAYER			"defLayer"
#define LAYERDOC_NODE_NAME_LAYERS					"layers"
#define LAYERDOC_NODE_NAME_LAYER_ID					"id"
#define LAYERDOC_NODE_NAME_LAYER_NAME				"name"
#define LAYERDOC_NODE_NAME_LAYER_VISIBLE			"visible"
#define LAYERDOC_NODE_NAME_LAYER_ENTITIES			"layer_entities"
#define LAYERDOC_NODE_NAME_ENTITIES					"entities"
#define LAYERDOC_NODE_NAME_LAYER_ENTITY_TYPE		"type"
#define LAYERDOC_NODE_NAME_LAYER_ENTITY_ID			"entityId"
#define LAYERDOC_NODE_NAME_LAYER_ENTITY_INS			"insPlcPath"

#define FEATDOC_VERSION									"2.0.1"
#define FEATDOC_NODE_NAME_VERSION						"version"
#define FEATDOC_NODE_NAME_MODELS						"models"
#define FEATDOC_NODE_NAME_MODEL_ID						"id"
#define FEATDOC_NODE_NAME_FEATS							"feats"
#define FEATDOC_NODE_NAME_FEAT_ID						"id"
#define FEATDOC_NODE_NAME_FEAT_TYPE						"type"
#define FEATDOC_NODE_NAME_FEAT_ISACTIVE					"isActive"
#define FEATDOC_NODE_NAME_FEAT_NAME						"name"
#define FEATDOC_NODE_NAME_FEAT_DATA						"data"
#define FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_STARTPNT	"startPoint"
#define FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ENDPNT		"endPoint"
#define FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ISEFFECTIVE	"isEffective"
#define FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ARRANGEMENT	"arrangement"
#define FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_COLOR		"color"
#define FEATDOC_NODE_NAME_FEAT_PIPE_TYPE				"pipeType"
#define FEATDOC_NODE_NAME_FEAT_PIPE_SECTIONTYPE			"pipeSectionType"
#define FEATDOC_NODE_NAME_FEAT_PIPE_DIAMETER			"diameter"
#define FEATDOC_NODE_NAME_FEAT_PIPE_TURNRADIUS			"turnRadius"
#define FEATDOC_NODE_NAME_FEAT_PIPE_HEIGHT				"height"
#define FEATDOC_NODE_NAME_FEAT_PIPE_WIDTH				"width"
#define FEATDOC_NODE_NAME_FEAT_PIPE_FIBERS				"pipeFibers"
#define FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_STARTPNT		"startPoint"
#define FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_ENDPNT		"endPoint"
#define FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CENTERPNT		"centerPoint"
#define FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CURVE			"curve"
#define FEATDOC_NODE_NAME_FEAT_HOLE_TYPE				"holeType"
#define FEATDOC_NODE_NAME_FEAT_HOLE_ANCHOR_TYPE			"holeAnchorType"
#define FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOM_TYPE			"holeBottomType"
#define FEATDOC_NODE_NAME_FEAT_HOLE_DEPTH				"depth"
#define FEATDOC_NODE_NAME_FEAT_HOLE_DEPTHTOL			"depthTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETER			"diameter"
#define FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETERTOL			"diameterTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLE			"bottomAngle"
#define FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLETOL		"bottomAngleTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLE		"counterAngle"
#define FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLETOL		"counterAngleTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTH		"counterDepth"
#define FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTHTOL		"counterDepthTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETER		"counterDiameter"
#define FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETERTOL	"counterDiameterTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_AXIS				"axis"
#define FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_DIR			"dir"
#define FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_ORIGIN			"origin"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD			"innerThread"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_LIMITTYPE	"threadLimitType"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_THREADTYPE	"threadType"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTH		"depth"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTHTOL	"depthTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETER	"diameter"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETERTOL	"diameterTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCH		"pitch"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCHTOL	"pitchTol"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_ISRIGHT		"isRightThread"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR	"connector"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTTYPE	"type"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTID		"entityId"
#define FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_PLCPATH	"plcPath"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT					"limit"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_TYPE				"limitType"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_VALUE				"value"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR			"connector"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTTYPE	"type"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTID	"entityId"
#define FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_PLCPATH	"plcPath"

#define FEATDOC_NODE_NAME_FEAT_MARK_TYPE					"markType"
#define FEATDOC_NODE_NAME_FEAT_MARK_COLOR					"color"
#define FEATDOC_NODE_NAME_FEAT_MARK_DEFPLANE				"defPlane"
#define FEATDOC_NODE_NAME_FEAT_MARK_CURVEWIDTH				"curveWidth"
#define FEATDOC_NODE_NAME_FEAT_MARK_CURVETYPE				"curveType"
#define FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE				"fillStyle"
#define FEATDOC_NODE_NAME_FEAT_MARK_CENTERPNT				"centerPoint"
#define FEATDOC_NODE_NAME_FEAT_MARK_MAJORRADIUS				"majorRadius"
#define FEATDOC_NODE_NAME_FEAT_MARK_MINORRADIUS				"minorRadius"
#define FEATDOC_NODE_NAME_FEAT_MARK_MIN						"min"
#define FEATDOC_NODE_NAME_FEAT_MARK_MAX						"max"
#define FEATDOC_NODE_NAME_FEAT_MARK_NORMAL					"normal"
#define FEATDOC_NODE_NAME_FEAT_MARK_ORIGIN					"origin"
#define FEATDOC_NODE_NAME_FEAT_MARK_POINTS					"points"
#define FEATDOC_NODE_NAME_FEAT_MARK_LEFTTOPPNT				"leftTopPoint"
#define FEATDOC_NODE_NAME_FEAT_MARK_RIGHTBOTTOMPNT			"rightBottomPoint"

//PMI宏定�?
#define PMIDOC_NODE_NAME_VERSION			"version"
#define PMIDOC_NODE_NAME_PMIS                "pmis"
#define PMIDOC_NODE_NAME_PROTOTYPES	"protoTypes"
#define PMIDOC_NODE_NAME_INSTANCES   "instances"
#define PMIDOC_NODE_NAME_PROTOTYPEID	"protoTypeID"
#define PMIDOC_NODE_NAME_MODEL_PMI     "model_pmis"
#define PMIDOC_NODE_NAME_INSTANCE_PMI "instance_pmis"
#define PMIDOC_NODE_NAME_ID   "id"

//View宏定�?
#define VIEWDOC_NODE_NAME_PROTOTYPEID	"modelID"
#define VIEWDOC_NODE_NAME_VERSION		"version"
#define VIEWDOC_NODE_NAME_VIEWS			"views"
#define VIEWDOC_NODE_NAME_MODEL_VIEW	"model_views"
#define VIEWDOC_NODE_NAME_ID   "id"
#define VIEWDOC_NODE_NAME_VIEW_PMI   "view_pmis"
#define VIEWDOC_NODE_NAME_PMI  "pmis"
#define VIEWDOC_NODE_NAME_VIEW_VIEW   "view_views"
#define VIEWDOC_NODE_NAME_VIEW_LINKVIEW  "views"

namespace HoteamSoft {
	namespace SVLLib {
		/************************************************************************/
		/* 对应的处理器--Start                                                                     */
		/************************************************************************/
		struct InstanceNameHandler : public BaseReaderHandler<UTF8<>, InstanceNameHandler> {
			bool Null() { cout << "Null()" << endl; return true; }
			bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
			bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
			bool Uint(unsigned u) { 
				//cout << "Uint(" << u << ")" << endl; 
				if (2 == vecObjectName.size() && "id" == vecObjectName[1])
				{
					vecInstanceID.push_back(u);
				}
				if (vecObjectName.size() >= 3)
				{
					return false;
				}
				vecObjectName.pop_back();
				return true; 
			}
			bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
			bool Uint64(uint64_t u) { 
				cout << "Uint64(" << u << ")" << endl; return true; 
			}
			bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
			bool String(const char* str, SizeType length, bool copy) { 
				//cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
				if (2 == vecObjectName.size() && "name" == vecObjectName[1])
				{
					vecInstanceName.push_back(str);
				}
				vecObjectName.pop_back();
				return true;
			}
			bool StartObject() 
			{ 
				//cout << "StartObject()" << endl; 
				return true; 
			}
			bool Key(const char* str, SizeType length, bool copy) { 
				//cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
				vecObjectName.push_back(str);
				return true;
			}
			bool EndObject(SizeType memberCount)
			{ 
				//cout << "EndObject(" << memberCount << ")" << endl; 
				return true; 
			}
			bool StartArray() 
			{ 
				//cout << "StartArray()" << endl;
				return true;
			}
			bool EndArray(SizeType elementCount) 
			{ 
				//cout << "EndArray(" << elementCount << ")" << endl; 
				return true; 
			}
			//引用变量
			vector<STK_UINT32> vecInstanceID;
			vector<string> vecInstanceName;
			vector<string> vecObjectName;
		};

		struct AttributeHandler : public BaseReaderHandler<UTF8<>, AttributeHandler> {
			bool Null() { cout << "Null()" << endl; return true; }
			bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
			bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
			bool Uint(unsigned u) { 
				//cout << "Uint(" << u << ")" << endl; 
				stkId = u;
				return true; 
			}
			bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
			bool Uint64(uint64_t u) { 
				cout << "Uint64(" << u << ")" << endl; return true; 
			}
			bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
			bool String(const char* str, SizeType length, bool copy) { 
				//cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
				if(bNeedAdd)
				{
					strValue = str;
					wstring wstrKey = ChineseCode::StringToWString(strKey);
					wstring wstrValue = ChineseCode::StringToWString(strValue);
					mapKeyToValue.insert(make_pair(wstrKey, wstrValue));
				}
				return true;
			}
			bool StartObject() 
			{ 
				//cout << "StartObject()" << endl; 
				return true; 
			}
			bool Key(const char* str, SizeType length, bool copy) { 
				//cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
				strKey = str;
				return true;
			}
			bool EndObject(SizeType memberCount)
			{ 
				//cout << "EndObject(" << memberCount << ")" << endl; 
				if(!bNeedAdd)
					return true;
				//判断ID是否为指定的ID
				bool bIdFinded = false;
				for (int i = 0; i < vecInstanceID.size(); i++)
				{
					if (stkId == vecInstanceID[i])
					{
						bIdFinded = true;
						break;
					}
				}
				if (bIdFinded)
				{
					mapIDToKeyValue.insert(make_pair(stkId, mapKeyToValue));
				}
				//清除Map
				map<std::wstring, std::wstring> emptyTemp;
				mapKeyToValue.swap(emptyTemp);
				return true; 
			}
			bool StartArray() 
			{ 
				//cout << "StartArray()" << endl;
				bNeedAdd = true;
				return true;
			}
			bool EndArray(SizeType elementCount) 
			{ 
				//cout << "EndArray(" << elementCount << ")" << endl; 
				bNeedAdd = false;
				return true; 
			}
			//引用变量
			vector<STK_UINT32> vecInstanceID;
			map<std::wstring, std::wstring> mapKeyToValue;
			map<STK_UINT32, map<std::wstring, std::wstring>> mapIDToKeyValue;
			STK_UINT32 stkId;
			string strKey;
			string strValue;
			bool bNeedAdd;
		};
		/************************************************************************/
		/* 对应的处理器--End                                                                     */
		/************************************************************************/
		std::wstring CRapidJsonTools::GetFileExt(const std::wstring& strFilePath)
		{
			wstring strExt = L"";
			int iWhich = strFilePath.find_last_of('.');
			if (iWhich == -1)
				return strExt;
			strExt = strFilePath.substr(iWhich + 1, strFilePath.size() - iWhich - 1);
			return strExt;
		}


		STK_STATUS CRapidJsonTools::WriteJsonFile(const std::wstring& strFilePath, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			////判断扩展名是否正�?
			//int iWhich = strFilePath.find_last_of('.');
			//if (-1 == iWhich) {
			//	return STK_ERROR_FILERD;
			//}
			//wstring strExt = GetFileExt(strFilePath);
			//if (strExt.empty()/* || _tcsicmp(strExt.c_str(), L"json") != 0*/)
			//{
			//	return STK_ERROR_FILERD;
			//}
			wstring outputFile = strFilePath;
			//写文�?
			string strPath;
			ChineseCode::UnicodeToUTF_8(outputFile.c_str(), strPath);

			StringBuffer buffer;
			Writer<StringBuffer> writer(buffer);
			jsonDoc.Accept(writer);

			if (result != STK_SUCCESS)
				return result;

			std::ofstream ofile;
#ifdef __MOBILE__
			ofile.open(strPath.c_str());
#else
			ofile.open(outputFile.c_str());
#endif
			if (ofile.fail())
			{
				return STK_ERROR_FILEWR;
			}
			//char UTF8BOM[4] = { '\xEF','\xBB','\xBF','\x0' };
			//ofile << UTF8BOM;
			ofile << buffer.GetString();
			ofile.close();
			return result;
		}


		STK_STATUS CRapidJsonTools::ReadJsonFile(const std::wstring& strFilePath, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			
			////判断扩展名是否正�?
			//wstring strExt = GetFileExt(strFilePath);
			//if (strExt.empty() || _tcsicmp(strExt.c_str(), L"json") != 0)
			//{
			//	return STK_ERROR_FILERD;
			//}


			//加载Json文件�?
			string strStream;
			std::ifstream ifile;

#ifdef __MOBILE__
			string strPath;
			ChineseCode::UnicodeToUTF_8(strFilePath.c_str(), strPath);
			ifile.open(strPath.c_str(), ios_base::in);
#else
			ifile.open(strFilePath.c_str(), ios_base::in);
#endif
			if (!ifile.is_open())
			{
				return STK_ERROR;
			}
			string strLine;
			while (getline(ifile, strLine))
			{
				strStream.append(strLine + "\n");
			}
			ifile.close();
			
			jsonDoc.Parse<0>(strStream.c_str());

			if (jsonDoc.HasParseError()) {
				return STK_ERROR;
			}
			if (!jsonDoc.IsObject()) {
				return STK_ERROR;
			}
			if(result != STK_SUCCESS)
			{
				return result;
			}

			return STK_SUCCESS;
		}


		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadJsonData(char ***i_bufP, const std::vector<int> &i_vcBufNum, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			string strStream;
			int numBuf = i_vcBufNum.size();
			//int index= 0;
			UINT   niOffsetSize = 0;
			for (int i = 0; i < numBuf; i++)
			{
				strStream.append((*i_bufP)[i]);
				MidFree((*i_bufP)[i]);
				(*i_bufP)[i] = NULL;
			}
			MidFree (*i_bufP);
			*i_bufP = NULL;
			//解析文件结构
#ifdef __MOBILE__
            //去除Info中的多余字段
            printf("HasParseError111 %s",strStream.c_str());
            int index = 0;
            if( !strStream.empty())
            {
                while( (index = strStream.find('\r',index)) != string::npos)
                {
                    strStream.erase(index,1);
                }
                index = 0;
                while( (index = strStream.find('\n',index)) != string::npos)
                {
                    strStream.erase(index,1);
                }
                index = 0;
                while( (index = strStream.find(' ',index)) != string::npos)
                {
                    if (index - 1 > -1) {
                        string s = strStream.substr(index-1,1);
                        if (s != "}") {
                            strStream = strStream.replace(index, 1, "$");
                        }else{
                            strStream.erase(index,1);
                        }
                    }
                }
            }
            index = 0;
            while( (index = strStream.find('$',index)) != string::npos)
            {
                strStream.replace(index, 1, " ");
            }
            
            if (strStream.length() > 0) {
                string tab = "}}}}}";
                if (strStream.find(tab) != string::npos) {
                    strStream.erase(strStream.find(tab.c_str())+tab.length() , strStream.length());
                }else{
                    tab = "}}}}";
                    if (strStream.find(tab) != string::npos) {
                        strStream.erase(strStream.find(tab.c_str())+tab.length() , strStream.length());
                        
                    }else{
                        tab = "]}}}";
                        if (strStream.find(tab) != string::npos) {
                            strStream.erase(strStream.find(tab.c_str())+tab.length() , strStream.length());
                        }else{
                            tab = "]}";
                            if (strStream.find(tab) != string::npos) {
                                strStream.erase(strStream.find(tab.c_str())+tab.length() , strStream.length());
                            }
                        }
                    }
                }
            }
            printf("HasParseError333 %s",strStream.c_str());
#endif
			jsonDoc.Parse<0>(strStream.c_str());
			if (jsonDoc.HasParseError()) {
				return STK_ERROR;
			}
			if (!jsonDoc.IsObject()) {
				return STK_ERROR;
			}
			return result;
		}

		STK_STATUS CRapidJsonTools::ReadJsonData(char *i_bufP, const unsigned int i_bufSize, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			string strStream;
			for (int i = 0; i  < i_bufSize; i++)
			{
				strStream.push_back(i_bufP[i]);
			}
			//strStream.append(i_bufP);
			//解析文件结构
			jsonDoc.Parse<0>(strStream.c_str());
			if (jsonDoc.HasParseError()) {
				return STK_ERROR;
			}
			if (!jsonDoc.IsObject()) {
				return STK_ERROR;
			}
			return result;
		}

		STK_STATUS CRapidJsonTools::ReadLineDoc(const std::wstring& strFilePath, void* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			Document jsonDoc;
			result = ReadJsonFile(strFilePath, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			ParseLine(jsonDoc, (Stk_Document*)pStkDocPtr);

			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ParseLine(Document& jsonDoc, Stk_Document* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;

			string strVersion = "";
			if (jsonDoc.HasMember(LAYERDOC_NODE_NAME_VERSION))
			{
				strVersion = jsonDoc[LAYERDOC_NODE_NAME_VERSION].GetString();
			}

			//models
			Value::MemberIterator modelsNodeIter = jsonDoc.FindMember(LINEDOC_NODE_NAME_MODELS);
			if (modelsNodeIter != jsonDoc.MemberEnd())
			{
				Value valModels = modelsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					Stk_ModelPtr pModelPtr = nullptr;
					if (valModel.HasMember(LINEDOC_NODE_NAME_ID))
					{
						STK_ID stkModelID;
						stkModelID = valModel[LINEDOC_NODE_NAME_ID].GetInt();
						pModelPtr = pStkDocPtr->GetModelByID(stkModelID);
					}
					if (pModelPtr == nullptr)
						continue;

					//读取线集信息
					if (valModel.HasMember(LINEDOC_NODE_NAME_LINESETS))
					{
						Value::Array valLineSets = valModel[LINEDOC_NODE_NAME_LINESETS].GetArray();
						Value valLineSet;
						for (int idxLineSet = 0; idxLineSet < valLineSets.Size(); idxLineSet++)
						{
							valLineSet = valLineSets[idxLineSet];
							if (valLineSet.HasMember(LINEDOC_NODE_NAME_ID))
							{
								Stk_LineSetPtr lineSetPtr = pModelPtr->AddLineSet(valLineSet[LINEDOC_NODE_NAME_ID].GetInt());
								LineSetFromJsonValue(lineSetPtr,valLineSet, jsonDoc);
							}
						}
					}

					//读参照线信息
					if (valModel.HasMember(LINEDOC_NODE_NAME_SUBLINES))
					{
						Value::Array valSubLines = valModel[LINEDOC_NODE_NAME_SUBLINES].GetArray();
						for (int idxSubLine = 0; idxSubLine < valSubLines.Size(); idxSubLine++)
						{
							pModelPtr->AddSubLine(CurveFromJsonValue(valSubLines[idxSubLine], jsonDoc));
						}
					}

					//读空间点信息
					if (valModel.HasMember(LINEDOC_NODE_NAME_POINTS))
					{
						Value::Array valPoints = valModel[LINEDOC_NODE_NAME_POINTS].GetArray();
						Value valPoint;
						for (int idxPoint = 0; idxPoint < valPoints.Size(); idxPoint++)
						{
							valPoint = valPoints[idxPoint];
							if (valPoint.HasMember(LINEDOC_NODE_NAME_ID))
							{
								Stk_SpacialPointEntityPtr spacialPointPtr = pModelPtr->AddSpacialPoint(valPoint[LINEDOC_NODE_NAME_ID].GetInt());
								SpacialPointFromJsonValue(spacialPointPtr, valPoint, jsonDoc);
							}
						}
					}
				}
			}

			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::WriteLineDoc(const std::wstring& strFilePath, Stk_DocumentPtr pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;

			Document jsonDoc;
			jsonDoc.SetObject();
			string strVersion;
			strVersion = LINEDOC_VERSION;
			jsonDoc.AddMember(LINEDOC_NODE_NAME_VERSION, strVersion, jsonDoc.GetAllocator());

			Value valModels;
			valModels.SetArray();

			std::vector<Stk_ModelPtr> vecModel;
			pStkDocPtr->ListModel(vecModel);
			for (int i = 0; i < vecModel.size(); i++)
			{
				Stk_ModelPtr modePtr = vecModel[i];
				if(modePtr == nullptr)
					continue;
				if(modePtr->GetAllLineSet().size() <=0 &&
					modePtr->GetAllSubLine().size()<=0 &&
					modePtr->GetAllSpacialPoint().size() <=0)
					continue;

				Value valModel;
				valModel.SetObject();
				valModel.AddMember(LINEDOC_NODE_NAME_ID, modePtr->GetID(), jsonDoc.GetAllocator());

				//保存线集信息
				map<int, Stk_LineSetPtr> mapLineSet = modePtr->GetAllLineSet();
				if(mapLineSet.size()>0)
				{
					Value valLineSets;
					valLineSets.SetArray();
					map<int, Stk_LineSetPtr>::iterator it;
					for (it = mapLineSet.begin(); it != mapLineSet.end(); it++)
					{
						Stk_LineSetPtr curLineSetPtr = it->second;
						if (curLineSetPtr == nullptr)
							continue;
						Value valLineSet;
						LineSetToJsonValue(curLineSetPtr, valLineSet, jsonDoc);

						valLineSets.PushBack(valLineSet, jsonDoc.GetAllocator());
					}
					valModel.AddMember(LINEDOC_NODE_NAME_LINESETS, valLineSets, jsonDoc.GetAllocator());
				}
				
				//保存参照线信�?
				map<int, Stk_CurveEntityPtr> mapSubLine = modePtr->GetAllSubLine();
				if (mapSubLine.size() > 0)
				{
					Value valSubLines;
					valSubLines.SetArray();
					map<int, Stk_CurveEntityPtr>::iterator it;
					for (it = mapSubLine.begin(); it != mapSubLine.end(); it++)
					{
						Stk_CurveEntityPtr curSubLinePtr = it->second;
						if (curSubLinePtr == nullptr)
							continue;
						Value valCurve;
						CurveToJsonValue(curSubLinePtr, valCurve, jsonDoc);

						valSubLines.PushBack(valCurve, jsonDoc.GetAllocator());
					}
					valModel.AddMember(LINEDOC_NODE_NAME_SUBLINES, valSubLines, jsonDoc.GetAllocator());
				}

				//保存点集信息
				map<int, Stk_SpacialPointEntityPtr> mapSpacialPoint = modePtr->GetAllSpacialPoint();
				if (mapSpacialPoint.size() > 0)
				{
					Value valSpacialPoints;
					valSpacialPoints.SetArray();
					map<int, Stk_SpacialPointEntityPtr>::iterator it;
					for (it = mapSpacialPoint.begin(); it != mapSpacialPoint.end(); it++)
					{
						Stk_SpacialPointEntityPtr curSpacialPointPtr = it->second;
						if (curSpacialPointPtr == nullptr)
							continue;
						Value valSpacialPoint;
						SpacialPointToJsonValue(curSpacialPointPtr, valSpacialPoint, jsonDoc);

						valSpacialPoints.PushBack(valSpacialPoint, jsonDoc.GetAllocator());
					}
					valModel.AddMember(LINEDOC_NODE_NAME_POINTS, valSpacialPoints, jsonDoc.GetAllocator());
				}
				
				valModels.PushBack(valModel, jsonDoc.GetAllocator());

			}
			vecModel.clear();
			if (valModels.Size() > 0)
			{
				jsonDoc.AddMember(LINEDOC_NODE_NAME_MODELS, valModels, jsonDoc.GetAllocator());
				WriteJsonFile(strFilePath, jsonDoc);
			}
			return result;
		}


		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::LineSetToJsonValue(Stk_LineSetPtr curLineSetPtr, Value& valLineSet,Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			
			if (curLineSetPtr == nullptr)
				return STK_ERROR;
			
			valLineSet.SetObject();
			valLineSet.AddMember(LINEDOC_NODE_NAME_ID, curLineSetPtr->GetID(), jsonDoc.GetAllocator());
			valLineSet.AddMember(LINEDOC_NODE_NAME_LINESET_TYPE, curLineSetPtr->GetType(), jsonDoc.GetAllocator());
			//valLineSet.AddMember(LINEDOC_NODE_NAME_LINESET_NAME, curLineSetPtr->GetName(), jsonDoc.GetAllocator());
			valLineSet.AddMember(LINEDOC_NODE_NAME_LINESET_VISIBLE, (INT)curLineSetPtr->GetDispStat(), jsonDoc.GetAllocator());
			STK_RGBA32 curLineSetColor = curLineSetPtr->GetColor();
			char szColor[128] = { 0 };
			sprintf(szColor, "%.8f %.8f %.8f %.8f", curLineSetColor.Red, curLineSetColor.Green, curLineSetColor.Blue, curLineSetColor.Alpha);
			string strColor = szColor;
			valLineSet.AddMember(LINEDOC_NODE_NAME_LINESET_COLOR, strColor, jsonDoc.GetAllocator());

			Value valCurves;
			valCurves.SetArray();
			//保存线列�?
			vector<Stk_CurveEntityPtr>& vecCurvePtr = curLineSetPtr->GetAllLines();
			for (int i = 0; i < vecCurvePtr.size(); i++)
			{
				Value valCurve;
				CurveToJsonValue(vecCurvePtr[i], valCurve, jsonDoc);
				valCurves.PushBack(valCurve, jsonDoc.GetAllocator());
			}
			valLineSet.AddMember(LINEDOC_NODE_NAME_CURVES, valCurves, jsonDoc.GetAllocator());
			return result;
		}

		STK_STATUS CRapidJsonTools::LineSetFromJsonValue(Stk_LineSetPtr lineSetPtr, Value& valLineSet, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (valLineSet.IsNull())
				return result;

			if (valLineSet.HasMember(LINEDOC_NODE_NAME_ID))
			{
				lineSetPtr->SetID(valLineSet[LINEDOC_NODE_NAME_ID].GetInt());
			}

			if (valLineSet.HasMember(LINEDOC_NODE_NAME_LINESET_TYPE))
			{
				lineSetPtr->SetType((StkLineSetTypeEnum)valLineSet[LINEDOC_NODE_NAME_LINESET_TYPE].GetInt());
			}
			if (valLineSet.HasMember(LINEDOC_NODE_NAME_LINESET_VISIBLE))
			{
				lineSetPtr->SetDispStat(valLineSet[LINEDOC_NODE_NAME_LINESET_VISIBLE].GetInt());
			}

			if (valLineSet.HasMember(LINEDOC_NODE_NAME_LINESET_COLOR))
			{
				STK_RGBA32 color;
				string strColor = valLineSet[LINEDOC_NODE_NAME_LINESET_COLOR].GetString();
				std::vector<std::string> vecColorValue;
				CStringTools::StringSplit(strColor, " ", vecColorValue);
				if (vecColorValue.size() == 4)
				{
					color.Red = atof(vecColorValue[0].c_str());
					color.Green = atof(vecColorValue[1].c_str());
					color.Blue = atof(vecColorValue[2].c_str());
					color.Alpha = atof(vecColorValue[3].c_str());
				}
				lineSetPtr->SetColor(color);
			}

			if (valLineSet.HasMember(LINEDOC_NODE_NAME_CURVES))
			{
				Value::Array valCurves = valLineSet[LINEDOC_NODE_NAME_CURVES].GetArray();
				Value valCurve;
				for (int idxCurve = 0; idxCurve < valCurves.Size(); idxCurve++)
				{
					lineSetPtr->AddLine(CurveFromJsonValue(valCurves[idxCurve], jsonDoc));
				}
			}
			return result;
		}


		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::CurveToJsonValue(Stk_CurveEntityPtr curvePtr, Value& valCurve, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (curvePtr == nullptr)
				return STK_ERROR;

			valCurve.SetObject();
			valCurve.AddMember(LINEDOC_NODE_NAME_ID, curvePtr->GetID(), jsonDoc.GetAllocator());
			valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_TYPE, curvePtr->GetCurveType(), jsonDoc.GetAllocator());
			valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DISPLAYTYPE, curvePtr->GetCurveDispType(), jsonDoc.GetAllocator());
			valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_USAGETYPE, curvePtr->GetUsageType(), jsonDoc.GetAllocator());
			valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_VISIBLE, (INT)curvePtr->GetCurveDispStat(), jsonDoc.GetAllocator());
			STK_RGBA32 curveColor = curvePtr->GetColor();
			char szColor[128] = { 0 };
			sprintf(szColor, "%.8f %.8f %.8f %.8f", curveColor.Red, curveColor.Green, curveColor.Blue, curveColor.Alpha);
			string strColor = szColor;
			valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_COLOR, strColor, jsonDoc.GetAllocator());

			
			switch (curvePtr->GetCurveType())
			{
			case CURVE_TYPE_UNKNOWN:
			case CURVE_TYPE_POLYLINE:
			{
				STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
				
				Stk_PolyLinePtr::DynamicCast(curvePtr)->GetRange(fUMin, fUMax);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MIN, fUMin, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MAX, fUMax, jsonDoc.GetAllocator());

				string strPnt = "";
				vector<STK_PNT32> vecPoints = Stk_PolyLinePtr::DynamicCast(curvePtr)->GetPoints();
				for (int i = 0; i < vecPoints.size(); i++)
				{
					if (strPnt.length() > 0)
						strPnt.append(" ");
					char szPnt[128] = { 0 };
					sprintf(szPnt, "%.8f %.8f %.8f", vecPoints[i].X, vecPoints[i].Y, vecPoints[i].Z);
					strPnt.append(szPnt);
				}
				if(strPnt.length()>0)
				{
					valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_POINTS, strPnt, jsonDoc.GetAllocator());
				}
			}
				break;
			case CURVE_TYPE_ELLIPSE:
			{
				STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
				Stk_ArcPtr::DynamicCast(curvePtr)->GetRange(fUMin, fUMax);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MIN, fUMin, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MAX, fUMax, jsonDoc.GetAllocator());

				STK_PNT32 centerPnt = Stk_ArcPtr::DynamicCast(curvePtr)->GetCenterPoint();
				char szPnt[128] = { 0 };
				sprintf(szPnt, "%.8f %.8f %.8f", centerPnt.X, centerPnt.Y, centerPnt.Z);
				string strCenterPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_CENTERPOINT, strColor, jsonDoc.GetAllocator());

				STK_DIR32 dirX, dirY, dirZ;
				Stk_ArcPtr::DynamicCast(curvePtr)->GetXYZDir(dirX, dirY, dirZ);
				string strDir = "";
				char szDir[128] = { 0 };
				sprintf(szDir, "%.8f %.8f %.8f", dirX.X, dirX.Y, dirX.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_X, strDir, jsonDoc.GetAllocator());
				sprintf(szDir, "%.8f %.8f %.8f", dirY.X, dirY.Y, dirY.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_Y, strDir, jsonDoc.GetAllocator());
				sprintf(szDir, "%.8f %.8f %.8f", dirZ.X, dirZ.Y, dirZ.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_Z, strDir, jsonDoc.GetAllocator());

				STK_FLOAT32 fMajorR, fMinorR;
				Stk_ArcPtr::DynamicCast(curvePtr)->GetRadius(fMajorR, fMinorR);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MAJORRADIUS, fMajorR, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MINORRADIUS, fMinorR, jsonDoc.GetAllocator());

				STK_PNT32 startPnt, endPnt;
				Stk_ArcPtr::DynamicCast(curvePtr)->GetCoordinatePnt(startPnt, endPnt);
				string strPnt = "";
				sprintf(szPnt, "%.8f %.8f %.8f", startPnt.X, startPnt.Y, startPnt.Z);
				strPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_STARTPNT, strPnt, jsonDoc.GetAllocator());
				sprintf(szPnt, "%.8f %.8f %.8f", endPnt.X, endPnt.Y, endPnt.Z);
				strPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_ENDPNT, strPnt, jsonDoc.GetAllocator());

				STK_FLOAT32 fStartPar, fEndPar;
				Stk_ArcPtr::DynamicCast(curvePtr)->GetParameter(fStartPar, fEndPar);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_STARTPAR, fStartPar, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_ENDPAR, fEndPar, jsonDoc.GetAllocator());

				STK_DIR32 dirNormal = Stk_ArcPtr::DynamicCast(curvePtr)->GetNormal();
				sprintf(szDir, "%.8f %.8f %.8f", dirNormal.X, dirNormal.Y, dirNormal.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_NORMAL, strDir, jsonDoc.GetAllocator());

				STK_DIR32 dirOrigin = Stk_ArcPtr::DynamicCast(curvePtr)->GetOriginDir();
				sprintf(szDir, "%.8f %.8f %.8f", dirOrigin.X, dirOrigin.Y, dirOrigin.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_ORIGIN, strDir, jsonDoc.GetAllocator());
			}
			break;
			case CURVE_TYPE_LINE:
			{
				STK_PNT32 startPnt = Stk_LinePtr::DynamicCast(curvePtr)->GetStartPoint();
				STK_PNT32 endPnt = Stk_LinePtr::DynamicCast(curvePtr)->GetEndPoint();
				char szPnt[128] = { 0 };
				sprintf(szPnt, "%.8f %.8f %.8f", startPnt.X, startPnt.Y, startPnt.Z);
				string strStartPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_STARTPNT, strStartPnt, jsonDoc.GetAllocator());
				sprintf(szPnt, "%.8f %.8f %.8f", endPnt.X, endPnt.Y, endPnt.Z);
				string strEndPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_ENDPNT, strEndPnt, jsonDoc.GetAllocator());
			}
				break;
			case CURVE_TYPE_NURBSCURVE:
			{
				STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
				Stk_NurbsCurvePtr::DynamicCast(curvePtr)->GetDomain(fUMin, fUMax);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MIN, fUMin, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MAX, fUMax, jsonDoc.GetAllocator());

				STK_UINT32 iDegree = Stk_NurbsCurvePtr::DynamicCast(curvePtr)->GetDegree();
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DEGREE, iDegree, jsonDoc.GetAllocator());

				vector<STK_UINT32> vecknotPlicities = Stk_NurbsCurvePtr::DynamicCast(curvePtr)->GetKnotMultiplicity();
				string strPlicities = "";
				for (int i = 0; i < vecknotPlicities.size(); i++)
				{
					char szPlicities[32] = { 0 };
					if (strPlicities.length() > 0)
					{
						sprintf(szPlicities, " %d", vecknotPlicities[i]);
					}
					else
					{
						sprintf(szPlicities, "%d", vecknotPlicities[i]);
					}
					strPlicities.append(szPlicities);
				}
				if (strPlicities.length() > 0)
				{
					valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_KNOTPLICITIES, strPlicities, jsonDoc.GetAllocator());
				}

				vector<STK_FLOAT32> vecKnots = Stk_NurbsCurvePtr::DynamicCast(curvePtr)->GetKnots();
				string strKnots = "";
				for (int i = 0; i < vecKnots.size(); i++)
				{
					char szKnots[32] = { 0 };
					if (strKnots.length() > 0)
					{
						sprintf(szKnots, " %.8f", vecKnots[i]);
					}
					else
					{
						sprintf(szKnots, "%.8f", vecKnots[i]);
					}
					strKnots.append(szKnots);
				}
				if (strKnots.length() > 0)
				{
					valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_KNOTS, strKnots, jsonDoc.GetAllocator());
				}

				string strPnt = "";
				vector<STK_PNT32> vecPoints = Stk_NurbsCurvePtr::DynamicCast(curvePtr)->GetPoints();
				for (int i = 0; i < vecPoints.size(); i++)
				{
					if (strPnt.length() > 0)
						strPnt.append(" ");
					char szPnt[128] = { 0 };
					sprintf(szPnt, "%.8f %.8f %.8f", vecPoints[i].X, vecPoints[i].Y, vecPoints[i].Z);
					strPnt.append(szPnt);
				}
				if (strPnt.length() > 0)
				{
					valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_POINTS, strPnt, jsonDoc.GetAllocator());
				}

				vector<STK_FLOAT32> vecWeights = Stk_NurbsCurvePtr::DynamicCast(curvePtr)->GetWeights();
				string strWeights = "";
				for (int i = 0; i < vecWeights.size(); i++)
				{
					char szWeights[32] = { 0 };
					if (strWeights.length() > 0)
					{
						sprintf(szWeights, " %.8f", vecWeights[i]);
					}
					else
					{
						sprintf(szWeights, "%.8f", vecWeights[i]);
					}
					strWeights.append(szWeights);
				}
				if (strWeights.length() > 0)
				{
					valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_WEIGHTS, strWeights, jsonDoc.GetAllocator());
				}
			}
			break;
			case CURVE_TYPE_HYPERBOLA:
			{
				STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
				Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetDomain(fUMin, fUMax);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MIN, fUMin, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MAX, fUMax, jsonDoc.GetAllocator());

				STK_PNT32 centerPnt = Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetCenterPoint();
				char szPnt[128] = { 0 };
				sprintf(szPnt, "%.8f %.8f %.8f", centerPnt.X, centerPnt.Y, centerPnt.Z);
				string strCenterPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_CENTERPOINT, strColor, jsonDoc.GetAllocator());

				STK_DIR32 dirX, dirY, dirZ;
				Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetXYZDir(dirX, dirY, dirZ);
				string strDir = "";
				char szDir[128] = { 0 };
				sprintf(szDir, "%.8f %.8f %.8f", dirX.X, dirX.Y, dirX.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_X, strDir, jsonDoc.GetAllocator());
				sprintf(szDir, "%.8f %.8f %.8f", dirY.X, dirY.Y, dirY.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_Y, strDir, jsonDoc.GetAllocator());
				sprintf(szDir, "%.8f %.8f %.8f", dirZ.X, dirZ.Y, dirZ.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_Z, strDir, jsonDoc.GetAllocator());

				STK_FLOAT32 fSemiAxis = Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetSemiAxis();
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_SEMIAXIS, fSemiAxis, jsonDoc.GetAllocator());
				STK_FLOAT32 fSemiImgAxis = Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetSemiImgAxis();
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_SEMIIMGAXIS, fSemiImgAxis, jsonDoc.GetAllocator());

				STK_PNT32 startPnt, endPnt;
				startPnt = Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetStartPoint();
				endPnt = Stk_HyperbolaPtr::DynamicCast(curvePtr)->GetEndPoint();
				string strPnt = "";
				sprintf(szPnt, "%.8f %.8f %.8f", startPnt.X, startPnt.Y, startPnt.Z);
				strPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_STARTPNT, strPnt, jsonDoc.GetAllocator());
				sprintf(szPnt, "%.8f %.8f %.8f", endPnt.X, endPnt.Y, endPnt.Z);
				strPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_ENDPNT, strPnt, jsonDoc.GetAllocator());
			}
				break;
			case CURVE_TYPE_PARABOLA:
			{
				STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
				Stk_ParabolaPtr::DynamicCast(curvePtr)->GetDomain(fUMin, fUMax);
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MIN, fUMin, jsonDoc.GetAllocator());
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_MAX, fUMax, jsonDoc.GetAllocator());

				STK_PNT32 centerPnt = Stk_ParabolaPtr::DynamicCast(curvePtr)->GetCenterPoint();
				char szPnt[128] = { 0 };
				sprintf(szPnt, "%.8f %.8f %.8f", centerPnt.X, centerPnt.Y, centerPnt.Z);
				string strCenterPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_CENTERPOINT, strColor, jsonDoc.GetAllocator());

				STK_DIR32 dirX, dirY, dirZ;
				Stk_ParabolaPtr::DynamicCast(curvePtr)->GetXYZDir(dirX, dirY, dirZ);
				string strDir = "";
				char szDir[128] = { 0 };
				sprintf(szDir, "%.8f %.8f %.8f", dirX.X, dirX.Y, dirX.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_X, strDir, jsonDoc.GetAllocator());
				sprintf(szDir, "%.8f %.8f %.8f", dirY.X, dirY.Y, dirY.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_Y, strDir, jsonDoc.GetAllocator());
				sprintf(szDir, "%.8f %.8f %.8f", dirZ.X, dirZ.Y, dirZ.Z);
				strDir = szDir;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_DIR_Z, strDir, jsonDoc.GetAllocator());

				STK_FLOAT32 fFocalDistance = Stk_ParabolaPtr::DynamicCast(curvePtr)->GetFocalDistance();
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_FOCALDIS, fFocalDistance, jsonDoc.GetAllocator());

				STK_PNT32 startPnt, endPnt;
				startPnt = Stk_ParabolaPtr::DynamicCast(curvePtr)->GetStartPoint();
				endPnt = Stk_ParabolaPtr::DynamicCast(curvePtr)->GetEndPoint();
				string strPnt = "";
				sprintf(szPnt, "%.8f %.8f %.8f", startPnt.X, startPnt.Y, startPnt.Z);
				strPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_STARTPNT, strPnt, jsonDoc.GetAllocator());
				sprintf(szPnt, "%.8f %.8f %.8f", endPnt.X, endPnt.Y, endPnt.Z);
				strPnt = szPnt;
				valCurve.AddMember(LINEDOC_NODE_NAME_CURVE_ENDPNT, strPnt, jsonDoc.GetAllocator());
			}
				break;
			default:
				break;
			}
			return result;
		}

		Stk_CurveEntityPtr CRapidJsonTools::CurveFromJsonValue(Value& valCurve, Document& jsonDoc)
		{
			Stk_CurveEntityPtr curvePtr = nullptr;

			StkCurveTypeEnum curveType = CURVE_TYPE_UNKNOWN;
			if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_TYPE))
			{
				curveType = (StkCurveTypeEnum)valCurve[LINEDOC_NODE_NAME_CURVE_TYPE].GetInt();
			}

			switch (curveType)
			{
			case CURVE_TYPE_UNKNOWN:
			case CURVE_TYPE_POLYLINE:
			{
				Stk_PolyLinePtr polyLinePtr = Stk_PolyLinePtr::CreateObject();
				curvePtr = polyLinePtr;
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MIN) && 
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MAX))
				{
					 STK_FLOAT32 fMin = valCurve[LINEDOC_NODE_NAME_CURVE_MIN].GetFloat();
					 STK_FLOAT32 fMax = valCurve[LINEDOC_NODE_NAME_CURVE_MAX].GetFloat();
					 polyLinePtr->SetRange(fMin, fMax);
				}
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_POINTS))
				{
					string strPoints = valCurve[LINEDOC_NODE_NAME_CURVE_POINTS].GetString();
					std::vector<std::string> vecPointsValue;
					CStringTools::StringSplit(strPoints, " ", vecPointsValue);
					std::vector<STK_PNT32> vecPoints;
					for(int i =0; i< vecPointsValue.size()/3; i++)
					{
						STK_PNT32 pnt;
						pnt.X = atof(vecPointsValue[i*3].c_str());
						pnt.Y = atof(vecPointsValue[i * 3 + 1].c_str());
						pnt.Z = atof(vecPointsValue[i * 3 + 2].c_str());
						vecPoints.push_back(pnt);
					}
					polyLinePtr->SetPoints(vecPoints);
				}
			}
			break;
			case CURVE_TYPE_ELLIPSE:
			{
				Stk_ArcPtr arcPtr = Stk_ArcPtr::CreateObject();
				curvePtr = arcPtr;
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MIN) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MAX))
				{
					STK_FLOAT32 fMin = valCurve[LINEDOC_NODE_NAME_CURVE_MIN].GetFloat();
					STK_FLOAT32 fMax = valCurve[LINEDOC_NODE_NAME_CURVE_MAX].GetFloat();
					arcPtr->SetRange(fMin, fMax);
				}
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_CENTERPOINT))
				{
					string strCenterPoint = valCurve[LINEDOC_NODE_NAME_CURVE_CENTERPOINT].GetString();
					std::vector<std::string> vecCenterPointValue;
					CStringTools::StringSplit(strCenterPoint, " ", vecCenterPointValue);
					if (vecCenterPointValue.size() == 3)
					{
						STK_PNT32 centerPnt;
						centerPnt.X = atof(vecCenterPointValue[0].c_str());
						centerPnt.Y = atof(vecCenterPointValue[1].c_str());
						centerPnt.Z = atof(vecCenterPointValue[2].c_str());
						arcPtr->SetCenterPoint(centerPnt);
					}
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_X) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_Y) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_Z))
				{
					STK_DIR32 dirX, dirY, dirZ;
					string strDir = "";
					std::vector<std::string> vecDirValue;

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_X].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirX.X = atof(vecDirValue[0].c_str());
						dirX.Y = atof(vecDirValue[1].c_str());
						dirX.Z = atof(vecDirValue[2].c_str());
					}

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_Y].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirY.X = atof(vecDirValue[0].c_str());
						dirY.Y = atof(vecDirValue[1].c_str());
						dirY.Z = atof(vecDirValue[2].c_str());
					}

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_Z].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirZ.X = atof(vecDirValue[0].c_str());
						dirZ.Y = atof(vecDirValue[1].c_str());
						dirZ.Z = atof(vecDirValue[2].c_str());
					}
					arcPtr->SetXYZDir(dirX, dirY, dirZ);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MAJORRADIUS) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MINORRADIUS))
				{
					STK_FLOAT32 fMajorR, fMinorR;
					fMajorR = valCurve[LINEDOC_NODE_NAME_CURVE_MAJORRADIUS].GetFloat();
					fMinorR = valCurve[LINEDOC_NODE_NAME_CURVE_MINORRADIUS].GetFloat();
					arcPtr->SetRadius(fMajorR, fMinorR);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_STARTPNT) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_ENDPNT))
				{
					STK_PNT32 startPnt, endPnt;
					string strPnt = "";
					std::vector<std::string> vecPntValue;

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_STARTPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						startPnt.X = atof(vecPntValue[0].c_str());
						startPnt.Y = atof(vecPntValue[1].c_str());
						startPnt.Z = atof(vecPntValue[2].c_str());
					}

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_ENDPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						endPnt.X = atof(vecPntValue[0].c_str());
						endPnt.Y = atof(vecPntValue[1].c_str());
						endPnt.Z = atof(vecPntValue[2].c_str());
					}

					arcPtr->SetCoordiantePnt(startPnt, endPnt);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_STARTPAR) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_ENDPAR))
				{
					STK_FLOAT32 fStartPar, fEndPar;
					fStartPar = valCurve[LINEDOC_NODE_NAME_CURVE_STARTPAR].GetFloat();
					fEndPar = valCurve[LINEDOC_NODE_NAME_CURVE_ENDPAR].GetFloat();
					arcPtr->SetParameter(fStartPar, fEndPar);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_NORMAL))
				{
					string strNormal = "";
					std::vector<std::string> vecNormalValue;

					strNormal = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_NORMAL].GetString();
					CStringTools::StringSplit(strNormal, " ", vecNormalValue);
					if (vecNormalValue.size() == 3)
					{
						STK_DIR32 dirNormal;
						dirNormal.X = atof(vecNormalValue[0].c_str());
						dirNormal.Y = atof(vecNormalValue[1].c_str());
						dirNormal.Z = atof(vecNormalValue[2].c_str());
						arcPtr->SetNormal(dirNormal);
					}
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_ORIGIN))
				{
					string strOrigin = "";
					std::vector<std::string> vecOriginValue;

					strOrigin = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_ORIGIN].GetString();
					CStringTools::StringSplit(strOrigin, " ", vecOriginValue);
					if (vecOriginValue.size() == 3)
					{
						STK_DIR32 dirOrigin;
						dirOrigin.X = atof(vecOriginValue[0].c_str());
						dirOrigin.Y = atof(vecOriginValue[1].c_str());
						dirOrigin.Z = atof(vecOriginValue[2].c_str());
						arcPtr->SetOriginDir(dirOrigin);
					}
				}
			}
			break;
			case CURVE_TYPE_LINE:
			{
				Stk_LinePtr linePtr = Stk_LinePtr::CreateObject();
				curvePtr = linePtr;

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_STARTPNT) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_ENDPNT))
				{
					STK_PNT32 startPnt, endPnt;
					string strPnt = "";
					std::vector<std::string> vecPntValue;

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_STARTPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						startPnt.X = atof(vecPntValue[0].c_str());
						startPnt.Y = atof(vecPntValue[1].c_str());
						startPnt.Z = atof(vecPntValue[2].c_str());
					}

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_ENDPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						endPnt.X = atof(vecPntValue[0].c_str());
						endPnt.Y = atof(vecPntValue[1].c_str());
						endPnt.Z = atof(vecPntValue[2].c_str());
					}

					linePtr->SetStartPoint(startPnt);
					linePtr->SetEndPoint(endPnt);
				}
			}
			break;
			case CURVE_TYPE_NURBSCURVE:
			{
				Stk_NurbsCurvePtr nurbsCurvePtr = Stk_NurbsCurvePtr::CreateObject();
				curvePtr = nurbsCurvePtr;

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MIN) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MAX))
				{
					STK_FLOAT32 fUMin = 0.0f, fUMax = 0.0f;
					
					fUMin = valCurve[LINEDOC_NODE_NAME_CURVE_MIN].GetFloat();
					fUMax = valCurve[LINEDOC_NODE_NAME_CURVE_MAX].GetFloat();
					
					nurbsCurvePtr->SetDomain(fUMin, fUMax);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DEGREE))
				{
					nurbsCurvePtr->SetDegree(valCurve[LINEDOC_NODE_NAME_CURVE_MAX].GetInt());
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_KNOTPLICITIES))
				{
					string strKnotMultiplicity = valCurve[LINEDOC_NODE_NAME_CURVE_KNOTPLICITIES].GetString();
					std::vector<std::string> vecKnotMultiplicityValue;

					CStringTools::StringSplit(strKnotMultiplicity, " ", vecKnotMultiplicityValue);
					for (int i = 0; i < vecKnotMultiplicityValue.size(); i++)
					{
						nurbsCurvePtr->AddKnotMultiplicity(atoi(vecKnotMultiplicityValue[i].c_str()));
					}
					
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_KNOTS))
				{
					string strKnots = valCurve[LINEDOC_NODE_NAME_CURVE_KNOTS].GetString();
					std::vector<std::string> vecKnotsValue;

					CStringTools::StringSplit(strKnots, " ", vecKnotsValue);
					for (int i = 0; i < vecKnotsValue.size(); i++)
					{
						nurbsCurvePtr->AddKnot(atof(vecKnotsValue[i].c_str()));
					}
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_POINTS))
				{
					string strPoints = valCurve[LINEDOC_NODE_NAME_CURVE_POINTS].GetString();
					std::vector<std::string> vecPointsValue;
					CStringTools::StringSplit(strPoints, " ", vecPointsValue);
					for (int i = 0; i < vecPointsValue.size() / 3; i++)
					{
						STK_PNT32 pnt;
						pnt.X = atof(vecPointsValue[i * 3].c_str());
						pnt.Y = atof(vecPointsValue[i * 3 + 1].c_str());
						pnt.Z = atof(vecPointsValue[i * 3 + 2].c_str());
						nurbsCurvePtr->AddPoint(pnt);
					}
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_WEIGHTS))
				{
					string strWeight = valCurve[LINEDOC_NODE_NAME_CURVE_WEIGHTS].GetString();
					std::vector<std::string> vecWeightValue;

					CStringTools::StringSplit(strWeight, " ", vecWeightValue);
					for (int i = 0; i < vecWeightValue.size(); i++)
					{
						nurbsCurvePtr->AddWeight(atof(vecWeightValue[i].c_str()));
					}
				}
			}
			break;
			case CURVE_TYPE_HYPERBOLA:
			{
				Stk_HyperbolaPtr hyperbolaPtr = Stk_HyperbolaPtr::CreateObject();
				curvePtr = hyperbolaPtr;
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MIN) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MAX))
				{
					STK_FLOAT32 fMin = valCurve[LINEDOC_NODE_NAME_CURVE_MIN].GetFloat();
					STK_FLOAT32 fMax = valCurve[LINEDOC_NODE_NAME_CURVE_MAX].GetFloat();
					hyperbolaPtr->SetDomain(fMin, fMax);
				}
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_CENTERPOINT))
				{
					string strCenterPoint = valCurve[LINEDOC_NODE_NAME_CURVE_CENTERPOINT].GetString();
					std::vector<std::string> vecCenterPointValue;
					CStringTools::StringSplit(strCenterPoint, " ", vecCenterPointValue);
					if (vecCenterPointValue.size() == 3)
					{
						STK_PNT32 centerPnt;
						centerPnt.X = atof(vecCenterPointValue[0].c_str());
						centerPnt.Y = atof(vecCenterPointValue[1].c_str());
						centerPnt.Z = atof(vecCenterPointValue[2].c_str());
						hyperbolaPtr->SetCenterPoint(centerPnt);
					}
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_X) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_Y) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_Z))
				{
					STK_DIR32 dirX, dirY, dirZ;
					string strDir = "";
					std::vector<std::string> vecDirValue;

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_X].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirX.X = atof(vecDirValue[0].c_str());
						dirX.Y = atof(vecDirValue[1].c_str());
						dirX.Z = atof(vecDirValue[2].c_str());
					}

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_Y].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirY.X = atof(vecDirValue[0].c_str());
						dirY.Y = atof(vecDirValue[1].c_str());
						dirY.Z = atof(vecDirValue[2].c_str());
					}

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_Z].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirZ.X = atof(vecDirValue[0].c_str());
						dirZ.Y = atof(vecDirValue[1].c_str());
						dirZ.Z = atof(vecDirValue[2].c_str());
					}
					hyperbolaPtr->SetXYZDir(dirX, dirY, dirZ);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_SEMIAXIS))
				{
					STK_FLOAT32 fSemiAxix = valCurve[LINEDOC_NODE_NAME_CURVE_SEMIAXIS].GetFloat();
					hyperbolaPtr->SetSemiAxis(fSemiAxix);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_SEMIIMGAXIS))
				{
					STK_FLOAT32 fSemiImgAxix = valCurve[LINEDOC_NODE_NAME_CURVE_SEMIIMGAXIS].GetFloat();
					hyperbolaPtr->SetSemiImgAxis(fSemiImgAxix);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_STARTPNT) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_ENDPNT))
				{
					STK_PNT32 startPnt, endPnt;
					string strPnt = "";
					std::vector<std::string> vecPntValue;

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_STARTPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						startPnt.X = atof(vecPntValue[0].c_str());
						startPnt.Y = atof(vecPntValue[1].c_str());
						startPnt.Z = atof(vecPntValue[2].c_str());
					}

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_ENDPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						endPnt.X = atof(vecPntValue[0].c_str());
						endPnt.Y = atof(vecPntValue[1].c_str());
						endPnt.Z = atof(vecPntValue[2].c_str());
					}
					hyperbolaPtr->SetStartPoint(startPnt);
					hyperbolaPtr->SetEndPoint( endPnt);
				}
			}
				break;
			case CURVE_TYPE_PARABOLA:
			{
				Stk_ParabolaPtr parabolaPtr = Stk_ParabolaPtr::CreateObject();
				curvePtr = parabolaPtr;
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MIN) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_MAX))
				{
					STK_FLOAT32 fMin = valCurve[LINEDOC_NODE_NAME_CURVE_MIN].GetFloat();
					STK_FLOAT32 fMax = valCurve[LINEDOC_NODE_NAME_CURVE_MAX].GetFloat();
					parabolaPtr->SetDomain(fMin, fMax);
				}
				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_CENTERPOINT))
				{
					string strCenterPoint = valCurve[LINEDOC_NODE_NAME_CURVE_CENTERPOINT].GetString();
					std::vector<std::string> vecCenterPointValue;
					CStringTools::StringSplit(strCenterPoint, " ", vecCenterPointValue);
					if (vecCenterPointValue.size() == 3)
					{
						STK_PNT32 centerPnt;
						centerPnt.X = atof(vecCenterPointValue[0].c_str());
						centerPnt.Y = atof(vecCenterPointValue[1].c_str());
						centerPnt.Z = atof(vecCenterPointValue[2].c_str());
						parabolaPtr->SetCenterPoint(centerPnt);
					}
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_X) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_Y) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DIR_Z))
				{
					STK_DIR32 dirX, dirY, dirZ;
					string strDir = "";
					std::vector<std::string> vecDirValue;

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_X].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirX.X = atof(vecDirValue[0].c_str());
						dirX.Y = atof(vecDirValue[1].c_str());
						dirX.Z = atof(vecDirValue[2].c_str());
					}

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_Y].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirY.X = atof(vecDirValue[0].c_str());
						dirY.Y = atof(vecDirValue[1].c_str());
						dirY.Z = atof(vecDirValue[2].c_str());
					}

					strDir = valCurve[LINEDOC_NODE_NAME_CURVE_DIR_Z].GetString();
					CStringTools::StringSplit(strDir, " ", vecDirValue);
					if (vecDirValue.size() == 3)
					{
						dirZ.X = atof(vecDirValue[0].c_str());
						dirZ.Y = atof(vecDirValue[1].c_str());
						dirZ.Z = atof(vecDirValue[2].c_str());
					}
					parabolaPtr->SetXYZDir(dirX, dirY, dirZ);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_FOCALDIS))
				{
					STK_FLOAT32 fFocalDistance = valCurve[LINEDOC_NODE_NAME_CURVE_FOCALDIS].GetFloat();
					parabolaPtr->SetFocalDistance(fFocalDistance);
				}

				if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_STARTPNT) &&
					valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_ENDPNT))
				{
					STK_PNT32 startPnt, endPnt;
					string strPnt = "";
					std::vector<std::string> vecPntValue;

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_STARTPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						startPnt.X = atof(vecPntValue[0].c_str());
						startPnt.Y = atof(vecPntValue[1].c_str());
						startPnt.Z = atof(vecPntValue[2].c_str());
					}

					strPnt = valCurve[LINEDOC_NODE_NAME_CURVE_ENDPNT].GetString();
					CStringTools::StringSplit(strPnt, " ", vecPntValue);
					if (vecPntValue.size() == 3)
					{
						endPnt.X = atof(vecPntValue[0].c_str());
						endPnt.Y = atof(vecPntValue[1].c_str());
						endPnt.Z = atof(vecPntValue[2].c_str());
					}
					parabolaPtr->SetStartPoint(startPnt);
					parabolaPtr->SetEndPoint(endPnt);
				}
			}
				break;
			default:
				break;
			}

			if (curvePtr == nullptr)
				return curvePtr;

			if (valCurve.HasMember(LINEDOC_NODE_NAME_ID))
			{
				curvePtr->SetID(valCurve[LINEDOC_NODE_NAME_ID].GetInt());
			}
			if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_TYPE))
			{
				curvePtr->SetCurveType((StkCurveTypeEnum)valCurve[LINEDOC_NODE_NAME_CURVE_TYPE].GetInt());
			}
			if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_DISPLAYTYPE))
			{
				curvePtr->SetCurveDispType((StkCurveDispTypeEnum)valCurve[LINEDOC_NODE_NAME_CURVE_DISPLAYTYPE].GetInt());
			}
			if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_USAGETYPE))
			{
				curvePtr->SetUsageType((StkCurveUsageEnum)valCurve[LINEDOC_NODE_NAME_CURVE_USAGETYPE].GetInt());
			}
			if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_VISIBLE))
			{
				curvePtr->SetCurveDispStat(valCurve[LINEDOC_NODE_NAME_CURVE_VISIBLE].GetInt());
			}
			if (valCurve.HasMember(LINEDOC_NODE_NAME_CURVE_COLOR))
			{
				STK_RGBA32 curveColor;
				string strCurveColor = valCurve[LINEDOC_NODE_NAME_CURVE_COLOR].GetString();
				std::vector<std::string> vecCurveColorValue;
				CStringTools::StringSplit(strCurveColor, " ", vecCurveColorValue);
				if (vecCurveColorValue.size() == 4)
				{
					curveColor.Red = atof(vecCurveColorValue[0].c_str());
					curveColor.Green = atof(vecCurveColorValue[1].c_str());
					curveColor.Blue = atof(vecCurveColorValue[2].c_str());
					curveColor.Alpha = atof(vecCurveColorValue[3].c_str());
				}
				curvePtr->SetColor(curveColor);
			}
			return curvePtr;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::SpacialPointToJsonValue(Stk_SpacialPointEntityPtr spacialPointPtr, Value& valSpacialPoint, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (spacialPointPtr == nullptr)
				return STK_ERROR;

			valSpacialPoint.SetObject();
			valSpacialPoint.AddMember(LINEDOC_NODE_NAME_ID, spacialPointPtr->GetID(), jsonDoc.GetAllocator());
			valSpacialPoint.AddMember(LINEDOC_NODE_NAME_POINTS_TYPE, spacialPointPtr->GetType(), jsonDoc.GetAllocator());
			//valSpacialPoint.AddMember(LINEDOC_NODE_NAME_POINTS_NAME, spacialPointPtr->GetName(), jsonDoc.GetAllocator());
			valSpacialPoint.AddMember(LINEDOC_NODE_NAME_POINTS_VISIBLE, spacialPointPtr->GetDispStat(), jsonDoc.GetAllocator());
			STK_RGBA32 spacialPointColor = spacialPointPtr->GetColor();
			char szColor[128] = { 0 };
			sprintf(szColor, "%.8f %.8f %.8f %.8f", spacialPointColor.Red, spacialPointColor.Green, spacialPointColor.Blue, spacialPointColor.Alpha);
			string strColor = szColor;
			valSpacialPoint.AddMember(LINEDOC_NODE_NAME_POINTS_COLOR, strColor, jsonDoc.GetAllocator());

			STK_PNT32 pntPos = spacialPointPtr->GetPntPosition();
			char szPnt[128] = { 0 };
			sprintf(szPnt, "%.8f %.8f %.8f", pntPos.X, pntPos.Y, pntPos.Z);
			string strPos = szPnt;
			valSpacialPoint.AddMember(LINEDOC_NODE_NAME_POINTS_POS, strPos, jsonDoc.GetAllocator());
            return result;
		}

		STK_STATUS CRapidJsonTools::SpacialPointFromJsonValue(Stk_SpacialPointEntityPtr pSpacialPointEntityPtr, Value& valSpacialPoint, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (pSpacialPointEntityPtr == nullptr || valSpacialPoint.IsNull())
				return result;

			if (valSpacialPoint.HasMember(LINEDOC_NODE_NAME_ID))
			{
				pSpacialPointEntityPtr->SetID(valSpacialPoint[LINEDOC_NODE_NAME_ID].GetInt());
			}

			if (valSpacialPoint.HasMember(LINEDOC_NODE_NAME_POINTS_TYPE))
			{
				pSpacialPointEntityPtr->SetType((StkSpacialPointTypeEnum)valSpacialPoint[LINEDOC_NODE_NAME_POINTS_TYPE].GetInt());
			}

			if (valSpacialPoint.HasMember(LINEDOC_NODE_NAME_POINTS_VISIBLE))
			{
				pSpacialPointEntityPtr->SetDispStat(valSpacialPoint[LINEDOC_NODE_NAME_POINTS_VISIBLE].GetInt());
			}
			

			if (valSpacialPoint.HasMember(LINEDOC_NODE_NAME_POINTS_COLOR))
			{
				STK_RGBA32 color;
				string strColor = valSpacialPoint[LINEDOC_NODE_NAME_POINTS_COLOR].GetString();
				std::vector<std::string> vecColorValue;
				CStringTools::StringSplit(strColor, " ", vecColorValue);
				if (vecColorValue.size() == 4)
				{
					color.Red = atof(vecColorValue[0].c_str());
					color.Green = atof(vecColorValue[1].c_str());
					color.Blue = atof(vecColorValue[2].c_str());
					color.Alpha = atof(vecColorValue[3].c_str());
				}
				pSpacialPointEntityPtr->SetColor(color);
			}

			if (valSpacialPoint.HasMember(LINEDOC_NODE_NAME_POINTS_POS))
			{
				STK_PNT32 posPnt;
				string strPnt = valSpacialPoint[LINEDOC_NODE_NAME_POINTS_POS].GetString();
				std::vector<std::string> vecPntValue;
				CStringTools::StringSplit(strPnt, " ", vecPntValue);
				if (vecPntValue.size() == 3)
				{
					posPnt.X = atof(vecPntValue[0].c_str());
					posPnt.Y = atof(vecPntValue[1].c_str());
					posPnt.Z = atof(vecPntValue[2].c_str());
				}
				pSpacialPointEntityPtr->SetPntPosition(posPnt);
			}
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadLoadInf(const std::wstring& strFilePath, Stk_LoadInf* pLoadInfPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pLoadInfPtr == nullptr)
				return STK_ERROR;

			Document jsonDoc;
			result = ReadJsonFile(strFilePath, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析Info信息
			result = ParseLoadInf(jsonDoc, pLoadInfPtr);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadLoadInf(char ***i_bufP, const std::vector<int> &i_vcBufNum, Stk_LoadInf* pLoadInfPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pLoadInfPtr == nullptr)
				return STK_ERROR;

			Document jsonDoc;
			result = ReadJsonData(i_bufP,  i_vcBufNum,  jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析Info信息
			result = ParseLoadInf(jsonDoc, pLoadInfPtr);
			return result;
		}

		STK_STATUS CRapidJsonTools::ReadLoadAttr(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_AttributeCollectionPtr pAttrCollection)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pAttrCollection == nullptr)
				return STK_ERROR;
			Document jsonDoc;
			result = ReadJsonData(i_bufP, i_vcBufNum, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析Info信息
			result = ParseLoadAttr(jsonDoc, pAttrCollection);
			return result;
		}

		STK_STATUS CRapidJsonTools::ReadLoadGeoAttr(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_GeometryAttributeCollectionPtr pGeoAttrCollection)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pGeoAttrCollection == nullptr)
				return STK_ERROR;
			Document jsonDoc;
			result = ReadJsonData(i_bufP, i_vcBufNum, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析Geometry信息
			result = ParseLoadGeoAttr(jsonDoc, pGeoAttrCollection);
			return result;
		}

		STK_STATUS CRapidJsonTools::ReadLoadPMI(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_PMIDocumentPtr pPMIDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pPMIDoc == nullptr)
				return STK_ERROR;
			Document jsonDoc;
			result = ReadJsonData(i_bufP, i_vcBufNum, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析PMI信息
			result = ParseLoadPMI(jsonDoc, pPMIDoc);
			return result;
		}
       
		STK_STATUS CRapidJsonTools::ReadLoadView(char ***i_bufP, const std::vector<int>& i_vcBufNum, Stk_ViewDocumentPtr pViewDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pViewDoc == nullptr)
				return STK_ERROR;
			Document jsonDoc;
			result = ReadJsonData(i_bufP, i_vcBufNum, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析View信息
			result = ParseLoadView(jsonDoc, pViewDoc);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadLoadInf(char *i_bufP, const int i_bufSize, Stk_LoadInf* pLoadInfPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pLoadInfPtr == nullptr)
				return STK_ERROR;

			Document jsonDoc;
			result = ReadJsonData(i_bufP,  i_bufSize,  jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析Info信息
			result = ParseLoadInf(jsonDoc, pLoadInfPtr);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ParseLoadInf(Document& jsonDoc, Stk_LoadInf* pLoadInfPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if(jsonDoc.HasMember(LOADINFDOC_NODE_NAME_VERSION))
			{
				pLoadInfPtr->SetVersion(jsonDoc[LOADINFDOC_NODE_NAME_VERSION].GetString());
			}
			else
			{
				pLoadInfPtr->SetVersion(LOADINFDOC_CURRENT_VERSION);
			}


			Value::MemberIterator infoIter = jsonDoc.FindMember(LOADINFDOC_NODE_NAME_INFO);
			if ((infoIter != jsonDoc.MemberEnd() && infoIter->value.IsObject()))
			{
				Value* valInfo = &infoIter->value;

				//读取属性列�?
				Value::MemberIterator attributesIter = valInfo->FindMember(LOADINFDOC_NODE_NAME_ATTRIBUTES);
				if ((attributesIter != valInfo->MemberEnd() && attributesIter->value.IsObject()))
				{
					Value* valAttributes = &attributesIter->value;
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_BOUNDINGBOX))
					{
						float boundingBox[2][3];
						string strBoundingBox = (*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_BOUNDINGBOX].GetString();
						std::vector<std::string> vecBoundingBoxValue;
						CStringTools::StringSplit(strBoundingBox, " ", vecBoundingBoxValue);
						if (vecBoundingBoxValue.size() == 6)
						{
							boundingBox[0][0] = atof(vecBoundingBoxValue[0].c_str());
							boundingBox[0][1] = atof(vecBoundingBoxValue[1].c_str());
							boundingBox[0][2] = atof(vecBoundingBoxValue[2].c_str());
							boundingBox[1][0] = atof(vecBoundingBoxValue[3].c_str());
							boundingBox[1][1] = atof(vecBoundingBoxValue[4].c_str());
							boundingBox[1][2] = atof(vecBoundingBoxValue[5].c_str());
						}
						pLoadInfPtr->SetBoundingBox(boundingBox);
					}

					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_INSCOUNT))
					{
						pLoadInfPtr->SetInstanceCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_INSCOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MODELCOUNT))
					{
						pLoadInfPtr->SetModelCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_MODELCOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHCOUNT))
					{
						pLoadInfPtr->setMeshCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHCOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_FACECOUNT))
					{
						pLoadInfPtr->SetFaceCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_FACECOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHEDGECOUNT))
					{
						pLoadInfPtr->setMeshEdgeCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHEDGECOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_TRIANGLECOUNT))
					{
						pLoadInfPtr->SetTriangleCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_TRIANGLECOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_PMICOUNT))
					{
						pLoadInfPtr->SetPMICount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_PMICOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_VIEWCOUNT))
					{
						pLoadInfPtr->SetViewCount((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_VIEWCOUNT].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_FILESIZE))
					{
						pLoadInfPtr->SetFileSize((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_FILESIZE].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHDATASIZE))
					{
						pLoadInfPtr->setMeshDataSize((*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHDATASIZE].GetInt());
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_PREVIEWIMAGE))
					{
						string strPreviewImage = (*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_PREVIEWIMAGE].GetString();
						pLoadInfPtr->SetPreviewImage(strPreviewImage);
					}
					if (valAttributes->HasMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_UNIT))
					{
						string strUnit = (*valAttributes)[LOADINFDOC_NODE_NAME_ATTRIBUTE_UNIT].GetString();
						pLoadInfPtr->SetUnit(strUnit);
					}
				}
				/************************************************************************/
				/* 读取转换信息                                                                     */
				/************************************************************************/
				Value::MemberIterator converterIter = valInfo->FindMember(LOADINFDOC_NODE_NAME_CONVERTER);
				if ((converterIter != valInfo->MemberEnd() && converterIter->value.IsObject()))
				{
					Value* valConverterP = &converterIter->value;
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_VERSION))
					{
						pLoadInfPtr->setConverterVersion((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_VERSION].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_CONVERTERTIME))
					{
						pLoadInfPtr->setConverterTime((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_CONVERTERTIME].GetInt());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_SOURCEFORMAT))
					{
						pLoadInfPtr->setSourceFormat((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_SOURCEFORMAT].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_SOURCEVERSION))
					{
						pLoadInfPtr->setSourceVersion((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_SOURCEVERSION].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_LINEARTOL))
					{
						pLoadInfPtr->setLinearTol((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_LINEARTOL].GetDouble());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_ANGULARTOL))
					{
						pLoadInfPtr->setAngularTol((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_ANGULARTOL].GetDouble());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_CONFIG))
					{
						pLoadInfPtr->setConvertConfig((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_CONFIG].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_BEGINTIME))
					{
						pLoadInfPtr->setConvertBeginTime((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_BEGINTIME].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_ENDTIME))
					{
						pLoadInfPtr->setConvertEndTime((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_ENDTIME].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_COMPUTERNAME))
					{
						pLoadInfPtr->setConvertComputerName((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_COMPUTERNAME].GetString());
					}
					if (valConverterP->HasMember(LOADINFDOC_NODE_NAME_CONVERTER_USERNAME))
					{
						pLoadInfPtr->setConvertUserName((*valConverterP)[LOADINFDOC_NODE_NAME_CONVERTER_USERNAME].GetString());
					}
				}
				//读取Loader信息
				Value::MemberIterator loaderIter = valInfo->FindMember(LOADINFDOC_NODE_NAME_LOADINF);
				if ((loaderIter != valInfo->MemberEnd() && loaderIter->value.IsObject()))
				{
					Value* valLoader = &loaderIter->value;

					if (valLoader->HasMember(LOADINFDOC_NODE_NAME_LOADHIDDEN))
					{
						pLoadInfPtr->SetIsLoadHidden((*valLoader)[LOADINFDOC_NODE_NAME_LOADHIDDEN].GetInt());
					}
					if (valLoader->HasMember(LOADINFDOC_NODE_NAME_DEFAULTGROUP))
					{
						pLoadInfPtr->SetDefaultGroupID((*valLoader)[LOADINFDOC_NODE_NAME_DEFAULTGROUP].GetInt());
					}

					//读取摄像�?
					Value::MemberIterator cameraIter = valLoader->FindMember(LOADINFDOC_NODE_NAME_CAMERA);
					if ((cameraIter != valLoader->MemberEnd() && cameraIter->value.IsObject()))
					{
						Stk_CameraPtr cameraPtr = Stk_CameraPtr::CreateObject();
						CameraToJsonValue(cameraPtr, cameraIter->value, jsonDoc);
						pLoadInfPtr->SetCamera(cameraPtr);
					}

					//读取模型分组
					Value::MemberIterator modelGroupsIter = valLoader->FindMember(LOADINFDOC_NODE_NAME_GROUPLIST);
					if ((modelGroupsIter != valLoader->MemberEnd() && modelGroupsIter->value.IsArray()))
					{
						Value::Array valModelGroups = (modelGroupsIter->value.GetArray());
						//Value::Array* valModelGroups = &(modelGroupsIter->value.GetArray());
						for (int i = 0; i < valModelGroups.Size(); i++)
						{
							Stk_ModelGroup* pModeGroup = new Stk_ModelGroup();
							ModelGroupFromJsonValue(pModeGroup, (valModelGroups)[i], jsonDoc);
							pLoadInfPtr->AddModelGroup(pModeGroup);
						}
					}
				}
				/************************************************************************/
				/* 读取场景节点信息                                                                     */
				/************************************************************************/
				//读取背景信息信息
				pLoadInfPtr->SetBackgroundType(0);
				pLoadInfPtr->SetBackGroundStyle(0);
				Value::MemberIterator sceneIter = valInfo->FindMember(LOADINFDOC_NODE_NAME_SCENE);
				if ((sceneIter != valInfo->MemberEnd() && sceneIter->value.IsObject()))
				{
					Value* sceneNode = &sceneIter->value;
					//读取背景
					Value::MemberIterator backgroundIter = sceneNode->FindMember(LOADINFDOC_NODE_NAME_BACKGROUND);
					if ((backgroundIter != sceneNode->MemberEnd() && backgroundIter->value.IsObject()))
					{
						Value* backgroundNode = &backgroundIter->value;
						if (backgroundNode->HasMember(LOADINFDOC_NODE_NAME_BACKGROUND_TYPE))
						{
							pLoadInfPtr->SetBackgroundType((*backgroundNode)[LOADINFDOC_NODE_NAME_BACKGROUND_TYPE].GetInt());
						}

						if (backgroundNode->HasMember(LOADINFDOC_NODE_NAME_BACKGROUND_STYLE))
						{
							pLoadInfPtr->SetBackGroundStyle((*backgroundNode)[LOADINFDOC_NODE_NAME_BACKGROUND_STYLE].GetInt());
						}

						pLoadInfPtr->GetBackgroundItems().clear();
						Value::MemberIterator itemsIter = backgroundNode->FindMember(LOADINFDOC_NODE_NAME_BACKGROUND_ITEMS);
						if (itemsIter != backgroundNode->MemberEnd() && itemsIter->value.IsObject())
						{
							Value& itemsNode = itemsIter->value;
							for (Value::MemberIterator it = itemsNode.MemberBegin(); it != itemsNode.MemberEnd(); it++) {
								string key = it->name.GetString();
								string val = it->value.GetString();
								pLoadInfPtr->SetBackgroundItem(key, val);
							}
						}

					}
				}
			}
			return result;
		}

		STK_STATUS CRapidJsonTools::ParseLoadAttr(Document& jsonDoc, Stk_AttributeCollectionPtr pAttrCollection)
		{
			//version
			Value::MemberIterator versionNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_VERSION);
			if (versionNodeIter != jsonDoc.MemberEnd())
			{
				string strVersion = versionNodeIter->value.GetString();
				wstring wstrVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
				pAttrCollection->SetVersion(wstrVersion);
			}
			//instances
			Value::MemberIterator instancesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_INSTANCES);
			if (instancesNodeIter != jsonDoc.MemberEnd())
			{
				Value valInstances = instancesNodeIter->value.GetArray();
				Value valInstance;
				for (int i = 0; i < valInstances.Size(); i++)
				{
					valInstance = valInstances[i];
					STK_ID stkID;
					Value::MemberIterator idNodeIter = valInstance.FindMember(ATTRIBUTE_NODE_NAME_ID);
					if(idNodeIter == valInstance.MemberEnd())
						continue;
					stkID = idNodeIter->value.GetInt();
					Value::MemberIterator memberIter = valInstance.MemberBegin();
					while (memberIter != valInstance.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName != L"id"){
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							pAttrCollection->AddInstanceAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
				//instancesNodeIter->value.Clear();
			}
			//models
			Value::MemberIterator modelsNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_MODELS);
			if (modelsNodeIter != jsonDoc.MemberEnd())
			{
				Value valModels = modelsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					STK_ID stkID;
					Value::MemberIterator idNodeIter = valModel.FindMember(ATTRIBUTE_NODE_NAME_ID);
					if (idNodeIter == valModel.MemberEnd())
						continue;
					stkID = idNodeIter->value.GetInt();
					Value::MemberIterator memberIter = valModel.MemberBegin();
					while (memberIter != valModel.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName != L"id")
						{
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							pAttrCollection->AddModelAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}

#if 0
			//meshFaces
			Value::MemberIterator meshFacesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_MESHFACES);
			if (meshFacesNodeIter != jsonDoc.MemberEnd())
			{
				Value valMeshFaces = meshFacesNodeIter->value.GetArray();
				Value valMeshFace;
				for (int i = 0; i < valMeshFaces.Size(); i++)
				{
					valMeshFace = valMeshFaces[i];
					STK_ID stkID;
					Value::MemberIterator memberIter = valMeshFace.MemberBegin();
					while (memberIter != valMeshFace.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName == L"id")
							stkID = memberIter->value.GetInt();
						else {
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							//pAttrCollection->AddMeshFaceAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}

			//meshEdges
			Value::MemberIterator meshEdgesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_MESHEDGES);
			if (meshEdgesNodeIter != jsonDoc.MemberEnd())
			{
				Value valMeshEdges = meshEdgesNodeIter->value.GetArray();
				Value valMeshEdge;
				for (int i = 0; i < valMeshEdges.Size(); i++)
				{
					valMeshEdge = valMeshEdges[i];
					STK_ID stkID;
					Value::MemberIterator memberIter = valMeshEdge.MemberBegin();
					while (memberIter != valMeshEdge.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName == L"id")
							stkID = memberIter->value.GetInt();
						else {
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							pAttrCollection->AddMeshFaceAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}
#endif
			return STK_SUCCESS;
		}

		STK_STATUS CRapidJsonTools::ParseLoadGeoAttr(Document& jsonDoc, Stk_GeometryAttributeCollectionPtr pGeoAttrCollection)
		{
			//version
			Value::MemberIterator versionNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_VERSION);
			if (versionNodeIter != jsonDoc.MemberEnd())
			{
				string strVersion = versionNodeIter->value.GetString();
				wstring wstrVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
				pGeoAttrCollection->SetVersion(wstrVersion);
			}
			//geometries
			Value::MemberIterator geometriesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_GEOMETRIES);
			if (geometriesNodeIter != jsonDoc.MemberEnd())
			{
				//models
				Value::MemberIterator modelsNodeIter = geometriesNodeIter->value.FindMember(ATTRIBUTEDOC_NODE_NAME_MODELS);
				if (modelsNodeIter != geometriesNodeIter->value.MemberEnd())
				{
					Value valModels = modelsNodeIter->value.GetArray();
					Value valModel;
					for (int i = 0; i < valModels.Size(); i++)
					{
						valModel = valModels[i];
						STK_ID stkModelID;
						Value::MemberIterator idNodeIter = valModel.FindMember(ATTRIBUTE_NODE_NAME_ID);
						if (idNodeIter == valModel.MemberEnd())
							continue;
						stkModelID = idNodeIter->value.GetInt();
						//meshFaces
						Value::MemberIterator meshFacesIter = valModel.FindMember(ATTRIBUTEDOC_NODE_NAME_MESHFACES);
						if (meshFacesIter != valModel.MemberEnd())
						{
							//Value::MemberIterator geoIter = meshFacesIter->value.FindMember(ATTRIBUTEDOC_NODE_NAME_GEOMETRY);
							//if (geoIter != meshFacesIter->value.MemberEnd())
							//{
							Value::MemberIterator geoAttrIter = meshFacesIter->value.MemberBegin();
							while (geoAttrIter != meshFacesIter->value.MemberEnd())
							{
								STK_ID stkMeshFaceID = -1; 
								string strMeshFaceID = geoAttrIter->name.GetString();
								stkMeshFaceID = atoi(strMeshFaceID.c_str());
								Value::MemberIterator geoAttrValueIter = geoAttrIter->value.MemberBegin();
								while (geoAttrValueIter != geoAttrIter->value.MemberEnd())
								{
									wstring strName = ChineseCode::UTF_8ToUnicode(geoAttrValueIter->name.GetString());
									wstring strValue = ChineseCode::UTF_8ToUnicode(geoAttrValueIter->value.GetString());
									pGeoAttrCollection->AddMeshFaceAttribute(stkModelID, stkMeshFaceID, strName, strValue);
									geoAttrValueIter++;
								}
								geoAttrIter++;
							}
							//}
						}
						//meshEdges
						Value::MemberIterator meshEdgesIter = valModel.FindMember(ATTRIBUTEDOC_NODE_NAME_MESHEDGES);
						if (meshEdgesIter != valModel.MemberEnd())
						{
							//Value::MemberIterator geoIter = meshEdgesIter->value.FindMember(ATTRIBUTEDOC_NODE_NAME_GEOMETRY);
							//if (geoIter != meshEdgesIter->value.MemberEnd())
							//{
							Value::MemberIterator geoAttrIter = meshEdgesIter->value.MemberBegin();
							while (geoAttrIter != meshEdgesIter->value.MemberEnd())
							{
								STK_ID stkMeshEdgeID = -1;
								string strMeshEdgeID = geoAttrIter->name.GetString();
								stkMeshEdgeID = atoi(strMeshEdgeID.c_str());
								Value::MemberIterator geoAttrValueIter = geoAttrIter->value.MemberBegin();
								while (geoAttrValueIter != geoAttrIter->value.MemberEnd())
								{
									wstring strName = ChineseCode::UTF_8ToUnicode(geoAttrValueIter->name.GetString());
									wstring strValue = ChineseCode::UTF_8ToUnicode(geoAttrValueIter->value.GetString());
									pGeoAttrCollection->AddMeshEdgeAttribute(stkModelID, stkMeshEdgeID, strName, strValue);
									geoAttrValueIter++;
								}
								geoAttrIter++;
							}
							//}
						}
					}
				}
				//instances
			}

			return STK_SUCCESS;
		}

		STK_STATUS CRapidJsonTools::ParseLoadPMI(Document& jsonDoc, Stk_PMIDocumentPtr pPMIDoc)
		{
			Value::MemberIterator versionNodeIter = jsonDoc.FindMember(PMIDOC_NODE_NAME_VERSION);
			if (versionNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;
			string strVersion = versionNodeIter->value.GetString();
			wstring wstrVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
			pPMIDoc->SetVersion(wstrVersion);

			//PMIS
			Value::MemberIterator PMIsNodeIter = jsonDoc.FindMember(PMIDOC_NODE_NAME_PMIS);
			if (PMIsNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;
			Value PMIsValue = PMIsNodeIter->value.GetArray();
			Value valPmi;
			for (int i = 0; i < PMIsValue.Size(); i++)
			{
				valPmi = PMIsValue[i];
				Stk_PMIPtr pPmi = Stk_PMIPtr::CreateObject();
				pPmi->FromJson(&valPmi, &jsonDoc);
				pPMIDoc->AddPMI(pPmi);
			}
			//model_pmi
			Value::MemberIterator model_pmiNodeIter = jsonDoc.FindMember(PMIDOC_NODE_NAME_MODEL_PMI);
			if (model_pmiNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valModels = model_pmiNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					STK_ID modelID = -1;
					vector<STK_ID> vcPmi;
					Value::MemberIterator idIter = valModel.FindMember(PMIDOC_NODE_NAME_ID);
					if (idIter != valModel.MemberEnd())
						modelID = idIter->value.GetInt();
					Value::MemberIterator pmisIter = valModel.FindMember(PMIDOC_NODE_NAME_PMIS);
					if (pmisIter != valModel.MemberEnd())
					{
						Value pmiIDs = pmisIter->value.GetArray();
						Value pmiID;
						for (int j = 0; j < pmiIDs.Size(); j++)
						{
							pmiID = pmiIDs[j];
							vcPmi.push_back(pmiID.GetInt());
						}
					}
					pPMIDoc->AddPMI(modelID, vcPmi);
				}
			}
			return STK_SUCCESS;
		}

		STK_STATUS CRapidJsonTools::ParseLoadView(Document& jsonDoc, Stk_ViewDocumentPtr pViewDoc)
		{
			//根据jsonDoc中的内容初始化m_mapmodelIDToViewList
			Value::MemberIterator VersionNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VERSION);
			if (VersionNodeIter != jsonDoc.MemberEnd())
			{
				string strVersion = VersionNodeIter->value.GetString();
				wstring  wstrVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
				pViewDoc->SetVersion(wstrVersion);
			}

			Value::MemberIterator viewsNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VIEWS);
			if (viewsNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;
			Value viewsValue = viewsNodeIter->value.GetArray();
			Value valView;
			for (int i = 0; i < viewsValue.Size(); i++)
			{
				valView = viewsValue[i];
				Stk_ViewPtr pView = Stk_ViewPtr::CreateObject();
				pView->FromJson(&valView, &jsonDoc);
				pViewDoc->AddView(pView);
			}

			//model_view
			Value::MemberIterator model_viewsNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_MODEL_VIEW);
			if (model_viewsNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valModels = model_viewsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					STK_ID modelID = -1;
					vector<STK_ID> vcView;
					Value::MemberIterator idIter = valModel.FindMember(VIEWDOC_NODE_NAME_ID);
					if (idIter != valModel.MemberEnd())
						modelID = idIter->value.GetInt();
					Value::MemberIterator viewsIter = valModel.FindMember(VIEWDOC_NODE_NAME_VIEWS);
					if (viewsIter != valModel.MemberEnd())
					{
						Value viewIDs = viewsIter->value.GetArray();
						Value viewID;
						for (int j = 0; j < viewIDs.Size(); j++)
						{
							viewID = viewIDs[j];
							vcView.push_back(viewID.GetInt());
						}
					}
					pViewDoc->AddViews(modelID, vcView);
				}
			}
			//instance_views 暂时缺省

			map<STK_ID, Stk_ViewPtr> mapViewIdToViewPtr = pViewDoc->GetViewMap();
			//view_pmis
			Value::MemberIterator view_pmisNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VIEW_PMI);
			if (view_pmisNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valViews = view_pmisNodeIter->value.GetArray();
				Value valView;
				for (int i = 0; i < valViews.Size(); i++)
				{
					valView = valViews[i];
					STK_ID stkViewID = -1;
					Value::MemberIterator idIter = valView.FindMember(VIEWDOC_NODE_NAME_ID);
					if (idIter != valView.MemberEnd())
						stkViewID = idIter->value.GetInt();
					
					map<STK_ID, Stk_ViewPtr>::iterator itorViewIdToViewPtr = mapViewIdToViewPtr.find(stkViewID);
					if (itorViewIdToViewPtr == mapViewIdToViewPtr.end())
						continue;
					Stk_ViewPtr pView = itorViewIdToViewPtr->second;
					if (pView == nullptr)
						continue;
					Value::MemberIterator pmisIter = valView.FindMember(VIEWDOC_NODE_NAME_PMI);
					if (pmisIter != valView.MemberEnd())
					{
						Value valPmiIDs = pmisIter->value.GetArray();
						Value valPmiID;
						for (int j = 0; j < valPmiIDs.Size(); j++)
						{
							valPmiID = valPmiIDs[j];
							pView->AddConnector(ENTITY_TYPE_PMI, valPmiID.GetInt());
						}
					}
				}
			}

			//view_views
			Value::MemberIterator view_viewsNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VIEW_VIEW);
			if (view_viewsNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valViews = view_viewsNodeIter->value.GetArray();
				Value valView;
				for (int i = 0; i < valViews.Size(); i++)
				{
					valView = valViews[i];
					STK_ID stkViewID = -1;
					Value::MemberIterator idIter = valView.FindMember(VIEWDOC_NODE_NAME_ID);
					if (idIter != valView.MemberEnd())
						stkViewID = idIter->value.GetInt();
					map<STK_ID, Stk_ViewPtr>::iterator itorViewIdToViewPtr = mapViewIdToViewPtr.find(stkViewID);
					if (itorViewIdToViewPtr == mapViewIdToViewPtr.end())
						continue;
					Stk_ViewPtr pView = itorViewIdToViewPtr->second;
					if (pView == nullptr)
						continue;
					Value::MemberIterator linkViewIter = valView.FindMember(VIEWDOC_NODE_NAME_VIEW_LINKVIEW);
					if (linkViewIter != valView.MemberEnd())
					{
						Value valLinkVIewIDs = linkViewIter->value.GetArray();
						Value valLinkViewID;
						for (int j = 0; j < valLinkVIewIDs.Size(); j++)
						{
							valLinkViewID = valLinkVIewIDs[j];
							pView->AddConnector(ENTITY_TYPE_VIEW, valLinkViewID.GetInt());
						}
					}
				}
			}

			return STK_SUCCESS;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::WriteLoadInf(const std::wstring& strFilePath, Stk_LoadInf* pLoadInfPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pLoadInfPtr == nullptr)
				return STK_ERROR;

			Document jsonDoc;
			jsonDoc.SetObject();
			string strVersion;
			strVersion = LOADINFDOC_CURRENT_VERSION;
			jsonDoc.AddMember(LOADINFDOC_NODE_NAME_VERSION, strVersion, jsonDoc.GetAllocator());

			Value valInfo;
			valInfo.SetObject();
			/************************************************************************/
			/* 文件属性相关信�?                                                                    */
			/************************************************************************/
			Value valAttributes;
			valAttributes.SetObject();
			float fBox[2][3];
			pLoadInfPtr->GetBoundingBox(fBox);
			char szBoundingBox[256] = { 0 };
			sprintf(szBoundingBox, "%.8f %.8f %.8f %.8f %.8f %.8f", fBox[0][0], fBox[0][1], fBox[0][2], fBox[1][0], fBox[1][1], fBox[1][2]);
			string strBoundingBox = szBoundingBox;
			//valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_BOUNDINGBOX, strBoundingBox, jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_INSCOUNT, pLoadInfPtr->GetInstanceCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MODELCOUNT, pLoadInfPtr->GetModelCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHCOUNT, pLoadInfPtr->getMeshCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_FACECOUNT, pLoadInfPtr->GetFaceCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHEDGECOUNT, pLoadInfPtr->getMeshEdgeCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_TRIANGLECOUNT, pLoadInfPtr->GetTriangleCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_PMICOUNT, pLoadInfPtr->GetPMICount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_VIEWCOUNT, pLoadInfPtr->GetViewCount(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_FILESIZE, pLoadInfPtr->GetFileSize(), jsonDoc.GetAllocator());
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_MESHDATASIZE, pLoadInfPtr->getMeshDataSize(), jsonDoc.GetAllocator());
			string strPreImage = pLoadInfPtr->GetPreviewImage();
			string strPreImageUTF8 = "";
#ifdef _WIN32
			ChineseCode::GB2312ToUTF_8(strPreImageUTF8, (char*)strPreImage.c_str(), strPreImage.length());
#else
			strPreImageUTF8 = strPreImage;
#endif
			//valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_PREVIEWIMAGE, strPreImageUTF8, jsonDoc.GetAllocator());
			string strUnit = pLoadInfPtr->GetUnit();
			string strUnitUTF8 = "";
#ifdef _WIN32
			ChineseCode::GB2312ToUTF_8(strUnitUTF8, (char*)strUnit.c_str(), strUnit.length());
#else
			strUnitUTF8 = strUnit;
#endif
			valAttributes.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTE_UNIT, strUnitUTF8, jsonDoc.GetAllocator());
			valInfo.AddMember(LOADINFDOC_NODE_NAME_ATTRIBUTES, valAttributes, jsonDoc.GetAllocator());
			/************************************************************************/
			/* 转换器信�?                                                                    */
			/************************************************************************/
			Value valConverter;
			valConverter.SetObject();
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_VERSION, pLoadInfPtr->getConverterVersion(), jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_COMPUTERNAME, pLoadInfPtr->getConvertComputerName(), jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_USERNAME, pLoadInfPtr->getConvertUserName(), jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_CONVERTERTIME, pLoadInfPtr->getConverterTime(), jsonDoc.GetAllocator());
			string strSourceFormat = pLoadInfPtr->getSourceFormat();
			string strSourceFormatUTF8 = "";
#ifdef _WIN32
			ChineseCode::GB2312ToUTF_8(strSourceFormatUTF8, (char*)strSourceFormat.c_str(), strSourceFormat.length());
#else
			strSourceFormatUTF8 = strSourceFormat;
#endif
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_SOURCEFORMAT, strSourceFormatUTF8, jsonDoc.GetAllocator());
			string strSourceVersion = pLoadInfPtr->getSourceVersion();
			string strSourceVersionUTF8 = "";
#ifdef _WIN32
			ChineseCode::GB2312ToUTF_8(strSourceVersionUTF8, (char*)strSourceVersion.c_str(), strSourceVersion.length());
#else
			strSourceVersionUTF8 = strSourceVersion;
#endif
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_SOURCEVERSION, strSourceVersionUTF8, jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_BEGINTIME, pLoadInfPtr->getConvertBeginTime(), jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_ENDTIME, pLoadInfPtr->getConvertEndTime(), jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_CONFIG, pLoadInfPtr->getConvertConfig(), jsonDoc.GetAllocator());
			//valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_LINEARTOL, pLoadInfPtr->getLinearTol(), jsonDoc.GetAllocator());
			//valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_ANGULARTOL, pLoadInfPtr->getAngularTol(), jsonDoc.GetAllocator());
			valConverter.AddMember(LOADINFDOC_NODE_NAME_CONVERTER_CONFIG, pLoadInfPtr->getConvertConfig(), jsonDoc.GetAllocator());
			valInfo.AddMember(LOADINFDOC_NODE_NAME_CONVERTER, valConverter, jsonDoc.GetAllocator());
			/************************************************************************/
			/* 文件加载器信�?                                                                    */
			/************************************************************************/
			Value valLoader;
			valLoader.SetObject();
			valLoader.AddMember(LOADINFDOC_NODE_NAME_LOADHIDDEN, pLoadInfPtr->IsLoadHidden(), jsonDoc.GetAllocator());
			valLoader.AddMember(LOADINFDOC_NODE_NAME_DEFAULTGROUP, pLoadInfPtr->GetDefaultGroupID(), jsonDoc.GetAllocator());
			
			Stk_CameraPtr cameraPtr = pLoadInfPtr->GetCamera();
			if(cameraPtr != nullptr)
			{
				Value valCamera;
				CameraToJsonValue(cameraPtr, valCamera, jsonDoc);
				valLoader.AddMember(LOADINFDOC_NODE_NAME_CAMERA, valCamera, jsonDoc.GetAllocator());
			}
			Value valGroups;
			valGroups.SetArray();
			vector<Stk_ModelGroup*>& vecModelGroup = pLoadInfPtr->GetAllGroup();
			for(int i = 0; i < vecModelGroup.size(); i++)
			{
				if(!vecModelGroup[i])
					continue;
				Value valGroup;
				ModelGroupToJsonValue(vecModelGroup[i], valGroup, jsonDoc);
				valGroups.PushBack(valGroup, jsonDoc.GetAllocator());
			}

			valLoader.AddMember(LOADINFDOC_NODE_NAME_GROUPLIST, valGroups, jsonDoc.GetAllocator());
			valInfo.AddMember(LOADINFDOC_NODE_NAME_LOADINF, valLoader, jsonDoc.GetAllocator());

			jsonDoc.AddMember(LOADINFDOC_NODE_NAME_INFO, valInfo, jsonDoc.GetAllocator());

			WriteJsonFile(strFilePath, jsonDoc);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ModelGroupToJsonValue(Stk_ModelGroup* modelGroupPtr, Value& valModelGroup, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (modelGroupPtr == NULL)
				return STK_ERROR;

			valModelGroup.SetObject();
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_GROUP_ID, modelGroupPtr->GetID(), jsonDoc.GetAllocator());
			string strGroupName = modelGroupPtr->GetName();
			string strGroupNameUTF8 = "";
#ifdef _WIN32
			ChineseCode::GB2312ToUTF_8(strGroupNameUTF8, (char*)strGroupName.c_str(), strGroupName.length());
#else
			strGroupNameUTF8 = strGroupName;
#endif
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_GROUP_NAME, strGroupNameUTF8, jsonDoc.GetAllocator());
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_GROUP_TYPE, modelGroupPtr->GetType(), jsonDoc.GetAllocator());
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_GROUP_PRECISE, modelGroupPtr->GetPrecise(), jsonDoc.GetAllocator());
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_GROUP_PRIORITY, modelGroupPtr->GetPriority(), jsonDoc.GetAllocator());
			
			std::vector<string>& vecPlcPath = modelGroupPtr->GetInsPlcPath();
			Value valItems;
			valItems.SetArray();
			for (int i = 0; i < vecPlcPath.size(); i++)
			{
				Value valItem;
				valItem.SetString(vecPlcPath[i].c_str(), jsonDoc.GetAllocator());
				valItems.PushBack(valItem, jsonDoc.GetAllocator());
			}
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_GROUP_ITEMS, valItems, jsonDoc.GetAllocator());

			Stk_CameraPtr cameraPtr = modelGroupPtr->GetCamera();
			Value valCamera;
			CameraToJsonValue(cameraPtr, valCamera, jsonDoc);
			valModelGroup.AddMember(LOADINFDOC_NODE_NAME_CAMERA, valCamera, jsonDoc.GetAllocator());
            return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ModelGroupFromJsonValue(Stk_ModelGroup* modelGroupPtr, Value& valModelGroup, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (modelGroupPtr == NULL || valModelGroup.IsNull())
				return STK_ERROR;
			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_GROUP_ID))
			{
				modelGroupPtr->SetID(valModelGroup[LOADINFDOC_NODE_NAME_GROUP_ID].GetInt());
			}
			
			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_GROUP_NAME))
			{
				string strGroupNameUTF8 = valModelGroup[LOADINFDOC_NODE_NAME_GROUP_NAME].GetString();
				string strGroupName = "";
#ifdef _WIN32
				ChineseCode::UTF_8ToGB2312(strGroupName, (char*)strGroupNameUTF8.c_str(), strGroupNameUTF8.length());
#else
				strGroupName = strGroupNameUTF8;
#endif
				modelGroupPtr->SetName(strGroupName);
			}
			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_GROUP_TYPE))
			{
				modelGroupPtr->SetID(valModelGroup[LOADINFDOC_NODE_NAME_GROUP_TYPE].GetInt());
			}
			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_GROUP_PRECISE))
			{
				modelGroupPtr->SetID(valModelGroup[LOADINFDOC_NODE_NAME_GROUP_PRECISE].GetInt());
			}
			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_GROUP_PRIORITY))
			{
				modelGroupPtr->SetID(valModelGroup[LOADINFDOC_NODE_NAME_GROUP_PRIORITY].GetInt());
			}

			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_GROUP_ITEMS))
			{
				std::vector<string> vecPlcPath;
				Value::Array valItems = valModelGroup[LOADINFDOC_NODE_NAME_GROUP_ITEMS].GetArray();
				Value valItem;
				for (int i = 0; i < valItems.Size(); i++)
				{
					valItem = valItems[i];
					vecPlcPath.push_back(valItem.GetString());
				}
				modelGroupPtr->SetInsPlcPath(vecPlcPath);
			}
			if (valModelGroup.HasMember(LOADINFDOC_NODE_NAME_CAMERA))
			{
				Stk_CameraPtr cameraPtr = Stk_CameraPtr::CreateObject();
				CameraFromJsonValue(cameraPtr, valModelGroup[LOADINFDOC_NODE_NAME_CAMERA], jsonDoc);
				modelGroupPtr->SetCamera(cameraPtr);
			}
            return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::CameraToJsonValue(Stk_CameraPtr cameraPtr, Value& valCamera, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (cameraPtr == nullptr)
				return STK_ERROR;
			char cTemp[256];

			valCamera.SetObject();
			valCamera.AddMember(CAMERA_NODE_NAME_PROJECTTYPE, cameraPtr->GetProjectType(), jsonDoc.GetAllocator());
			valCamera.AddMember(CAMERA_NODE_NAME_FOV, cameraPtr->GetAngle(), jsonDoc.GetAllocator());

			if (!(cameraPtr->GetMatrixFlag()))
			{
				STK_PNT32 originPnt = cameraPtr->GetOriginPoint();
				sprintf(cTemp, "%.8f %.8f %.8f", originPnt.X, originPnt.Y, originPnt.Z);
				string strOrigin = cTemp;
				valCamera.AddMember(CAMERA_NODE_NAME_ORIGIN, strOrigin, jsonDoc.GetAllocator());

				STK_DIR32 targetVector = cameraPtr->GetTargetVector();
				sprintf(cTemp, "%.8f %.8f %.8f", targetVector.X, targetVector.Y, targetVector.Z);
				string strTargetVector = cTemp;
				valCamera.AddMember(CAMERA_NODE_NAME_TARGETVEC, strTargetVector, jsonDoc.GetAllocator());

				STK_DIR32 upVector = cameraPtr->GetUpVector();
				sprintf(cTemp, "%.8f %.8f %.8f", upVector.X, upVector.Y, upVector.Z);
				string strUpVector = cTemp;
				valCamera.AddMember(CAMERA_NODE_NAME_UPVEC, strUpVector, jsonDoc.GetAllocator());

			}
			else
			{
				STK_MTX32 matrix = cameraPtr->GetMatrix();
				string strMatirx = "";
				sprintf(cTemp, "%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f",
					matrix.PlcMatrix[0][0], matrix.PlcMatrix[0][1], matrix.PlcMatrix[0][2], matrix.PlcMatrix[0][3],
					matrix.PlcMatrix[1][0], matrix.PlcMatrix[1][1], matrix.PlcMatrix[1][2], matrix.PlcMatrix[1][3],
					matrix.PlcMatrix[2][0], matrix.PlcMatrix[2][1], matrix.PlcMatrix[2][2], matrix.PlcMatrix[2][3]/*,
					matrix.PlcMatrix[3][0], matrix.PlcMatrix[3][1], matrix.PlcMatrix[3][2], matrix.PlcMatrix[3][3]*/);
				strMatirx = cTemp;
				valCamera.AddMember(CAMERA_NODE_NAME_MATIX, strMatirx, jsonDoc.GetAllocator());
			}
			valCamera.AddMember(CAMERA_NODE_NAME_ASPECTRATIO, cameraPtr->GetAspectRatio(), jsonDoc.GetAllocator());
			valCamera.AddMember(CAMERA_NODE_NAME_NEARDISTANCE, cameraPtr->GetNearDistance(), jsonDoc.GetAllocator());
			valCamera.AddMember(CAMERA_NODE_NAME_FARDISTANCE, cameraPtr->GetFarDistance(), jsonDoc.GetAllocator());
			valCamera.AddMember(CAMERA_NODE_NAME_FOCALDISTANCE, cameraPtr->GetFocalDistance(), jsonDoc.GetAllocator());
			valCamera.AddMember(CAMERA_NODE_NAME_HEIGHT, cameraPtr->GetHeight(), jsonDoc.GetAllocator());
            return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::CameraFromJsonValue(Stk_CameraPtr cameraPtr, Value& valCamera, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (cameraPtr == nullptr || valCamera.IsNull())
				return STK_ERROR;
			if (valCamera.HasMember(CAMERA_NODE_NAME_PROJECTTYPE))
			{
				cameraPtr->SetProjectType(valCamera[CAMERA_NODE_NAME_PROJECTTYPE].GetInt());
			}
			if (valCamera.HasMember(CAMERA_NODE_NAME_FOV))
			{
				cameraPtr->SetAngle(valCamera[CAMERA_NODE_NAME_FOV].GetFloat());
			}
			if (valCamera.HasMember(CAMERA_NODE_NAME_ASPECTRATIO))
			{
				cameraPtr->SetAspectRatio(valCamera[CAMERA_NODE_NAME_ASPECTRATIO].GetFloat());
			}
			if (valCamera.HasMember(CAMERA_NODE_NAME_NEARDISTANCE))
			{
				cameraPtr->SetNearDistance(valCamera[CAMERA_NODE_NAME_NEARDISTANCE].GetFloat());
			}
			if (valCamera.HasMember(CAMERA_NODE_NAME_FARDISTANCE))
			{
				cameraPtr->SetFarDistance(valCamera[CAMERA_NODE_NAME_FARDISTANCE].GetFloat());
			}
			if (valCamera.HasMember(CAMERA_NODE_NAME_FOCALDISTANCE))
			{
				cameraPtr->SetFocalDistance(valCamera[CAMERA_NODE_NAME_FOCALDISTANCE].GetFloat());
			}
			if (valCamera.HasMember(CAMERA_NODE_NAME_HEIGHT))
			{
				cameraPtr->SetHeight(valCamera[CAMERA_NODE_NAME_HEIGHT].GetFloat());
			}

			if (valCamera.HasMember(CAMERA_NODE_NAME_ORIGIN))
			{
				STK_PNT32 pntOrigin;
				string strOrigin = valCamera[CAMERA_NODE_NAME_ORIGIN].GetString();
				std::vector<std::string> vecOriginValue;
				CStringTools::StringSplit(strOrigin, " ", vecOriginValue);
				if (vecOriginValue.size() == 3)
				{
					pntOrigin.X = atof(vecOriginValue[0].c_str());
					pntOrigin.Y = atof(vecOriginValue[1].c_str());
					pntOrigin.Z = atof(vecOriginValue[2].c_str());
				}
				cameraPtr->SetOriginPoint(pntOrigin);
			}

			if (valCamera.HasMember(CAMERA_NODE_NAME_TARGETVEC))
			{
				STK_DIR32 dirTargetVector;
				string strTargetVector = valCamera[CAMERA_NODE_NAME_TARGETVEC].GetString();
				std::vector<std::string> vecTargetVector;
				CStringTools::StringSplit(strTargetVector, " ", vecTargetVector);
				if (vecTargetVector.size() == 3)
				{
					dirTargetVector.X = atof(vecTargetVector[0].c_str());
					dirTargetVector.Y = atof(vecTargetVector[1].c_str());
					dirTargetVector.Z = atof(vecTargetVector[2].c_str());
				}
				cameraPtr->SetTargetVector(dirTargetVector);
			}

			if (valCamera.HasMember(CAMERA_NODE_NAME_UPVEC))
			{
				STK_DIR32 dirUpVector;
				string strUpVector = valCamera[CAMERA_NODE_NAME_UPVEC].GetString();
				std::vector<std::string> vecUpVector;
				CStringTools::StringSplit(strUpVector, " ", vecUpVector);
				if (vecUpVector.size() == 3)
				{
					dirUpVector.X = atof(vecUpVector[0].c_str());
					dirUpVector.Y = atof(vecUpVector[1].c_str());
					dirUpVector.Z = atof(vecUpVector[2].c_str());
				}
				cameraPtr->SetUpVector(dirUpVector);
			}

			if (valCamera.HasMember(CAMERA_NODE_NAME_MATIX))
			{
				STK_MTX32 matrix;
				string strMatrix = valCamera[CAMERA_NODE_NAME_MATIX].GetString();
				std::vector<std::string> vecMatrixValue;
				CStringTools::StringSplit(strMatrix, " ", vecMatrixValue);
				if (vecMatrixValue.size() >= MTX_LINE_COUNT*MTX_COL_COUNT)
				{
					for (int ii = 0; ii < MTX_LINE_COUNT; ii++)
					{
						for (int jj = 0; jj < MTX_COL_COUNT; jj++)
						{
							matrix.PlcMatrix[ii][jj] = atof(vecMatrixValue[ii * 4 + jj].c_str());
						}
					}
				}
				vecMatrixValue.clear();
				cameraPtr->SetMatrix(matrix);

				cameraPtr->SetMatrixFlag(true);
			}
			else
			{
				cameraPtr->SetMatrixFlag(false);
			}
            return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadLayer(char ***i_bufP, const std::vector<int> &i_vcBufNum, void* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pStkDocPtr == nullptr)
				return STK_ERROR;

			Document jsonDoc;
			result = ReadJsonData(i_bufP, i_vcBufNum, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析图层文件信息
			result = ParseLayer(jsonDoc, (Stk_Document*)pStkDocPtr);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadLayerDoc(const std::wstring& strFilePath, void* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			Document jsonDoc;
			result = ReadJsonFile(strFilePath, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析图层文件信息
			result = ParseLayer(jsonDoc, (Stk_Document*)pStkDocPtr);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ParseLayer(Document& jsonDoc, Stk_Document* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			
			string strVersion = "";
			if (jsonDoc.HasMember(LAYERDOC_NODE_NAME_VERSION))
			{
				strVersion = jsonDoc[LAYERDOC_NODE_NAME_VERSION].GetString();
			}
			
			//models
			Value::MemberIterator modelsNodeIter = jsonDoc.FindMember(LAYERDOC_NODE_NAME_MODELS);
			if (modelsNodeIter != jsonDoc.MemberEnd())
			{
				Value valModels = modelsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					Stk_ModelPtr pModelPtr = nullptr;
					if (valModel.HasMember(LAYERDOC_NODE_NAME_MODEL_ID))
					{
						STK_ID stkModelID;
						stkModelID = valModel[LAYERDOC_NODE_NAME_MODEL_ID].GetInt();
						pModelPtr = pStkDocPtr->GetModelByID(stkModelID);
					}
					if(pModelPtr == nullptr)
						continue;

					//读取图层与模型的关联信息
					map<int, vector<Stk_ConnectorPtr>> mapLayerConnector;
					if (valModel.HasMember(LAYERDOC_NODE_NAME_LAYER_ENTITIES))
					{
						Value::Array valLayerEntities = valModel[LAYERDOC_NODE_NAME_LAYER_ENTITIES].GetArray();
						Value valLayerEntity;
						for (int i = 0; i < valLayerEntities.Size(); i++)
						{
							valLayerEntity = valLayerEntities[i];
							int iLayerId = -1;
							if (!valLayerEntity.HasMember(LAYERDOC_NODE_NAME_LAYER_ID))
							{
								continue;
							}
							iLayerId = valLayerEntity[LAYERDOC_NODE_NAME_LAYER_ID].GetInt();
							vector<Stk_ConnectorPtr> vecConnector;
							if (valLayerEntity.HasMember(LAYERDOC_NODE_NAME_ENTITIES))
							{
								Value::Array valEntities = valLayerEntity[LAYERDOC_NODE_NAME_ENTITIES].GetArray();
								Value valEntity;
								for (int i = 0; i < valEntities.Size(); i++)
								{
									Stk_ConnectorPtr pLayerEntityConnectorPtr = Stk_ConnectorPtr::CreateObject();
									valEntity = valEntities[i];
									int entityType = -1;
									if (valEntity.HasMember(LAYERDOC_NODE_NAME_LAYER_ENTITY_TYPE))
									{
										pLayerEntityConnectorPtr->SetEntityType(ENTITY_TYPE_LAYER, (StkEntityTypeEnum)valEntity[LAYERDOC_NODE_NAME_LAYER_ENTITY_TYPE].GetInt());
									}
									if (valEntity.HasMember(LAYERDOC_NODE_NAME_LAYER_ENTITY_ID))
									{
										pLayerEntityConnectorPtr->SetLinkNode(valEntity[LAYERDOC_NODE_NAME_LAYER_ENTITY_ID].GetInt());
										pLayerEntityConnectorPtr->SetIsLinkNode(true);
									}
									else
									{
										pLayerEntityConnectorPtr->SetIsLinkNode(false);
									}
									if (valEntity.HasMember(LAYERDOC_NODE_NAME_LAYER_ENTITY_INS))
									{
										pLayerEntityConnectorPtr->SetLinkProto(ChineseCode::UTF_8ToUnicode(valEntity[LAYERDOC_NODE_NAME_LAYER_ENTITY_INS].GetString()));
										pLayerEntityConnectorPtr->SetIsLinkProto(true);
									}
									else
									{
										pLayerEntityConnectorPtr->SetIsLinkProto(false);
									}
									vecConnector.push_back(pLayerEntityConnectorPtr);
								}
							}
							
							mapLayerConnector.insert(make_pair(iLayerId, vecConnector));
						}
					}

					//读取图层信息
					Value::MemberIterator itLayerInfosSetsIter = valModel.FindMember(LAYERDOC_NODE_NAME_INFOSET);
					if (itLayerInfosSetsIter != valModel.MemberEnd())
					{
						Value::Array valLayerInfoSets = itLayerInfosSetsIter->value.GetArray();
						Value valLayerInfoSet;
						for (int i = 0; i < valLayerInfoSets.Size(); i++)
						{
							valLayerInfoSet = valLayerInfoSets[i];
							STK_ID stkLayerInfoSetID = -1;
							if (valLayerInfoSet.HasMember(LAYERDOC_NODE_NAME_INFOSET_ID))
							{
								stkLayerInfoSetID = valLayerInfoSet[LAYERDOC_NODE_NAME_INFOSET_ID].GetInt();
							}
							Stk_LayerInfosSetPtr pLayerInfosSet = pModelPtr->AddLayerInfosSet(stkLayerInfoSetID);
							if (valLayerInfoSet.HasMember(LAYERDOC_NODE_NAME_INFOSET_DEFLAYER))
							{
								int iDefLayer = valLayerInfoSet[LAYERDOC_NODE_NAME_INFOSET_DEFLAYER].GetInt();
								pLayerInfosSet->SetDefaultLayer(iDefLayer);
							}
							if (valLayerInfoSet.HasMember(LAYERDOC_NODE_NAME_INFOSET_DEFLAYERFILTER))
							{
								int iDefLayerFilter = valLayerInfoSet[LAYERDOC_NODE_NAME_INFOSET_DEFLAYERFILTER].GetInt();
								pLayerInfosSet->SetDefaultFilter(iDefLayerFilter);
							}

							//读取图层列表
							if (valLayerInfoSet.HasMember(LAYERDOC_NODE_NAME_LAYERS))
							{
								Value::Array vallLayers = valLayerInfoSet[LAYERDOC_NODE_NAME_LAYERS].GetArray();
								Value vallLayer;
								for(int iLayerIdx = 0; iLayerIdx<vallLayers.Size(); iLayerIdx++)
								{
									Stk_LayerPtr pLayerPtr = Stk_LayerPtr::CreateObject();
									pLayerInfosSet->AddLayer(pLayerPtr);
									vallLayer = vallLayers[iLayerIdx];
									int iLayerID = -1;
									if (vallLayer.HasMember(LAYERDOC_NODE_NAME_LAYER_ID))
									{
										iLayerID = vallLayer[LAYERDOC_NODE_NAME_LAYER_ID].GetInt();
										pLayerPtr->SetLayer(iLayerID);
									}
									if (vallLayer.HasMember(LAYERDOC_NODE_NAME_LAYER_NAME))
									{
										string strLayerName = vallLayer[LAYERDOC_NODE_NAME_LAYER_NAME].GetString();
										pLayerPtr->SetLayerName(ChineseCode::UTF_8ToUnicode(strLayerName.c_str()));
									}
									if (vallLayer.HasMember(LAYERDOC_NODE_NAME_LAYER_VISIBLE))
									{
										int iLayerVisible = vallLayer[LAYERDOC_NODE_NAME_LAYER_VISIBLE].GetInt();
										pLayerPtr->SetDisplay(iLayerVisible);
									}

									//获取图层关联模型信息
									map<int, vector<Stk_ConnectorPtr>>::iterator itConnector = mapLayerConnector.find(iLayerID);
									if(itConnector!= mapLayerConnector.end())
									{
										for (int iConnectorIdx = 0; iConnectorIdx < itConnector->second.size(); iConnectorIdx++)
										{
											pLayerPtr->AddNodeConnector(itConnector->second[iConnectorIdx]);
										}
									}
								}
							}
							
							//读取图层筛选器列表
							if (valLayerInfoSet.HasMember(LAYERDOC_NODE_NAME_LAYERFILTERS))
							{
								Value::Array valLayerFilters = valLayerInfoSet[LAYERDOC_NODE_NAME_LAYERFILTERS].GetArray();
								int iLayerFiltersNum = valLayerFilters.Size();
								Value vallLayerFilter;
								for (int iLayerFilterIdx=0; iLayerFilterIdx<iLayerFiltersNum; iLayerFilterIdx++)
								{
									Stk_LayerFilterPtr pLayerFilterPtr = Stk_LayerFilterPtr::CreateObject();
									pLayerInfosSet->AddLayerFilter(pLayerFilterPtr);
									vallLayerFilter = valLayerFilters[iLayerFilterIdx];
									if (vallLayerFilter.HasMember(LAYERDOC_NODE_NAME_LAYERFILTERS_ID))
									{
										int iLayerFilterID = vallLayerFilter[LAYERDOC_NODE_NAME_LAYERFILTERS_ID].GetInt();
										pLayerFilterPtr->SetID(iLayerFilterID);
									}
									if (vallLayerFilter.HasMember(LAYERDOC_NODE_NAME_LAYERFILTERS_NAME))
									{
										string strLayerFilterName = vallLayerFilter[LAYERDOC_NODE_NAME_LAYERFILTERS_NAME].GetString();
										pLayerFilterPtr->SetLayerFilterName(ChineseCode::UTF_8ToUnicode(strLayerFilterName.c_str()));
									}
									if (vallLayerFilter.HasMember(LAYERDOC_NODE_NAME_LAYERFILTERS_DESC))
									{
										string strLayerFilterDesc = vallLayerFilter[LAYERDOC_NODE_NAME_LAYERFILTERS_DESC].GetString();
										pLayerFilterPtr->SetDescription(ChineseCode::UTF_8ToUnicode(strLayerFilterDesc.c_str()));
									}

									//读取图层筛选器是记录的图层显示隐藏状�?
									if (vallLayerFilter.HasMember(LAYERDOC_NODE_NAME_LAYERFILTERS_SELECTED))
									{
										Value::Array valSelectedLayters = vallLayerFilter[LAYERDOC_NODE_NAME_LAYERFILTERS_SELECTED].GetArray();
										Value valSelectedLayter;
										for (int iSelectedLayerIdx = 0; iSelectedLayerIdx <valSelectedLayters.Size(); iSelectedLayerIdx++)
										{
											valSelectedLayter = valSelectedLayters[iSelectedLayerIdx];
											if (!valSelectedLayter.HasMember(LAYERDOC_NODE_NAME_LAYER_ID) || 
												!valSelectedLayter.HasMember(LAYERDOC_NODE_NAME_LAYERFILTER_ISSELECTED))
											{
												continue;
											}
											int iLayerID = valSelectedLayter[LAYERDOC_NODE_NAME_LAYERFILTERS_ID].GetInt();
											int iIsSelected = valSelectedLayter[LAYERDOC_NODE_NAME_LAYERFILTER_ISSELECTED].GetInt();
											pLayerFilterPtr->AddMapLayerState(iLayerID, iIsSelected);
										}
									}
								}
							}
						}
					}
					mapLayerConnector.clear();
				}
			}

			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::WriteLayerDoc(const std::wstring& strFilePath, Stk_DocumentPtr pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			Document jsonDoc;
			jsonDoc.SetObject();
			string strVersion;
			strVersion = LAYERDOC_VERSION;
			jsonDoc.AddMember(LAYERDOC_NODE_NAME_VERSION, strVersion, jsonDoc.GetAllocator());

			Value valModels;
			valModels.SetArray();

			std::vector<Stk_ModelPtr> vecModel;
			pStkDocPtr->ListModel(vecModel);
			for (int i = 0; i < vecModel.size(); i++)
			{
				Stk_ModelPtr modePtr = vecModel[i];
				if (modePtr == nullptr)
					continue;
				map<int, Stk_LayerInfosSetPtr> mapLayerInfosSet = modePtr->GetAllLayerInfosSet();
				if (mapLayerInfosSet.size() <= 0)
					continue;

				Value valModel;
				valModel.SetObject();
				valModel.AddMember(LAYERDOC_NODE_NAME_MODEL_ID, modePtr->GetID(), jsonDoc.GetAllocator());
				
				Value valLayerInfosSets;
				valLayerInfosSets.SetArray();
				Value valLayers_Entities;
				valLayers_Entities.SetArray();
				map<int, Stk_LayerInfosSetPtr>::iterator it;
				for (it = mapLayerInfosSet.begin(); it != mapLayerInfosSet.end(); it++)
				{
					Stk_LayerInfosSetPtr curLayerInfosSetPtr = it->second;
					if (curLayerInfosSetPtr == nullptr)
						continue;
					Value valLayerInfosSet;
					valLayerInfosSet.SetObject();
					valLayerInfosSet.AddMember(LAYERDOC_NODE_NAME_INFOSET_ID, curLayerInfosSetPtr->GetID(), jsonDoc.GetAllocator());
					if(curLayerInfosSetPtr->GetDefaultLayer()>0)
					{
						valLayerInfosSet.AddMember(LAYERDOC_NODE_NAME_INFOSET_DEFLAYER, curLayerInfosSetPtr->GetDefaultLayer(), jsonDoc.GetAllocator());
					}
					if(curLayerInfosSetPtr->GetDefaultFilter() != nullptr)
					{
						valLayerInfosSet.AddMember(LAYERDOC_NODE_NAME_INFOSET_DEFLAYERFILTER, curLayerInfosSetPtr->GetDefaultFilter()->GetID(), jsonDoc.GetAllocator());
					}
					
					//保存图层信息
					Value valLayers;
					valLayers.SetArray();
					int iLayerNum = curLayerInfosSetPtr->GetLayerNum();
					for (int layerIdx = 0; layerIdx < iLayerNum; layerIdx++)
					{
						HoteamSoft::SVLLib::Stk_LayerPtr layerPtr = curLayerInfosSetPtr->GetLayerByIndex(layerIdx);
						if (layerPtr == nullptr)
							continue;
						Value valLayer;
						valLayer.SetObject();
						valLayer.AddMember(LAYERDOC_NODE_NAME_LAYER_ID, layerPtr->GetLayer(), jsonDoc.GetAllocator());
						string strName = "";
						ChineseCode::UnicodeToUTF_8(layerPtr->GetLayerName().c_str(), strName);
						valLayer.AddMember(LAYERDOC_NODE_NAME_LAYER_NAME, strName, jsonDoc.GetAllocator());
						valLayer.AddMember(LAYERDOC_NODE_NAME_LAYER_VISIBLE, (int)layerPtr->IsDisplay(), jsonDoc.GetAllocator());
						valLayers.PushBack(valLayer, jsonDoc.GetAllocator());	

						//保存图层关联的对�?
						int iEntityNum = layerPtr->GetNodeConnectorNum();
						if(iEntityNum>0)
						{
							Value valLayer_Entities;
							valLayer_Entities.SetObject();
							valLayer_Entities.AddMember(LAYERDOC_NODE_NAME_LAYER_ID, layerPtr->GetLayer(), jsonDoc.GetAllocator());
							Value valEntities;
							valEntities.SetArray();

							for (int entityIdx = 0; entityIdx < iEntityNum; entityIdx++)
							{
								HoteamSoft::SVLLib::Stk_ConnectorPtr curConnectorPtr = layerPtr->GetNodeConnectorByIndex(entityIdx);
								if (curConnectorPtr == nullptr)
									continue;
								Value valEntity;
								valEntity.SetObject();
								HoteamSoft::SVLLib::StkEntityTypeEnum ePointingEnt;
								HoteamSoft::SVLLib::StkEntityTypeEnum ePointedEnt;
								curConnectorPtr->GetEntityType(ePointingEnt, ePointedEnt);
								valEntity.AddMember(LAYERDOC_NODE_NAME_LAYER_ENTITY_TYPE, ePointedEnt, jsonDoc.GetAllocator());
								if (curConnectorPtr->IsLinkNode())
								{
									valEntity.AddMember(LAYERDOC_NODE_NAME_LAYER_ENTITY_ID, curConnectorPtr->GetLinkNode(), jsonDoc.GetAllocator());
								}
								if (curConnectorPtr->IsLinkProto())
								{
									string strPlcPath = "";
									ChineseCode::UnicodeToUTF_8(curConnectorPtr->GetLinkProto().c_str(), strPlcPath);
									valEntity.AddMember(LAYERDOC_NODE_NAME_LAYER_ENTITY_INS, strPlcPath, jsonDoc.GetAllocator());
								}
								valEntities.PushBack(valEntity, jsonDoc.GetAllocator());
							}
							valLayer_Entities.AddMember(LAYERDOC_NODE_NAME_ENTITIES, valEntities, jsonDoc.GetAllocator());
							valLayers_Entities.PushBack(valLayer_Entities, jsonDoc.GetAllocator());
						}
					}
					valLayerInfosSet.AddMember(LAYERDOC_NODE_NAME_LAYERS, valLayers, jsonDoc.GetAllocator());				

					//保存图层筛选器
					Value valLayerFilters;
					valLayerFilters.SetArray();
					vector<Stk_LayerFilterPtr> vecLayerFilterPtr = curLayerInfosSetPtr->GetAllLayerFilters();
					for (int layerFilterIdx = 0; layerFilterIdx < vecLayerFilterPtr.size(); layerFilterIdx++)
					{
						HoteamSoft::SVLLib::Stk_LayerFilterPtr layerFilterPtr = vecLayerFilterPtr[layerFilterIdx];
						if (layerFilterPtr == nullptr)
							continue;
						Value valLayerFilter;
						valLayerFilter.SetObject();
						valLayerFilter.AddMember(LAYERDOC_NODE_NAME_LAYERFILTERS_ID, layerFilterPtr->GetID(), jsonDoc.GetAllocator());
						string strName = "";
						ChineseCode::UnicodeToUTF_8(layerFilterPtr->GetLayerFilterName().c_str(), strName);
						valLayerFilter.AddMember(LAYERDOC_NODE_NAME_LAYERFILTERS_NAME, strName, jsonDoc.GetAllocator());
						string strDesc = "";
						ChineseCode::UnicodeToUTF_8(layerFilterPtr->GetDescription().c_str(), strDesc);
						valLayerFilter.AddMember(LAYERDOC_NODE_NAME_LAYERFILTERS_DESC, strDesc, jsonDoc.GetAllocator());

						//保存图层选中状�?
						map<STK_INT32, STK_BOOL>& mapLayerStates = layerFilterPtr->GetAllMapLayerStates();
						Value valSelectedLayers;
						valSelectedLayers.SetArray();
						map<STK_INT32, STK_BOOL>::iterator itSelectedLayer;
						for (itSelectedLayer = mapLayerStates.begin(); itSelectedLayer != mapLayerStates.end(); itSelectedLayer++)
						{
							
							Value valSelectedLayer;
							valSelectedLayer.SetObject();
							valSelectedLayer.AddMember(LAYERDOC_NODE_NAME_LAYER_ID, itSelectedLayer->first, jsonDoc.GetAllocator());
							valSelectedLayer.AddMember(LAYERDOC_NODE_NAME_LAYERFILTER_ISSELECTED, (int)itSelectedLayer->second, jsonDoc.GetAllocator());
					
							valSelectedLayers.PushBack(valSelectedLayer, jsonDoc.GetAllocator());
						}
						valLayerFilter.AddMember(LAYERDOC_NODE_NAME_LAYERFILTERS_SELECTED, valSelectedLayers, jsonDoc.GetAllocator());

						valLayerFilters.PushBack(valLayerFilter, jsonDoc.GetAllocator());
					}
					valLayerInfosSet.AddMember(LAYERDOC_NODE_NAME_LAYERFILTERS, valLayerFilters, jsonDoc.GetAllocator());

					valLayerInfosSets.PushBack(valLayerInfosSet, jsonDoc.GetAllocator());
				}
				valModel.AddMember(LAYERDOC_NODE_NAME_INFOSET, valLayerInfosSets, jsonDoc.GetAllocator());
				valModel.AddMember(LAYERDOC_NODE_NAME_LAYER_ENTITIES, valLayers_Entities, jsonDoc.GetAllocator());
				valModels.PushBack(valModel, jsonDoc.GetAllocator());

			}
			vecModel.clear();
			if (valModels.Size() > 0)
			{
				jsonDoc.AddMember(LAYERDOC_NODE_NAME_MODELS, valModels, jsonDoc.GetAllocator());
				WriteJsonFile(strFilePath, jsonDoc);
			}
			return result;
		}


		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadFeatDoc(const std::wstring& strFilePath, void* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			Document jsonDoc;
			result = ReadJsonFile(strFilePath, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析图层文件信息
			result = ParseFeat(jsonDoc, (Stk_Document*)pStkDocPtr);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ReadFeat(char ***i_bufP, const std::vector<int> &i_vcBufNum, void* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			if (pStkDocPtr == nullptr)
				return STK_ERROR;

			Document jsonDoc;
			result = ReadJsonData(i_bufP, i_vcBufNum, jsonDoc);
			if (result != STK_SUCCESS)
			{
				return result;
			}
			//解析图层文件信息
			result = ParseFeat(jsonDoc, (Stk_Document*)pStkDocPtr);
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::ParseFeat(Document& jsonDoc, Stk_Document* pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;

			string strVersion = "";
			if (jsonDoc.HasMember(FEATDOC_NODE_NAME_VERSION))
			{
				strVersion = jsonDoc[FEATDOC_NODE_NAME_VERSION].GetString();
			}

			//models
			Value::MemberIterator modelsNodeIter = jsonDoc.FindMember(FEATDOC_NODE_NAME_MODELS);
			if (modelsNodeIter != jsonDoc.MemberEnd())
			{
				Value valModels = modelsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					Stk_ModelPtr pModelPtr = nullptr;
					if (valModel.HasMember(FEATDOC_NODE_NAME_MODEL_ID))
					{
						STK_ID stkModelID;
						stkModelID = valModel[FEATDOC_NODE_NAME_MODEL_ID].GetInt();
						pModelPtr = pStkDocPtr->GetModelByID(stkModelID);
					}
					if (pModelPtr == nullptr)
						continue;

					//读取特征信息
					Value::MemberIterator itFeatsIter = valModel.FindMember(FEATDOC_NODE_NAME_FEATS);
					if (itFeatsIter != valModel.MemberEnd())
					{
						Value::Array valFeats = itFeatsIter->value.GetArray();
						Value valFeat;
						for (int i = 0; i < valFeats.Size(); i++)
						{
							valFeat = valFeats[i];
							STK_ID featID = -1;
							StkFeatTypeEnum eFeatType = FEAT_TYPE_UNKNOWN;
							if (valFeat.HasMember(FEATDOC_NODE_NAME_FEAT_ID))
							{
								featID = valFeat[FEATDOC_NODE_NAME_FEAT_ID].GetInt();
							}
							Stk_FeatPtr pFeat = pModelPtr->AddFeat(featID);
							if (valFeat.HasMember(FEATDOC_NODE_NAME_FEAT_TYPE))
							{
								eFeatType = (StkFeatTypeEnum)valFeat[FEATDOC_NODE_NAME_FEAT_TYPE].GetInt();
								pFeat->SetFeatType(eFeatType);
							}
							if (valFeat.HasMember(FEATDOC_NODE_NAME_FEAT_ISACTIVE))
							{
								bool iActive = valFeat[FEATDOC_NODE_NAME_FEAT_ISACTIVE].GetBool();
								pFeat->SetActiveed(iActive);
							}
							if (valFeat.HasMember(FEATDOC_NODE_NAME_FEAT_NAME))
							{
								string strName = valFeat[FEATDOC_NODE_NAME_FEAT_NAME].GetString();
								pFeat->SetFeatName(ChineseCode::UTF_8ToUnicode(strName.c_str()));
							}

							HoteamSoft::SVLLib::Stk_Feat_MagneticLinePtr magneticLinePtr  = nullptr;
							HoteamSoft::SVLLib::Stk_Feat_PipePtr pipePtr  = nullptr;
							Value::MemberIterator featDataIter = valFeat.FindMember(FEATDOC_NODE_NAME_FEAT_DATA);
							if (featDataIter != valFeat.MemberEnd())
							{
								switch (eFeatType)
								{
								case HoteamSoft::SVLLib::FEAT_TYPE_HOLE:
									break;
								case HoteamSoft::SVLLib::FEAT_TYPE_PIPE:
									pipePtr = Stk_Feat_PipePtr::CreateObject();
									PipeFromJsonValue(pipePtr, featDataIter->value, jsonDoc);
									pFeat->SetFeatPipe(pipePtr);
									break;
								case HoteamSoft::SVLLib::FEAT_TYPE_MAGNETIC_LINE:
									magneticLinePtr = Stk_Feat_MagneticLinePtr::CreateObject();
									MagneticLineFromJsonValue(magneticLinePtr,featDataIter->value,jsonDoc);
									pFeat->SetMagneticLine(magneticLinePtr);
									break;
								case HoteamSoft::SVLLib::FEAT_TYPE_MARK:
									break;
								default:
									break;
								}
								
							}

						}
					}
				}
			}
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::WriteFeatDoc(const std::wstring& strFilePath, Stk_DocumentPtr pStkDocPtr)
		{
			STK_STATUS result = STK_SUCCESS;
			Document jsonDoc;
			jsonDoc.SetObject();
			string strVersion;
			strVersion = FEATDOC_VERSION;
			jsonDoc.AddMember(FEATDOC_NODE_NAME_VERSION, strVersion, jsonDoc.GetAllocator());

			Value valModels;
			valModels.SetArray();

			std::vector<Stk_ModelPtr> vecModel;
			pStkDocPtr->ListModel(vecModel);
			for (int i = 0; i < vecModel.size(); i++)
			{
				Stk_ModelPtr modePtr = vecModel[i];
				if (modePtr == nullptr)
					continue;
				map<int, Stk_FeatPtr> mapFeat = modePtr->GetAllFeat();
				if (mapFeat.size() <= 0)
					continue;

				Value valModel;
				valModel.SetObject();
				valModel.AddMember(FEATDOC_NODE_NAME_MODEL_ID, modePtr->GetID(), jsonDoc.GetAllocator());

				Value valFeats;
				valFeats.SetArray();
				
				map<int, Stk_FeatPtr>::iterator it;
				for (it = mapFeat.begin(); it != mapFeat.end(); it++)
				{
					Stk_FeatPtr featPtr = it->second;
					if (featPtr == nullptr)
						continue;
					Value valFeat;
					valFeat.SetObject();

					StkFeatTypeEnum eFeatType = featPtr->GetFeatType();
					valFeat.AddMember(FEATDOC_NODE_NAME_FEAT_ID, featPtr->GetID(), jsonDoc.GetAllocator());
					valFeat.AddMember(FEATDOC_NODE_NAME_FEAT_TYPE, eFeatType, jsonDoc.GetAllocator());
					valFeat.AddMember(FEATDOC_NODE_NAME_FEAT_ISACTIVE, featPtr->IsActiveed(), jsonDoc.GetAllocator());
					string strName = "";
					ChineseCode::UnicodeToUTF_8(featPtr->GetFeatName().c_str(), strName);
					valFeat.AddMember(FEATDOC_NODE_NAME_FEAT_NAME, strName, jsonDoc.GetAllocator());
					Value valFeatData;
					switch (eFeatType)
					{
					case FEAT_TYPE_MAGNETIC_LINE:
					{
						Stk_Feat_MagneticLinePtr magneticLinePtr = featPtr->GetMagneticLine();
						if(magneticLinePtr != nullptr)
						{
							MagneticLineToJsonValue(magneticLinePtr, valFeatData, jsonDoc);
						}
					}
						break;
					case FEAT_TYPE_HOLE:
					{
						Stk_Feat_HolePtr holePtr = featPtr->GetFeatHole();
						if (holePtr != nullptr)
						{
							HoleToJsonValue(holePtr, valFeatData, jsonDoc);
						}
					}
						break;
					case FEAT_TYPE_PIPE:
					{
						Stk_Feat_PipePtr pipePtr = featPtr->GetFeatPipe();
						if (pipePtr != nullptr)
						{
							PipeToJsonValue(pipePtr, valFeatData, jsonDoc);
						}
					}
						break;
					case FEAT_TYPE_MARK:
						break;
					default:
						valFeatData.SetObject();
						break;
					}
					
					valFeat.AddMember(FEATDOC_NODE_NAME_FEAT_DATA, valFeatData, jsonDoc.GetAllocator());
					valFeats.PushBack(valFeat, jsonDoc.GetAllocator());
				}
				valModel.AddMember(FEATDOC_NODE_NAME_FEATS, valFeats, jsonDoc.GetAllocator());
				valModels.PushBack(valModel, jsonDoc.GetAllocator());

			}
			vecModel.clear();
			if (valModels.Size() > 0)
			{
				jsonDoc.AddMember(FEATDOC_NODE_NAME_MODELS, valModels, jsonDoc.GetAllocator());
				WriteJsonFile(strFilePath, jsonDoc);
			}
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::MagneticLineToJsonValue(Stk_Feat_MagneticLinePtr magneticLinePtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			valFeatData.SetObject();
			char cTemp[256] = { 0 };
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ISEFFECTIVE, magneticLinePtr->IsEffective(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ARRANGEMENT, magneticLinePtr->GetArrangementType(), jsonDoc.GetAllocator());

			STK_PNT32 startPnt = magneticLinePtr->GetStartPoint();
			sprintf(cTemp, "%.8f %.8f %.8f", startPnt.X, startPnt.Y, startPnt.Z);
			string strStartPnt = cTemp;
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_STARTPNT, strStartPnt, jsonDoc.GetAllocator());

			STK_PNT32 endPnt = magneticLinePtr->GetEndPoint();
			sprintf(cTemp, "%.8f %.8f %.8f", endPnt.X, endPnt.Y, endPnt.Z);
			string strEndPnt = cTemp;
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ENDPNT, strEndPnt, jsonDoc.GetAllocator());

			STK_RGBA32 color = magneticLinePtr->GetColor();
			sprintf(cTemp, "%.8f %.8f %.8f %.8f", color.Red, color.Green, color.Blue, color.Alpha);
			string strColor = cTemp;
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_COLOR, strColor, jsonDoc.GetAllocator());
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::MagneticLineFromJsonValue(Stk_Feat_MagneticLinePtr magneticLinePtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (magneticLinePtr == nullptr || valFeatData.IsNull())
				return STK_ERROR;
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ISEFFECTIVE))
			{
				magneticLinePtr->SetEffective(valFeatData[FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ISEFFECTIVE].GetBool());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ARRANGEMENT))
			{
				magneticLinePtr->SetArrangementType((StkMagneticArrangementEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ARRANGEMENT].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_STARTPNT))
			{
				STK_PNT32 startPnt;
				string strStartPnt = valFeatData[FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_STARTPNT].GetString();
				std::vector<std::string> vecStartPntValue;
				CStringTools::StringSplit(strStartPnt, " ", vecStartPntValue);
				if (vecStartPntValue.size() == 3)
				{
					startPnt.X = atof(vecStartPntValue[0].c_str());
					startPnt.Y = atof(vecStartPntValue[1].c_str());
					startPnt.Z = atof(vecStartPntValue[2].c_str());
				}
				magneticLinePtr->SetStartPoint(startPnt);
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ENDPNT))
			{
				STK_PNT32 endPnt;
				string strEndPnt = valFeatData[FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_ENDPNT].GetString();
				std::vector<std::string> vecEndPntValue;
				CStringTools::StringSplit(strEndPnt, " ", vecEndPntValue);
				if (vecEndPntValue.size() == 3)
				{
					endPnt.X = atof(vecEndPntValue[0].c_str());
					endPnt.Y = atof(vecEndPntValue[1].c_str());
					endPnt.Z = atof(vecEndPntValue[2].c_str());
				}
				magneticLinePtr->SetEndPoint(endPnt);
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_COLOR))
			{
				STK_RGBA32 color ;
				string strColor = valFeatData[FEATDOC_NODE_NAME_FEAT_MAGNETICLINE_COLOR].GetString();
				std::vector<std::string> vecColorValue;
				CStringTools::StringSplit(strColor, " ", vecColorValue);
				if (vecColorValue.size() == 4)
				{
					color.Red = atof(vecColorValue[0].c_str());
					color.Green = atof(vecColorValue[1].c_str());
					color.Blue = atof(vecColorValue[2].c_str());
					color.Alpha = atof(vecColorValue[3].c_str());
				}
				magneticLinePtr->SetColor(color);
			}
			return result;
		}

		STK_STATUS CRapidJsonTools::HoleToJsonValue(Stk_Feat_HolePtr holePtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			if (holePtr == nullptr || valFeatData.IsNull())
				return STK_ERROR;
			valFeatData.SetObject();
			char cTemp[256] = { 0 };
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_ANCHOR_TYPE, holePtr->GetAnchorType(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_TYPE, holePtr->GetType(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOM_TYPE, holePtr->GetBottomType(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_DEPTH, holePtr->GetDepth(), jsonDoc.GetAllocator());
			if (holePtr->IsHasDepthTol())
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_DEPTHTOL, holePtr->GetDepthWithTol(), jsonDoc.GetAllocator());
			}
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETER, holePtr->GetDiameter(), jsonDoc.GetAllocator());
			if (holePtr->IsHasDiameterTol())
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETERTOL, holePtr->GetDiameterWithTol(), jsonDoc.GetAllocator());
			}
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLE, holePtr->GetBottomAngle(), jsonDoc.GetAllocator());
			if (holePtr->IsHasBottomAngleTol())
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLETOL, holePtr->GetBottomAngleTol(), jsonDoc.GetAllocator());
			}
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLE, holePtr->GetCounterAngle(), jsonDoc.GetAllocator());
			if (holePtr->IsHasCounterAngleTol())
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLETOL, holePtr->GetCounterAngleTol(), jsonDoc.GetAllocator());
			}
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTH, holePtr->GetCounterDepth(), jsonDoc.GetAllocator());
			if (holePtr->IsHasCounterDepthTol())
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTHTOL, holePtr->GetCounterDepthTol(), jsonDoc.GetAllocator());
			}
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETER, holePtr->GetCounterDiameter(), jsonDoc.GetAllocator());
			if (holePtr->IsHasCounterDiameterTol())
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETERTOL, holePtr->GetCounterDiameterTol(), jsonDoc.GetAllocator());
			}

			Stk_Feat_Axis* pAxis = holePtr->GetFeatAxis();
			if(pAxis)
			{
				Value valAxis;
				valAxis.SetObject();

				STK_DIR32 dir = pAxis->GetDirection();
				sprintf(cTemp, "%.8f %.8f %.8f", dir.X, dir.Y, dir.Z);
				string strDir = cTemp;
				valAxis.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_DIR, strDir, jsonDoc.GetAllocator());
				
				STK_PNT32 origin = pAxis->GetOrigin();
				sprintf(cTemp, "%.8f %.8f %.8f", origin.X, origin.Y, origin.Z);
				string strOrigin = cTemp;
				valAxis.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_ORIGIN, strOrigin, jsonDoc.GetAllocator());
				
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_AXIS, valAxis, jsonDoc.GetAllocator());
			}

			Stk_FeatInnerThread* pInnerThread = holePtr->GetInnerThread();
			if (pInnerThread)
			{
				Value valInnerThread;
				valInnerThread.SetObject();
				valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_LIMITTYPE, pInnerThread->GetThreadLimitType(), jsonDoc.GetAllocator());
				valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_THREADTYPE, pInnerThread->GetThreadType(), jsonDoc.GetAllocator());
				valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTH, pInnerThread->GetDepth(), jsonDoc.GetAllocator());
				if (pInnerThread->IsDepthHasTol())
				{
					valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTHTOL, pInnerThread->GetDepthTol(), jsonDoc.GetAllocator());
				}
				valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETER, pInnerThread->GetDiameter(), jsonDoc.GetAllocator());
				if (pInnerThread->IsHasDiameterTol())
				{
					valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETERTOL, pInnerThread->GetDiameterTol(), jsonDoc.GetAllocator());
				}
				valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCH, pInnerThread->GetPitch(), jsonDoc.GetAllocator());
				if (pInnerThread->IsHasPitchTol())
				{
					valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCHTOL, pInnerThread->GetPitchTol(), jsonDoc.GetAllocator());
				}
				valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_ISRIGHT, pInnerThread->IsRightThreaded(), jsonDoc.GetAllocator());

				Stk_ConnectorPtr pConnector = pInnerThread->GetThreadLimitReference();
				if(pConnector!= nullptr)
				{
					Value valConnector;
					valConnector.SetObject();
					StkEntityTypeEnum ePointingEnt;
					StkEntityTypeEnum ePointedEnt;
					pConnector->GetEntityType(ePointingEnt, ePointedEnt);
					valConnector.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTTYPE, ePointedEnt, jsonDoc.GetAllocator());
					if(pConnector->IsLinkNode())
					{
						valConnector.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTID, pConnector->GetLinkNode(), jsonDoc.GetAllocator());
					}
					if (pConnector->IsLinkProto())
					{
						string strPlcPath = "";
						ChineseCode::UnicodeToUTF_8(pConnector->GetLinkProto().c_str(), strPlcPath);
						valConnector.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_PLCPATH, strPlcPath, jsonDoc.GetAllocator());
					}
					valInnerThread.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD, valConnector, jsonDoc.GetAllocator());
				}

				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD, valInnerThread, jsonDoc.GetAllocator());
			}

			Stk_FeatLimit* pLimit = holePtr->GetFeatLimit();
			if (pLimit)
			{
				Value valLimit;
				valLimit.SetObject();
				valLimit.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_TYPE, pLimit->GetType(), jsonDoc.GetAllocator());
				valLimit.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_VALUE, pLimit->GetValue(), jsonDoc.GetAllocator());
				
				Stk_ConnectorPtr pConnector = pLimit->GetReferenceObject();
				if (pConnector != nullptr)
				{
					Value valConnector;
					valConnector.SetObject();
					StkEntityTypeEnum ePointingEnt;
					StkEntityTypeEnum ePointedEnt;
					pConnector->GetEntityType(ePointingEnt, ePointedEnt);
					valConnector.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTTYPE, ePointedEnt, jsonDoc.GetAllocator());
					if (pConnector->IsLinkNode())
					{
						valConnector.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTID, pConnector->GetLinkNode(), jsonDoc.GetAllocator());
					}
					if (pConnector->IsLinkProto())
					{
						string strPlcPath = "";
						ChineseCode::UnicodeToUTF_8(pConnector->GetLinkProto().c_str(), strPlcPath);
						valConnector.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_PLCPATH, strPlcPath, jsonDoc.GetAllocator());
					}
					valLimit.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR, valConnector, jsonDoc.GetAllocator());
				}

				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT, valLimit, jsonDoc.GetAllocator());
			}
			return result;
		}

		STK_STATUS CRapidJsonTools::HoleFromJsonValue(Stk_Feat_HolePtr holePtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			if (holePtr == nullptr || valFeatData.IsNull())
				return STK_ERROR;

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_ANCHOR_TYPE))
			{
				holePtr->SetAnchorType((StkHoleAnchorTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_ANCHOR_TYPE].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_TYPE))
			{
				holePtr->SetType((StkHoleTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_TYPE].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOM_TYPE))
			{
				holePtr->SetBottomType((StkHoleBottomTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOM_TYPE].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_DEPTH))
			{
				holePtr->SetDepth(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_DEPTH].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_DEPTHTOL))
			{
				holePtr->SetDepthHasTol(true);
				holePtr->SetDepthWithTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_DEPTHTOL].GetFloat());
			}
			else
			{
				holePtr->SetDepthHasTol(false);
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETER))
			{
				holePtr->SetDiameter(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETER].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETERTOL))
			{
				holePtr->SetDiameterHasTol(true);
				holePtr->SetDiameterWithTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_DIAMETERTOL].GetFloat());
			}
			else
			{
				holePtr->SetDiameterHasTol(false);
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLE))
			{
				holePtr->SetBottomAngle(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLE].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLETOL))
			{
				holePtr->SetBottomAngleHasTol(true);
				holePtr->SetBottomAngleTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_BOTTOMANGLETOL].GetFloat());
			}
			else
			{
				holePtr->SetBottomAngleHasTol(false);
			}
			
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLE))
			{
				holePtr->SetCounterAngle(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLE].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLETOL))
			{
				holePtr->SetCounterAngleHasTol(true);
				holePtr->SetCounterAngleTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERANGLETOL].GetFloat());
			}
			else
			{
				holePtr->SetCounterAngleHasTol(false);
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTH))
			{
				holePtr->SetCounterDepth(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTH].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTHTOL))
			{
				holePtr->SetCounterDepthHasTol(true);
				holePtr->SetCounterDepthTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDEPTHTOL].GetFloat());
			}
			else
			{
				holePtr->SetCounterDepthHasTol(false);
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETER))
			{
				holePtr->SetCounterDiameter(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETER].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETERTOL))
			{
				holePtr->SetCounterDiameterHasTol(true);
				holePtr->SetCounterDiameterTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_COUNTERDIAMETERTOL].GetFloat());
			}
			else
			{
				holePtr->SetCounterDiameterHasTol(false);
			}


			Stk_Feat_Axis* pAxis = holePtr->GetFeatAxis();
			if (pAxis)
			{
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_AXIS))
				{
					if(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_AXIS].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_DIR))
					{
						STK_DIR32 dir;
						string strDir = valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_AXIS][FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_DIR].GetString();
						std::vector<std::string> vecDirValue;
						CStringTools::StringSplit(strDir, " ", vecDirValue);
						if (vecDirValue.size() == 3)
						{
							dir.X = atof(vecDirValue[0].c_str());
							dir.Y = atof(vecDirValue[1].c_str());
							dir.Z = atof(vecDirValue[2].c_str());
						}
						pAxis->SetDirection(dir);
					}
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_AXIS].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_ORIGIN))
					{
						STK_PNT32 origin;
						string strOrigin = valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_AXIS][FEATDOC_NODE_NAME_FEAT_HOLE_AXIS_ORIGIN].GetString();
						std::vector<std::string> vecOriginValue;
						CStringTools::StringSplit(strOrigin, " ", vecOriginValue);
						if (vecOriginValue.size() == 3)
						{
							origin.X = atof(vecOriginValue[0].c_str());
							origin.Y = atof(vecOriginValue[1].c_str());
							origin.Z = atof(vecOriginValue[2].c_str());
						}
						pAxis->SetOrigin(origin);
					}
				}
			}

			Stk_FeatInnerThread* pInnerThread = holePtr->GetInnerThread();
			if (pInnerThread)
			{
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD))
				{
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_LIMITTYPE))
					{
						pInnerThread->SetThreadLimitType((StkThreadLimitTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_LIMITTYPE].GetInt());
					}
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_THREADTYPE))
					{
						pInnerThread->SetThreadType((StkThreadTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_THREADTYPE].GetInt());
					}

					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTH))
					{
						pInnerThread->SetDepth(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTH].GetFloat());
					}
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTHTOL))
					{
						pInnerThread->SetDepthHasTol(true);
						pInnerThread->SetDepthTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DEPTHTOL].GetFloat());
					}
					else
					{
						pInnerThread->SetDepthHasTol(false);
					}

					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETER))
					{
						pInnerThread->SetDiameter(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETER].GetFloat());
					}
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETERTOL))
					{
						pInnerThread->SetDiameterHasTol(true);
						pInnerThread->SetDiameterTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_DIAMETERTOL].GetFloat());
					}
					else
					{
						pInnerThread->SetDiameterHasTol(false);
					}

					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCH))
					{
						pInnerThread->SetPitch(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCH].GetFloat());
					}
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCHTOL))
					{
						pInnerThread->SetPitchHasTol(true);
						pInnerThread->SetPitchTol(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_PITCHTOL].GetFloat());
					}
					else
					{
						pInnerThread->SetPitchHasTol(false);
					}
					pInnerThread->SetRightThreaded(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_ISRIGHT].GetInt());

					Stk_ConnectorPtr pConnector = pInnerThread->GetThreadLimitReference();
					if (pConnector != nullptr)
					{
						StkEntityTypeEnum ePointedEnt;
						if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTTYPE))
						{
							ePointedEnt = (StkEntityTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTTYPE].GetInt();
							pConnector->SetEntityType(ENTITY_TYPE_UNKNOWN, ePointedEnt);
						}
						if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTID))
						{
							pConnector->SetLinkNode(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_ENTTYPE].GetInt());
							pConnector->SetIsLinkNode(true);
						}
						else
						{
							pConnector->SetIsLinkNode(false);
						}

						if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_PLCPATH))
						{
							pConnector->SetLinkProto(ChineseCode::UTF_8ToUnicode(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD][FEATDOC_NODE_NAME_FEAT_HOLE_INNERTHREAD_CONNECTOR_PLCPATH].GetString()));
							pConnector->SetIsLinkProto(true);
						}
						else
						{
							pConnector->SetIsLinkProto(false);
						}
					}
				}
			}

			Stk_FeatLimit* pLimit = holePtr->GetFeatLimit();
			if (pLimit)
			{
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT))
				{
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_TYPE))
					{
						pLimit->SetType((StkLimitTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT][FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_TYPE].GetInt());
					}
					if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_VALUE))
					{
						pLimit->SetValue(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT][FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_VALUE].GetFloat());
					}

					Stk_ConnectorPtr pConnector = pLimit->GetReferenceObject();
					if (pConnector != nullptr)
					{
						StkEntityTypeEnum ePointedEnt;
						if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTTYPE))
						{
							ePointedEnt = (StkEntityTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT][FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTTYPE].GetInt();
							pConnector->SetEntityType(ENTITY_TYPE_UNKNOWN, ePointedEnt);
						}
						if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTID))
						{
							pConnector->SetLinkNode(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT][FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_ENTTYPE].GetInt());
							pConnector->SetIsLinkNode(true);
						}
						else
						{
							pConnector->SetIsLinkNode(false);
						}

						if (valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT].HasMember(FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_PLCPATH))
						{
							pConnector->SetLinkProto(ChineseCode::UTF_8ToUnicode(valFeatData[FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT][FEATDOC_NODE_NAME_FEAT_HOLE_LIMIT_CONNECTOR_PLCPATH].GetString()));
							pConnector->SetIsLinkProto(true);
						}
						else
						{
							pConnector->SetIsLinkProto(false);
						}
					}
				}
			}
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::PipeToJsonValue(Stk_Feat_PipePtr pipePtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			valFeatData.SetObject();
			char cTemp[256] = { 0 };
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_TYPE, pipePtr->GetPipeType(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_SECTIONTYPE, pipePtr->GetSectionType(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_DIAMETER, pipePtr->GetDiameter(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_TURNRADIUS, pipePtr->GetTurnRadius(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_HEIGHT, pipePtr->GetHeight(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_WIDTH, pipePtr->GetWidth(), jsonDoc.GetAllocator());
			
			Value valPipeFibers;
			valPipeFibers.SetArray();
			int iPipeFiberNum = pipePtr->GetPipeFiberNum();
			for (int i = 0; i < iPipeFiberNum; i++)
			{
				Stk_Pipe_Fiber_Segment*pPipeFiber = pipePtr->GetPipeFiber(i);
				if(!pPipeFiber)
					continue;
				Value valPipeFiber;
				valPipeFiber.SetObject();

				STK_PNT32 centerPnt = pPipeFiber->GetCenterPoint();
				sprintf(cTemp, "%.8f %.8f %.8f", centerPnt.X, centerPnt.Y, centerPnt.Z);
				string strCenterPnt = cTemp;
				valPipeFiber.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CENTERPNT, strCenterPnt, jsonDoc.GetAllocator());

				STK_PNT32 startPnt = pPipeFiber->GetStartPoint();
				sprintf(cTemp, "%.8f %.8f %.8f", startPnt.X, startPnt.Y, startPnt.Z);
				string strStartPnt = cTemp;
				valPipeFiber.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_STARTPNT, strStartPnt, jsonDoc.GetAllocator());

				STK_PNT32 endPnt = pPipeFiber->GetEndPoint();
				sprintf(cTemp, "%.8f %.8f %.8f", endPnt.X, endPnt.Y, endPnt.Z);
				string strEndPnt = cTemp;
				valPipeFiber.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_ENDPNT, strEndPnt, jsonDoc.GetAllocator());

				Value valCurve;
				valCurve.SetObject();
				CurveToJsonValue(pPipeFiber->GetCurve(), valCurve, jsonDoc);
				valPipeFiber.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CURVE, valCurve, jsonDoc.GetAllocator());
				valPipeFibers.PushBack(valPipeFiber, jsonDoc.GetAllocator());
			}
			if (iPipeFiberNum > 0)
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBERS, valPipeFibers, jsonDoc.GetAllocator());
			}
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::PipeFromJsonValue(Stk_Feat_PipePtr pipePtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (pipePtr == nullptr || valFeatData.IsNull())
				return STK_ERROR;

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_TYPE))
			{
				pipePtr->SetPipeType((StkPipeTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_TYPE].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_SECTIONTYPE))
			{
				pipePtr->SetSectionType((StkPipeSectionTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_SECTIONTYPE].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_DIAMETER))
			{
				pipePtr->SetDiameter(valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_DIAMETER].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_TURNRADIUS))
			{
				pipePtr->SetTurnRadius(valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_TURNRADIUS].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_HEIGHT))
			{
				pipePtr->SetHeight(valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_HEIGHT].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_WIDTH))
			{
				pipePtr->SetWidth(valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_WIDTH].GetFloat());
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBERS))
			{
				Value::Array valFibers = valFeatData[FEATDOC_NODE_NAME_FEAT_PIPE_FIBERS].GetArray();
				Value valFiber;
				for (int i = 0; i < valFibers.Size(); i++)
				{
					Stk_Pipe_Fiber_Segment* pFiber = new Stk_Pipe_Fiber_Segment();
					pipePtr->AddPipeFiber(pFiber);
					valFiber = valFibers[i];

					if (valFiber.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CENTERPNT))
					{
						STK_PNT32 centerPnt;
						string strCenterPnt = valFiber[FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CENTERPNT].GetString();
						std::vector<std::string> vecCenterPntValue;
						CStringTools::StringSplit(strCenterPnt, " ", vecCenterPntValue);
						if (vecCenterPntValue.size() == 3)
						{
							centerPnt.X = atof(vecCenterPntValue[0].c_str());
							centerPnt.Y = atof(vecCenterPntValue[1].c_str());
							centerPnt.Z = atof(vecCenterPntValue[2].c_str());
						}
						pFiber->SetCenterPoint(centerPnt);
					}

					if (valFiber.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_STARTPNT))
					{
						STK_PNT32 startPnt;
						string strStartPnt = valFiber[FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_STARTPNT].GetString();
						std::vector<std::string> vecStartPntValue;
						CStringTools::StringSplit(strStartPnt, " ", vecStartPntValue);
						if (vecStartPntValue.size() == 3)
						{
							startPnt.X = atof(vecStartPntValue[0].c_str());
							startPnt.Y = atof(vecStartPntValue[1].c_str());
							startPnt.Z = atof(vecStartPntValue[2].c_str());
						}
						pFiber->SetStartPoint(startPnt);
					}

					if (valFiber.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_ENDPNT))
					{
						STK_PNT32 endPnt;
						string strEndPnt = valFiber[FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_ENDPNT].GetString();
						std::vector<std::string> vecEndPntValue;
						CStringTools::StringSplit(strEndPnt, " ", vecEndPntValue);
						if (vecEndPntValue.size() == 3)
						{
							endPnt.X = atof(vecEndPntValue[0].c_str());
							endPnt.Y = atof(vecEndPntValue[1].c_str());
							endPnt.Z = atof(vecEndPntValue[2].c_str());
						}
						pFiber->SetEndPoint(endPnt);
					}

					if (valFiber.HasMember(FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CURVE))
					{
						pFiber->SetCurve(CurveFromJsonValue(valFiber[FEATDOC_NODE_NAME_FEAT_PIPE_FIBER_CURVE], jsonDoc));
					}
				}
			}

			return result;
		}


		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::MarkToJsonValue(Stk_Feat_MarkPtr markPtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;
			valFeatData.SetObject();
			if (markPtr == nullptr || valFeatData.IsNull())
				return STK_ERROR;
			StkMarkTypeEnum eMarkType = markPtr->GetType();
			char cTemp[256] = { 0 };
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_TYPE, eMarkType, jsonDoc.GetAllocator());
			STK_RGBA32 color = markPtr->GetColor();
			sprintf(cTemp, "%.8f %.8f %.8f %.8f", color.Red, color.Green, color.Blue, color.Alpha);
			string strColor = cTemp;
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_COLOR, strColor, jsonDoc.GetAllocator());

			STK_MTX32 mtxDefPlane = markPtr->GetDefPlane();
			sprintf(cTemp, "%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f", 
				mtxDefPlane.PlcMatrix[0][0], mtxDefPlane.PlcMatrix[0][1], mtxDefPlane.PlcMatrix[0][2], mtxDefPlane.PlcMatrix[0][3],
				mtxDefPlane.PlcMatrix[1][0], mtxDefPlane.PlcMatrix[1][1], mtxDefPlane.PlcMatrix[1][2], mtxDefPlane.PlcMatrix[1][3], 
				mtxDefPlane.PlcMatrix[2][0], mtxDefPlane.PlcMatrix[2][1], mtxDefPlane.PlcMatrix[2][2], mtxDefPlane.PlcMatrix[2][3]);
			string strDefPlane = cTemp;
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_DEFPLANE, strDefPlane, jsonDoc.GetAllocator());

			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_CURVEWIDTH, markPtr->GetCurveWidth(), jsonDoc.GetAllocator());
			valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_CURVETYPE, markPtr->GetCurveType(), jsonDoc.GetAllocator());
			
			switch (eMarkType)
			{
			case HoteamSoft::SVLLib::MARK_TYPE_RECTANGLE:
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE, Stk_Mark_RectAnglePtr::DynamicCast(markPtr)->GetFillStyle(), jsonDoc.GetAllocator());
				STK_PNT32 pntLeftTop, pntRightBottom;
				Stk_Mark_RectAnglePtr::DynamicCast(markPtr)->GetAnchorPoint(pntLeftTop, pntRightBottom);
				
				sprintf(cTemp, "%.8f %.8f %.8f", pntLeftTop.X, pntLeftTop.Y, pntLeftTop.Z);
				string strLeftTopPnt = cTemp;
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_LEFTTOPPNT, strLeftTopPnt, jsonDoc.GetAllocator());
				sprintf(cTemp, "%.8f %.8f %.8f", pntRightBottom.X, pntRightBottom.Y, pntRightBottom.Z);
				string strRightBottomPnt = cTemp;
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_RIGHTBOTTOMPNT, strRightBottomPnt, jsonDoc.GetAllocator());
			}
				break;
			case HoteamSoft::SVLLib::MARK_TYPE_ELLIPSE:
			{
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE, Stk_Mark_EllipsePtr::DynamicCast(markPtr)->GetFillStyle(), jsonDoc.GetAllocator());
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_MAJORRADIUS, Stk_Mark_EllipsePtr::DynamicCast(markPtr)->GetMajorRadius(), jsonDoc.GetAllocator());
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_MINORRADIUS, Stk_Mark_EllipsePtr::DynamicCast(markPtr)->GetMinorRadius(), jsonDoc.GetAllocator());

				STK_PNT32 centerPnt = Stk_Mark_EllipsePtr::DynamicCast(markPtr)->GetCenter();
				sprintf(cTemp, "%.8f %.8f %.8f", centerPnt.X, centerPnt.Y, centerPnt.Z);
				string strCenterPnt = cTemp;
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_CENTERPNT, strCenterPnt, jsonDoc.GetAllocator());

				STK_DIR32 dirNormal = Stk_Mark_EllipsePtr::DynamicCast(markPtr)->GetNormal();
				sprintf(cTemp, "%.8f %.8f %.8f", dirNormal.X, dirNormal.Y, dirNormal.Z);
				string strNormal = cTemp;
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_NORMAL, strNormal, jsonDoc.GetAllocator());

				STK_DIR32 dirOrigin = Stk_Mark_EllipsePtr::DynamicCast(markPtr)->GetOriginDir();
				sprintf(cTemp, "%.8f %.8f %.8f", dirOrigin.X, dirOrigin.Y, dirOrigin.Z);
				string strOrigin = cTemp;
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_ORIGIN, strOrigin, jsonDoc.GetAllocator());
			}
				break;
			case HoteamSoft::SVLLib::MARK_TYPE_FREEHAND:
			{
				STK_FLOAT32 fUMin, fUMax;
				Stk_Mark_FreeHandPtr::DynamicCast(markPtr)->GetDomain(fUMin, fUMax);
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_MIN, fUMin, jsonDoc.GetAllocator());
				valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_MAX, fUMax, jsonDoc.GetAllocator());

				string strPnts = "";
				int iPointNum = Stk_Mark_FreeHandPtr::DynamicCast(markPtr)->GetPointsNum();
				for (int i = 0; i < iPointNum; i++)
				{
					STK_PNT32 pnt = Stk_Mark_FreeHandPtr::DynamicCast(markPtr)->GetPointByIndex(i);
					sprintf(cTemp, "%.8f %.8f %.8f ", pnt.X, pnt.Y, pnt.Z);
					strPnts.append(cTemp);
				}
				if(iPointNum>0)
				{
					valFeatData.AddMember(FEATDOC_NODE_NAME_FEAT_MARK_POINTS, strPnts, jsonDoc.GetAllocator());
				}
			}
				break;
			default:
				break;
			}
			return result;
		}

		HoteamSoft::SVLLib::STK_STATUS CRapidJsonTools::MarkFromJsonValue(Stk_Feat_MarkPtr markPtr, Value& valFeatData, Document& jsonDoc)
		{
			STK_STATUS result = STK_SUCCESS;

			if (markPtr == nullptr || valFeatData.IsNull())
				return STK_ERROR;

			StkMarkTypeEnum eMarkType = (StkMarkTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_TYPE].GetInt();
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_TYPE))
			{
				markPtr->SetType(eMarkType);
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_CURVETYPE))
			{
				markPtr->SetCurveType((StkCurveDispTypeEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_CURVETYPE].GetInt());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_CURVEWIDTH))
			{
				markPtr->SetCurveWidth(valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_CURVEWIDTH].GetFloat());
			}
			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_COLOR))
			{
				STK_RGBA32 color;
				string strColor = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_COLOR].GetString();
				std::vector<std::string> vecColorValue;
				CStringTools::StringSplit(strColor, " ", vecColorValue);
				if (vecColorValue.size() == 4)
				{
					color.Red = atof(vecColorValue[0].c_str());
					color.Green= atof(vecColorValue[1].c_str());
					color.Blue = atof(vecColorValue[2].c_str());
					color.Alpha = atof(vecColorValue[3].c_str());
				}
				markPtr->SetColor(color);
			}

			if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_DEFPLANE))
			{
				STK_MTX32 defPlane;
				string strDefPlane = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_DEFPLANE].GetString();
				std::vector<std::string> vecDefPlaneValue;
				CStringTools::StringSplit(strDefPlane, " ", vecDefPlaneValue);
				if (vecDefPlaneValue.size() >= MTX_LINE_COUNT*MTX_COL_COUNT)
				{
					for (int i = 0; i < MTX_LINE_COUNT; i++)
					{
						for (int j = 0; j < MTX_COL_COUNT; j++)
						{
							defPlane.PlcMatrix[i][j] = atof(vecDefPlaneValue[i*MTX_LINE_COUNT + j].c_str());
						}
					}
				}
				markPtr->SetDefPlane(defPlane);
			}
			

			switch (eMarkType)
			{
			case HoteamSoft::SVLLib::MARK_TYPE_RECTANGLE:
			{
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE))
				{
					Stk_Mark_RectAnglePtr::DynamicCast(markPtr)->SetFillStyle((StkMarkFillStyleEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE].GetInt());
				}
				
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_LEFTTOPPNT) &&
					valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_RIGHTBOTTOMPNT))
				{
					STK_PNT32 leftTopPnt;
					string strLeftTopPnt = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_LEFTTOPPNT].GetString();
					std::vector<std::string> vecLeftTopValue;
					CStringTools::StringSplit(strLeftTopPnt, " ", vecLeftTopValue);
					if (vecLeftTopValue.size() == 3)
					{
						leftTopPnt.X = atof(vecLeftTopValue[0].c_str());
						leftTopPnt.Y = atof(vecLeftTopValue[1].c_str());
						leftTopPnt.Z = atof(vecLeftTopValue[2].c_str());
					}

					STK_PNT32 rightBottomPnt;
					string strRightBottomPnt = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_RIGHTBOTTOMPNT].GetString();
					std::vector<std::string> vecRightBottomValue;
					CStringTools::StringSplit(strRightBottomPnt, " ", vecRightBottomValue);
					if (vecRightBottomValue.size() == 3)
					{
						rightBottomPnt.X = atof(vecRightBottomValue[0].c_str());
						rightBottomPnt.Y = atof(vecRightBottomValue[1].c_str());
						rightBottomPnt.Z = atof(vecRightBottomValue[2].c_str());
					}
					Stk_Mark_RectAnglePtr::DynamicCast(markPtr)->SetAnchorPoint(leftTopPnt, rightBottomPnt);
				}
			}
				break;
			case HoteamSoft::SVLLib::MARK_TYPE_ELLIPSE:
			{
				if(valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE))
				{
					Stk_Mark_EllipsePtr::DynamicCast(markPtr)->SetFillStyle((StkMarkFillStyleEnum)valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_FILLSTYLE].GetInt());
				}
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_MAJORRADIUS))
				{
					Stk_Mark_EllipsePtr::DynamicCast(markPtr)->SetMajorRadius(valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_MAJORRADIUS].GetFloat());
				}
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_MINORRADIUS))
				{
					Stk_Mark_EllipsePtr::DynamicCast(markPtr)->SetMinorRadius(valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_MINORRADIUS].GetFloat());
				}

				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_CENTERPNT))
				{
					STK_PNT32 centerPnt;
					string strCenterPnt = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_CENTERPNT].GetString();
					std::vector<std::string> vecCenterPntValue;
					CStringTools::StringSplit(strCenterPnt, " ", vecCenterPntValue);
					if (vecCenterPntValue.size() == 3)
					{
						centerPnt.X = atof(vecCenterPntValue[0].c_str());
						centerPnt.Y = atof(vecCenterPntValue[1].c_str());
						centerPnt.Z = atof(vecCenterPntValue[2].c_str());
					}
					Stk_Mark_EllipsePtr::DynamicCast(markPtr)->SetCenter(centerPnt);
				}

				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_NORMAL))
				{
					STK_DIR32 dirNormal;
					string strNormal = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_NORMAL].GetString();
					std::vector<std::string> vecNormalValue;
					CStringTools::StringSplit(strNormal, " ", vecNormalValue);
					if (vecNormalValue.size() == 3)
					{
						dirNormal.X = atof(vecNormalValue[0].c_str());
						dirNormal.Y = atof(vecNormalValue[1].c_str());
						dirNormal.Z = atof(vecNormalValue[2].c_str());
					}
					Stk_Mark_EllipsePtr::DynamicCast(markPtr)->SetNormal(dirNormal);
				}

				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_ORIGIN))
				{
					STK_DIR32 dirOrigin;
					string strOrigin = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_ORIGIN].GetString();
					std::vector<std::string> vecOriginValue;
					CStringTools::StringSplit(strOrigin, " ", vecOriginValue);
					if (vecOriginValue.size() == 3)
					{
						dirOrigin.X = atof(vecOriginValue[0].c_str());
						dirOrigin.Y = atof(vecOriginValue[1].c_str());
						dirOrigin.Z = atof(vecOriginValue[2].c_str());
					}
					Stk_Mark_EllipsePtr::DynamicCast(markPtr)->SetOriginDir(dirOrigin);
				}
			}
				break;
			case HoteamSoft::SVLLib::MARK_TYPE_FREEHAND:
			{
				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_MIN) &&
					valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_MAX))
				{
					STK_FLOAT32 fMin = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_MIN].GetFloat();
					STK_FLOAT32 fMax = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_MIN].GetFloat();
					Stk_Mark_FreeHandPtr::DynamicCast(markPtr)->SetDomain(fMin, fMax);
				}


				if (valFeatData.HasMember(FEATDOC_NODE_NAME_FEAT_MARK_POINTS))
				{
					string strPoints = valFeatData[FEATDOC_NODE_NAME_FEAT_MARK_POINTS].GetString();
					std::vector<std::string> vecPointsValue;
					CStringTools::StringSplit(strPoints, " ", vecPointsValue);
					for (int i = 0; i < vecPointsValue.size() / 3; i++)
					{
						STK_PNT32 pnt;
						pnt.X = atof(vecPointsValue[i * 3].c_str());
						pnt.Y = atof(vecPointsValue[i * 3 + 1].c_str());
						pnt.Z = atof(vecPointsValue[i * 3 + 2].c_str());
						Stk_Mark_FreeHandPtr::DynamicCast(markPtr)->AddPoint(pnt);
					}
				}
			}
				break;
			default:
				break;
			}
			return result;
		}

		STK_STATUS CRapidJsonTools::getInstanceNames(string& i_AttributeBuf, map<STK_UINT32, std::wstring>& i_mapInstanceIDtoName)
		{
			STK_STATUS status = STK_SUCCESS;
			InstanceNameHandler handler;
			Reader reader;
			StringStream ss(i_AttributeBuf.c_str());
			reader.Parse(ss, handler);
			if (handler.vecInstanceID.size() != handler.vecInstanceName.size())
				return STK_ERROR;
			int instanceCount = handler.vecInstanceID.size();
			for (int i = 0; i < instanceCount; i++)
			{
				string strInstanceName = handler.vecInstanceName[i];
				//wstring wstrInstanceName = ChineseCode::StringToWString(strInstanceName);
				wstring wstrInstanceName = ChineseCode::UTF_8ToUnicode(strInstanceName.c_str());
				i_mapInstanceIDtoName.insert(make_pair(handler.vecInstanceID[i], wstrInstanceName));
			}
			//清空数据
			handler.vecInstanceID.clear();
			handler.vecInstanceName.clear();
			handler.vecInstanceID.shrink_to_fit();
			handler.vecInstanceName.shrink_to_fit();
			return status;
		}

		STK_STATUS CRapidJsonTools::getAttributesByIds(string& i_AttributeBuf, vector<STK_UINT32> i_vecId, map<STK_UINT32, map<std::wstring, std::wstring>>& i_mapKeyToValue)
		{
			STK_STATUS status = STK_SUCCESS;
			AttributeHandler handler;
			Reader reader;
			StringStream ss(i_AttributeBuf.c_str());
			handler.vecInstanceID = i_vecId;
			handler.bNeedAdd = false;
			reader.Parse(ss, handler);
			i_mapKeyToValue = handler.mapIDToKeyValue;
			return status;
		}

        string CRapidJsonTools::RemoveExcess(string& strStream){
            printf("strStreamstrStream 0 : %s",strStream.c_str());
            int index = 0;
            if( !strStream.empty())
            {
                while( (index = strStream.find('\r',index)) != string::npos)
                {
                    strStream.erase(index,1);
                }
                index = 0;
                while( (index = strStream.find('\n',index)) != string::npos)
                {
                    strStream.erase(index,1);
                }
                index = 0;
                while( (index = strStream.find(' ',index)) != string::npos)
                {
                    if (index - 1 > -1) {
                        string s = strStream.substr(index-1,1);
                        if (s != "}") {
                            strStream = strStream.replace(index, 1, "$");
                        }else{
                            strStream.erase(index,1);
                        }
                    }
                }
            }
            index = 0;
            while( (index = strStream.find('$',index)) != string::npos)
            {
                strStream.replace(index, 1, " ");
            }
            printf("strStreamstrStream 1 : %s",strStream.c_str());
            return strStream;
        }
	}//svllib	

}//hoteamsoft
