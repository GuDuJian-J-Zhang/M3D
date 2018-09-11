/**@file
 *@brief
 *@author	XL
 *@date		2015-7-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_NOTE_FACTORY_H_
#define     M3D_NOTE_FACTORY_H_

#include 	"m3d/base/Object.h"

namespace M3D
{
class Vector2;
class Vector3;
class Note;
class SequenceNumberNote;
class TextNote;
class VoiceNote;
class ThreeDGesturesNote;
class SceneManager;
class Properties;
struct Gesture3DInfo;
struct CommonNoteInfo;
class BoundingBox;
class Model;
class Body;
class Face;
class Mesh;
class SceneNode;
struct InsectMeshSetInfo
{
	vector<Vector3>* meshSet;
	BoundingBox* box;
};

/**
 * @brief 创建批注类。用来创建文本批注、语音批注和序号批注
 */
class M3D_API NoteFactory: public Object
{
public:
	/**
	 * @brief 创建语音批注
	 * @param screenPnt 屏幕坐标点
	 * @param text  (保留参数）
	 * @param type 类型
	 * @param scene 场景指针
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateVoiceNote(Vector2& screenPnt, const string& text, int type, SceneManager* scene);

	/**
	 * @brief 创建文本批注
	 * @param firtShapId shape ID
	 * @param screenPnt 拾取的屏幕点
	 * @param text 文本内容
	 * @param type 类型
	 * @param scene 场景
	 * @param properties 附加属性
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateTextNote(int firtShapId, Vector2& screenPnt, const string& text, int type, SceneManager* scene,
			Properties* properties = NULL);

	/**
	 * @brief 创建序号批注
	 * @param firtShapId shape ID
	 * @param screenPnt 屏幕点
	 * @param text 文本内容
	 * @param scene 场景
	 * @param properties 附加属性
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateSequenceNumberNote(int firtShapId, Vector2& screenPnt, const string& text, SceneManager* scene,
			Properties* properties = NULL);

	/**
	 * @brief 从XML中创建文本批注
	 * @param scene 场景
	 * @param xmlElement 传来的XML内容
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateTextNoteFromXMLElement(SceneManager* scene, const string& xmlElement);

	/**
	 * @brief 从xml中创建语音批注
	 * @param scene 场景
	 * @param xmlElement xml
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateVoiceNoteFromXMLElement(SceneManager* scene, const string& xmlElement);

	/**
	 * @brief 序号批注
	 * @param scene 场景
	 * @param xmlElement xml
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateSequenceNoteFromXMLElement(SceneManager* scene, const string& xmlElement);

	/**
	 * @brief 从Json串中创建
	 * @param scene 场景
	 * @param JSONValue Json字符串
	 * @return 返回批注 失败 为NULL
	 */
	static Note* CreateTextNoteFromJSON(SceneManager* scene, const string& JSONValue);
	/**
	 * @brief 将文本批注转化为XML字符串
	 * @param scene 场景
	 * @param textNote 文本批注
	 * @return 返回批注 失败 为空字符串
	 */
	static string TextNoteToXMLElement(SceneManager* scene, TextNote* textNote);

	/**
	 * @brief 序号批注转化为XML
	 * @param scene 场景
	 * @param sequenceNote 序号批注
	 * @return 返回批注 失败 为空字符串
	 */
	static string SequenceNoteToXMLElement(SceneManager* scene, SequenceNumberNote* sequenceNote);

	/**
	 * @brief 语音批注转化为XML
	 * @param scene 场景
	 * @param voiceNote 语音批注
	 * @return 返回批注 失败 为空字符串
	 */
	static string VoiceNoteToXMLElement(SceneManager* scene, VoiceNote* voiceNote);
	/**
	 * @brief 文本批注转化为Json串  TODO
	 * @param scene 场景
	 * @param textNote 文本批注
	 * @return 返回字符串
	 */
	static string TextNoteToJSONValue(SceneManager* scene, TextNote* textNote);
	/**
	 * @brief 将文本批注加入场景
	 * @param scene 场景
	 * @param note 批注
	 * @return 成功 true 失败 false
	 */
	static bool AddNoteToScene(SceneManager* scene, Note* note);

	/**
	 * @brief 创建空的textnote
	 * @param scene 场景
	 * @return 返回批注，失败 NULL
	 */
	static Note* CreateTextNote(SceneManager* scene);

	/**
	 * @brief 更改批注图片显示位置
	 * @return 成功 true 失败 false
	 */
	static bool UpdateNoteImagePos(Note* note, Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 编辑批注文本
	 * @param note 批注
	 * @param text 文本内容
	 * @param scene 场景
	 * @return 成功 true 失败 false
	 */
	static bool UpdateNoteTextValue(Note* note, string text, SceneManager * scene);

	static Note* CreateThreeDGestureNote(Gesture3DInfo& gesture3DInfos,SceneManager * scene);

	static Note* CreateCommonNote(CommonNoteInfo& commonNoteInfo,SceneManager * scene);

	static string ThreeDGestureNoteToXMLElement(SceneManager* scene,ThreeDGesturesNote* threeDGesturesNote);

	static Note* CreateThreeDGestureNoteFromXMLElement(SceneManager* scene,const string& xmlElement);

private:
	/**
	 * @brief 创建文本批注
	 * @param firtShapId shape ID
	 * @param screenPnt 屏幕坐标点
	 * @param text 文本
	 * @param scene 场景
	 * @param properties 附加属性
	 * @return 返回批注 失败 NULL
	 */
	static Note* CreatePntTextNote(int firtShapId, Vector2& screenPnt, const string& text, SceneManager* scene,
			Properties* properties = NULL);

	/**
	 * @brief 创建序号批注
	 * @param firtShapId shape ID
	 * @param screenPnt 屏幕坐标点
	 * @param text 文本内容
	 * @param scene 场景
	 * @param properties 附加属性
	 * @return 返回批注 失败为NULL
	 */
	static Note* CreatePntSequenceNumberNote(int firtShapId, Vector2& screenPnt, const string& text,
			SceneManager* scene, Properties* properties = NULL);

	/**
	 * @brief 创建语音批注
	 * @param screenPnt 屏幕坐标点
	 * @param text 文本
	 * @param scene 场景
	 * @return 成功返回批注 失败NULL
	 */
	static Note* CreatePntVoiceNote(Vector2& screenPnt, const string& text, SceneManager* scene);
	/**
	 * 获取语音批注显示大小，因为不同的模型，需要构造不同大小的语音批注，使其看起来美观
	 * @return
	 */
//	static  Vector2 GetVoiceNoteSize(SceneManager* scene);
//	static ImageBoard*  createNoteTextsImage(SceneManager* scene,
//			vector<Texts2D*>& textsArray, Vector3 & position);
	static void CreateNoteRecImage();

private:
	//一些工具函数
	static	void GetMeshSet(BoundingBox& box, vector<Vector3>&triangleSet, SceneManager* scene);
	static void GetInsectShape(void* data, Model* node);
	static void GetInsectModel(void * data, Model * model);
	static void GetInsectBody(vector<Vector3>& lineDatas,  Body * body, BoundingBox& lineBox);
	static void GetInsectFace(vector<Vector3>& lineDatas,  Face * face, BoundingBox& lineBox);
	static void GetInsectMesh(vector<Vector3>& lineDatas, Mesh * mesh, BoundingBox& lineBox);
	static void InsectPoint(SceneManager * scene,vector<Vector3> & originScreenPnts,vector<Vector3> & origin3DPnts,vector<vector<Vector3> > &finalPoints);
	static void SplitPolyLine(Gesture3DInfo & originalInfo,Gesture3DInfo& currentInfo,SceneManager * scene);
public:
	NoteFactory();

	virtual ~NoteFactory();
public:
	/*
	 * 以以下为序列化和反序列化view文件的标签定义，详见view描述
	 */
	const static string Serializer_XML_Element_SVL;

	const static string Serializer_XML_Element_Header ;
	const static string Serializer_XML_Element_Version ;
	const static string Serializer_XML_Element_Author ;
	const static string Serializer_XML_Element_Created ;
	const static string Serializer_XML_Element_Generator ;

	const static string Serializer_XML_Element_Model ;
	const static string Serializer_XML_Element_Users ;
	const static string Serializer_XML_Element_User ;
	const static string Serializer_XML_Element_Notes ;
	const static string Serializer_XML_Element_TextNote ;
	const static string Serializer_XML_Element_VoiceNote;
	const static string Serializer_XML_Element_SequenceNote;
	const static string Serializer_XML_Element_ThreeDGestureNote;
	const static string Serializer_XML_Element_Color ;

	const static string Serializer_XML_Element_VoiceData;
	const static string Serializer_XML_Element_ThreeDGestureNoteLine;

	const static string Serializer_XML_Element_Leaders ;
	const static string Serializer_XML_Element_Leader ;
	const static string Serializer_XML_Element_Terminator ;
	const static string Serializer_XML_Element_Location ;
	const static string Serializer_XML_Element_Direction ;
	const static string Serializer_XML_Element_Polyline ;
	const static string Serializer_XML_Element_Point ;
	const static string Serializer_XML_Element_ExtendLines ;

	const static string Serializer_XML_Element_CompositeTexts ;
	const static string Serializer_XML_Element_CompositeText ;
	const static string Serializer_XML_Element_Texts ;
	const static string Serializer_XML_Element_Text ;
	const static string Serializer_XML_Element_TextStyle ;
	const static string Serializer_XML_Element_CharHeight ;
	const static string Serializer_XML_Element_LineSpacing ;
	const static string Serializer_XML_Element_Font ;
	const static string Serializer_XML_Element_UsageType ;

		// public final static String Serializer_XML_Element_Location = "Location";
	const static string Serializer_XML_Element_Rotation ;
	const static string Serializer_XML_Element_OuterBox ;
	const static string Serializer_XML_Element_ProjectMatrix ;
	const static string Serializer_XML_Element_Attributes ;
	const static string Serializer_XML_Element_Attribute;

	const static string Serializer_XML_Attribute_ID;
    const static string Serializer_XML_Attribute_SpacePoint;
	const static string Serializer_XML_Attribute_UserID ;
	const static string Serializer_XML_Attribute_Created ;
	const static string Serializer_XML_Attribute_IsParallelScreen ;
	const static string Serializer_XML_Attribute_IsFix ;
	const static string Serializer_XML_Attribute_Visible ;
	const static string Serializer_XML_Attribute_R ;
	const static string Serializer_XML_Attribute_G ;
	const static string Serializer_XML_Attribute_B ;
	const static string Serializer_XML_Attribute_Color ;
	const static string Serializer_XML_Attribute_X ;
	const static string Serializer_XML_Attribute_Y ;
	const static string Serializer_XML_Attribute_Z ;
	const static string Serializer_XML_Attribute_Value ;
	const static string Serializer_XML_Attribute_Type ;
	const static string Serializer_XML_Attribute_Key ;
	const static string Serializer_XML_Attribute_URI ;
	const static string Serializer_XML_Attribute_Width;
	const static string Serializer_XML_Attribute_Height ;
	const static string Serializer_XML_Attribute_Name ;
	const static string Serializer_XML_Attribute_Min;
	const static string Serializer_XML_Attribute_Max;


	const static string Serializer_XML_Attribute_Format;

};

}
#endif

