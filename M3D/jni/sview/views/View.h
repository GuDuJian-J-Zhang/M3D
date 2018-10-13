/**@file
 *@brief	View类头文件
 *@author   XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *提供View类的声明
 *
 */

#ifndef M3D_VIEW_H_
#define M3D_VIEW_H_
#include "m3d/M3D.h"
#include "m3d/M3DStates.h"
#include "IView.h"

#include "m3d/model/PMIData.h"
#include "m3d/base/ControlInfo.h"
#include "m3d/model/Model.h"

#include "m3d/base/Mutex.h"
#include "m3d/model/ModelView.h"
#include "sview/manipulator/TouchHandler.h"
#include "sview/io/Reader.h"

#include "sview/io/SVL2AsynReader.h"

#include "sview/extern/PerspectiveOperator.h"
#include "animation/SimulationCommon/Animation_def.h"

namespace M3D
{
class RenderManager;
class SceneManager;
class ModelManager;
class Model;
class GroupNode;
class IDCreator;
class ModelView;
class MovedMatrixInfo;
class Shape;
class SceneGroundNode;
class Texture;
class AxisNode;
class DraggerManager; 
class SectionManager;
class GroundNode;
}

using namespace M3D;

namespace NS_SimulationAnimation
{
	class CSimulationAnimationManager;
	class CSBehaviorAction;
}
using NS_SimulationAnimation::CSimulationAnimationManager;
class CTickTimer;
class CAnimationCallBackFunction;

typedef map<string, MovedMatrixInfo> SceneState;

namespace SVIEW
{

class Reader;
class Selector;
class Parameters;
class TouchHandler;
class CommonTouchHandler;
class WalkthroughHandler;
class OribitControlHandler;
class DraggerHandler;

class M3D_API ViewReadListener: public ReaderListener{
public:
	ViewReadListener();
	virtual ~ViewReadListener();
	virtual void OnProcessPercent(Reader* reader,float percent);
	virtual void OnBegin(Reader* reader);
	virtual void OnEnd(Reader* reader);

	float GetPercent();
	void SetPercent(float percent);

	void SetReadFileCount(int readFileCount);

	void SetReadFileIndex(int readFileIndex);

private:
	float m_readPercent; //纪录当前读取的进度;

	float m_readSegRange;//纪录一个文件占用百分比的长度
	int m_readFileIndex;//纪录当前读取到第几个文件了
	int m_readFileCount;//纪录总文件个数

	mutable M3D::Mutex m_mutex;//!<线程锁
};

    
typedef void (*RefreshFun)(void* instanceClass);
#ifdef WIN32
    typedef void(_stdcall ModelSelectedCB)(int dwModelID,bool selected);
#else
    typedef void(ModelSelectedCB)(int dwModelID,bool selected);
#endif


/**@class View
 * @brief View类  负责和上层的交互操作
 *
 * View类
 *
 */
class M3D_API View: public IView
{
public:
	View();
	virtual ~View();
	Vector3 getRayIntersectNormal();
	Vector3 getRayIntersectPos();
	/**
	 * 获取选择器
	 * @return
	 */
	Selector* GetSelector();
	/**
	 * 获取ID生成器
	 * @return
	 */
	IDCreator* GetIDCreator();
	/**
	 * 获取Parameters
	 * @return
	 */
	Parameters* GetParameters();
	/**
	 * 获取model
	 * @return
	 */
	M3D::Model* GetModel();
	/**
	 * 获取PMI映射
	 * @return
	 */
	map<int,PMIData*>* GetPMIMap();
	/**
	 * 获取所有model的映射表
	 * @return
	 */
    const map<string, Model*>& GetAllModelMap();

    /**
     * @brief 复位相机
     */
    //void ReSetCamera();
    /**
     * @brief 初始化相机
     */
    void InitCamera(bool useAni = true);

    /**
     * 设置当前视图
     * @param curModelView
     */
    void SetCurrentModelView(ModelView *curModelView);

    /**
     * 获取当前视图
     * @return
     */
    ModelView* GetCurrentModelView();
  // const vector<ModelView*>& GetAllViewList(){return GetModel()->GetModelViewList();}


    /**
     * 从xml中加载所有用户视图
     * @param xmlPath
     * @return
     */
    bool LoadAllUserViews(string &xmlPath);
    /**
     * 保存所有的用户视图
     * @param xmlPath
     * @return
     */
    bool SaveAllUserViews(string &xmlPath);

    /**
     * string 与  ModelView 的转化
     * @param modelView
     * @return
     */
    string LoadString(ModelView *modelView);
    /**
     * 加载所有模型视图
     * @param xmlString
     * @return
     */
    ModelView * LoadModelView(string &xmlString);


    /**
     * string 与  ModelView 的转化
     * @param modelView
     * @return
     */
    string LoadStringNew(ModelView *modelView);
    /**
     * 加载所有模型视图
     * @param xmlString
     * @return
     */
    ModelView * LoadModelViewNew(string &jsonString);

    //视图相关
    /**
     * @brief 获取当前的状态存到View中去，一个view表示当前状态的快照
     * @param newView 传出当前状态快照
     * @return ture表示成功
     */
     bool UpdateViewByCurrentScene(ModelView *modelView);
	 bool UpdateSpecialViewByCurrentScene(ModelView *modelView);

     /**
      * 获取默认模型视图，如果不存在，则创建一个
      * @return
      */
     ModelView* GetDefaultModelView();

	/**
	 *  @brief 初始化场景，添加默认的摄像机灯光等节点早scene中去
	 */
	virtual void Initial();

	/**
	 * @brief 触摸按下操作
	 * @param p 存储x1，y1,x2,y2,....xn-1,yn-1
	 * @param n 触摸点的个数
	 */
	virtual void TouchDown(float* p, int n);
	/**
	 * @brief 触摸点移动
	 * @param moveType
	 * @param p 存储x1，y1,x2,y2,....xn-1,yn-1
	 * @param n 触摸点的个数
	 */
	virtual void TouchMove(int moveType, float* p, int n);
	/**
	 * @brief 触摸点抬起
	 * @param p 存储x1，y1,x2,y2,....xn-1,yn-1
	 * @param n 触摸点的个数
	 */
	virtual void TouchUp(float* p, int n);

	/**
	 * @brief update View的大小
	 * @param width
	 * @param height
	 */
	virtual void WindowSizeChange(int width, int height);

	/**
	 * @brief java层，系统回调，刷新操作
	 */
	virtual void OnDraw();

	virtual void RebindUIRenderFBO();
	/**
	 * @brief 执行刷新操作
	 */
	virtual void RequestDraw();

	void RequestUILayerDrawCB(int refreshTime = 1);

	/**
	 * 开始渐进显示
	 */
	virtual void RequestDelayDraw();

	/**
	 * @brief 读取操作
	 * @param path 绝对路径
     * @param fontPath 外部传入的字体绝对路径
	 */
    virtual M3D_STATUS ReadFile(string & path);

	virtual M3D_STATUS ReadFiles(vector<string>& paths);

	virtual M3D_STATUS ReadFile(char * fileBuffer, int length);

		/**
	 * @brief 读取操作
	 * @param path 绝对路径
     * @param fontPath 外部传入的字体绝对路径
	 */
    virtual M3D_STATUS AppendReadFile(string & path);

    /**
     * @brief 保存文件
     * @param path 绝对路径
     */
    virtual M3D_STATUS SaveFile(string &path);
    /**
     * @brief SVL文件保存为SVLX文件
     * @param srcPath SVL路径
     * @param targetPath SVLX路径
     */
    virtual M3D_STATUS SaveSVLFileToSVLX(const string& srcPath,const string &targetPath);
    /**
     * @brief 添加到SVLX文件
     * @param srcPath 源文件路径
     * @param svlxPath SVLX路径
     * @param srcName 文件名称(完整-带后缀)
     */
    virtual M3D_STATUS AddFileToSVLX(const string& srcPath, const string &svlxPath,const string &srcName);

    /**
     * @brief 保存SVLX文件
     * @param tmpPath 源文件解压后的临时路径
     * @param srcPath 源SVLX路径
     * @param srcName 压缩包中实际文件名
     */
    virtual M3D_STATUS SaveSVLXFile(const string& tmpPath,const string &srcPath,const string &srcName);
    /**
     * 获取当前读取进度
     * @return 当前读取进度
     */
    virtual float GetReadPercent();

    /**
	 * @brief 关闭文件
	 *
	 */
	virtual void CloseFile();

	/**
	 * @brief 获得到场景管理 成员对象 地址
	 * @return @see M3D::SceneManager
	 */
	SceneManager* GetSceneManager(void);

	/**
	 * @brief 得到渲染管理器对象的得只
	 * @return 渲染管理器对象地址
	 */
	RenderManager *GetRenderManager(void);

	/**
	 * @brief 得到模型管理器，装配用
	 * @return 模型管理器
	 */
	ModelManager * GetModelManager();

	/**
	 * @brief 设置场景管理器
	 * @param sceneManager 外部对象管理器
	 */
	void SetSceneManager(SceneManager * sceneManager);
	/**
	 * @brief 设置渲染管理器
	 * @param renderManager 渲染管理器地址
	 */
	void SetRenderManager(RenderManager* renderManager);

	/**
	 * @brief 设置模型管理器
	 * @param modelManager 外部模型管理器
	 */
	void SetModelManager(ModelManager * modelManager);

	/**
	 * @brief 设置剖切平面
	 * @param direction 方向
	 * @param position  位置
	 * @param isShowClipPlane 是否显示示意面
	 * @param isShowCutPlane  是否显示盖面
	 */
	void SetClipPlane(int direction, float position, bool isShowClipPlane,
			bool isShowCappingPlane, bool isReverseClip);

	/**
	* @brief 设置多个剖切平面
	* @param directionX YZ剖面方向
	* @param directionY XZ剖面方向
	* @param directionZ XY剖面方向
	* @param positionX  YZ剖面位置
	* @param positionY  XZ剖面位置
	* @param positionZ  XY剖面位置
	* @param isShowClipPlane 是否显示示意面
	* @param isShowCapPlane  是否显示盖面
	*/
	void SetMultiClipPlane(int directionX, int directionY, int directionZ,
		float positionX, float positionY, float positionZ, bool isShowClipPlane,
		bool isShowCappingPlane, bool isReverseClip);

	/**
	 * @brief 清除所有的剖面
	 */
	void ClearClipPlane();

    /**
     * 设置爆炸视图
     * @param stype
     * @param pos
     * @param useAnimation
     * @return true表示成功 false表示失败
     */
    virtual bool SetExplosiveView(int stype,int pos = 100,bool useAnimation = false);
	/**
	* 设置爆炸视图
	* @param stype
	* @param pos
	* @param useAnimation
	* @param arrayModels 选择爆炸的功能
	* @return true表示成功 false表示失败
	*/
	virtual bool SetExplosiveView(vector<Model*> arrayModels,int stype, int pos = 100, bool useAnimation = false);
	
	virtual bool SetExplosiveViewWithDirection(vector<Model*> arrayModels, int stype, int pos = 100, Vector3 direction = Vector3(0,0,0));
	/*
		设置值时，不带旋转的
	*/
	virtual bool SetExplosiveViewWithoutRestore(int stype, int pos = 100, bool useAnimation = false);

    virtual ExplosiveViewOperator* GetExplosiveView();

    /**
     * 关闭爆炸图
     * @return true表示成功 false表示失败
     */
    virtual bool CloseExplisiveView();

	///获得和设置实例的矩阵
	/**
	 * @brief 设置名称为plcPath节点，对应的配置矩阵为matrix
	 * @param plcPath 节点名称
	 * @param matrix  节点配置矩阵
	 */
	void SetMatrixByPlcPath(char* plcPath, float matrix[4][4]);
	/**
	 * @brief 获取名称为plcPath节点，对应的配置矩阵为matrix
	 * @param plcPath 节点名称
	 * @param matrix  节点配置矩阵
	 */
	void GetMatrixByPlcPath(char* plcPath, float matrix[4][4]);
	/**
	 * @brief 设置摄像机位置
	 * @param matrix 摄像机矩阵
	 */
	void SetSceneMatrix(float matrix[4][4]);
	/**
	 * @brief 设置摄像机的缩放比例
	 * @param scale 缩放比例
	 */
	void SetSceneScale(float scales[3]);

	///设置视图
	/**
	 * @brief 加载第一个视图
	 * @param viewType 视图对应的类型
	 */
	void SetPerspective(int viewType);
    
    /**
     * @brief 获取当前绘制模式
     * @return drawMode 绘制模式：0=实体、1=半透明、2=网格、3=线框
     */
    int GetDrawMode();

	/**
	 * @brief 设置绘制模式
	 * @param drawMode 绘制模式：0=实体、1=半透明、2=网格、3=线框
	 */
	void SetDrawMode(int drawMode);

	//设置PMI可见性
	bool SetPMIVisible(int pmiID,bool visable);

	/**
	 * @brief 重置场景到最初加载状态
	 */
	void RestoreView();
	void RestoreView(bool useAni);

	void RequestRestoreViewIfNeed();
	/**
	 * @brief 拾取操作，返回拾取到shape对应的ID，返回-1表示未拾取任何shape
	 * @param winx 屏幕坐标x
	 * @param winY 屏幕坐标y
	 * @param type 需要拾取的类型
	 * @return 拾取到shape的ID
	 */
	IShape* GetPickShape(float x, float y, int shapeType, int geoType);

	///框选操作
	vector<IShape*> GetFramePickShape(const Vector2& leftTop, const Vector2& rightBottom, int shapeType,
		int geoType,int framePickType);
 
    /**
	 * @brief 设置能够拾取到的类型
	 * @param shapeType 允许拾取的类型
	 */
	void SetSelectType(int shapeType);

	/**
	 * @brief 设置旋转中心
	 * @param x 屏幕坐标x
	 * @param y 屏幕坐标y
	 * @return true 设置成功
	 */
	bool SetRotationCenter(float x, float y,int centerType=0);

	/**
	 * @brief 复位旋转中心
	 * @return true表示成功
	 */
	void SetDefaultRotateCenter();


	///移动选中的shapes
	/**
	 * @brief 移动选择器中所有的shape，移动距离为 从prix，priY---》curX，curY
	 * @param priX 上一次屏幕坐标X
	 * @param priY 上一次屏幕坐标Y
	 * @param curX 当前屏幕坐标X
	 * @param curY 当前屏幕坐标Y
	 */
	void TranslateSelectedShapes(float priX, float priY, float curX,
			float curY);

	/**
	 * @brief 移动ID为id，类型为type的shape，，移动距离为 从prix，priY---》curX，curY
	 * @param priX
	 * @param priY
	 * @param curX
	 * @param curY
	 * @param id
	 * @param type
	 */
	void TranslateShape(float priX, float priY, float curX, float curY, int id,
			int type);

	/**
	 * @brief 复位选择其中所有的shape到最初加载位置
	 */
	void ResetSelectedShapes();
	/**
	* 设置所有PMI的显隐状态
	* @param isVisible
	*/
	void SetAllPMISVisible(bool isVisible);
	/**
	 * @brief 复位ID为id的shape到最初加载位置
	 * @param id
	 */
	void ResetSelectedShapes(M3D::IShape* shape);

	///设置相关
	/**
	 * @brief 保存当前配置到XML文件中
	 */
	void SaveSettingXML();

	/**
	 * @brief 添加Handler
	 * @param x 屏幕坐标X
	 * @param y 屏幕坐标Y
	 * @param type Handler的类型
	 * @return 成功返回生成Handler的ID，失败返回-1
	 */
	int AddHandle(float x, float y, int type);

	/**
	 * @brief 得到空间中的三维点
	 * @param x 屏幕坐标X
	 * @param y 屏幕坐标Y
	 * @param type 得到空间三维点类型，未实现功能
	 * @param type 是否
	 * @return 空间中的点
	 */
	Vector3 GetSelectedPoint(float x, float y, int type, bool inModel=true);

	Vector3 GetUILayerSelectedPoint(float x, float y);

	/**
	 * @brief 在空间中x，y，z位置创建handler
	 * @param x 空间X
	 * @param y 空间Y
	 * @param z 空间Z
	 * @param type 创建Handler类型
	 * @return 成功返回生成Handler的ID，失败返回-1
	 */
	int AddHandle(float x, float y, float z, int type);
	/**
	 * @brief 移除ID为 id的handler
	 * @param id 移除Handler的ID
	 *
	 */
	void RemoveHandle(int id);

	/**
	 * @brief 添加shape
	 * @param x 空间坐标X
	 * @param y 空间坐标Y
	 * @param Z 空间坐标Z
	 * @param type 添加shape类型
	 * @return 添加进入shape的ID
	 */
	int AddShape(float x, float y, float z, int type);
	/**
	 * @brief 添加shape
	 * @param x 屏幕坐标X
	 * @param y 屏幕坐标Y
	 * @param type 添加shape类型
	 * @return 添加进入shape的ID
	 */
	int AddShape(float x, float y, int type);
	/**
	 * @brief 移除场景中ID为 id的shape
	 * @param id 移除shape的ID
	 * @return tru表示移除成功，false表示移除失败
	 */
	void RemoveShape(int id);

	/**
	 * @brief 获取ID为id，类型为type的shape在空间中的当前位置
	 * @param id 待获取shape的ID
	 * @param type 待获取shape的类型
	 * @param pos  传出shape的当前位置
	 */
	void GetShapePos(int id, int type, float* p);

	/**
	 * 获取ID为id，类型为type的形状的颜色
	 * @param id
	 * @param type
	 * @return 颜色对象
	 */
	Color GetShapeColor(int id,int type);

	/**
	 * @brief 添加批注
	 * @param type 添加批注的类型
	 * @return 返回创建成功批注的ID，返回-1表示没有添加成功
	 */
	int AddNote(int type);

	int AddLineLengthNote(int lineID, float x,  float y);
	int AddRadiusLengthNote(int radiusID, float x, float y);

	void moveLineLengthNote(int lineNoteID, float x,float y);
	void moveRadiusLengthNote(int radiusNoteID, float x,float y);

	/**
	 * @brief 移除ID为 noteId的批注
	 * @param noteId 批注的ID
	 */
	void RemoveNote(int noteId);

	/**
	 * @brief 添加点到批注中
	 * @param parentId
	 * @param parentType
	 * @param pntBuf
	 */
	void AddPoint(int parentId, int parentType, float pntBuf[]);
	/**
	 * @brief 添加线到批注中
	 * @param parentId
	 * @param parentType
	 * @param startPointBuf
	 * @param endPointBuf
	 * @param width
	 * @param startArrow
	 * @param endArrow
	 */
	void AddLine(int parentId, int parentType, float startPointBuf[],
			float endPointBuf[], int width, int startArrow, int endArrow);


	/**
	 * @brief 添加折线到对象
	 * @param parentId
	 * @param parentType
	 * @param pointCnt
	 * @param pointBuf
	 * @param indexCnt
	 * @param indexBuf
	 */
	void AddPolyLine(int parentId, int parentType,int pointCnt,
			float pointBuf[],int indexCnt,M3D_INDEX_TYPE indexBuf[]);


	/**
	 * @brief 添加文本到批注中
	 * @param parentId
	 * @param parentType
	 * @param posBuf
	 * @param text
	 * @param fontSize
	 */
	void AddText(int parentId, int parentType, float posBuf[], string text,
			float fontSize);
	/**
	 * @brief view状态切换
	 * @param viewId View对应的ID
	 * @param isAnni ture开启过度动画，false不开启
	 */
	void ShowModelView(int viewId, bool isAnni);
	//----------------------------------------------------------------

	/**
	 * @brief 获取当前场景状态
	 */
	void GetCurrentState();

	/**
	 * @brief 设置当前场景状态
	 */
	void SetCurrentState();


	/**
	 * 设置默认工作节点
	 *  @see WorkNodeGroup
	 */
	void SetDefaultWorkNodes();

	/**
	 * @brief 获得显示窗口的宽度
	 * @return 窗口宽度
	 */
	int GetViewWidth();

	/**
	 * @brief 获得显示窗口高度
	 * @return 窗口高度
	 */
	int GetViewHeight();

	/**
	 * 设置当前是否使用平行投影
	 * @param enable
	 */
	void setOrthographic(bool enable);

	/**
	 * 获取当前是否使用平行投影
	 * @return
	 */
	bool GetOrthographic();

	/**
	 * 设置当前是否使用平行投影
	 * @param enable
	 */
	void setWalkThrough(TouchHandlerType type);

	/**
	 * 获取当前是否使用平行投影
	 * @return
	 */
	TouchHandlerType GetWalkThrough();

	/**
	 * 获取id
	 * @return
	 */
	int GetID();

	/**
	 * 设置id
	 * @param viewID
	 */
	void SetID(int viewID);

	/**
	 * 设置显示回调函数，
	 * @note 主要在ios 端调用
	 * @param RefreshCallBackClassInstance
	 * @param m_RefreshFun
	 */
    void SetRefreshFun(void* RefreshCallBackClassInstance,
                       RefreshFun m_RefreshFun);

    /**
     * 得到刷新函数
     * @note 主要在ios端调用
     * @return
     */
    RefreshFun GetRefreshFun();
    
    //动画结束回调函数
    /**
     * @brief @see IView相关定义
     * @param viewInstance
     */
    static void AnimationFinishCallback(void* viewInstance);
 
    /**
     * @brief @see IView相关定义
     * @param file
     * @return
     */
	virtual bool AnimationOpen( const string& file );

	/**
	 * @brief @see IView相关定义
	 * @param xmlData
	 * @return
	 */
	virtual bool AnimationOpenXMLData(const string& xmlData);

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual bool AnimationContinuousPlayState();

	/**
	 * @brief @see IView相关定义
	 * @param isLoop
	 * @return
	 */
	virtual bool AnimationContinuousPlay(bool isLoop);

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual bool AnimationPlay();

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual bool AnimationRewind();

	/**
	 * @brief @see IView相关定义
	 * @param percent
	 * @return
	 */
	virtual bool AnimationSetTick(float percent);

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual float AnimationGetTick();

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual bool AnimationIsPlaying();

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual bool AnimationTransitionIsPlaying();

	/**
	 * @brief @see IView相关定义
	 * @param percentage
	 * @return
	 */
	virtual bool AnimationExecute(float percentage );

	/**
	 * @brief @see IView相关定义
	 * @param isPlayCamera
	 * @return
	 */
	virtual bool AnimationPlayCamera(bool isPlayCamera);

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual bool AnimationGetPlayCamera();

	/**
	 * @brief @see IView相关定义
	 * @param speed
	 * @return
	 */
	virtual bool AnimationPlaySpeed(float speed);

	/**
	 * @brief @see IView相关定义
	 * @param processId
	 * @param stepId
	 * @return
	 */
	virtual bool AnimationSetActiveStep(int processId,int stepId);

	/**
	 * 获取动画xml数据
	 * @return
	 */
	virtual string& GetAnimationXMLData();

	/**
	 * 设置动画xml数据
	 * @param xmlData
	 */
	virtual void SetAnimationXMLData(const string& xmlData);

	/**
	 * @brief @see IView相关定义
	 * @return
	 */
	virtual string GetDataManagerVersion();

	/**
	 * @brief @see IView相关定义
	 * @param path
	 * @return
	 */
	virtual IShape* GetShapeBySVLPath(const string& path);

	/**
	 * @brief @see IView相关定义
	 * @param path
	 * @return
	 */
	virtual IShape* GetShapeByM3DPath(const string& path);
    
	/**
	 * @brief @see IView相关定义
	 * @param shapeId
	 * @return
	 */
    virtual IShape* GetShape(int shapeId);
 
    ///背景相关操作
    /**
     * 设置背景颜色，从顶端到底部渐变色
     * @param topColor 顶端颜色
     * @param bottomColor 底部颜色
     * @return true设置成功，false设置失败
     */
    virtual bool SetBackgroundColor(const Color& topColor,const Color& bottomColor);
    
    /**
     * 得到当前的背景色
     * @param topColor 顶端颜色
     * @param bottomColor 底部颜色
     * @return true获取成功 false获取失败
     */
    virtual bool GetBackgroundColor(Color& topColor,Color& bottomColor);

    /**
     * 设置背景图片
     * @param path 背景图片路径
     * @param mappingStyle 背景图片映射方式
     * @return true设置成功，false设置失败
     */
    virtual bool SetBackgroundImage(const string& path,int mappingStyle);

    /**
     * 设置是否使用背景图片
     * @param useImage true 使用背景图片 false不使用
     */
    virtual void SetBackgroundUseImage(bool useImage);

    /**
     * 获取当前是否使用背景图片
     * @return  true 使用背景图片 false不使用
     */
    virtual bool GetBackgroundUseImage();

	bool SetBackgroundTexture(Texture * texture);

	bool AddBackgroundSkyBoxTexture(string name, Texture* texture);
	void SetBackgroundUseSkyBox(bool useSkyBox);
	bool GetBackgroundUseSkyBox();
    bool GetBackgroundUseColor();
	void SetBackgroundUseColor(bool useColor);
	void KeepBackgroundState();
	void RestoreBackgroundState();
	bool GetInterBackgroundState();
    void SetInterBackgroundState(bool isInter);
	//使用json来配置场景信息
	void LoadScene(const string& sceneInfo);

	void UpdateDrawLimit();

	void UpDateAnimationLimit();

    NS_SimulationAnimation::CSimulationAnimationManager* GetSimulationMgr();

	void SetSimulationMgr(NS_SimulationAnimation::CSimulationAnimationManager* pSAManager);

    /**
     * 结束当前动画，包括持续旋转操作
     * 由于进行视口切换等操作时，
     * 不知道需要结束持续旋转的状态
     * 因此提供此接口
     */
    bool CloseSceneAnimation();

    /**
     * 获取shape附加属性
     * @param shapeId
     * @return
     */
	string GetShapeProperties(int shapeId);

	/**
	 * 设置shape附加属性
	 * @param shapeId
	 * @param properties
	 * @return
	 */
	bool   SetShapePorperties(int shapeId,const string& properties);

	/**
	 * 获取shape状态
	 * @param value
	 * @return
	 */
	string GetShapeStates(const string& value);
	/**
	 * 获取shape状态
	 * @param value
	 * @return
	 */
	bool   SetShapeStates(const string& value);

	/**
	 * 根据当前显示的模型，自动缩放到合适比例显示
	 * @return
	 */
	bool FitScaleView();

	/**
	 * 是否绘制坐标轴
	 * @param show
	 */
	void SetShowAxis(bool show);

	/**
	 * 获取显示坐标轴状态
	 */
	bool IsShowAxis();

	/**
	 * 获取模型相关的资源路径
	 * @return
	 */
	set<string> GetSourceFiles();

	TouchHandler * GetTouchHandler()
	{
		return m_workTouchHandler;
	}


	void SetIsDragger(bool isDragger);
    
    /**
     * 锁定场景,使线程独占场景资源
     * 当在外部修改场景内容时调用，能够对渲染线程进行锁定
     * 防止写冲突
     */
    void Lock();
    
    /**
     * 解锁场景,释放线程独占资源
     * 是否Lock锁定的资源
     */
    void UnLock();


    void OnAnimationTick();

    void CreateDefaultView(bool value);

	/**
	* 获取当前文件的路径
	* @param data
	*/
	string GetCurFilePath()
	{
		return m_CurFilePath;
	}
	/**
	 * 获取自定义用户属性
	 * @param key 键
	 * @return 值
	 */
	const string& GetUserData(const string& key);

	/**
	 * 添加自定义用户属性
	 * @param key 键
	 * @param value 值
	 */
	void AddUserData(const string& key,const string& value);

	/**
	 *
	 * @param GLESVersion
	 */
	void InitRender(int GLESVersion);
	bool ReadSingleModel(const string& path,Model* & singleModel);
	
	void IsUseStereoMode(bool state);
	bool GetStereoModeState();
	//以下为展示功能------
	void StartRotateAndExplosiveTimer();
	void EndRotateAndExplosiveTimer();
	void RestoreShowMode();
	void SetShowModeSpeed(float speed);
	/**
     @param projectionType 0-平行投影 1-透视投影
     */
	void SetCameraProjectionType(int projectionType);
    /**
     获取投影类型
     
     @return 0-平行投影 1-透视投影
     */
	int GetCameraProjectionType();
    /**
     * 设置所有PMI的显隐状态
     * @param isVisible
     */
	void FoucusView(BoundingBox& foucusBox, bool useAni);
	PerspectiveOperator* GetPerspectiveOperator();
    
    map<int,vector<string> > GetFixPMIInfo();
	int GetSVLXFileItem(const std::string& i_strFileName, unsigned int& o_bufSize, char** o_bufP, bool b_ByExt = true);
    /**
     文件移动到某位置
     
     @param i_strFileName 文件名
     @param i_strOutFilePath 新路径
     @param bByExt 是否识别后缀
     @return
     */
    M3D_STATUS GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt);
    
    M3D_STATUS CopySVLXFileItemToFileByReOpen(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt);
	bool CreateNewReader();
	void CloseReader();
	//异步读取完成之后，释放SVLLib管理的内存
	bool AsynReadClose();

	//FOR 中广核

	//打开场景，创建顶级节点
	bool AsynReadOpenScene();

	//从buffer中获取Model
	Model* AsynGetModel(char* fileLength, int bufferLength, const string& filePath);
 
	//添加模型到上级模型中
	Model* AsynCreateTopModel();

	//添加模型到上级模型中
	bool AsynAddModel(Model* parentModel,Model* addModel);
	//删除模型从上级模型中
	bool AsynRemoveModel(Model* removeModel);

	bool SetAsynFillModelBuffer(char* fileLength,int bufferLength);

	M3D_STATUS AsynFillModel(Model*  singleModel);

	bool AsynResotreView();
	
	//viewmode info文件中viewmode节点状态存储
	void KeepViewModeState();
	void RestoreViewModeState();
	M3D_STATUS LoadAttribute();
	bool IsLoadAttribute();
	void HasModelAttribute(Model* model, bool& bHas);

	void OpenForceNormalEffect();
	void CloseForceNormalEffect();

	bool GetConstraintMode();

	int GetWalkThroughDirection();
	M3D::DraggerManager* GetDraggerManager();
	M3D::SectionManager* GetSectionManager();
	void UpdateAnimationInitTargetObjects(vector<TARGETOBJECTINFO*> &vcObjectInfo);
	int GetAnimationInitTargetObjectsCount();

	float GetUnitScale();
	void InitAinmationPlayCB();

	void SetModelSelectedCB(ModelSelectedCB* modelSelectedCB);

	void NotifyModelSected(M3D::IShape* shape, bool selected);

	void updateScreenBox();
    
    //批注数据解析
    void ParseAnnotation(const string& value);
    const string& GetGestureJsonData(const string& key);
    void AddGestureJsonData(const string& key, const string& value);
    const string& GetTextJsonData(const string& key);
    
    void AddTextJsonData(const string& key, const string& value);
    
    const string& GetSequenceJsonData(const string& key);
    
    void AddSequenceJsonData(const string& key, const string& value);
    
private:
	CTimer m_rotateAndExplosiveTimer;
	static void * RotateAndExplosiveTask(void * data);
	static bool m_isNeedRotate ;
	static bool m_excStep1 ;
	static bool m_excStep2 ;
	static bool isRotateComplete ;
	static float explosivePercent ;
	static float explorsiveSpeed ;

	//----------------------------------
private:
    //初始化动画，主要进行当前状态到动画播放状态的插值过渡效果
    void AnimationInit();

//	void GetModelAllSubModel(M3D::Model* model, vector<int>& ids);

	/**
	 * 由于线程同步原因，在执行其他改变场景状态的线程时，不允许响应Touch操作
	 * 主要受视图切换动画线程影响，才加入此判断操作
	 * @return
	 */
	bool AllowTouch();

	/**
	 * 设置动画开始播放时场景的状态，主要设置PMI不可见
	 */
	void SaveAnimatinState();
	/**
	 * 设置动画结束播放时场景的状态，主要设置PMI可见
	 */
	void RestoreAnimationState();

	/**
	 * 动画开始回调函数，由于需要在动画开始时，实现状态插值操作，状态
	 * 插值完成后，需要通知开始播放动画，在此函数中进行。
	 * @param data
	 */
	static void AnimationStartCallback(void* data);

	bool GetAnimationDisState(ControlInfo& controlInfo);

	M3D_STATUS ReadFilesNormalMode(vector<string>& paths);

	M3D_STATUS ReadFilesSpeedMode(vector<string>& paths);
	
	void ShowModelViewAnimation(ModelView *pView);

public:
	float m_fUnitScale;
	void SetUnitScale(float scale);

	void ChechErrorPoint(M3D::Model* model);

public:
	Reader* m_Reader;
private:
	bool m_isInitialized;//!<是否初始化
    string m_CurFilePath;//!<当前文件路径
	bool allowDraw;
	M3D::Model* m_Model;//!<模型
	mutable M3D::Mutex m_mutex;//!<线程锁
	map<string,string> m_userDatas;//用户自定义属性
    map<string,string> m_allTextNoteJsonData;//所有文本批注json数据
    map<string,string> m_allSequeNoteJsonData;//所有序号批注json数据
    map<string,string> m_allGeasureNoteJsonData;//所有手势批注json数据
    set<string> m_srcFiles;
//    map<string,Model*> m_AllModelMap;
    //
    map<string,Model*> m_ColorChangedModelMap;//!<被改变颜色的模型列表 改变颜色操作后添加

	RenderManager * m_RenderManager;//!<渲染管理器
	SceneManager * m_SceneManager;//!<场景管理器
	ModelManager * m_modelManager;//!<模型管理器

	TouchHandler* m_workTouchHandler;//!<

	CommonTouchHandler* m_TouchEventHandler;//!<用于平行投影
	WalkthroughHandler* m_walkthroughTouchHandler;//!<用于透视投影
	OribitControlHandler* m_oribitControlTouchHandler;
	DraggerHandler * m_draggerHandler;//!<用于模型装配

	bool m_isDragger;//!<正在拖拽
	Selector* m_Selector;//!<选择筛选器
	IDCreator* m_IDCreator;//!<ID创建器
	Parameters* m_Parameters;//!<parameters

	NS_SimulationAnimation::CSimulationAnimationManager* m_pSAManager ;

	static CTickTimer *pTDriver ;
    
    ModelView *m_curModelView;//!<当前视图
    
	bool m_bNewCreate;

	bool m_isAnimationPlayCamera;//!<动画所需的变量
	bool m_isAnimationShowPMI;//!<动画所需的变量

	bool m_isNeedChangeViewAnimation;//!<动画在播放时，是否需要过渡动画

	bool m_bHasAni ;
	int m_ID;
    
    int m_curDrawMode;//!<当前绘制模式：0=实体、1=半透明、2=网格、3=线框

	string m_AnimationXMLData;//!<动画数据

    void* m_RefreshCallBackClassInstance;//!<刷新回调类的实例

    float m_animationPlaySpeed;//!<动画播放速度
    bool m_animationContinuousPlay;//!<动画继续播放

    float m_DrawLimit;//!<限制绘制

    //使用漫游模式
    TouchHandlerType  m_useWalkThrough;//!<

    RefreshFun m_RefreshFun;//!<
 
    BackgroundNode* m_backgroundNode;//!<背景节点
    bool m_isInterBackground;//是否使用内部背景
    AxisNode* m_axisNode;//!<坐标轴节点

    SceneGroundNode * m_sceneGroundNode;//!<地面节点

    ExplosiveViewOperator* m_ExplosiveViewOperator;//!<爆炸操作

	DraggerManager* _draggerManager;
	SectionManager* _sectionManager;
	ModelView  *m_tempDefaultView;//!<默认视图

	bool m_draggerInit;

	bool m_enableCreateDefaultView;

	ViewReadListener* m_readListener;

	//bool m_isSpeedModel; //!<浏览速度优先方式

	PerspectiveOperator* m_perpectiveOperator; //!< 视图切换操作类
    map<int,vector<string> > m_fixPMIInfo;

	//viewmode info文件中viewmode节点状态存储
	TouchHandlerType m_viewModeTouchHandlerType;
	Vector3 m_viewModeUpDirection;
	bool m_viewModeOribit;
	bool m_viewModeControlLockXY;
	bool m_viewModeFreeLook;
 
	//纪录下读取时的包围盒
	BoundingBox m_firstReadBox;

	GroundNode * m_groundNode;//!<地面节点
	CAnimationCallBackFunction* m_pAnimationCallBackFunction;//!<动画播放回退处理回退函数类


	ModelSelectedCB* m_modelSelectedCB; //模型选中回调函数，
};

} ///namespace

#endif /*M3D_VIEW_H_*/
