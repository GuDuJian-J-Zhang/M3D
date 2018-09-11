

#ifndef M3DMANIPULATOR_TRANSLATE_BOX_DRAGGER
#define M3DMANIPULATOR_TRANSLATE_BOX_DRAGGER 1

#include "m3d/handler/Translate1DDragger.h"
#include "m3d/handler/Translate2DDragger.h"
#include "m3d/handler/Translate3DDragger.h"
#include "m3d/handler/dragger.h"

namespace M3D {

class M3D_API BoxDraggerCallback :public DraggerCallback
	{
	public:
		BoxDraggerCallback();
		BoxDraggerCallback(Dragger* dragger);
		virtual ~BoxDraggerCallback();
 
		virtual bool receive(const MotionCommand&);
		virtual bool receive(const TranslateInLineCommand& command);
		virtual bool receive(const TranslateInPlaneCommand& command);
 
		M3D::Dragger* GetBoxDragger() const { return m_boxDragger; }
		void SetBoxDragger(M3D::Dragger* val) { m_boxDragger = val; }

		M3D::Dragger* GetDragger() const { return m_dragger; }
		void SetDragger(M3D::Dragger* val) { m_dragger = val; }
	private:
		Dragger* m_dragger;
		Dragger* m_boxDragger;
		Vector3 m_startPos;
};
 
/**
 * Dragger for performing translation in all three axes.
 */
class M3D_API TranslateBoxDragger:public CompositeDragger
{
    public:
		TranslateBoxDragger();
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();
 
		void SetScene(M3D::SceneManager* val);

		//通过Dragger来更新位置
		void Update(const Vector3& newPos,int draggerIndex);

		void SetBoxSize(const Vector3& leftBottom,const Vector3& rightTop);
 
		void GetBoxSize(Vector3& leftBottom, Vector3& rightTop);
 
    protected:
        virtual ~TranslateBoxDragger();
		Translate2DDragger* _leftTopDragger;
		Translate2DDragger* _leftBottomDragger;
		Translate2DDragger*  _rightTopDragger;
		Translate2DDragger*  _rightBottomDragger;
		Translate2DDragger*  _centerDragger;
};


}

#endif
