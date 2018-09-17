#include "m3d/base/glue/GlueObj.h"
#include "m3d/M3D.h"
#include "m3d/SceneManager.h"
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/IDCreator.h"

using SVIEW::Parameters;

namespace M3D
{

GlueObj::GlueObj()
{
	this->Init();
}

void GlueObj::Init()
{
	m_env = 0;
	m_obj = 0;
    createImageFun = 0;
	DragModelCBFun = 0;
	measureTextFun = 0;
	refreshCB = NULL;
}

GlueEnv* GlueObj::GetGlueEnv()
{
	return m_env;
}

void GlueObj::SetGlueEnv(GlueEnv * env)
{
	this->m_env = env;
}

Glueobject GlueObj::GetGluObject()
{
	return m_obj;
}

void GlueObj::SetGluObject(Glueobject obj)
{
	m_obj = obj;
}
void GlueObj::SetCreateImageFun(CREATEIMAGE fun)
{
    createImageFun = fun;
}
    
CREATEIMAGE GlueObj::GetCreateImageFun()
{
    return createImageFun;
}
    
void GlueObj::SetDragModelCBFun(DRAGMODELCB fun)
{
	DragModelCBFun = fun;
}

DRAGMODELCB GlueObj::GetDragModelCBeFun()
{
	return DragModelCBFun;
}

void GlueObj::RequestUILayerDrawCB(int refreshTime /*= 0*/)
{
	if (refreshCB != NULL)
	{
		refreshCB(refreshTime);
	}
}

RefreshCB GlueObj::GetRefreshCB() const
{
	return refreshCB;
}

string GlueObj::createImage(Shape2DSet* shapeSet, SceneManager* scene)
{
	string imagePath = "";
#ifdef __ANDROID__
	//生成json串
	string shapesetValue = shapeSet->toJson();
	//在java端生成图片,返回图片地址
	JNIEnv * env = this->m_env;
	//构造java端的shape2DSet
	jclass clazz_jShape2dSet = env->FindClass("ht/svbase/model2d/Shape2DSet");
	jmethodID construct = env->GetMethodID(clazz_jShape2dSet, "<init>", "()V");
	if (construct == NULL) {
		LOGE("Shape2DSet method construct ID not found");
		return imagePath;
	}
	jobject jShap2dSet = env->NewObject(clazz_jShape2dSet,construct);

	jmethodID fromString = env->GetMethodID(clazz_jShape2dSet, "fromString","(Ljava/lang/String;)Z");
	if (fromString == NULL) {
		LOGE("Shape2DSet method fromString ID not found");
		return imagePath;
	} else {
		//构造c风格字符串
		jstring str= env->NewStringUTF(shapesetValue.c_str());
		env->CallBooleanMethod(jShap2dSet, fromString, str);
		env->DeleteLocalRef(str);
	}
	env->DeleteLocalRef(clazz_jShape2dSet);
	string cUUID = "";
	//java端生成UUID string
	jclass clazz_jUUIDGenerator = env->FindClass("ht/svbase/util/UUIDGenerator");
	jmethodID getUUID = env->GetStaticMethodID(clazz_jUUIDGenerator, "getUUID", "()Ljava/lang/String;");
	if (getUUID == NULL) {
		LOGE(" UUIDGenerator getUUID ID not found");
		return imagePath;
	} else {
		jstring  dataString = (jstring)env->CallStaticObjectMethod(clazz_jUUIDGenerator,getUUID);
		cUUID = JStrToStr(env, dataString);
		env->DeleteLocalRef(dataString);
		env->DeleteLocalRef(clazz_jUUIDGenerator);
	}
	//通过java端的CanvasHelper来绘制图片，到指定位置中
	//构造图片路径
	string path = Parameters::Instance()->m_appWorkPath + Parameters::Instance()->m_measureImageTempPath + cUUID +".png";
	//构造java路径
	jstring jPath = env->NewStringUTF(path.c_str());
	//java端生成图片
	jclass clazz_jCanvasHelper = env->FindClass("ht/svbase/model2d/CanvasHelper");
	jmethodID createImage = env->GetStaticMethodID(clazz_jCanvasHelper, "createImage", "(Lht/svbase/model2d/Shape2DSet;Ljava/lang/String;)Z");
	if (createImage == NULL) {
		LOGE(" CanvasHelper createImage ID not found");
		return imagePath;
	} else {
		jboolean createState = env->CallStaticBooleanMethod(clazz_jCanvasHelper,createImage,
				jShap2dSet,jPath);
		if(createState)
		{
			imagePath = path;
		}
		env->DeleteLocalRef(clazz_jCanvasHelper);
	}
	env->DeleteLocalRef(jShap2dSet);
	env->DeleteLocalRef(jPath);

#endif
#ifdef __IOS__
	 if(createImageFun){
	        string cUUID = IDCreator::GetUUID();
	        //构造图片路径
	        string path = Parameters::Instance()->m_appWorkPath + Parameters::Instance()->m_measureImageTempPath + cUUID +".png";
	        if(createImageFun(shapeSet,path.c_str()))
	        {
	            imagePath = path;
	        }
	    }
#endif

#ifdef WIN32
	if (createImageFun) {
		string cUUID = IDCreator::GetUUID();
		//构造图片路径
		string path = Parameters::Instance()
			->m_appWorkPath + Parameters::Instance()
			->m_measureImageTempPath + cUUID + ".png";
		if (createImageFun(shapeSet, path.c_str()))
		{
			imagePath = path;
		}
	}
#endif
	return imagePath;
}

void GlueObj::SetRefreshCB(RefreshCB val)
{
	refreshCB = val;
}

MeasureTextCB GlueObj::GetMeasureTextFun() const
{
	return measureTextFun;
}

void GlueObj::SetMeasureTextFun(MeasureTextCB val)
{
	measureTextFun = val;
}

}
