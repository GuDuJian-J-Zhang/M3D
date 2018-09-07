#include "m3d/extras/note/ThreeDGesturesNote.h"
namespace M3D
{
	ThreeDGesturesNote::ThreeDGesturesNote()
	{
		Init();
	}
	void ThreeDGesturesNote::Init()
	{
		this->SetType(SHAPETYPE::SHAPE_THREED_GESTURE_NOTE);
	}

	void ThreeDGesturesNote::SetLine3DIndex(int start,int end)
	{
		m_line3DIndex.push_back(start);
		m_line3DIndex.push_back(end);

	}
	vector<int>& ThreeDGesturesNote::GetLine3DIndex()
	{
		return this->m_line3DIndex;
	}

	void ThreeDGesturesNote::SetOrigin3DPointIndex(vector<int> &indexs)
	{
		m_origin3DPointIndex.push_back(indexs);
	}
	vector<vector<int> >& ThreeDGesturesNote::GetOrigin3DPointIndex()
	{
		return m_origin3DPointIndex;
	}

vector<vector<Vector3> >& ThreeDGesturesNote::GetOriginalProjectPns()
{
	return this->m_originalProjPnts;
}
void ThreeDGesturesNote::SetOriginalProjectPns(vector<Vector3>& pnts)
{
	this->m_originalProjPnts.push_back(pnts);
}
vector<Color>& ThreeDGesturesNote::GetOriginalProjectColors(){
    return this->m_originalProjColors;
}
    void ThreeDGesturesNote::SetOriginalProjectColors(Color color){
        this->m_originalProjColors.push_back(color);
    }
    vector<int>& ThreeDGesturesNote::GetOriginalProjectWidths(){
        return this->m_originalProjWidths;
    }
    void ThreeDGesturesNote::SetOriginalProjectWidths(int width){
        this->m_originalProjWidths.push_back(width);
    }
    //对象是否显示空间点
bool ThreeDGesturesNote::GetIsShowSpacePoint(){
    return m_isShowSpacePoint;
}
void ThreeDGesturesNote::SetIsShowSpacePoint(bool isShow){
    m_isShowSpacePoint = isShow;
}
CommonNote::CommonNote():Note()
{
	Init();
}

CommonNote::~CommonNote()
{

}

void CommonNote::Init()
{
	this->SetType(SHAPETYPE::SHAPE_COMMON_NOTE);
}

}
