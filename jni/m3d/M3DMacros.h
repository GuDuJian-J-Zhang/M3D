/**@file
 *@brief	M3D显示库，定义的一系列常量和宏
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef   M3D_MACROS_H
#define  M3D_MACROS_H

#include "m3d/M3D.h"

#define M3D_OFFSET_TYPE unsigned long long

#ifdef WIN32
#define IDTYPE unsigned long long int
#else
#define IDTYPE int
#endif

#ifdef __MOBILE__
#define M3D_INDEX_TYPE  unsigned short
#endif

#ifdef _WIN32
#define M3D_INDEX_TYPE unsigned int 
#endif

namespace M3D{

const extern string NO_VALUE ;
/**
 * M3D中用到的宏
 */
const extern string ROOT;
const extern M3D_API string MAINGROUP;
const extern M3D_API string MAINMODELROOT;
const extern string MAINCAMERA;
const extern string MAINDIRTLIGHT;
const extern string MAINPOTLIGHT;
const extern string MAINPOINTIGHT;
const extern string AXIS;
const extern string FPS_FLAG;
const extern string BACKGROUNDCOLOR ;
const extern string SCENEGROUND ;

const extern string HANDLER_GROUPNODE;

const extern string SECTION_GROUP_PATH;
const extern string NOTE_GROUP_PATH;
const extern string ANNOTATION_GROUP_PATH;
const extern string MEASURE_GROUP_PATH;
const extern string LIGHT_GROUP_PATH;
const extern string CAMERA_GROUP_PATH;

const extern string TRANSFORM_HANDLER_NODE;
const extern string ROTATECENTERNODE;
const extern string AXISHANDLENODE;
const extern string PLANHANDLENODE;

const extern string X_TRAN_HANDLER;
const extern string Y_TRAN_HANDLER;
const extern string Z_TRAN_HANDLER;

const extern string X_ROTATE_HANDLER;
const extern string Y_ROTATE_HANDLER ;
const extern string Z_ROTATE_HANDLER;

const extern string X_SCALE_HANDLER;
const extern string Y_SCALE_HANDLER;
const extern string Z_SCALE_HANDLER ;

const extern int SCENE_NODE;

const extern int CAMERA_NODE_BEGIN;
const extern int CAMERA_NODE;

const extern int CAMERA_NODE_END;

const extern int SHAPE_NODE_BEGIN;
const extern int SHAPE_NODE;

const extern int SHAPE_NODE_END;

const extern int GROUP_NODE_BEGIN;
const extern int GROUP_NODE;
const extern int MODEL_NODE;

const extern int GROUP_NODE_END;

const extern int AND_NODE;
const extern int SUB_NODE;

const extern int HANDLER_NODE_BEGIN;
const extern int HANDLER_NODE;
const extern int HANDLER_NODE_VOICEANNOTATION;
const extern int HANDLER_NODE_TEXTaNNOTATION;
const extern int HANDLER_NODE_POINT;
const extern int HANDLER_GROUP_NODE;
const extern int HANDLER_NODE_ROTATECENTER;
const extern int HANDLER_NODE_PLAN;

const extern int NOTE_GROUP_NODE;
const extern int MEASURE_GROUP_NODE;
const extern int CAMERA_GROUP_NODE;
const extern int LIGHT_GROUP_NODE;
const extern int SECTION_NODE;

const extern int HANDLER_NODE_END;

const extern int GRAPHICAL_MODEL;
const extern int GRAPHICAL_PMI;

const extern int HANDLER_SHAPE_BEGIN;
const extern int HANDLER_SHAPE;
const extern int HANDLER_ANNOTATION_VOICE;
const extern int HANDLER_ANNOTATION_TEXT;
const extern int HANDLER_ROTATECENTER;
const extern int HANDLER_POINT;
const extern int HANDLER_SHAPE_END;

const extern int EXTERN_NODE_BEGIN;
const extern int AXIS_NODE;
const extern int BACKGROUNDCOLOR_NODE;
const extern int SECTION_NODE;

const extern int FPS_NODE;
const extern int TEXT_NODE;

const extern int INTERACTIVEINFO_NODE;

const extern int SCENEGROUND_NODE;

const extern int EXTERN_NODE_END;

const extern int PICKTYPE_BASE;
const extern int PICKTYPE_MODEL;
const extern int PICKTYPE_PMI;
const extern int PICKTYPE_NOTE;
const extern int PICKTYPE_FACE;
const extern int PICKTYPE_LINE;
const extern int PICKTYPE_POINT;
const extern int PICKTYPE_OTHER;

const extern int NOTE_NODE;

const extern int PERSPECTIVE; // perspective projection
const extern int ORTHO; // ortho projection

const extern float LINE_WIDTH;

const extern float NEAR_CLIP_PLANE_FACTOR; // 近平面比例
const extern float FAR_CLIP_PLANE_FACTOR; // 远平面比例

const extern float NEAR_CLIP_PLANE_VALUE;//近平面的值

const extern float CAMERA_POSFACTOR;//摄像机 距离模型中心 占整个包围盒的距离

const extern string M3D_DEFAULT_MATERIAL;

}
#endif /*M3D_MACROS_H*/
