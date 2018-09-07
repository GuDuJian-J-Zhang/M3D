#ifndef M3D_THREE_D_GESTURES_NOTE_H_
#define M3D_THREE_D_GESTURES_NOTE_H_

#include "m3d/model/Note.h"
#include "m3d/model/PolyLine.h"
#include "m3d/M3D.h"
namespace M3D 
{
	struct Gesture3DInfo
	{
		vector<PolyLine> m_pointSet;
		vector<Color>m_colorSet;
		vector<int>m_lineWidthSet;
	};
	struct CommonNoteInfo
	{
		Vector3 m_start;
		Vector3 m_end;
		Vector2 m_imageSize;
		string m_imagePath;
	};
	class M3D_API ThreeDGesturesNote :public Note
	{
	public:
		ThreeDGesturesNote();
		void SetLine3DIndex(int start,int end);
		vector<int>& GetLine3DIndex();
		void SetOrigin3DPointIndex(vector<int>& indexs);
		vector<vector<int> >& GetOrigin3DPointIndex();

		vector<vector<Vector3> >& GetOriginalProjectPns();
		void SetOriginalProjectPns(vector<Vector3>& pnts);
        vector<Color>& GetOriginalProjectColors();
        void SetOriginalProjectColors(Color color);
        vector<int>& GetOriginalProjectWidths();
        void SetOriginalProjectWidths(int width);
        
        //对象是否显示空间点
        bool GetIsShowSpacePoint();
        void SetIsShowSpacePoint(bool isShow);
	private:
		void Init();
        bool m_isShowSpacePoint;
		vector<int> m_line3DIndex;
		vector<vector<int> > m_origin3DPointIndex;
		vector<vector<Vector3> > m_originalProjPnts;
		vector<Color> m_originalProjColors;
        vector<int> m_originalProjWidths;
	};

	class CommonNote :public Note
	{
	public:
		CommonNote();
		virtual ~CommonNote();
	private:
		void Init();
	};
}
#endif /*M3D_THREE_D_GESTURES_NOTE_H_*/
