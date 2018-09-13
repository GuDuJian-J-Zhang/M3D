

#ifndef M3DMANIPULATOR_TRANSLATEMINUSAXISDRAGGER
#define M3DMANIPULATOR_TRANSLATEMINUSAXISDRAGGER 1

#include "m3d/handler/Translate1DDragger.h"
#include "m3d/handler/Translate2DDragger.h"
#include "m3d/handler/dragger.h"

namespace M3D {

	/**
	* Dragger for performing translation in all three axes.
	*/
	class M3D_API TranslateMinusAxisDragger :public CompositeDragger
	{
	public:
		TranslateMinusAxisDragger();
		void setupDefaultGeometry();
		void SetXAxisVisible(bool val);

		void SetMinusXAxisVisible(bool val);

		void SetScene(M3D::SceneManager* val);
		//µ•÷·∑ΩœÚ
		void SetOrientation(int orientation);
		void SetOrientation(Vector3 dir);
		bool handle(const TouchEvent& ea);
	protected:
		virtual ~TranslateMinusAxisDragger();
		Translate1DDragger* _xDragger;
		Translate1DDragger* _nxDragger;
	};
}

#endif
