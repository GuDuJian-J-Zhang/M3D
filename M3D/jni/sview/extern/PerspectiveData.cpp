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

const ControlInfo PerspectiveData::BACKISOME_MVMATRIX = ControlInfo(
	0.27984396, -0.11591448, 0.88047838, -0.36470398);
//上前左
const ControlInfo PerspectiveData::UPFRONTLEFT_MVMATRIX = ControlInfo(
	-0.88047838, -0.36470398, -0.27984396, -0.11591448);
//上后右
const ControlInfo PerspectiveData::UPBEHINDRIGHT_MVMATRIX = ControlInfo(
	0.27984396, -0.11591448, -0.88047838, -0.36470398);

//上后左
const ControlInfo PerspectiveData::UPBEHINDLEFT_MVMATRIX = ControlInfo(
	0.27984396, -0.11591448, 0.88047838, 0.36470398);
//下前右
const ControlInfo PerspectiveData::DOWNFRONTRIGHT_MVMATRIX = ControlInfo(
	0.88047838, -0.36470398, -0.27984396, -0.11591448);

//下前左
const ControlInfo PerspectiveData::DOWNFRONTLEFT_MVMATRIX = ControlInfo(
	-0.88047838, 0.36470398, -0.27984396, -0.11591448);

//下后右
const ControlInfo PerspectiveData::DOWNBEHINDRIGHT_MVMATRIX = ControlInfo(
	0.27984396, -0.11591448, -0.88047838, 0.36470398);

//上后
const ControlInfo PerspectiveData::UPBEHIND_MVMATRIX = ControlInfo(
	0.3826828, 0.923879743, 0, 0);
//上前
const ControlInfo PerspectiveData::UPFRONT_MVMATRIX = ControlInfo(
	0.9238793, 0.382683963, 0, 0);
//上左
const ControlInfo PerspectiveData::UPLEFT_MVMATRIX = ControlInfo(
	0.6532822, 0.65327996, 0.270598948, -0.270598173);
//上右
const ControlInfo PerspectiveData::UPRIGHT_MVMATRIX = ControlInfo(
	0.6532821, 0.6532804, -0.270597577, 0.270598382);

//下后
const ControlInfo PerspectiveData::DOWNBEHIND_MVMATRIX = ControlInfo(
	-0.382681966, 0.923880041, 0, 0);
//下前
const ControlInfo PerspectiveData::DOWNFRONT_MVMATRIX = ControlInfo(
	0.9238796, -0.3826833, 0, 0);
//下左
const ControlInfo PerspectiveData::DOWNLEFT_MVMATRIX = ControlInfo(
	0.65328145, -0.6532815, 0.270598054, 0.270598084);
//下右
const ControlInfo PerspectiveData::DOWNRIGHT_MVMATRIX = ControlInfo(
	0.653280854, -0.6532829, -0.2705978, -0.2705978);

//前左
const ControlInfo PerspectiveData::FRONTLEFT_MVMATRIX = ControlInfo(
	0.9238795, 0.0, 0.382683456, 0.0);
//前右
const ControlInfo PerspectiveData::RONTRIGHT_MVMATRIX = ControlInfo(
	0.9238795, 0.0, -0.382683456, 0.0);
//后左
const ControlInfo PerspectiveData::BEHINDLEFT_MVMATRIX = ControlInfo(
	0.382683426, 0.0, 0.9238795, 0.0);
//后右
const ControlInfo PerspectiveData::BEHINDRIGHT_MVMATRIX = ControlInfo(
	0.382683426, 0.0, -0.9238795, 0.0);

const ControlInfo PerspectiveData::DEFAULT_MVMATRIX = ControlInfo(
	0.88047838, 0.36470398, -0.27984396, -0.11591448);
}
