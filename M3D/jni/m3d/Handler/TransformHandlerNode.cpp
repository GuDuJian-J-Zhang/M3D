#include "m3d/Handler/TransformHandlerNode.h"
#include "m3d/Handler/Handler.h"
#include "m3d/Handler/Rotate1DHandler.h"
#include "m3d/Handler/Scale1DHandler.h"
#include "m3d/Handler/TransformHandlerNode.h"
#include "m3d/Handler/Translate1DHandler.h"
#include "m3d/scene/ShapeNode.h"
namespace M3D
{
TransformHandlerNode::TransformHandlerNode() :
		HandlerNode()
{
	Init();
}

TransformHandlerNode::~TransformHandlerNode()
{
	if(m_xTran)
	{
		delete m_xTran;
		m_xTran = NULL;
	}

	if(m_yTran)
	{
		delete m_yTran;
		m_yTran = NULL;
	}

	if(m_xTran)
	{
		delete m_xTran;
		m_xTran = NULL;
	}

	if(m_xRotate)
	{
		delete m_xRotate;
		m_xRotate = NULL;
	}

	if(m_yRotate)
	{
		delete m_yRotate;
		m_yRotate = NULL;
	}

	if(m_zRotate)
	{
		delete m_zRotate;
		m_zRotate = NULL;
	}

	if(m_xScale)
	{
		delete m_xScale;
		m_xScale = NULL;
	}

	if(m_yScale)
	{
		delete m_yScale;
		m_yScale = NULL;
	}

	if(m_zScale)
	{
		delete m_zScale;
		m_zScale = NULL;
	}
}

void TransformHandlerNode::Init()
{
	m_xTran = NULL;
	m_yTran = NULL;
	m_zTran = NULL;

	m_xRotate = NULL;
	m_yRotate = NULL;
	m_zRotate = NULL;

	m_xScale = NULL;
	m_yScale = NULL;
	m_zScale = NULL;
 
	m_xTran = new Translate1DHandler();
	m_yTran = new Translate1DHandler();
	m_zTran = new Translate1DHandler();

	m_xTran->SetColor(Color::RED);
	m_yTran->SetColor(Color::GREEN);
	m_zTran->SetColor(Color::BLUE);

	m_xRotate = new Rotate1DHandler();
	m_yRotate = new Rotate1DHandler();
	m_zRotate = new Rotate1DHandler();
	m_xRotate->SetColor(Color::RED);
	m_yRotate->SetColor(Color::GREEN);
	m_zRotate->SetColor(Color::BLUE);

	m_xScale = new Scale1DHandler();
	m_yScale = new Scale1DHandler();
	m_zScale = new Scale1DHandler();
	m_xScale->SetColor(Color::RED);
	m_yScale->SetColor(Color::GREEN);
	m_zScale->SetColor(Color::BLUE);

	m_xTran->SetName(X_TRAN_HANDLER);
	m_yTran->SetName(Y_TRAN_HANDLER);
	m_zTran->SetName(Z_TRAN_HANDLER);

	m_xRotate->SetName(X_ROTATE_HANDLER);
	m_yRotate->SetName(Y_ROTATE_HANDLER);
	m_zRotate->SetName(Z_ROTATE_HANDLER);

	m_xScale->SetName(X_SCALE_HANDLER);
	m_yScale->SetName(Y_SCALE_HANDLER);
	m_zScale->SetName(Z_SCALE_HANDLER);

	ShapeNode* xTranNode =  new ShapeNode();
	xTranNode->SetShape(m_xTran);
    Quaternion rotationX(0,Vector3(1,0,0));
    xTranNode->SetRotation(rotationX);
	this->AddChild(xTranNode);

	ShapeNode* yTranNode =  new ShapeNode();
	yTranNode->SetShape(m_yTran);
    Quaternion rotationY(90,Vector3(0,1,0));
     yTranNode->SetRotation(rotationY);
	this->AddChild(yTranNode);

	ShapeNode* zTranNode =  new ShapeNode();
	zTranNode->SetShape(m_xTran);
    Quaternion rotationZ(90,Vector3(1,0,0));
    zTranNode->SetRotation(rotationZ);
	this->AddChild(zTranNode);

	ShapeNode* xRotateNode =  new ShapeNode();
	xRotateNode->SetShape(m_xRotate);
	this->AddChild(xRotateNode);

	ShapeNode* yRotateNode =  new ShapeNode();
	yRotateNode->SetShape(m_yRotate);
	this->AddChild(yRotateNode);

	ShapeNode* zRotateNode =  new ShapeNode();
	zRotateNode->SetShape(m_zRotate);
	this->AddChild(zRotateNode);

	ShapeNode* xScaleNode =  new ShapeNode();
	xScaleNode->SetShape(m_xScale);
	this->AddChild(xScaleNode);

	ShapeNode* yScaleNode =  new ShapeNode();
	yScaleNode->SetShape(m_yScale);
	this->AddChild(yScaleNode);

	ShapeNode* zScaleNode =  new ShapeNode();
	zScaleNode->SetShape(m_zScale);
	this->AddChild(zScaleNode);
    
}

}

