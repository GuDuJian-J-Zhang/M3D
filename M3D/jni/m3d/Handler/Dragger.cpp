

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Command.h"
#include "sview/views/View.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Edge.h"
#include "../utils/StringHelper.h"
#include "../utils/MeshHelper.h"
#include "RotateCylinderDragger.h"
#include "../extras/modelmanager/DraggerCallbacks.h"
#include "../extras/modelmanager/PivotCallbacks.h"

namespace M3D {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////
	//// DraggerTransformCallback
	////
	//DraggerTransformCallback::DraggerTransformCallback(M3D::MatrixTransform* transform):
	//    _transform(transform)
	//{
	//}
	//
	//bool DraggerTransformCallback::receive(const MotionCommand& command)
	//{
	//    if (!_transform) return false;
	//
	//    switch (command.getStage())
	//    {
	//        case MotionCommand::START:
	//        {
	//            // Save the current matrix
	//            _startMotionMatrix = _transform->getMatrix();
	//
	//            // Get the LocalToWorld and WorldToLocal matrix for this node.
	//            M3D::NodePath nodePathToRoot;
	//            computeNodePathToRoot(*_transform,nodePathToRoot);
	//            _localToWorld = M3D::computeLocalToWorld(nodePathToRoot);
	//            _worldToLocal = M3D::Matrix3x4::inverse(_localToWorld);
	//
	//            return true;
	//        }
	//        case MotionCommand::MOVE:
	//        {
	//            // Transform the command's motion matrix into local motion matrix.
	//            M3D::Matrix3x4 localMotionMatrix = _localToWorld * command.getWorldToLocal()
	//                                            * command.getMotionMatrix()
	//                                            * command.getLocalToWorld() * _worldToLocal;
	//
	//            // Transform by the localMotionMatrix
	//            _transform->setMatrix(localMotionMatrix * _startMotionMatrix);
	//
	//            return true;
	//        }
	//        case MotionCommand::FINISH:
	//        {
	//            return true;
	//        }
	//        case MotionCommand::NONE:
	//        default:
	//            return false;
	//    }
	//}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// PointerInfo
	//
	PointerInfo::PointerInfo() :
		_nearPoint(M3D::Vector3()),
		_farPoint(M3D::Vector3()),
		_eyeDir(M3D::Vector3(0, 0, 1))
	{
		m_camera = nullptr;
		// _hitIter = _hitList.begin();
	}


	void PointerInfo::setNearFarPoints(M3D::Vector3 nearPoint, M3D::Vector3 farPoint)
	{
		_nearPoint = nearPoint;
		_farPoint = farPoint;
		_eyeDir = farPoint - nearPoint;
	}


	void PointerInfo::getNearFarPoints(M3D::Vector3& nearPoint, M3D::Vector3& farPoint) const
	{
		nearPoint = _nearPoint;
		farPoint = _farPoint;
	}

	bool PointerInfo::contains(const Dragger* node) const
	{
		// if (node && _hitIter!=_hitList.end()) return std::find((*_hitIter).first.begin(), (*_hitIter).first.end(), node) != (*_hitIter).first.end();
		// else return false;
		return (this->_pickDragger == node);
	}


	void PointerInfo::setCamera(M3D::CameraNode* camera)
	{
		if (camera)
		{
			m_camera = camera;
			M3D::Vector3 eye, center, up;
			camera->GetView().GetLookAt(eye, center, up);
			_eyeDir = eye - center;
		}
		else
		{
			//_MVPW = _inverseMVPW = Matrix4::IDENTITY;
			_eyeDir = Vector3::FORWARD;
		}
	}

	void PointerInfo::setIntersection(Dragger* nodePath, const M3D::Vector3& intersectionPoint)
	{
		_pickDragger = nodePath;
		_intersectPoint = intersectionPoint;

		//TODO 获取Dragger相关对应的矩阵
		M3D::Matrix3x4 worlToLoacalMatrix = _pickDragger->GetWorldTransform();
		_LocalIntersectPoint = worlToLoacalMatrix.Inverse()*intersectionPoint;
	}


	void PointerInfo::setMousePosition(float pixel_x, float pixel_y)
	{
		projectWindowXYIntoObject(M3D::Vector2(pixel_x, pixel_y), _nearPoint, _farPoint);
	}

	bool PointerInfo::projectWindowXYIntoObject(const M3D::Vector2& windowCoord, M3D::Vector3& nearPoint, M3D::Vector3& farPoint) const
	{
		if (m_camera)
		{
			CameraNode* camera = m_camera;
			const Viewport& viewPort = camera->GetViewPort();
			nearPoint = viewPort.ScreenToWorldPoint(windowCoord.m_x, windowCoord.m_y, camera->GetNearClip());
			farPoint = viewPort.ScreenToWorldPoint(windowCoord.m_x, windowCoord.m_y, camera->GetHalfClip());
		}

		return true;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Dragger
	//
	Dragger::Dragger() :GroupNode(),
		_handleEvents(false),
		_draggerActive(false),
		_activationModKeyMask(0),
		_activationKeyEvent(0),
		_activationPermittedByModKeyMask(false),
		_activationPermittedByKeyEvent(false)
	{
		_parentDragger = this;
		_selfUpdater = new DraggerTransformCallback(this);
		AddRefMe(_selfUpdater);
		_drawModel = NULL;
		this->preSelected = false;
		m_draggerCB = nullptr;
		m_draggerCB2 = nullptr;
		m_bMoveModel = true;
		/* getOrCreateStateSet()->setDataVariance(M3D::Object::DYNAMIC);*/
	}

	//Dragger::Dragger(const Dragger& rhs, const M3D::CopyOp& copyop):
	//    M3D::MatrixTransform(rhs, copyop),
	//    _handleEvents(rhs._handleEvents),
	//    _draggerActive(false),
	//    _activationModKeyMask(rhs._activationModKeyMask),
	//    _activationKeyEvent(rhs._activationKeyEvent),
	//    _activationPermittedByModKeyMask(false),
	//    _activationPermittedByKeyEvent(false)
	//{
	//    OSG_NOTICE<<"CompositeDragger::CompositeDragger(const CompositeDragger& rhs, const M3D::CopyOp& copyop) not Implemented yet."<<std::endl;
	//}

	Dragger::~Dragger()
	{
		ReleaseMe(_selfUpdater);
	}

	//void Dragger::setHandleEvents(bool flag)
	//{
	//    if (_handleEvents == flag) return;
	//
	//    _handleEvents = flag;
	//
	//    // update the number of children that require an event traversal to make sure this dragger recieves events.
	//    if (_handleEvents) setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()+1);
	//    else if (getNumChildrenRequiringEventTraversal()>=1) setNumChildrenRequiringEventTraversal(getNumChildrenRequiringEventTraversal()-1);
	//}
	//
	//void Dragger::addConstraint(Constraint* constraint)
	//{
	//    // check to make sure constaint hasn't already been attached.
	//    for(Constraints::iterator itr = _constraints.begin();
	//        itr != _constraints.end();
	//        ++itr)
	//    {
	//        if (*itr == constraint) return;
	//    }
	//
	//    _constraints.push_back(constraint);
	//}

	//void Dragger::removeConstraint(Constraint* constraint)
	//{
	//    for(Constraints::iterator itr = _constraints.begin();
	//        itr != _constraints.end();
	//        )
	//    {
	//        if (*itr == constraint)
	//        {
	//            _constraints.erase(itr);
	//            return;
	//        } else
	//        {
	//            ++itr;
	//        }
	//    }
	//}

	//void Dragger::addTransformUpdating(M3D::MatrixTransform* transform)
	//{
	//    addDraggerCallback(new DraggerTransformCallback(transform));
	//}

	//void Dragger::removeTransformUpdating(M3D::MatrixTransform* transform)
	//{
	//    for(Dragger::DraggerCallbacks::iterator itr = _draggerCallbacks.begin();
	//        itr != _draggerCallbacks.end();
	//        )
	//    {
	//        DraggerCallback* dc = itr->get();
	//        DraggerTransformCallback* dtc = dynamic_cast<DraggerTransformCallback*>(dc);
	//        if (dtc && dtc->getTransform()==transform)
	//        {
	//            itr = _draggerCallbacks.erase(itr);
	//        }
	//        else
	//        {
	//            ++itr;
	//        }
	//    }
	//
	//}

	void Dragger::addDraggerCallback(DraggerCallback* dc)
	{
		for (DraggerCallbacks::iterator itr = _draggerCallbacks.begin();
			itr != _draggerCallbacks.end();
			++itr)
		{
			if (*itr == dc) return;
		}
		dc->AddRef();
		_draggerCallbacks.push_back(dc);
	}

	void Dragger::removeDraggerCallback(DraggerCallback* dc)
	{
		DraggerCallbacks::iterator itrEraser = _draggerCallbacks.end();
		for (DraggerCallbacks::iterator itr = _draggerCallbacks.begin();
			itr != _draggerCallbacks.end();
			++itr)
		{
			if (*itr == dc) break;
		}
		if (itrEraser != _draggerCallbacks.end())
		{
			(*itrEraser)->Release();
			_draggerCallbacks.erase(itrEraser);
		}
	}

	//void Dragger::traverse(M3D::NodeVisitor& nv)
	//{
	//    if (_handleEvents && nv.getVisitorType()==M3D::NodeVisitor::EVENT_VISITOR)
	//    {
	//         EventVisitor* ev = dynamic_cast< EventVisitor*>(&nv);
	//        if (ev)
	//        {
	//            for( EventQueue::Events::iterator itr = ev->getEvents().begin();
	//                itr != ev->getEvents().end();
	//                ++itr)
	//            {
	//                 GUIEventAdapter* ea = itr->get();
	//                if (handle(*ea, *(ev->getActionAdapter()))) ea->setHandled(true);
	//            }
	//        }
	//        return;
	//    }
	//
	//    MatrixTransform::traverse(nv);
	//}

	bool Dragger::handle(const TouchEvent& ea)
	{
		for (int i = 0; i < this->Size(); i++)
		{
			Dragger* subDragger = dynamic_cast<Dragger*>(this->GetChild(i));
			if (subDragger)
			{
				subDragger->handle(ea);
			}
		}

		if (ea.getHandled()) return false;

		SVIEW::View* view = ea.GetView();
		if (!view) return false;

		bool handled = false;

		bool activationPermitted = true;

		if (activationPermitted || _draggerActive)
		{
			switch (ea.getEventType())
			{
			case TouchEvent::PUSH:
			{
				//进行拾取，判断是否和拖拽器表示的模型相交了
				_pointer.reset();
				Dragger* pickDragger = Dragger::GetPickDragger(view, ea, _pointer);
				Dragger* dragger = pickDragger;
				if (dragger)
				{
					if (dragger == this)
					{
						CameraNode *rootCamera = view->GetSceneManager()->GetCamera();

						_pointer.setCamera(rootCamera);
						_pointer.setMousePosition(ea.getX(), ea.getY());
						dragger->handle(_pointer, ea);
						dragger->setDraggerActive(true);

						handled = true;
					}
				}
			}
			case TouchEvent::DRAG:
			case TouchEvent::RELEASE:
			{
				if (_draggerActive)
				{
					//_pointer._hitIter = _pointer._hitList.begin();
					_pointer.setMousePosition(ea.getX(), ea.getY());

					handle(_pointer, ea);

					handled = true;
				}
				break;
			}
			default:
				break;
			}

			if (_draggerActive && ea.getEventType() == TouchEvent::RELEASE)
			{
				setDraggerActive(false);
				_pointer.reset();
			}
		}

		if (handled)
		{
			ea.setHandled(handled);
		}

		return handled;
	}


	void Dragger::SetPreSelect(bool preSelected)
	{

	}

	bool Dragger::receive(const MotionCommand& command)
	{
		if (_selfUpdater != NULL) return _selfUpdater->receive(command);
		else return false;
	}

	void Dragger::dispatch(MotionCommand& command)
	{
		// apply any constraints
		//for(Constraints::iterator itr = _constraints.begin();
		//    itr != _constraints.end();
		//    ++itr)
		//{
		//    (*itr)->constrain(command);
		//}

		//// move self
		getParentDragger()->receive(command);

		Matrix3x4 ma;
		for (DraggerCallbacks::iterator itr = getParentDragger()->getDraggerCallbacks().begin();
			itr != getParentDragger()->getDraggerCallbacks().end();
			++itr)
		{
			if (typeid(**itr).name() == typeid(ModelDraggerCallback).name())
			{
				if (!getParentDragger()->m_bMoveModel)
					continue;
			}

			(*itr)->receive(command);
		}

		//回调函数
		if (getParentDragger()->m_draggerCB != nullptr)
		{
			int nStage = command.getStage();
			getParentDragger()->m_draggerCB(nStage, m_strName);
		}
	}


	M3D::Dragger* Dragger::GetPickDragger(SVIEW::View* view, const TouchEvent& ea, PointerInfo& pointerInfo)
	{
		Dragger* pickDragger = NULL;
		SceneManager* scene = view->GetSceneManager();// ->GetHandlerGroup()->RayPick();

		RayPickAction* rayPickAction = new RayPickAction(scene);
		rayPickAction->SetPickShapeType(SHAPE_MODEL);
		rayPickAction->SetPickGeoType(0);
		//rayPickAction->SetInterctType(1);//设置成线相交的方式

		rayPickAction->SetRay((int)ea.getX(), (int)ea.getY());
		rayPickAction->SetUseclipPlane(false);
		//LOGI("setpoint is ok");

		//首先处理UI元素拾取的拖拽
		SceneNode* group = view->GetSceneManager()->GetHandlerGroup();
		group->RayPick(rayPickAction);

		IShape* shape = rayPickAction->GetNearPickShape();
		if (shape)
		{
			if (shape->GetType() == SHAPE_MODEL || shape->GetType() == SHAPE_IMAGE_MODEL)
			{
				Model* model = (Model*)shape;
				Dragger* dragger = (Dragger*)model->GetUserData();
				pickDragger = dragger;
				if(dragger)
					pointerInfo.setIntersection(pickDragger, rayPickAction->GetNearestPickPoint());
			}
		}
		delete rayPickAction;

		return pickDragger;
	}


	void Dragger::SetOrigScale(M3D::Vector3 val)
	{
		_OrigScale = val;
		GroupNode::SetScale(_OrigScale);
	}


	void Dragger::FindVisiableObject(RenderAction* renderAction)
	{
		if (this->_drawModel)
		{
			if (this->getDraggerActive())
			{
				this->_drawModel->SetColor(this->getPickColor());
			}
			else if (this->GetPreSelected())
			{
				this->_drawModel->SetColor(this->GetPreSelectColor());
			}
			else
			{
				this->_drawModel->SetColor(this->getColor());
			}
		}

		GroupNode::FindVisiableObject(renderAction);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// CompositeDragger
	//
	//CompositeDragger::CompositeDragger(const CompositeDragger& rhs, const M3D::CopyOp& copyop):
	//    Dragger(rhs, copyop)
	//{
	//    OSG_NOTICE<<"CompositeDragger::CompositeDragger(const CompositeDragger& rhs, const M3D::CopyOp& copyop) not Implemented yet."<<std::endl;
	//}

	bool CompositeDragger::handle(const PointerInfo& pi, const TouchEvent& ea)
	{
		// Check if the dragger node is in the nodepath.
		if (!pi.contains(this))
			return false;

		for (DraggerList::iterator itr = _draggerList.begin(); itr != _draggerList.end(); ++itr)
		{
			if ((*itr)->handle(pi, ea))
				return true;
		}
		return false;
	}
	bool CompositeDragger::containsDragger(const Dragger* dragger) const
	{
		for (DraggerList::const_iterator itr = _draggerList.begin(); itr != _draggerList.end(); ++itr)
		{
			if (*itr == dragger) return true;
		}
		return false;
	}

	CompositeDragger::DraggerList::iterator CompositeDragger::findDragger(const Dragger* dragger)
	{
		for (DraggerList::iterator itr = _draggerList.begin(); itr != _draggerList.end(); ++itr)
		{
			if (*itr == dragger) return itr;
		}
		return _draggerList.end();
	}

	CompositeDragger::CompositeDragger() :Dragger()
	{

	}

	bool CompositeDragger::addDragger(Dragger *dragger)
	{
		if (dragger && !containsDragger(dragger))
		{
			dragger->AddRef();
			_draggerList.push_back(dragger);
			return true;
		}
		else return false;

	}

	bool CompositeDragger::removeDragger(Dragger *dragger)
	{
		DraggerList::iterator itr = findDragger(dragger);
		if (itr != _draggerList.end())
		{
			(*itr)->Release();
			_draggerList.erase(itr);
			return true;
		}
		else return false;

	}

	void CompositeDragger::setParentDragger(Dragger* dragger)
	{
		for (DraggerList::iterator itr = _draggerList.begin(); itr != _draggerList.end(); ++itr)
		{
			(*itr)->setParentDragger(dragger);
		}
		Dragger::setParentDragger(dragger);
	}

	//class ForceCullCallback : public M3D::Drawable::CullCallback
	//{
	//    public:
	//        virtual bool cull(M3D::NodeVisitor*, M3D::Drawable*, M3D::State*) const
	//        {
	//            return true;
	//        }
	//};

	//void osgManipulator::setDrawableToAlwaysCull(M3D::Drawable& drawable)
	//{
	//    ForceCullCallback* cullCB = new ForceCullCallback;
	//    drawable.setCullCallback (cullCB);    
	//}
	//
	//void osgManipulator::setMaterialColor(const M3D::Color& color, M3D::Node& node)
	//{
	//    M3D::Material* mat = dynamic_cast<M3D::Material*>(node.getOrCreateStateSet()->getAttribute(M3D::StateAttribute::MATERIAL));
	//    if (! mat)
	//    {
	//        mat = new M3D::Material;
	//        mat->setDataVariance(M3D::Object::DYNAMIC);
	//        node.getOrCreateStateSet()->setAttribute(mat);
	//    }
	//    mat->setDiffuse(M3D::Material::FRONT_AND_BACK, color);
	//}


	DraggerTransformCallback::DraggerTransformCallback() :DraggerCallback()
	{
		this->SetDragger(NULL);
	}


	DraggerTransformCallback::DraggerTransformCallback(Dragger* dragger) :DraggerCallback()
	{
		this->SetDragger(dragger);
	}

	DraggerTransformCallback::~DraggerTransformCallback()
	{
		//ReleaseMe(m_dragger);
	}

	bool DraggerTransformCallback::receive(const MotionCommand& command)
	{
		this->UpdateDraggerTip(command);
		if (command.getStage() == MotionCommand::MOVE)
		{
			if (command.GetType() == MotionCommand::Type::TYPE_TRANSLATELINE)
			{
				this->receive((TranslateInLineCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_TRANSLATEINPLANE)
			{
				this->receive((TranslateInPlaneCommand&)command);
			}
			else if (command.GetType() == MotionCommand::Type::TYPE_ROTATE3D)
			{
				this->receive((Rotate3DCommand&)command);
			}
		}
		return false;
	}


	bool DraggerTransformCallback::receive(const TranslateInLineCommand& command)
	{
		if (this->m_dragger)
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			m_dragger->Translate(translation, M3D::TS_WORLD);
		}
		return true;
	}


	bool DraggerTransformCallback::receive(const TranslateInPlaneCommand& command)
	{
		if (this->m_dragger)
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			m_dragger->Translate(translation, M3D::TS_WORLD);
		}
		return true;
	}


	bool DraggerTransformCallback::receive(const Scale1DCommand& command)
	{
		if (this->m_dragger)
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			//m_dragger->Translate(translation, M3D::TS_WORLD);
		}

		return true;
	}


	bool DraggerTransformCallback::receive(const Scale2DCommand& command)
	{
		if (this->m_dragger)
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			//m_dragger->Translate(translation, M3D::TS_WORLD);
		}

		return true;
	}


	bool DraggerTransformCallback::receive(const ScaleUniformCommand& command)
	{
		if (this->m_dragger)
		{
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			//m_dragger->Translate(translation, M3D::TS_WORLD);
		}
		return true;
	}


	bool DraggerTransformCallback::receive(const Rotate3DCommand& command)
	{
		if (this->m_dragger)
		{
			Quaternion rotation = command.getDeltaRotation();

			rotation = command.getLocalToWorld().Rotation()*rotation*command.getLocalToWorld().Rotation().Inverse();
			m_dragger->Rotate(rotation, M3D::TS_WORLD);
		}
		return true;
	}

	M3D::Dragger* DraggerTransformCallback::GetDragger() const
	{
		return m_dragger;
	}

	void DraggerTransformCallback::SetDragger(M3D::Dragger* val)
	{
		//ReleaseMe(m_dragger);
		m_dragger = val;
		//AddRefMe(m_dragger);
	}

	string CreateIipTextImage(string& tipImageText, SceneManager* scene)
	{
		return MeshHelper::createTextImage(scene, tipImageText, 12.0f);
	}

	void DraggerTransformCallback::UpdateDraggerTip(const MotionCommand & command)
	{
		UpdteAixsLineTip(command);
		UpdteAixsPlaneTip(command);
		UpdteRotationTip(command);
		//UpdteScaleTip(command);
	}

	void DraggerTransformCallback::UpdteAixsLineTip(const MotionCommand& command)
	{
		M3D::Dragger* refDragger = command.GetRefDragger();
		if (!refDragger || !refDragger->GetDrawModel())
		{
			return;
		}

		//更新X轴
		wstring draggerName = refDragger->GetDraggerName();
		if (draggerName == L"XAxis"
			|| draggerName == L"YAxis"
			|| draggerName == L"ZAxis")
		{
			TranslateInLineCommand& translateInLineCommand = (TranslateInLineCommand&)command;
			Translate1DDragger* translate1dDragger = (Translate1DDragger*)refDragger;
			//
			M3D::MotionCommand::Stage stage = command.getStage();
			const static string XAXISLINENAME = "XAXISLINE";
			const static string XAXISLIE_X = XAXISLINENAME + "+X";
			const static string XAXISLIE_NX = XAXISLINENAME + "-X";

			HandlerGroup* handlerGroup = refDragger->GetScene()->GetHandlerGroup();

			if (stage == MotionCommand::START)
			{
				Model* xAxisLineModel = new Model();
				//添加两条线
				Vector3 lineStartPnt = Vector3::ZERO;

				Vector3 lineEndPnt = Vector3(0, 0, -10000);

				vector<Vector3> pnts;
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineStartPnt = Vector3::ZERO;

				lineEndPnt = Vector3(0, 0, 10000);

				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);

				xAxisLineModel->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineModel->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(1)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->SetName(XAXISLINENAME);

				m_startPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);

				xAxisLineModel->SetOrigPlcMatrix(translateInLineCommand.getLocalToWorld());

				handlerGroup->AddDraggerTip(xAxisLineModel, XAXISLINENAME);
			}
			else if (stage == MotionCommand::MOVE)
			{
				//在轴线上面添加辅助线
				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}

				Vector3 textPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);
				xAxisLineX = new Model();

				vector<Vector3> pnts;
				pnts.push_back(m_startPos);
				pnts.push_back(textPos);
				xAxisLineX->AddLineData(pnts);
				xAxisLineX->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineX->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::WHITE);

				//ImageModel* textImageX = new ImageModel();

				//创建图片
				/*float length = translateInLineCommand.getTranslation().Length();
				string strLength = StringHelper::floatToString(length);
				if (textImagePath.size()>0)
				{
					FileHelper::RemoveFile(textImagePath);
					textImagePath = "";
				}*/

				//textImagePath = CreateIipTextImage(strLength, refDragger->GetScene());
				//textImageX->SetImagePath(textImagePath);

				//textImageX->SetName(XAXISLIE_X);  Vector2 tempSize(1.0f, 1.0f);
				//textImageX->SetImageSize((textPos + m_startPos) / 2, tempSize);

				handlerGroup->AddDraggerTip(xAxisLineX, XAXISLIE_X);
			}
			else if (stage == MotionCommand::FINISH)
			{
				//删除添加的模型
				Model* xAxisLine = handlerGroup->GetDraggerTip(XAXISLINENAME);
				if (xAxisLine)
				{
					handlerGroup->RemoveDraggerTip(XAXISLINENAME);
				}

				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}
			}
		}
	}

	void DraggerTransformCallback::UpdteAixsPlaneTip(const MotionCommand& command)
	{
		M3D::Dragger* refDragger = command.GetRefDragger();
		if (!refDragger || !refDragger->GetDrawModel())
		{
			return;
		}

		//更新X轴
		wstring draggerName = refDragger->GetDraggerName();
		if (draggerName == L"XYAxis"
			|| draggerName == L"XZAxis"
			|| draggerName == L"YZAxis")
		{
			TranslateInLineCommand& translateInLineCommand = (TranslateInLineCommand&)command;
			Translate1DDragger* translate1dDragger = (Translate1DDragger*)refDragger;
			//
			M3D::MotionCommand::Stage stage = command.getStage();
			const static string XAXISLINENAME = "XAXISLINE";
			const static string XAXISLIE_X = XAXISLINENAME + "+X";
			const static string XAXISLIE_NX = XAXISLINENAME + "-X";

			HandlerGroup* handlerGroup = refDragger->GetScene()->GetHandlerGroup();

			if (stage == MotionCommand::START)
			{
				Model* xAxisLineModel = new Model();
				//添加两条线
				Vector3 lineStartPnt = Vector3::ZERO;

				//构建XZ平面在此基础上进行偏移
				Vector3 lineEndPnt = Vector3(0, 0, -10000);

				vector<Vector3> pnts;
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(0, 0, 10000);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(-10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				lineEndPnt = Vector3(10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				//设置这两条线的颜色
				xAxisLineModel->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(1)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->GetBodys()->at(2)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(3)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->SetName(XAXISLINENAME);

				m_startPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);

				xAxisLineModel->SetOrigPlcMatrix(translateInLineCommand.getLocalToWorld());

				handlerGroup->AddDraggerTip(xAxisLineModel, XAXISLINENAME);
			}
			else if (stage == MotionCommand::MOVE)
			{
				//在轴线上面添加辅助线
				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}

				Vector3 textPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);
				xAxisLineX = new Model();

				vector<Vector3> pnts;
				pnts.push_back(m_startPos);
				pnts.push_back(textPos);
				xAxisLineX->AddLineData(pnts);
				xAxisLineX->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineX->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::WHITE);

				//ImageModel* textImageX = new ImageModel();

				//创建图片
				/*float length = translateInLineCommand.getTranslation().Length();
				string strLength = StringHelper::floatToString(length);
				if (textImagePath.size()>0)
				{
				FileHelper::RemoveFile(textImagePath);
				textImagePath = "";
				}*/

				//textImagePath = CreateIipTextImage(strLength, refDragger->GetScene());
				//textImageX->SetImagePath(textImagePath);

				//textImageX->SetName(XAXISLIE_X);  Vector2 tempSize(1.0f, 1.0f);
				//textImageX->SetImageSize((textPos + m_startPos) / 2, tempSize);

				handlerGroup->AddDraggerTip(xAxisLineX, XAXISLIE_X);
			}
			else if (stage == MotionCommand::FINISH)
			{
				//删除添加的模型
				Model* xAxisLine = handlerGroup->GetDraggerTip(XAXISLINENAME);
				if (xAxisLine)
				{
					handlerGroup->RemoveDraggerTip(XAXISLINENAME);
				}

				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}
			}
		}
		else if (draggerName == L"XYZAxis")
		{
			TranslateInLineCommand& translateInLineCommand = (TranslateInLineCommand&)command;
			Translate1DDragger* translate1dDragger = (Translate1DDragger*)refDragger;
			//
			M3D::MotionCommand::Stage stage = command.getStage();
			const static string XAXISLINENAME = "XAXISLINE";
			const static string XAXISLIE_X = XAXISLINENAME + "+X";
			const static string XAXISLIE_NX = XAXISLINENAME + "-X";

			HandlerGroup* handlerGroup = refDragger->GetScene()->GetHandlerGroup();

			if (stage == MotionCommand::START)
			{
				Model* xAxisLineModel = new Model();
				//添加两条线
				Vector3 lineStartPnt = Vector3::ZERO;

				Vector3 lineEndPnt = Vector3(0, 0, -10000);

				vector<Vector3> pnts;
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineStartPnt = Vector3::ZERO;
				lineEndPnt = Vector3(0, 0, 10000);

				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(-10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				lineEndPnt = Vector3(0, -10000, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				lineEndPnt = Vector3(0, 10000, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				//设置这两条线的颜色
				xAxisLineModel->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(1)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->GetBodys()->at(2)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(3)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->GetBodys()->at(4)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(5)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->SetName(XAXISLINENAME);

				m_startPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);

				Matrix3x4 transformInLine = translateInLineCommand.getLocalToWorld();

				Matrix3x4 transformNotContainRotation(transformInLine.Translation(), Quaternion::IDENTITY, 1.0f);

				xAxisLineModel->SetOrigPlcMatrix(transformNotContainRotation);

				handlerGroup->AddDraggerTip(xAxisLineModel, XAXISLINENAME);
			}
			else if (stage == MotionCommand::MOVE)
			{
				//在轴线上面添加辅助线
				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}

				Vector3 textPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);
				xAxisLineX = new Model();

				vector<Vector3> pnts;
				pnts.push_back(m_startPos);
				pnts.push_back(textPos);
				xAxisLineX->AddLineData(pnts);
				xAxisLineX->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineX->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::WHITE);

				//ImageModel* textImageX = new ImageModel();

				//创建图片
				/*float length = translateInLineCommand.getTranslation().Length();
				string strLength = StringHelper::floatToString(length);
				if (textImagePath.size()>0)
				{
				FileHelper::RemoveFile(textImagePath);
				textImagePath = "";
				}*/

				//textImagePath = CreateIipTextImage(strLength, refDragger->GetScene());
				//textImageX->SetImagePath(textImagePath);

				//textImageX->SetName(XAXISLIE_X);  Vector2 tempSize(1.0f, 1.0f);
				//textImageX->SetImageSize((textPos + m_startPos) / 2, tempSize);

				handlerGroup->AddDraggerTip(xAxisLineX, XAXISLIE_X);
			}
			else if (stage == MotionCommand::FINISH)
			{
				//删除添加的模型
				Model* xAxisLine = handlerGroup->GetDraggerTip(XAXISLINENAME);
				if (xAxisLine)
				{
					handlerGroup->RemoveDraggerTip(XAXISLINENAME);
				}

				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}
			}
		}
	}


	void DraggerTransformCallback::UpdteRotationTip(const MotionCommand& command)
	{
		M3D::Dragger* refDragger = command.GetRefDragger();
		if (!refDragger || !refDragger->GetDrawModel())
		{
			return;
		}

		//更新X轴
		wstring draggerName = refDragger->GetDraggerName();
		if (draggerName == L"XRotate"
			|| draggerName == L"YRotate"
			|| draggerName == L"ZRotate")
		{
			Rotate3DCommand& translateInLineCommand = (Rotate3DCommand&)command;
			RotateCylinderDragger* translate1dDragger = (RotateCylinderDragger*)refDragger;
			//
			M3D::MotionCommand::Stage stage = command.getStage();
			const static string XAXISLINENAME = "XAXISLINE";
			const static string XAXISLIE_X = XAXISLINENAME + "+X";
			const static string XAXISLIE_NX = XAXISLINENAME + "-X";

			HandlerGroup* handlerGroup = refDragger->GetScene()->GetHandlerGroup();

			if (stage == MotionCommand::START)
			{
				Model* xAxisLineModel = new Model();
				//添加两条线
				Vector3 lineStartPnt = Vector3::ZERO;

				//构建XZ平面在此基础上进行偏移
				Vector3 lineEndPnt = Vector3(0, 0, -10000);

				vector<Vector3> pnts;
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(0, 0, 10000);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				//设置这两条线的颜色
				xAxisLineModel->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(1)->GetEdges()->at(0)->SetColor(Color::YELLOW);
	
				xAxisLineModel->SetName(XAXISLINENAME);

				m_startPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);

				Matrix3x4 transformInLine = translateInLineCommand.getLocalToWorld();

				if (draggerName == L"ZRotate")
				{
					Matrix3x4 transformNotContainRotation(transformInLine.Translation(), Quaternion::IDENTITY, 1.0f);
					xAxisLineModel->SetOrigPlcMatrix(translateInLineCommand.getLocalToWorld());
				}
				else if (draggerName == L"XRotate")
				{
					M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
					Matrix3x4 transformNotContainRotation(transformInLine.Translation(), rotation, 1.0f);
					xAxisLineModel->SetOrigPlcMatrix(translateInLineCommand.getLocalToWorld());
				}
				else if (draggerName == L"YRotate")
				{
					M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
					Matrix3x4 transformNotContainRotation(transformInLine.Translation(), rotation, 1.0f);
					xAxisLineModel->SetOrigPlcMatrix(translateInLineCommand.getLocalToWorld());
				}
				handlerGroup->AddDraggerTip(xAxisLineModel, XAXISLINENAME);
			}
			else if (stage == MotionCommand::MOVE)
			{
				//在轴线上面添加辅助线
				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}

				Vector3 textPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);
				xAxisLineX = new Model();

				vector<Vector3> pnts;
				pnts.push_back(m_startPos);
				pnts.push_back(textPos);
				xAxisLineX->AddLineData(pnts);
				xAxisLineX->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineX->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::WHITE);

				//ImageModel* textImageX = new ImageModel();

				//创建图片
				/*float length = translateInLineCommand.getTranslation().Length();
				string strLength = StringHelper::floatToString(length);
				if (textImagePath.size()>0)
				{
				FileHelper::RemoveFile(textImagePath);
				textImagePath = "";
				}*/

				//textImagePath = CreateIipTextImage(strLength, refDragger->GetScene());
				//textImageX->SetImagePath(textImagePath);

				//textImageX->SetName(XAXISLIE_X);  Vector2 tempSize(1.0f, 1.0f);
				//textImageX->SetImageSize((textPos + m_startPos) / 2, tempSize);

				handlerGroup->AddDraggerTip(xAxisLineX, XAXISLIE_X);
			}
			else if (stage == MotionCommand::FINISH)
			{
				//删除添加的模型
				Model* xAxisLine = handlerGroup->GetDraggerTip(XAXISLINENAME);
				if (xAxisLine)
				{
					handlerGroup->RemoveDraggerTip(XAXISLINENAME);
				}

				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}
			}
		}
	}


	void DraggerTransformCallback::UpdteScaleTip(const MotionCommand& command)
	{
		M3D::Dragger* refDragger = command.GetRefDragger();
		if (!refDragger || !refDragger->GetDrawModel())
		{
			return;
		}

		//更新X轴
		wstring draggerName = refDragger->GetDraggerName();
		if (draggerName == L"XYAxis"
			|| draggerName == L"XZAxis"
			|| draggerName == L"YZAxis")
		{
			TranslateInLineCommand& translateInLineCommand = (TranslateInLineCommand&)command;
			Translate1DDragger* translate1dDragger = (Translate1DDragger*)refDragger;
			//
			M3D::MotionCommand::Stage stage = command.getStage();
			const static string XAXISLINENAME = "XAXISLINE";
			const static string XAXISLIE_X = XAXISLINENAME + "+X";
			const static string XAXISLIE_NX = XAXISLINENAME + "-X";

			HandlerGroup* handlerGroup = refDragger->GetScene()->GetHandlerGroup();

			if (stage == MotionCommand::START)
			{
				Model* xAxisLineModel = new Model();
				//添加两条线
				Vector3 lineStartPnt = Vector3::ZERO;

				//构建XZ平面在此基础上进行偏移
				Vector3 lineEndPnt = Vector3(0, 0, -10000);

				vector<Vector3> pnts;
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(0, 0, 10000);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(-10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				lineEndPnt = Vector3(10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				//设置这两条线的颜色
				xAxisLineModel->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(1)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->GetBodys()->at(2)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(3)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->SetName(XAXISLINENAME);

				m_startPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);

				xAxisLineModel->SetOrigPlcMatrix(translateInLineCommand.getLocalToWorld());

				handlerGroup->AddDraggerTip(xAxisLineModel, XAXISLINENAME);
			}
			else if (stage == MotionCommand::MOVE)
			{
				//在轴线上面添加辅助线
				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}

				Vector3 textPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);
				xAxisLineX = new Model();

				vector<Vector3> pnts;
				pnts.push_back(m_startPos);
				pnts.push_back(textPos);
				xAxisLineX->AddLineData(pnts);
				xAxisLineX->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineX->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::WHITE);

				//ImageModel* textImageX = new ImageModel();

				//创建图片
				/*float length = translateInLineCommand.getTranslation().Length();
				string strLength = StringHelper::floatToString(length);
				if (textImagePath.size()>0)
				{
				FileHelper::RemoveFile(textImagePath);
				textImagePath = "";
				}*/

				//textImagePath = CreateIipTextImage(strLength, refDragger->GetScene());
				//textImageX->SetImagePath(textImagePath);

				//textImageX->SetName(XAXISLIE_X);  Vector2 tempSize(1.0f, 1.0f);
				//textImageX->SetImageSize((textPos + m_startPos) / 2, tempSize);

				handlerGroup->AddDraggerTip(xAxisLineX, XAXISLIE_X);
			}
			else if (stage == MotionCommand::FINISH)
			{
				//删除添加的模型
				Model* xAxisLine = handlerGroup->GetDraggerTip(XAXISLINENAME);
				if (xAxisLine)
				{
					handlerGroup->RemoveDraggerTip(XAXISLINENAME);
				}

				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}
			}
		}
		else if (draggerName == L"XYZAxis")
		{
			TranslateInLineCommand& translateInLineCommand = (TranslateInLineCommand&)command;
			Translate1DDragger* translate1dDragger = (Translate1DDragger*)refDragger;
			//
			M3D::MotionCommand::Stage stage = command.getStage();
			const static string XAXISLINENAME = "XAXISLINE";
			const static string XAXISLIE_X = XAXISLINENAME + "+X";
			const static string XAXISLIE_NX = XAXISLINENAME + "-X";

			HandlerGroup* handlerGroup = refDragger->GetScene()->GetHandlerGroup();

			if (stage == MotionCommand::START)
			{
				Model* xAxisLineModel = new Model();
				//添加两条线
				Vector3 lineStartPnt = Vector3::ZERO;

				Vector3 lineEndPnt = Vector3(0, 0, -10000);

				vector<Vector3> pnts;
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineStartPnt = Vector3::ZERO;
				lineEndPnt = Vector3(0, 0, 10000);

				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(-10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				lineEndPnt = Vector3(10000, 0, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				lineEndPnt = Vector3(0, -10000, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);


				lineEndPnt = Vector3(0, 10000, 0);
				pnts.clear();
				pnts.push_back(lineStartPnt);
				pnts.push_back(lineEndPnt);
				xAxisLineModel->AddLineData(pnts);

				//设置这两条线的颜色
				xAxisLineModel->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(1)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->GetBodys()->at(2)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(3)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->GetBodys()->at(4)->GetEdges()->at(0)->SetColor(Color::RED);
				xAxisLineModel->GetBodys()->at(5)->GetEdges()->at(0)->SetColor(Color::YELLOW);

				xAxisLineModel->SetName(XAXISLINENAME);

				m_startPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);

				Matrix3x4 transformInLine = translateInLineCommand.getLocalToWorld();

				Matrix3x4 transformNotContainRotation(transformInLine.Translation(), Quaternion::IDENTITY, 1.0f);

				xAxisLineModel->SetOrigPlcMatrix(transformNotContainRotation);

				handlerGroup->AddDraggerTip(xAxisLineModel, XAXISLINENAME);
			}
			else if (stage == MotionCommand::MOVE)
			{
				//在轴线上面添加辅助线
				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}

				Vector3 textPos = translateInLineCommand.getLocalToWorld()*Vector4(Vector3::ZERO, 1);
				xAxisLineX = new Model();

				vector<Vector3> pnts;
				pnts.push_back(m_startPos);
				pnts.push_back(textPos);
				xAxisLineX->AddLineData(pnts);
				xAxisLineX->AddLineData(pnts);
				//设置这两条线的颜色
				xAxisLineX->GetBodys()->at(0)->GetEdges()->at(0)->SetColor(Color::WHITE);

				//ImageModel* textImageX = new ImageModel();

				//创建图片
				/*float length = translateInLineCommand.getTranslation().Length();
				string strLength = StringHelper::floatToString(length);
				if (textImagePath.size()>0)
				{
				FileHelper::RemoveFile(textImagePath);
				textImagePath = "";
				}*/

				//textImagePath = CreateIipTextImage(strLength, refDragger->GetScene());
				//textImageX->SetImagePath(textImagePath);

				//textImageX->SetName(XAXISLIE_X);  Vector2 tempSize(1.0f, 1.0f);
				//textImageX->SetImageSize((textPos + m_startPos) / 2, tempSize);

				handlerGroup->AddDraggerTip(xAxisLineX, XAXISLIE_X);
			}
			else if (stage == MotionCommand::FINISH)
			{
				//删除添加的模型
				Model* xAxisLine = handlerGroup->GetDraggerTip(XAXISLINENAME);
				if (xAxisLine)
				{
					handlerGroup->RemoveDraggerTip(XAXISLINENAME);
				}

				Model* xAxisLineX = handlerGroup->GetDraggerTip(XAXISLIE_X);
				if (xAxisLineX)
				{
					handlerGroup->RemoveDraggerTip(XAXISLIE_X);
				}
			}
		}
	}

}

