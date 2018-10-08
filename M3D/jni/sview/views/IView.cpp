/**@file
 *@brief	IView接口类cpp文件
 *@author
 *@date
 *@version	1.0
 *
 *提供IView接口类方法的实现
 *
 */

#include "IView.h"


#include "m3d/model/Shape.h"

namespace SVIEW {

int IView::newID = 0;
IView::IView() {
	Initial();
}

IView::~IView() {
	newID = 0;
}
void IView::OnDraw() {

}

void IView::RequestDraw() {

}

void IView::Initial() {
	newID =(long)this;
}

void IView::TouchDown(float* p, int n) {

}

void IView::TouchMove(float* p, int n) {

}

void IView::TouchUp(float* p, int n) {

}

void IView::WindowSizeChange(int width, int height) {

}

M3D_STATUS  IView::ReadFile(string & path) {
	return M3D_STATUS::M_ERROR;
}
void IView::CloseFile()
{

}

bool IView::SetBackgroundColor(float* topColor,float* bottomColor,int ColorNum)
{
   return false;
}
    
bool IView::SetBackgroundColor(const Color& topColor,const Color& bottomColor)
{
    return false;
}

bool IView::GetBackgroundColor(Color& topColor,Color& bottomColor)
{
    return false;
}

bool IView::AnimationExecute(float percentage)
{
	return false;
}

bool IView:: AnimationPlayCamera(bool isPlayCamera)
{
	return false;
}

bool IView::AnimationPlaySpeed(float speed)
{

	return false;
}

bool IView::AnimationSetActiveStep(int processId,int stepId)
{
	return false;
}

bool IView:: AnimationGetPlayCamera()
{
	return false;
}

bool IView::AnimationIsPlaying()
{
	return false;
}

bool IView::AnimationTransitionIsPlaying()
{
	return false;
}

float IView::AnimationGetTick()
{
    return 0.0f;
}

bool IView::AnimationSetTick(float percent)
{
	return false;
}

bool IView::AnimationRewind()
{
	return false;
}

bool IView::AnimationPlay()
{
	return false;
}

bool IView::AnimationContinuousPlay()
{
	return false;
}

bool IView::AnimationOpen( const string& file )
{
	return false;
}

string IView::GetDataManagerVersion()
{
	return "ERROR";
}

Shape* IView::GetShapeBySVLPath(const string& path)
{
	return NULL;
}
    
Shape* IView::GetShape(int shapeId)
{
    return NULL;
}

bool IView::SetExplosiveView(int stype,int pos,bool useAnimation)
{
	return false;
}

bool IView::CloseExplisiveView()
{
	return false;
}

bool IView::CloseSceneAnimation()
{
	return false;
}
 
}

