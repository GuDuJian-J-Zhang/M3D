#include "PerspectiveData.h"
#include "m3d/utils/Trackball.h"

namespace M3D
{
const float PerspectiveData::DISTRI = 0.0000;

const ControlInfo PerspectiveData::FRONT_MVMATRIX = ControlInfo(1.0,0.0, 0.0, 0.0);
const ControlInfo PerspectiveData::BACK_MVMATRIX = ControlInfo(0.0, 0.0, 1.0, 0.0);

const ControlInfo PerspectiveData::LEFT_MVMATRIX = ControlInfo(0.70710677,0, -0.70710677, 0);
const ControlInfo PerspectiveData::RIGHT_MVMATRIX = ControlInfo(0.70710677,0, 0.70710677, 0);

const ControlInfo PerspectiveData::TOP_MVMATRIX = ControlInfo(0.70710677 ,-0.70710677, 0, 0);
const ControlInfo PerspectiveData::BOTTOM_MVMATRIX = ControlInfo(0.70710677,0.70710677, 0, 0);

const ControlInfo PerspectiveData::ISOMETRIC_MVMATRIX = ControlInfo(
	0.88047838, 0.36470398, -0.27984396, -0.11591448);

const ControlInfo PerspectiveData::DEFAULT_MVMATRIX = ControlInfo(
	0.88047838, 0.36470398, -0.27984396, -0.11591448);
}
