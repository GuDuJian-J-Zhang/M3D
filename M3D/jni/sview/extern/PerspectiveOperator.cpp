#include <stdlib.h>

#include "Utility.h"

#include "sview/extern/PerspectiveOperator.h"
#include "m3d/scene/SceneNode.h"
#include "sview/extern/WorkNodes.h"
#include "m3d/SceneManager.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/views/View.h"

#include "m3d/utils/Trackball.h"
#include "m3d/utils/M3DTools.h"

namespace M3D
{
    void PerspectiveOperator::Show(SVIEW::View* pView, int viewType,
                                   bool allowRotate, bool allowTran, bool allowScale)
    {
       Apply(pView, viewType, allowRotate, allowTran, allowScale);
    }
    
	void PerspectiveOperator::Show(SVIEW::View* pView, const ControlInfo& cameraMatrix,
			bool allowRotate , bool allowTran , bool allowScale,int keepMsecond)
	{
		 Apply(pView, cameraMatrix, allowRotate, allowTran, allowScale, keepMsecond);
	}

    PerspectiveOperator::PerspectiveOperator()
    {
        Initialize();
    }
    
    void PerspectiveOperator::Show(SVIEW::View* pView, float* pMatrix,
                                   bool allowRotate, bool allowTran, bool allowScale)
    {

    }
    
    PerspectiveOperator::PerspectiveOperator(SVIEW::View* pView, int viewType,
                                             bool allowRotate, bool allowTran, bool allowScale)
    {
        Initialize();
    }
    void PerspectiveOperator::Apply(SVIEW::View* pView, int viewType,
                                    bool allowRotate, bool allowTran, bool allowScale)
    {
        this->m_pView = pView;
        this->m_pSelectedNodes = this->m_pView->GetWorkNodes();
        this->m_iViewType = viewType;
        
        this->m_bAllowRotate = allowRotate;
        this->m_bAllowScale = allowScale;
        this->m_bAllowTran = allowTran;

        HandleStandardView();
    }
    
    void  PerspectiveOperator::Apply(SVIEW::View* pView, float* pMatrix, bool allowRotate,
    		bool allowTran , bool allowScale )
    {
    	if(pMatrix != NULL){
    		memcpy(m_pMatrix,pMatrix,16*sizeof(float));
    	}

        this->m_pView = pView;
        this->m_pSelectedNodes = this->m_pView->GetWorkNodes();

        this->m_bAllowRotate = allowRotate;
        this->m_bAllowScale = allowScale;
        this->m_bAllowTran = allowTran;

        ControlInfo camereMatrix;

        CreatePerspectiveAnimation(camereMatrix,
        		m_bAllowRotate, m_bAllowTran, m_bAllowScale);

    }

    void PerspectiveOperator::Apply(SVIEW::View* pView, const ControlInfo& camereMatrix, bool allowRotate,
    		bool allowTran, bool allowScale,int keepMsecond)
    {
    	this->m_animationKeepMsecond = keepMsecond;

        this->m_bAllowRotate = allowRotate;
        this->m_bAllowScale = allowScale;
        this->m_bAllowTran = allowTran;

    	CreatePerspectiveAnimation(camereMatrix,
    	        		m_bAllowRotate, m_bAllowTran, m_bAllowScale);
    }

    PerspectiveOperator::PerspectiveOperator(SVIEW::View* pView, float* pMatrix,
                                             bool allowRotate, bool allowTran, bool allowScale)
    {

    }
    
    PerspectiveOperator::~PerspectiveOperator()
    {
        if(this->m_animationTo != NULL)
        {
            delete  this->m_animationTo;
            this->m_animationTo = NULL;
        }
        if(this->m_animationmvMatrix != NULL)
        {
            delete  this->m_animationmvMatrix;
            this->m_animationmvMatrix = NULL;
        }
        if(this->m_animationdistimvMatrix != NULL)
        {
            delete  this->m_animationdistimvMatrix;
            this->m_animationdistimvMatrix = NULL;
        }

        if(m_midRotation != NULL)
        {
        	delete[] this->m_midRotation;
        }

        if(m_midScale != NULL)
        {
        	delete[] this->m_midScale;
        }

        if(m_midTran != NULL)
        {
        	delete[] this->m_midTran;
        }
    }
    
    void PerspectiveOperator::HandleStandardView()
    {
        switch (m_iViewType)
        {
            case PerspectiveData::FRONT:
                CreatePerspectiveAnimation(PerspectiveData::FRONT_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
            case PerspectiveData::BACK:
                CreatePerspectiveAnimation(PerspectiveData::BACK_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
                
            case PerspectiveData::LEFT:
                CreatePerspectiveAnimation(PerspectiveData::LEFT_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
            case PerspectiveData::RIGHT:
                CreatePerspectiveAnimation(PerspectiveData::RIGHT_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
                
            case PerspectiveData::TOP:
                CreatePerspectiveAnimation(PerspectiveData::TOP_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
            case PerspectiveData::BOTTOM:
                CreatePerspectiveAnimation(PerspectiveData::BOTTOM_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
                
            case PerspectiveData::ISOMETRIC:
                CreatePerspectiveAnimation(PerspectiveData::ISOMETRIC_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
            case PerspectiveData::DEFAULT:
                CreatePerspectiveAnimation(PerspectiveData::DEFAULT_MVMATRIX,
                                           m_bAllowRotate, m_bAllowTran, m_bAllowScale);
                break;
            default:
                break;
        }
    }
    
    void PerspectiveOperator::CreatePerspectiveAnimation(const ControlInfo & to,
                                                         bool allowRotate, bool allowTran, bool allowScale)
    {
        //正在进行动画
        if(GetAnimationState()){
            //结束当前动画
            FinishAnimation();
        }
        //开始新动画
        m_currentAniFrame = 0;
        *m_animationTo = to;
        Trackball::ISMOVING = true;
        m_pSelectedNodes->GetMVMatrix(*m_animationmvMatrix, *m_animationdistimvMatrix);
        m_animationdistimvMatrix->rotation = to.rotation;
        
        StartAnimation();
 
        Trackball::ISMOVING = false;
    }

	void PerspectiveOperator::ExecuteCommonCameraAnimation(SVIEW::View* view,const Vector3& disPosition,
		const Quaternion& disRotation, const Vector3& disScale, bool allowTran,
		bool allowRotate, bool allowScale)
	{
		//正在进行动画
		if (GetAnimationState()) {
			//结束当前动画
			FinishAnimation();
		}

		//开始新动画
		m_currentAniFrame = 0;

		this->m_bAllowRotate = allowRotate;
		this->m_bAllowScale = allowScale;
		this->m_bAllowTran = allowTran;

		CameraNode* camera = view->GetSceneManager()->GetCamera();

		//得到摄像机当前状态
		this->m_animationmvMatrix->moveVector = camera->GetWorldPosition();
		this->m_animationmvMatrix->rotation = camera->GetWorldRotation();
		this->m_animationmvMatrix->scaleFactor = camera->GetZoom();

		this->m_animationTo->moveVector = disPosition;
		this->m_animationTo->rotation = disRotation;
		this->m_animationTo->scaleFactor = disScale.m_x;

		Trackball::ISMOVING = true;

		StartCommonAnimation();

		Trackball::ISMOVING = false;
	}
    
    void PerspectiveOperator::createMVMatrixAnimation(
                                                      const ControlInfo& mvMatrix, const ControlInfo&to,
                                                      bool allowRotate, bool allowTran, bool allowScale)
    {
        // LOGI("CreateRotationAnimation   Begin");
        int slerpCount = 20;
        Quaternion* midRotation = new Quaternion[slerpCount];
        M3DTOOLS::RotationSlerp(mvMatrix.rotation, to.rotation, slerpCount, midRotation);
        
        float* midScale = new float[slerpCount];
        M3DTOOLS::FloatSlerp(mvMatrix.scaleFactor, to.scaleFactor, slerpCount, midScale);
        Vector3* midTran = new Vector3[slerpCount];
        M3DTOOLS::VecSlerp(mvMatrix.moveVector, to.moveVector, slerpCount, midTran);
        
        for (int i = 0; i < slerpCount; i++)
        {
            if (allowRotate)
            {
                ApplyRotationToSelectedNode(midRotation[i]);
            }
            if (allowScale)
            {
                ApplyScaleToSelectedNode(midScale[i]);
            }
            if (allowTran)
            {
                ApplyMovementToSelectedNode(midTran[i]);
            }
            
            // this->pSceneManager-> UpdataSceneBox();
            this->m_pView->RequestDraw();
#ifdef __MOBILE__
            usleep(MILLISECONDS * 30);
#endif
        }
        //	LOGI("CreateRotationAnimation");
        delete[] midRotation;
    }
    
    void PerspectiveOperator::CreateRotationAnimation(const Quaternion& from,
                                                      const Quaternion& to)
    {
        // LOGI("CreateRotationAnimation   Begin");
        //		int slerpCount = 20;// ���������?
        //
        //		//ROTATION* midRotation=RotationSlerp( from, to,slerpCount，);
        //
        //		for (int i=0;i<slerpCount;i++)
        //		{
        //			ApplyRotationToSelectedNode(midRotation[i]);
        //			 pSceneManager->RequestRedraw();
        //			usleep(MILLISECONDS* 30);
        //		}
        //	//	LOGI("CreateRotationAnimation");
        //		delete [] midRotation;
    }
    
    void PerspectiveOperator::ApplyRotationToSelectedNode(const Quaternion& rotation)
    {
        m_pSelectedNodes->SetRotationAroundCenter(rotation);
    }
    
    void PerspectiveOperator::ApplyMovementToSelectedNode(const Vector3& mov)
    {
        m_pSelectedNodes->SetPosition(mov);
    }
    
    void PerspectiveOperator::ApplyScaleToSelectedNode(float & scale)
    {
        m_pSelectedNodes->SetZoom(1/scale);
        
    }
    
    void PerspectiveOperator::SetAnimationState(bool animationState)
    {
        this->m_isAnimation = animationState;
    }
    
    
    bool PerspectiveOperator::GetAnimationState()
    {
        return  m_timer.IsStart();
    }
    
    //开始线程动画
    bool PerspectiveOperator::StartAnimation()
    {
        ControlInfo mvMatrix = *(this->m_animationmvMatrix);
        ControlInfo to = *(this->m_animationTo);

        M3DTOOLS::RotationSlerp(mvMatrix.rotation, to.rotation, m_slerpCount, m_midRotation);
        M3DTOOLS::FloatSlerp(mvMatrix.scaleFactor, to.scaleFactor, m_slerpCount, m_midScale);
        M3DTOOLS::VecSlerp(mvMatrix.moveVector, to.moveVector, m_slerpCount, m_midTran);

    	m_timer.SetTimer(timerTask, this, 0, m_animationKeepMsecond);
    	m_timer.StartTimer();

        return true;
    }

	bool PerspectiveOperator::StartCommonAnimation()
	{
		ControlInfo mvMatrix = *(this->m_animationmvMatrix);
		ControlInfo to = *(this->m_animationTo);

		M3DTOOLS::RotationSlerp(mvMatrix.rotation, to.rotation, m_slerpCount, m_midRotation);
		M3DTOOLS::FloatSlerp(mvMatrix.scaleFactor, to.scaleFactor, m_slerpCount, m_midScale);
		M3DTOOLS::VecSlerp(mvMatrix.moveVector, to.moveVector, m_slerpCount, m_midTran);

		m_timer.SetTimer(commonTimerTask, this, 0, m_animationKeepMsecond);
		m_timer.StartTimer();

		return true;
	}

    //结束线程动画
    bool PerspectiveOperator::FinishAnimation()
    {
    	if( m_timer.IsStart())
    	{
        m_timer.StopTimer();
    	 if(this->animationFinishCallback)
    	 {
    		this->animationFinishCallback(this->animationFinishCallbackData);
    		this->SetAnimationFinishCallback(NULL,NULL);
    	}

    	}
    	m_currentAniFrame = 0;
    	m_animationKeepMsecond = DEFAULIT_TIMES;
        return true;
    }
    
    void PerspectiveOperator::SetAnimationFinishCallback(AnimationFinishCallBack callbackfun , void* data)
    {
        this->animationFinishCallback = callbackfun;
        this->animationFinishCallbackData = data;
    }
    
    void* PerspectiveOperator::ThreadAnimationFun(void* arg)
    {
        
//        PerspectiveOperator* perspectiveOperator = (PerspectiveOperator*)arg;
//
//        perspectiveOperator->SetAnimationState(true);
//
//
//
//        for (int i = 0; i < m_slerpCount; i++)
//        {
//            if(perspectiveOperator->m_isFnishAnimation)
//            {
//                break;
//            }
//
//            usleep(MILLISECONDS * 30);
//
//            if (allowRotate)
//            {
//                perspectiveOperator->ApplyRotationToSelectedNode(midRotation[i]);
//            }
//            if (allowScale)
//            {
//                perspectiveOperator->ApplyScaleToSelectedNode(midScale[i]);
//            }
//            if (allowTran)
//            {
//                perspectiveOperator->ApplyMovementToSelectedNode(midTran[i]);
//            }
//
//           perspectiveOperator->m_pView->RequestDraw();
//
//        }
//
//        perspectiveOperator->m_isFnishAnimation = false;
//        //	LOGI("CreateRotationAnimation");
//        delete[] midRotation;
//
//        if(perspectiveOperator->animationFinishCallback)
//        {
//            perspectiveOperator->animationFinishCallback(perspectiveOperator->animationFinishCallbackData);
//            perspectiveOperator->SetAnimationFinishCallback(NULL,NULL);
//        }
//
//        perspectiveOperator->SetAnimationState(false);
        return NULL;
    }

	void* PerspectiveOperator::commonTimerTask(void* arg)
	{
		PerspectiveOperator* perspectiveOperator = (PerspectiveOperator*)arg;
		SVIEW::View* view = perspectiveOperator->m_pView;
		bool allowRotate = perspectiveOperator->m_bAllowRotate;
		bool allowTran = perspectiveOperator->m_bAllowTran;
		bool allowScale = perspectiveOperator->m_bAllowScale;

		Quaternion* midRotation = perspectiveOperator->m_midRotation;
		float* midScale = perspectiveOperator->m_midScale;
		Vector3* midTran = perspectiveOperator->m_midTran;

		int currentFram = perspectiveOperator->m_currentAniFrame++;

		CameraNode* camera = view->GetSceneManager()->GetCamera();

		if (allowRotate)
		{
			camera->SetRotation(midRotation[currentFram]);
		}
		if (allowScale)
		{
			camera->SetZoom(midScale[currentFram]);
		}
		if (allowTran)
		{
			camera->SetWorldPosition(midTran[currentFram]);
		}

		perspectiveOperator->m_pView->RequestDraw();

		if (currentFram >= perspectiveOperator->m_slerpCount - 1) {
			perspectiveOperator->m_pView->RequestDelayDraw();
			perspectiveOperator->FinishAnimation();
		}

		return NULL;
	}
    
    void* PerspectiveOperator::timerTask(void* arg)
    {
        PerspectiveOperator* perspectiveOperator = (PerspectiveOperator*)arg;

        bool allowRotate = perspectiveOperator->m_bAllowRotate;
        bool allowTran = perspectiveOperator->m_bAllowTran;
        bool allowScale = perspectiveOperator->m_bAllowScale;

        Quaternion* midRotation = perspectiveOperator->m_midRotation;
        float* midScale = perspectiveOperator->m_midScale;
        Vector3* midTran = perspectiveOperator->m_midTran;

        int currentFram = perspectiveOperator->m_currentAniFrame++;

        if (allowRotate)
        {
            perspectiveOperator->ApplyRotationToSelectedNode(midRotation[currentFram]);
        }
        if (allowScale)
        {
            perspectiveOperator->ApplyScaleToSelectedNode(midScale[currentFram]);
        }
        if (allowTran)
        {
            perspectiveOperator->ApplyMovementToSelectedNode(midTran[currentFram]);
        }

       perspectiveOperator->m_pView->RequestDraw();

       if(currentFram >= perspectiveOperator->m_slerpCount-1){
    	   perspectiveOperator->m_pView->RequestDelayDraw();
    	   perspectiveOperator->FinishAnimation();
       }

       return NULL;
    }

    void PerspectiveOperator::Initialize()
    {
        this->SetAnimationState(false);
        
        this->m_animationTo = new ControlInfo();
        this-> m_animationmvMatrix = new ControlInfo();
        this->m_animationdistimvMatrix = new ControlInfo();
        
        this->m_slerpCount = 10;
        m_midRotation = new Quaternion[this->m_slerpCount];
        m_midScale = new float[this->m_slerpCount];
        m_midTran = new Vector3[this->m_slerpCount];

        memset(m_pMatrix,0,16*sizeof(float));

        animationFinishCallback = NULL;
        animationFinishCallbackData = NULL;

    	m_animationKeepMsecond = DEFAULIT_TIMES;
    }
    
}

