/**@file
 *@brief	场景序列化器XML文件版
 *@author	bing
 *@date		2014-1-7
 *@version	1.0
 *
 *场景序列化器XML文件版
 *
 */

#ifndef XMLHELPER_H_
#define XMLHELPER_H_

#include "sview/SView.h"
#include "tinyxml/tinyxml2.h"
using   tinyxml2::XMLDocument;
using   tinyxml2::XMLElement;
using   tinyxml2::XMLNode;
using   tinyxml2::XMLText;

namespace M3D
{
	class ModelView;
 
}

#include "m3d/model/Note.h"
#include "m3d/model/PolyLine.h"
#include "m3d/base/Vector3.h"
using namespace M3D;
namespace SVIEW
{

/**@class XmlSceneSerializer
 * @brief 场景序列化器XML文件版
 *
 * 实现场景到xml文件的序列化和反序列化
 *
 */
class M3D_API XmlSceneSerializer {
public:
	XmlSceneSerializer();
	~XmlSceneSerializer();
    /*设置当前视图
     *
     * */
    void XmlSetCurrentModelView(int curModelViewId);
    /*设置手势批注线点集
     *
     * */
    void XmlSetGesturePointlines(vector<PolyLine> Pointlines);
    /*设置手势批注线颜色集
     *
     * */
    void XmlSetGesturePointlineColors(vector<Color>colors);
	/**@brief 加载xml文档到doc
	 * @param xmlPath 文件路径
	 * @param createIfNotFound 如果不存在文件，是否创建
	 * @return XMLDocument对象指针
	 */
	bool Load(string& xmlPath,bool createIfNotFound);

	/**@brief 创建一个新Doc
	 * @return 创建是否成功
	 */
	bool CreateDocument();

	/**@brief 创建一个Model在svl节点下
	 *
	 */
	bool CreateModel();

	/**@brief 保存doc文档到xml
	 * @param doc doc文档对象指针
	 * @param xmlPath 文件路径
	 * @return 如果成功，返回true，否则false
	 */
	bool Save(string& xmlPath);

	/**@brief 在doc文档中创建视图节点
	 * @param viewList 系统中所有用户视图列表
	 * @return 如果成功，返回true，否则false
	 */
	bool CreateViews(vector<ModelView *>* viewList);

	/**
	 * @brief 在文档的Notes节点的最后中插入一个note
	 * @param pNote
	 */
	void CreateNote(Note *pNote);

	/*根据TextNote对象创建一个文本节点
	 * */
	XMLElement* CreateTextNote(Note *pNote);
    XMLElement* CreateGestureNote(XMLElement* viewElement,ModelView*pView);
    
    /* @brief 根据ModelView对象创建一个文本节点
     *
     * @param 返回文本串
     * */
    string LoadStringFromModelView();
    /* @brief 根据xmlString字符串创建一个ModelView
     *
     * @param 返回ModelView
     * */
    ModelView * LoadModelViewFromXml(string &xmlString);
    
	/**@brief 从doc中删除指定id的视图
	 * @param doc doc文档对象指针
	 * @param viewId 视图id
	 * @return 如果成功，返回true，否则false
	 */
	bool DeleteView(int viewId);

	/**@brief 从doc中获取指定id的视图
	 * @param doc doc文档对象指针
	 * @param viewId 视图id
	 * @return 视图对象（ModelView）实例
	 * @deprecated 将在下一版本删除
	 */
	ModelView* LoadModelView(int viewId);

	/**@brief 从doc中获取所有视图对象
	 * @param doc doc文档对象指针
	 * @param pOutViewList 获取到的视图列表
	 */
	void LoadAllModelView(vector<ModelView*>* pOutViewList);

	/**
	 * 从xml中获取所有用户Note
	 * @param outNoteList
	 * @return
	 */
	vector<Note*>& LoadAllNotes(vector<Note*>& outNoteList);
    void LoadAllGestureNotes(vector<PolyLine>& polyLineList,vector<Color>& colors);
/**
 * 为了与PC保持一致，不在loadallnotes中更改。采用新函数，读取view中添加的notes
 * @param outNoteList
 * @return
 */
	vector<Note*>& LoadAllViewNotes(vector<Note*>& outNoteList);



private :

	XMLDocument* m_Doc;


	/**@brief 从视图element中获取一个视图对象实例
	 * @param viewElement 视图element
	 * @return 视图对象（ModelView）实例
	 * @see M3D::ModelView
	 */
	ModelView* GetViewFromElement(XMLElement* viewElement);

	/**从xmlelement中提取note对象
	 *
	 * @param noteElement
	 * @param note
	 * @return
	 */
	virtual Note& GetNoteFromElement(XMLElement* noteElement,Note& note);


	/**
	 * XML合并
	 * @param node 需要合并的XML根
	 * @param doc
	 * @return
	 */
	XMLNode* Clone(XMLNode* node,XMLDocument* doc);

	void CreateNoteStringFromDoc(ModelView * pView,XMLElement* viewElement);

	//tools

	/**@brief 三维点 序列化 成字符串的方法
	 * @param point 三维点
	 * @param outStr 要输出的字符串指针
	 */
	static void StringFromPoint(M3D::Vector3& point,char* outStr);

	/**
	 * 字符串 反序列化 为三维点
	 * @param inStr
	 * @param outPoint
	 */
	static void StringToPoint(const char* inStr,M3D::Vector3& outPoint);


	/**@brief vector<int> 序列化成字符串
	 * @param intList int列表
	 * @param outStr 要输出的字符串指针
	 */
	static void StringFromIntList(vector<int>& intList,char* outStr);

	/**
	 * 字符串反序列化为vector<int>
	 * @param intListStr
	 * @param outIntList
	 * @return
	 */
	static vector<int>& StringToIntList(const char* intListStr,vector<int>& outIntList);

	/**@brief vector<float> 序列化成字符串
	 * @param floatList float列表
	 * @param outStr 要输出的字符串指针
	 */
	static void StringFromFloatList(vector<float>& floatList,char* outStr);

	static void StringFromColor(Color &color,char* outStr);

	/**@brief 字符串反序列化为浮点数组
	 * @param floatListStr float列表字符串
	 * @param outFloatList float列表
	 */
	static vector<float>& StringToFloatList(const char* floatListStr,vector<float>& outFloatList);

	/*@brief格式化
	 *
	 *
	 * */
	static void StringToColor(const char* hexStr,Color& targetColor);

};

}

#endif /* XMLHELPER_H_ */
