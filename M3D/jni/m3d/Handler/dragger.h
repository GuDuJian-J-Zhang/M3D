

#ifndef M3DMANIPULATOR_DRAGGER
#define M3DMANIPULATOR_DRAGGER 1


#include "m3d/handler/Command.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/GroupNode.h"
#include "sview/views/View.h"
#include "m3d/handler/Handler.h"

namespace M3D
{
class CompositeDragger;
class MotionCommand;
class TranslateInLineCommand;
class TranslateInPlaneCommand;
class Scale1DCommand;
class Scale2DCommand;
class ScaleUniformCommand;
class Rotate3DCommand;
class Dragger;

//定义回调函数类型
#ifdef WIN32
typedef void(_stdcall M3D_DRAGGERCB)(const int& nCmdType, wstring& strDraggerName);
typedef void(_stdcall M3D_DRAGGERCB2)(const int& nCmdType, wstring& strDraggerName, Matrix3x4& matrix3x4);
#else
typedef void( M3D_DRAGGERCB)(const int& nCmdType, wstring& strDraggerName);
typedef void(M3D_DRAGGERCB2)(const int& nCmdType, wstring& strDraggerName, Matrix3x4& matrix3x4);
#endif // WIN32

class M3D_API DraggerCallback :  public Object
{
    public:
        DraggerCallback():
			Object() {
			this->SetScene(NULL);
		}

		M3D::SceneManager* GetScene() const { return m_scene; }
		void SetScene(M3D::SceneManager* val) { m_scene = val; }

        /**
         * Receive motion commands. Returns true on success.
         */
        virtual bool receive(const MotionCommand&) { return false; }
        virtual bool receive(const TranslateInLineCommand& command)  { return receive((MotionCommand&)command); }
        virtual bool receive(const TranslateInPlaneCommand& command) { return receive((MotionCommand&)command); }
        virtual bool receive(const Scale1DCommand& command)          { return receive((MotionCommand&)command); }
        virtual bool receive(const Scale2DCommand& command)          { return receive((MotionCommand&)command); }
        virtual bool receive(const ScaleUniformCommand& command)     { return receive((MotionCommand&)command); }
        virtual bool receive(const Rotate3DCommand& command)         { return receive((MotionCommand&)command); }
private:
	SceneManager* m_scene;
};
//
//class M3D_API DraggerTransformCallback : public DraggerCallback
//{
//    public:
//
//        DraggerTransformCallback(M3D::MatrixTransform* transform);
//
//        virtual bool receive(const MotionCommand&);
//
//        M3D::MatrixTransform* getTransform() { return _transform.get(); }
//        const M3D::MatrixTransform* getTransform() const { return _transform.get(); }
//
//    protected:
//
//        M3D::observer_ptr<M3D::MatrixTransform> _transform;
//        M3D::Matrix3x4 _startMotionMatrix;
//
//        M3D::Matrix3x4 _localToWorld;
//        M3D::Matrix3x4 _worldToLocal;
//};


class M3D_API PointerInfo
{
    public:

        PointerInfo();

        //PointerInfo(const PointerInfo& rhs):
        //    _hitList(rhs._hitList),
        //    _nearPoint(rhs._nearPoint),
        //    _farPoint(rhs._farPoint),
        //    _eyeDir(rhs._eyeDir)
        //{
        //    //_hitIter = _hitList.begin();
        //}

        void reset()
        {
            //_hitList.clear();
            //_hitIter = _hitList.begin();
            setCamera(0);
			_pickDragger = NULL;
        }


        //bool completed() const { return _hitIter==_hitList.end(); }

        //void next()
        //{
        //    if (!completed()) ++_hitIter;
        //}

       // typedef std::pair<M3D::NodePath, M3D::Vector3> NodePathIntersectionPair;
       // typedef std::list< NodePathIntersectionPair> IntersectionList;


        M3D::Vector3 getLocalIntersectPoint() const { return _LocalIntersectPoint; }
		void setLocalIntersectPoint(M3D::Vector3& intersectPoint){   _LocalIntersectPoint = intersectPoint; }

        void setNearFarPoints (M3D::Vector3 nearPoint, M3D::Vector3 farPoint);

        const M3D::Vector3& getEyeDir() const {return _eyeDir;}

        void getNearFarPoints( M3D::Vector3& nearPoint, M3D::Vector3& farPoint) const;

        bool contains(const Dragger* node) const;

        void setCamera(M3D::CameraNode* camera);

        void setIntersection(Dragger* nodePath, const M3D::Vector3& intersectionPoint); 

        void setMousePosition(float pixel_x, float pixel_y);

    protected:
        bool projectWindowXYIntoObject(const M3D::Vector2& windowCoord, M3D::Vector3& nearPoint, M3D::Vector3& farPoint) const;

    public:
        //IntersectionList _hitList;
     
    protected:
		M3D::Vector3 _LocalIntersectPoint;
		M3D::Vector3 _intersectPoint;
		Dragger* _pickDragger;

        M3D::Vector3 _nearPoint,_farPoint;
        M3D::Vector3 _eyeDir;

        //M3D::Matrix4 _MVPW;
        //M3D::Matrix4 _inverseMVPW;

		M3D::CameraNode* m_camera;

};

class M3D_API DraggerTransformCallback :public DraggerCallback
{
public:
	DraggerTransformCallback();
	DraggerTransformCallback(Dragger* dragger);
	virtual ~DraggerTransformCallback();

	/**
	* Receive motion commands. Returns true on success.
	*/
	virtual bool receive(const MotionCommand&);
	virtual bool receive(const TranslateInLineCommand& command);
	virtual bool receive(const TranslateInPlaneCommand& command);
	virtual bool receive(const Scale1DCommand& command);
	virtual bool receive(const Scale2DCommand& command);
	virtual bool receive(const ScaleUniformCommand& command);
	virtual bool receive(const Rotate3DCommand& command);

	M3D::Dragger* GetDragger() const;
	void SetDragger(M3D::Dragger* val);

	//在这里更新拖拽器提示信息，可以根据需要重写
	virtual void UpdateDraggerTip(const MotionCommand& command);

	void UpdteAixsLineTip(const MotionCommand& command);

	void UpdteAixsPlaneTip(const MotionCommand& command);
	void UpdteRotationTip(const MotionCommand & command);
	void UpdteScaleTip(const MotionCommand & command);
private:
	Dragger* m_dragger;

	string textImagePath;
	Vector3 m_startPos;
};

 
/**
 * Base class for draggers. Concrete draggers implement the pick event handler
 * and generate motion commands (translate, rotate, ...) and sends these
 * command to all the DraggerCallbacks & Transforms that are connected to the Dragger that generates the
 * commands.
 */
class M3D_API Dragger : public GroupNode
{
public:
		Dragger();
		// Dragger(const Dragger& rhs, const M3D::CopyOp& copyop = M3D::CopyOp::SHALLOW_COPY);

		virtual ~Dragger();
        /**
         * Set/Get parent dragger. For simple draggers parent points to itself.
         * For composite draggers parent points to the parent dragger that uses
         * this dragger.
         */
        virtual void setParentDragger(Dragger* parent) { _parentDragger = parent; }
        Dragger* getParentDragger() { return _parentDragger; }
        const Dragger* getParentDragger() const { return _parentDragger; }

        /** Returns 0 if this Dragger is not a CompositeDragger. */
        virtual const CompositeDragger* getComposite() const { return 0; }

        /** Returns 0 if this Dragger is not a CompositeDragger. */
        virtual CompositeDragger* getComposite() { return 0; }

        //void setHandleEvents(bool flag);
        //bool getHandleEvents() const { return _handleEvents; }

        void setActivationModKeyMask(unsigned int mask) { _activationModKeyMask = mask; }
        unsigned int getActivationModKeyMask() const { return _activationModKeyMask; }

        void setActivationKeyEvent(int key) { _activationKeyEvent = key; }
        int getActivationKeyEvent() const { return _activationKeyEvent; }

        bool handle(const TouchEvent& ea);
        virtual bool handle(const PointerInfo&, const TouchEvent&) { return false; }

		void SetPreSelect(bool preSelected);

        //typedef std::vector< Constraint> > Constraints;

        //void addConstraint(Constraint* constraint);
        //void removeConstraint(Constraint* constraint);

        //Constraints& getConstraints() { return _constraints; }
        //const Constraints& getConstraints() const { return _constraints; }


        typedef std::vector<DraggerCallback* > DraggerCallbacks;

		virtual void addDraggerCallback(DraggerCallback* dc);
		virtual void removeDraggerCallback(DraggerCallback* dc);

        DraggerCallbacks& getDraggerCallbacks() { return _draggerCallbacks; }
        const DraggerCallbacks& getDraggerCallbacks() const { return _draggerCallbacks; }

		virtual void ClearDraggerCallbacks() {
			for (int i = 0;i<_draggerCallbacks.size();i++)
			{
				_draggerCallbacks.at(i)->Release();
			}
			_draggerCallbacks.clear();
		}

       // void addTransformUpdating(MatrixTransform* transform);
       // void removeTransformUpdating(MatrixTransform* transform);

        /** Setup default geometry for dragger. */
        virtual void setupDefaultGeometry() {}

        virtual bool receive(const MotionCommand& command);
        void dispatch(MotionCommand& command);

        void setDraggerActive(bool active) { _draggerActive = active; }
        bool getDraggerActive() const { return _draggerActive; }

		static Dragger* GetPickDragger(SVIEW::View* view, const TouchEvent& ea,PointerInfo& pointerInfo);

		M3D::SceneManager* GetScene() const { return _scene; }
		void SetScene(M3D::SceneManager* val) { _scene = val; }

		M3D::Vector3 GetOrigScale() const { return _OrigScale; }
		void SetOrigScale(M3D::Vector3 val);

		bool GetNeedScale() const { return _needScale; }
		void SetNeedScale(bool val) { _needScale = val; }

		/**
		* @brief 设置隐藏状态
		*/
		virtual void SetVisible(bool isVisible)
		{
			SceneNode::SetVisible(isVisible);
			this->SetNeedScale(true);
		}

		M3D::Color GetPreSelectColor() const { return _PreSelectColor; }
		void SetPreSelectColor(M3D::Color val) { _PreSelectColor = val; }

		virtual void FindVisiableObject(RenderAction* renderAction);

		/** Set/Get color for dragger. */
		inline void setColor(const M3D::Color& color) {
			_color = color;
		}
		inline const M3D::Color& getColor() const { return _color; }

		/**
		* Set/Get pick color for dragger. Pick color is color of the dragger
		* when picked. It gives a visual feedback to show that the dragger has
		* been picked.
		*/
		inline void setPickColor(const M3D::Color& color) {
			_pickColor = color;
		}
		inline const M3D::Color& getPickColor() const { return _pickColor; }
		bool GetPreSelected() const { return preSelected; }
		void SetPreSelected(bool val) { preSelected = val; }
		//拖拽器回调
		M3D_DRAGGERCB* m_draggerCB;
		M3D_DRAGGERCB2* m_draggerCB2;
		void SetDraggerCB(M3D_DRAGGERCB* draggerCB) { m_draggerCB = draggerCB; }
		M3D_DRAGGERCB* GetDraggerCB() { return m_draggerCB; }
		void SetDraggerCB2(M3D_DRAGGERCB2* draggerCB2) { m_draggerCB2 = draggerCB2; }
		M3D_DRAGGERCB2* GetDraggerCB2() { return m_draggerCB2; }		
		//设置拖拽器的名称
		void SetDraggerName(wstring strName) { m_strName = strName; }
		wstring GetDraggerName(){return m_strName;}
		bool IsMoveModel() const { return m_bMoveModel; }
		void SetMoveModelStatus(bool val) { m_bMoveModel = val; }

		M3D::Model* GetDrawModel() const { return _drawModel; }
		void SetDrawModel(M3D::Model* val) { _drawModel = val; }
protected:
 
        bool                            _handleEvents;
        bool                            _draggerActive;

        unsigned int                    _activationModKeyMask;
        int                             _activationKeyEvent;
        bool                            _activationPermittedByModKeyMask;
        bool                            _activationPermittedByKeyEvent;

        PointerInfo     _pointer;

        Dragger*                        _parentDragger;

        DraggerCallback*   _selfUpdater;
        //Constraints                     _constraints;
        DraggerCallbacks                _draggerCallbacks;

		SceneManager* _scene;

		Vector3 _OrigScale;

		bool _needScale;

		bool preSelected;
		M3D::Color     _PreSelectColor;

		Model* _drawModel;

		M3D::Color                       _color;
		M3D::Color                       _pickColor;
		wstring		m_strName;
		bool	m_bMoveModel;//拖拽时是否移动模型
};


/**
 * CompositeDragger allows to create complex draggers that are composed of a
 * hierarchy of Draggers.
 */
class M3D_API CompositeDragger : public Dragger
{
    public:
		CompositeDragger();
		//CompositeDragger(const CompositeDragger& rhs, const M3D::CopyOp& copyop = M3D::CopyOp::SHALLOW_COPY);

		virtual ~CompositeDragger() {
			DraggerList::iterator itr = _draggerList.begin();
			while (itr != _draggerList.end())
			{
				(*itr)->Release(); itr++;
			}
		}

        typedef std::vector<Dragger*> DraggerList;

        virtual const CompositeDragger* getComposite() const { return this; }
        virtual CompositeDragger* getComposite() { return this; }

        virtual void setParentDragger(Dragger* parent);

        virtual bool handle(const PointerInfo& pi, const TouchEvent& ea);

        // Composite-specific methods below
        virtual bool addDragger(Dragger* dragger);
        virtual bool removeDragger(Dragger* dragger);
        unsigned int getNumDraggers() const { return _draggerList.size(); }
        Dragger* getDragger(unsigned int i) { return _draggerList[i]; }
        const Dragger* getDragger(unsigned int i) const { return _draggerList[i]; }
        bool containsDragger(const Dragger* dragger) const;
        DraggerList::iterator findDragger(const Dragger* dragger);
 
    protected:
        DraggerList _draggerList;
};

///**
// * Culls the drawable all the time. Used by draggers to have invisible geometry
// * around lines and points so that they can be picked. For example, a dragger
// * could have a line with an invisible cylinder around it to enable picking on
// * that line.
// */
//void M3D_API setDrawableToAlwaysCull(M3D::Drawable& drawable);
//
///**
// * Convenience function for setting the material color on a node.
// */
//void M3D_API setMaterialColor(const M3D::Color& color, M3D::Node& node);

}

#endif
