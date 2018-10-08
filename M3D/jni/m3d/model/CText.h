/**@file
 *@brief
 *@author	bing
 *@date		2014-2-20
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_CTEXT_H_
#define M3D_CTEXT_H_
#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"

class FTFont;

namespace M3D
{

/**@class CText
 * @brief 表示一个空间中的文本串，使用utf-8编码
 *
 */
class M3D_API CText
{
public:
	CText();
	virtual ~CText();

	/**
	 * 得到显示定位点位置
	 * @return
	 */
	Vector3& GetInnerLoc()
	{
		return m_pntInnerLoc;
	}
	/**
	 * 设置显示定位点位置
	 * @param loc
	 */
	void SetInnerLoc(const Vector3& loc)
	{
		m_pntInnerLoc = loc;
	}

	/**
	 * 得到文本
	 * @return
	 */
	string& GetText()
	{
		return m_Text;
	}

	/**
	 * 设置文本
	 * @param str
	 */
	void SetText(string str)
	{
		m_Text = str;
	}
    /**
     * 获取文本language
     * @return
     */
    string& GetLanguageType()
    {
        return m_LanguageType;
    }
    
    /**
     * 设置文本
     * @param str
     */
    void SetLanguageType(string typeStr)
    {
        m_LanguageType = typeStr;
    }
	/**
	 * 获取当前文本 X,Y旋转向量
	 * @param xAxis
	 * @param yAxis
	 */
	void GetInnerXYAxis(Vector3& xAxis, Vector3& yAxis);
	/**
	 * 设置 X,Y旋转向量
	 * @param xAxis
	 * @param yAxis
	 */
	void SetInnerXYAxis(const Vector3& xAxis, const Vector3& yAxis);

	/**
	 * 获取当前字符宽高
	 * @param oWidth
	 * @param oHeight
	 */
	void GetCharWidthHeight(float& oWidth, float& oHeight);
	/**
	 * 设置当前字符宽高
	 * @param iWidth
	 * @param iHeight
	 */
	void SetCharWidthHeight(const float& iWidth, const float& iHeight);

	/**
	 * 得到字符间隔
	 * @return
	 */
	float GetCharSpacing()
	{
		return m_fCharDis;
	}

	/**
	 * 设置字符间隔
	 * @param i_fCharDis
	 */
	void SetCharSpacing(const float& i_fCharDis)
	{
		m_fCharDis = i_fCharDis;
	}

	/**
	 * 字体路径
	 * @param ofont
	 */
	static void GetFontFilePath(char* ofont);
	/**
	 * 设置字体路径
	 * @param ifont
	 */
	static void SetFontFilePath(const char* ifont);

	/**
	 * 根据当前属性创建显示数据buff
	 */
	void CreateBuff();
	static void InitRender();

	/**
	 * 得到字体解析器
	 * @return
	 */
	static FTFont* GetRender();

private:
	Vector3 m_pntInnerLoc; //!< 定位点
	Vector3 m_dirAxis[2]; //!< X,Y旋转向量,[0]->X向量,[1]->Y向量
	float m_fCharSize[2]; //!< 字符宽高,[0]->宽；[1]->高
	float m_fCharDis; //!< 字符间隔
	static char m_FontFilePath[256]; //!< 字库文件路径
	string m_Text; //!< 字符串内容
    string m_LanguageType; //!< language类型
	vector<float> m_VertexBuff; /// 顶点buff
	vector<M3D_INDEX_TYPE> m_IndexBuff; /// 索引buff

	static FTFont *fontRender;//!< 字体解析器

};
}

#endif /* CTEXT_H_ */
