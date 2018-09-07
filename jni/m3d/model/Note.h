/**@file
 *@brief	Note类头文件
 *@author	bing
 *@date		2013-11-27
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Note类头文件
 *
 */

#ifndef M3D_NOTE_H_
#define M3D_NOTE_H_
#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/model/ComText.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/model/Note.h"
#include "m3d/graphics/Billboard.h"

namespace M3D
{
class ImageBoard;
class PolyLine;
class Shape;
class BoundingBox;
class Line3D;
class Point;
class RenderAction;
/**
 * 属性的封装，通过key可以设置和获取属性
 */
class M3D_API Properties
{
public:
    Properties();
    /**
     * 获取属性
     */
	string GetProperty(string key);
	/**
	 * 设置属性
	 */
	void SetProperty(string key, string value);
	/**
	 * 清空所有属性
	 */
	void ClearProperties();

	/**
	 * 根据key删除属性值
	 * @param key
	 * @return
	 */
	bool DeleteProperty(string key);

	/**
	 * 复制属性到传入参数中
	 * @param properties
	 * @return
	 */
	bool CopyProperties(Properties* properties);

	/**
	 * 获取内部属性存储表
	 * @return
	 */
	map<string, string>& GetPropertyMap()   {return m_appendInfo;}

private:
	map<string, string> m_appendInfo; ///附加信息
};

/**
 * @class Note
 * @brief Note类
 *
 * 表示一个多种图形的组合，如标注、测量等。包含点、线、折线、平面、文字等多种基本类型的图形。
 */
class M3D_API Note: public Shape, public Renderable, public Properties
{
public:
	/*批注类型：-1=未定义，0=文本TextNote，1=voiceNote,2=videoNote,3=GestureNote,4=ImageNote...
	 *
	 */
	enum NoteType
	{
		Undefine = -1,
		TextNote = 0,
		VoiceNote = 1,
		VidioNote = 2,
		GestureNote = 3,
		ImageNote = 4,
		ThreeDGestureNote = 5
	};

	Note();
	virtual ~Note();

	void AddPoint(Point* pnt);

	void AddLine(Line3D* line);
	/**
	 * 添加引出线
	 * @param polyLine
	 */
	void AddPolyLine(PolyLine* polyLine);

	void AddImage(ImageBoard* imageBoard);

	void AddText(ComText* ct);

	void AddText(const string& text, const Vector3& pos, float fontSize);

	virtual void SetSelected(bool select);
	/**
	 * 添加外框
	 * @param polyLine 外框线
	 */
	void AddOutFrame(PolyLine* polyLine);

	virtual void RayPick(RayPickAction* action);

	virtual void ComputeBox();

	virtual void FindVisiableObject(RenderAction* renderAction);

	void SetVisiableObject(RenderAction* renderAction);

	bool IsDirty();

	void MarkDirty();

	/**
	 * 是否固定大小显示
	 * @return
	 */
	bool IsFix()
	{
		return m_IsFix;
	}

	/**
	 * 设置是否使用固定大小显示
	 * @param IsFix
	 */
	void SetFix(bool IsFix)
	{
		m_IsFix = IsFix;
	}

	/**
	 * 是否平齐屏幕
	 * @return
	 */
	bool IsParallelScreen()
	{
		return m_IsParallelScreen;
	}
	/**
	 * 设置平齐屏幕显示
	 * @param IsParallelScreen
	 */
	void SetParallelScreen(bool IsParallelScreen)
	{
		m_IsParallelScreen = IsParallelScreen;
	}

	/**
	 * 获取停靠点
	 * @return
	 */
	Vector3 GetAnchorPnt();

	/**
	 * 是否包含引出线
	 * @return
	 */
	bool HasLeader()
	{
		return hasLeader;
	}

	/**
	 * 是指是否包含引出先标识
	 * @param flag
	 */
	void SetHasLeader(bool flag)
	{
		hasLeader = flag;
	}

	Billboard* GetBillBoard();

	/**
	 *获取显示图片列表
	 * @return
	 */
	vector<ImageBoard*>& GetImageBoards();
	Vector3 GetImageBoardPosition();

	virtual string GetTextValue();
	virtual void SetTextValue(string& value);

private:
	///计算包围盒
	void Clear();
	void Init();

	Vector3 GetTextsCenter();

public:
	vector<Point*> m_PointList; //!<点列表

	vector<Line3D*> m_LineList; //!<线列表

	vector<PolyLine*> m_PolyLineList; //!<折线列表

	vector<ComText*> m_ComTexts; //!<复合文本列表

	vector<PolyLine*> m_outFrameLineList; //!<外框

	vector<ImageBoard*> m_imageBoardList; //!<图片列表

	bool m_IsParallelScreen; //!<是否平行于屏幕
	bool m_IsFix;//!<是否固定大小

	vector<Vector3> m_drawPoints; //!<构造出待绘制的点

	vector<Vector3> m_drawLines; //!<构造出待绘制的线

	bool m_isDirty;//!<脏标识

protected:
	bool hasLeader;//!<包含引出线
	mutable Mutex m_mutex; //!<线程锁对象
	Billboard m_bindBillboard; //!< 文本显示时绑定面的控制面
};

}

#endif /* NOTE_H_ */
