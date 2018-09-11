#ifndef _H_ANIMATION_DEF_H
#define _H_ANIMATION_DEF_H

// define size for big buffers
#define SA_BUFFER_SIZE_BIG			4096

// define size for small buffers
#define SA_BUFFER_SIZE_SMALL		256

#include <string.h>

#define ANIMATION_VERSION "2.6"

#define TARGETOBJECTTYPE_INS	"PLCID"
#define TARGETOBJECTTYPE_CAM	"CAMERA"
#define TARGETOBJECTTYPE_PMI	"PMI"
#define TARGETOBJECTTYPE_CLIP	"CLIP"
#define TARGETOBJECTTYPE_SOUND	"SOUND"
#define TARGETOBJECTTYPE_IMAGE	"IMAGE"
#define TARGETOBJECTTYPE_FOLDER	"FOLDER"
#define TARGETOBJECTTYPE_ZOOM	"ZOOM"
#define TARGETOBJECTTYPE_GROUP	"GROUPID"
#define TARGETOBJECTTYPE_ARROW	"ARROW"
#define TARGETOBJECTTYPE_TOOL	"TOOL"

#define TARGETOBJECTTYPE_NAME_INS	"模型"
#define TARGETOBJECTTYPE_NAME_CAM	"摄像机"
#define TARGETOBJECTTYPE_NAME_PMI	"PMI"
#define TARGETOBJECTTYPE_NAME_CLIP	"剖面"
#define TARGETOBJECTTYPE_NAME_SOUND	"声音"
#define TARGETOBJECTTYPE_NAME_IMAGE	"图片"
#define TARGETOBJECTTYPE_NAME_ZOOM	"局部放大"
#define TARGETOBJECTTYPE_NAME_GROUP	"分组"
//公差相关模型
#define ANIMATION_D_TOL					(0.9)
#define ANIMATION_D_TOL2				(0.00001)


enum  TargetObjectType
{
	TARGETOBJECT_TYPE_INS = 0,    //实例
	TARGETOBJECT_TYPE_CAM,        //摄像机
	TARGETOBJECT_TYPE_PMI,        //PMI
	TARGETOBJECT_TYPE_CLIP,       //切面
	TARGETOBJECT_TYPE_SOUND,      //声音
	TARGETOBJECT_TYPE_IMAGE,      //图片
};
//零件、PMI等信息   add by qinyp 15/11/03
struct TargetObjectInfo
{
	char m_Name[SA_BUFFER_SIZE_SMALL];       //名称
	int  m_ID;                                //ID号
	char m_Path[SA_BUFFER_SIZE_SMALL]; //对象全路径
	char m_InsPath[SA_BUFFER_SIZE_SMALL];  //路径实例部分 
	int m_EntId;                                  //路径ID部分 
	float m_Pos[3];//= {0.0f,0.0f,0.0f };                             //位置
	float m_Quat[4];                             //旋转
	float m_Scale[3];                           //缩放
	bool m_bVisible;                          //显隐
	float m_Color[3];                           //颜色
	float m_Trans;                            //透明度
	int m_Type;                               
	TargetObjectInfo()
	{
		strcpy(m_Name, "");
		m_ID = -1;
		strcpy(m_Path, "");
		strcpy(m_InsPath, "");
		m_EntId = -1;
		m_bVisible = false;
		m_Trans = -1.0;
		m_Type = -1;

		m_Pos[0] = m_Pos[1] = m_Pos[2] = 0.0f;
		m_Quat[0] = m_Quat[1] = m_Quat[2] = 0.0f;
		m_Quat[3] = 1.0f;
		m_Scale[0] = m_Scale[1] = m_Scale[2] = 1.0f;
		m_Color[0] = m_Color[1] = m_Color[2] = -1.0f;
	}

	~TargetObjectInfo()
	{
		strcpy(m_Name, "");
		m_ID = -1;
		strcpy(m_Path, "");
		strcpy(m_InsPath, "");
		m_EntId = -1;
		m_bVisible = false;
		m_Trans = -1.0;
		m_Type = -1;
	}
};

typedef struct TargetObjectInfo TARGETOBJECTINFO,*PTARGETOBJECTINFO;

#endif //_H_ANIMATION_DEF_H