
#include "m3d/handler/TranslateBoxDragger.h"
#include "m3d/scene/ShapeNode.h"

using namespace M3D;

BoxDraggerCallback::BoxDraggerCallback()
{
	this->SetDragger(NULL);
	this->SetBoxDragger(NULL);
}
BoxDraggerCallback::BoxDraggerCallback(Dragger* dragger)
{
	this->SetDragger(dragger);
	this->SetBoxDragger(NULL);
}
BoxDraggerCallback:: ~BoxDraggerCallback()
{

}

bool BoxDraggerCallback::receive(const MotionCommand& command)
{
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
	}
	return false;
}

bool BoxDraggerCallback::receive(const TranslateInLineCommand& command)
{
	M3D::Dragger* refDragger = command.GetRefDragger();
	if (!refDragger || !refDragger->GetDrawModel())
	{
		return false;
	}
	if (refDragger)
	{
		if (this->m_boxDragger)
		{
			//首先更新一下拖拽点的位置
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			refDragger->Translate(translation, M3D::TS_WORLD);

			//根据名字确定更新其他几个点的逻辑
			wstring draggerName = refDragger->GetDraggerName();
			TranslateBoxDragger* translateBoxDragger = (TranslateBoxDragger*)this->m_boxDragger;
			if (draggerName == L"BoxLeftTop")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 1);
			}
			else if (draggerName == L"BoxRightTop")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 2);
			}
			else if (draggerName == L"BoxRightBottom")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 3);
			}
			else if (draggerName == L"BoxLeftBottom")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 0);
			}
			else if (draggerName == L"BoxCenter")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 4);
			}
		}
	}
	return true;
}

bool BoxDraggerCallback::receive(const TranslateInPlaneCommand& command)
{
	M3D::Dragger* refDragger = command.GetRefDragger();
	if (!refDragger || !refDragger->GetDrawModel())
	{
		return false;
	}
 	if (refDragger)
	{
		if (this->m_boxDragger)
		{
			//首先更新一下拖拽点的位置
			Vector3 translation = command.getTranslation();
			translation = command.getLocalToWorld()*Vector4(translation, 0);
			refDragger->Translate(translation, M3D::TS_WORLD);

			//根据名字确定更新其他几个点的逻辑
			wstring draggerName = refDragger->GetDraggerName();
			TranslateBoxDragger* translateBoxDragger = (TranslateBoxDragger*)this->m_boxDragger;
			if (draggerName == L"BoxLeftTop")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 1);
			}
			else if (draggerName == L"BoxRightTop")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 2);
			}
			else if (draggerName == L"BoxRightBottom")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 3);
			}
			else if (draggerName == L"BoxLeftBottom")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 0);
			}
			else if (draggerName == L"BoxCenter")
			{
				Vector3 newPos = refDragger->GetWorldPosition();
				translateBoxDragger->Update(newPos, 4);
			}
		}
	}
	return true;
}

TranslateBoxDragger::TranslateBoxDragger():CompositeDragger()
{
	_leftTopDragger = new Translate2DDragger(M3D::Plane(Vector3(.0, 0.0, 1.0), 0.0));
	_leftTopDragger->AddRef();
	_leftTopDragger->SetDraggerName(L"BoxLeftTop");
    AddChild(_leftTopDragger);
    addDragger(_leftTopDragger);
    
    _leftBottomDragger = new Translate2DDragger(M3D::Plane(Vector3(.0, 0.0, 1.0), 0.0));
	_leftBottomDragger->AddRef();
	_leftBottomDragger->SetDraggerName(L"BoxLeftBottom");
	AddChild(_leftBottomDragger);
    addDragger(_leftBottomDragger);
    
    _rightTopDragger = new Translate2DDragger(M3D::Plane(Vector3(.0, 0.0, 1.0), 0.0));
	_rightTopDragger->AddRef();
	_rightTopDragger->SetDraggerName(L"BoxRightTop");
	AddChild(_rightTopDragger);
    addDragger(_rightTopDragger);
 
	_rightBottomDragger = new Translate2DDragger(M3D::Plane(Vector3(.0, 0.0, 1.0), 0.0));
	_rightBottomDragger->AddRef();
	_rightBottomDragger->SetDraggerName(L"BoxRightBottom");
	AddChild(_rightBottomDragger);
	addDragger(_rightBottomDragger);

	_centerDragger = new Translate2DDragger(M3D::Plane(Vector3(.0, 0.0, 1.0), 0.0));
	_centerDragger->AddRef();
	_centerDragger->SetDraggerName(L"BoxCenter");
	AddChild(_centerDragger);
	addDragger(_centerDragger);

    setParentDragger(getParentDragger());
	BoxDraggerCallback* boxDraggerCallback = new BoxDraggerCallback(this);
	_selfUpdater = boxDraggerCallback;
	boxDraggerCallback->SetBoxDragger(this);
}
       
TranslateBoxDragger::~TranslateBoxDragger()
{
	ReleaseMe(_leftTopDragger);
	ReleaseMe(_leftBottomDragger);
	ReleaseMe(_rightTopDragger);
	ReleaseMe(_rightBottomDragger);
	ReleaseMe(_centerDragger);
}
 
void TranslateBoxDragger::setupDefaultGeometry()
{
	const float AxisAlpha = 1.0f;
 
	_leftTopDragger->setupDefaultImageGeometry();
	_leftBottomDragger->setupDefaultImageGeometry();
	_rightTopDragger->setupDefaultImageGeometry();
	_rightBottomDragger->setupDefaultImageGeometry();
	_centerDragger->setupDefaultImageGeometry();
 
    // Send different colors for each dragger.
	_leftTopDragger->setColor(M3D::Color(1.0f, 0.0f, 0.0f, AxisAlpha));
	_leftBottomDragger->setColor(M3D::Color(0.0f, 1.0f, 0.0f, AxisAlpha));
	_rightTopDragger->setColor(M3D::Color(0.0f, 0.0f, 1.0f, AxisAlpha));
 
	_leftTopDragger->setPickColor(Color::YELLOW);
	_leftBottomDragger->setPickColor(Color::YELLOW);
	_rightTopDragger->setPickColor(Color::YELLOW);

	_leftTopDragger->SetPreSelectColor(Color::YELLOW);
	_leftBottomDragger->SetPreSelectColor(Color::YELLOW);
	_rightTopDragger->SetPreSelectColor(Color::YELLOW);
}
 
void M3D::TranslateBoxDragger::SetScene(M3D::SceneManager* val)
{
	CompositeDragger::SetScene(val);
	_leftTopDragger->SetScene(val);
	_leftBottomDragger->SetScene(val);
	_rightTopDragger->SetScene(val);
	_rightBottomDragger->SetScene(val);
	_centerDragger->SetScene(val);
}

void M3D::TranslateBoxDragger::SetBoxSize(const Vector3& leftBottom, const Vector3& rightTop)
{
	Vector3 leftTop(leftBottom.m_x,rightTop.m_y,leftBottom.m_z);
	Vector3 rightBottom(rightTop.m_x, leftBottom.m_y, leftBottom.m_z);

	//重新设置Dragger的位置
	_leftTopDragger->SetWorldPosition(leftTop);
	_leftBottomDragger->SetWorldPosition(leftBottom);
	_rightTopDragger->SetWorldPosition(rightTop);
	_rightBottomDragger->SetWorldPosition(rightBottom);

	_centerDragger->SetWorldPosition((leftBottom + rightTop)/2);
}

void M3D::TranslateBoxDragger::Update(const Vector3& newPos, int draggerIndex)
{
	Vector3 leftBottom = _leftBottomDragger->GetWorldPosition();
	Vector3 rightTop = _rightTopDragger->GetWorldPosition();

	//leftBottom
	if (draggerIndex == 0)
	{
		leftBottom = newPos;
	}
	//leftTop
	else if (draggerIndex == 1)
	{
		leftBottom = Vector3(newPos.m_x, leftBottom.m_y, leftBottom.m_z);
		rightTop = Vector3(rightTop.m_x, newPos.m_y, rightTop.m_z);
	}
	//rightTop
	else if (draggerIndex == 2)
	{
		rightTop = newPos;
	}
	//rightBottom
	else if (draggerIndex == 3)
	{
		leftBottom = Vector3(leftBottom.m_x, newPos.m_y, leftBottom.m_z);
		rightTop = Vector3(newPos.m_x, rightTop.m_y, rightTop.m_z);
	}
	//center
	else if (draggerIndex == 4)
	{
		Vector3 center = newPos;
		float width = rightTop.m_x - leftBottom.m_x;
		float height = rightTop.m_y - leftBottom.m_y;


		leftBottom =Vector3(center.m_x - width/2,center.m_y -height/2,leftBottom.m_z) ;
		rightTop = Vector3(center.m_x + width / 2, center.m_y + height / 2, leftBottom.m_z);
	}

	this->SetBoxSize(leftBottom, rightTop);
}

void  M3D::TranslateBoxDragger::GetBoxSize(Vector3& leftBottom, Vector3& rightTop)
{
	leftBottom = _leftBottomDragger->GetWorldPosition();
	rightTop = _rightTopDragger->GetWorldPosition();
}
 
