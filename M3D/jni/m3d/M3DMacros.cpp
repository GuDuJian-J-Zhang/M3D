
#include "m3d/M3DMacros.h"

namespace M3D{

/**
 * M3D中用到的宏
 */
const  string NO_VALUE = "";
const string ROOT = "M3D";
const string MAINGROUP = "M3D|MAIN";
const string MAINMODELROOT = "M3D|MAIN|PATH";
const string MAINCAMERA = "M3D|MAIN|CAMERA";
const string MAINDIRTLIGHT = "M3D|MAIN|DIRLIGHT";
const string MAINPOTLIGHT = "M3D|MAIN|POTLIGHT";
const string MAINPOINTIGHT = "M3D|MAIN|POINTLIGHT";
const string AXIS = "M3D|AIXS";
const string FPS_FLAG = "M3D|FPS";
const string BACKGROUNDCOLOR = "M3D|BACKGROUNDCOLOR";
const string SCENEGROUND = "M3D|SCENEGROUND";

const string HANDLER_GROUPNODE = "M3D|MAIN|HANDLER_GROUP";

const string NOTE_GROUP_PATH = "M3D|MAIN|NOTEGROUPNODE";
const string MEASURE_GROUP_PATH = "M3D|MAIN|MEASUREGROUPNODE";

const string TRANSFORM_HANDLER_NODE = "M3D|MAIN|TRANSFORM_HANDLER_NODE";
const string ROTATECENTERNODE = "M3D|MAIN|ROTATECENTERNODE";
const string AXISHANDLENODE = "M3D|MAIN|AXISHANDLENODE";
const string PLANHANDLENODE = "M3D|MAIN|PLANHANDLENODE";

const string X_TRAN_HANDLER = "X_TRAN_HANDLER";
const string Y_TRAN_HANDLER = "Y_TRAN_HANDLER";
const string Z_TRAN_HANDLER = "Z_TRAN_HANDLER";

const string X_ROTATE_HANDLER = "X_ROTATE_HANDLER";
const string Y_ROTATE_HANDLER = "Y_ROTATE_HANDLER";
const string Z_ROTATE_HANDLER = "Z_ROTATE_HANDLER";

const string X_SCALE_HANDLER = "X_SCALE_HANDLER";
const string Y_SCALE_HANDLER = "Y_SCALE_HANDLER";
const string Z_SCALE_HANDLER = "Z_SCALE_HANDLER";

const int SCENE_NODE = 0;

const int CAMERA_NODE_BEGIN = 10;
const int CAMERA_NODE = 11;

const int CAMERA_NODE_END = 19;

const int SHAPE_NODE_BEGIN = 30;
const int SHAPE_NODE = 31;

const int SHAPE_NODE_END = 39;

const int GROUP_NODE_BEGIN = 40;
const int GROUP_NODE = 41;
const int MODEL_NODE = 42;

const int GROUP_NODE_END = 49;

const int AND_NODE = 51;
const int SUB_NODE = 61;

const int HANDLER_NODE_BEGIN = 70;
const int HANDLER_NODE = 71;
const int HANDLER_NODE_VOICEANNOTATION = 72;
const int HANDLER_NODE_TEXTaNNOTATION = 73;
const int HANDLER_NODE_POINT = 74;
const int HANDLER_GROUP_NODE = 75;
const int HANDLER_NODE_ROTATECENTER = 76;
const int HANDLER_NODE_PLAN = 77;

const int NOTE_GROUP_NODE = 78;
const int MEASURE_GROUP_NODE = 79;

const int HANDLER_NODE_END = 99;

const int GRAPHICAL_MODEL = 100;
const int GRAPHICAL_PMI = 101;

const int HANDLER_SHAPE_BEGIN = 150;
const int HANDLER_SHAPE = HANDLER_SHAPE_BEGIN + 1;
const int HANDLER_ANNOTATION_VOICE = HANDLER_SHAPE_BEGIN + 2;
const int HANDLER_ANNOTATION_TEXT = HANDLER_SHAPE_BEGIN + 3;
const int HANDLER_ROTATECENTER = HANDLER_SHAPE_BEGIN + 4;
const int HANDLER_POINT = HANDLER_SHAPE_BEGIN + 5;
const int HANDLER_SHAPE_END = 199;

const int EXTERN_NODE_BEGIN = 200;
const int AXIS_NODE = 201;
const int BACKGROUNDCOLOR_NODE = 202;
const int SECTION_NODE = 203;

const int FPS_NODE = 204;
const int TEXT_NODE = 205;

const int INTERACTIVEINFO_NODE = 206;

const int SCENEGROUND_NODE = 207;

const int EXTERN_NODE_END = 300;

const int PICKTYPE_BASE = 1000;
const int PICKTYPE_MODEL = 1001;
const int PICKTYPE_PMI = 1002;
const int PICKTYPE_NOTE = 1003;
const int PICKTYPE_FACE = 1004;
const int PICKTYPE_LINE = 1005;
const int PICKTYPE_POINT = 1006;
const int PICKTYPE_OTHER = 1007;

const int NOTE_NODE = 2000;

const int PERSPECTIVE = 1; // perspective projection
const int ORTHO = 2; // ortho projection

const  float NEAR_CLIP_PLANE_FACTOR = 0.001f ; // 近平面比例
const  float FAR_CLIP_PLANE_FACTOR =10.5f; // 远平面比例

const  float NEAR_CLIP_PLANE_VALUE = 0.01;//近平面的值

const float CAMERA_POSFACTOR = 0.8f;

}
