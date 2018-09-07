#include "m3d/M3D.h"
#include "GLDrawer20.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"

#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/Section.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/graphics/TextNode.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/ResourceManager.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"

//#include "m3d/utils/ViewPortCuller.h"

#include "Utility.h"
#ifdef __MOBILE__
#include <FTGL/ftgles.h>
#endif
#include "sview/views/Parameters.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/ComText.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/graphics/Texture.h"
#include "m3d/renderer/gl20/ShaderProgram.h"
#include "m3d/renderer/gl20/ShaderManager.h"

using namespace SVIEW;

namespace M3D
{
bool GLDrawer20::useVBO = true;
bool GLDrawer20::firstUp = true;
float GLDrawer20::drawLimit = 0.0f;
GLuint GLDrawer20::gProgram = 0;
GLuint GLDrawer20::gvPositionHandle = 0;
GLuint GLDrawer20::gvModelViewHandle = 0;
GLuint GLDrawer20::gvProjectionHandle = 0;
GLuint GLDrawer20::gvColorHandle = 0;

static const char gVertexShader[] = "attribute vec4 vPosition;\n"
		"attribute vec4 a_color;\n"
		"uniform mat4 modelView;\n"
		"uniform mat4 projection;\n"
		"varying vec4 v_color;\n"
		"void main() {\n"
		"  gl_Position =projection*modelView*vPosition;\n"
//	"  gl_Position = vPosition;\n"
		"  v_color = a_color;\n"
		"}\n";

static const char gFragmentShader[] = "precision mediump float;\n"
		"varying vec4 v_color;\n"
		"void main() {\n"
		"  gl_FragColor = v_color;\n"
		"}\n";



//draw axis
void GLDrawer20::DrawAxis(SceneNode* node, RenderAction* action)
{
	if (Parameters::Instance()->m_IsShowAxis)
	{
		if (!action || !node)
		{
			return;
		}

		ShaderManager * shaderManager = action->GetShaderMananger();
		if (!shaderManager)
		{
			return;
		}

		ShaderProgram * axisEffect = shaderManager->GetEffect(
				ShaderManager::Axis);
		if (!axisEffect)
		{
			return;
		}

		glDisable(GL_DEPTH_TEST);
		AxisNode* axis = (AxisNode*) node;

		glViewport(axis->m_iViewX, axis->m_iViewY, axis->m_iW, axis->m_iW);
		//设置投影矩阵 模型视图矩阵 一些初始化

		Matrix4 modelViewMatrix = node->GetGLWorldTransform();
		Matrix4 projectionMatrix;
		projectionMatrix.Ortho(axis->m_ProArray[0], axis->m_ProArray[1], axis->m_ProArray[2],
				axis->m_ProArray[3], axis->m_ProArray[4], axis->m_ProArray[5]);
		projectionMatrix = projectionMatrix.Transpose();
		axisEffect->UseProgram();

		ShaderParameter * position = axisEffect->GetShaderAttributeParameter(VSP_POSITION);

		axisEffect->SetVertexAttribPointer(position->m_location,3,GL_FLOAT,0,axis->axisPntX);
		axisEffect->EnableAttributeArray(position->m_location);

		Matrix4 mvp = modelViewMatrix*projectionMatrix;
		//Matrix4 mvp = Parameters::Instance()->m_testMVP;

		axisEffect->SetUniformValue(VSP_MVPMAT,mvp);

		axisEffect->SetUniformValue(VSP_UCOLOR,axis->m_xColor);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntXNum / 3);

		axisEffect->SetVertexAttribPointer(position->m_location,3,GL_FLOAT,0,axis->axisPntY);

		axisEffect->SetUniformValue(VSP_UCOLOR,axis->m_yColor);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntYNum / 3);

		axisEffect->SetVertexAttribPointer(position->m_location,3,GL_FLOAT,0,axis->axisPntZ);

		axisEffect->SetUniformValue(VSP_UCOLOR,axis->m_zColor);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntZNum / 3);

		axisEffect->SetVertexAttribPointer(position->m_location,3,GL_FLOAT,0,axis->axisPntO);

		axisEffect->SetUniformValue(VSP_UCOLOR,axis->m_oColor);
		glDrawArrays(GL_TRIANGLES, 0, axis->m_iPntONum / 3);

		axisEffect->DisableAttributeArray(position->m_location);
		axisEffect->ReleaseShaderProgram();

		if (axis->m_xImage && axis->m_yImage && axis->m_zImage)
		{
			Matrix4 identity = Matrix4::IDENTITY;
			GLShapeDrawer20::DrawImageBoard(action,axis->m_xImage, modelViewMatrix, projectionMatrix, identity);
			GLShapeDrawer20::DrawImageBoard(action, axis->m_yImage, modelViewMatrix, projectionMatrix, identity);
			GLShapeDrawer20::DrawImageBoard(action, axis->m_zImage, modelViewMatrix, projectionMatrix, identity);
		}

		glEnable(GL_DEPTH_TEST);
	}
}

void GLDrawer20::DrawFPS(SceneNode* node, RenderAction* action)
{


}

void GLDrawer20::DrawText(SceneNode* node, RenderAction* action)
{

}

//initial
void GLDrawer20::InitialGL()
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
    if (Parameters::Instance()->m_BackTransparent == true) {
        glClearColor(0.0, 0.0, 0.0, 0);
    }else{
        glClearColor(0.5, 0.5, 0.1, 1);
    }
	glFrontFace(GL_CCW);
	glClearStencil(0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#ifdef WIN32
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_MULTISAMPLE);
#endif
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

}

void GLDrawer20::LoadMatrix(SceneNode* sceneNode, RenderContext* gl)
{
//	Matrix4& plcMatrix = sceneNode->GetPlcMatrix();
//	Vector3& pos = sceneNode->GetPosition();
//	Quaternion& rotate = sceneNode->GetRotation();
//
//	gl->MultMatrix(plcMatrix);
//	gl->Translate(pos.x, pos.y,pos.z);
//	if ((rotate.angle < -DELTA_ROT20)
//			|| (rotate.angle > DELTA_ROT20))
//		gl->Rotate(rotate.angle, rotate.axis.x,rotate.axis.y,rotate.axis.z);
}

void GLDrawer20::LoadContextStateToShader(RenderContext* gl)
{
	// Load the Matrix to shader
//	Matrix4 projection;
//	gl->GetFloatv(GL_PROJECTION_MATRIX, projection);
//	glUniformMatrix4fv(gvProjectionHandle, 1, GL_FALSE, projection.GetData());
//	Matrix4 m1;
//	gl->GetFloatv(GL_MODELVIEW_MATRIX, m1);
//	glUniformMatrix4fv(gvModelViewHandle, 1, GL_FALSE, m1.GetData());
}

void GLDrawer20::DrawCamera(SceneNode* sceneNode, RenderAction* action)
{
//	CameraNode* camera = (CameraNode*) sceneNode;
//	RenderContext* gl = NULL;
//	if (action != NULL)
//	{
//		if (action->m_pvbodataCache != NULL)
//		{
//			if (action->m_pvbodataCache->GetBindState() == true)
//			{
//				action->m_pvbodataCache->SetBindState(false);
//			}
//		}
//		gl = action->GetGLContext();
//		action->m_pCameraNodeCache = camera;
//	}
//	gl->PushMatrix();
//	camera->LoadMatrix(gl);
//
//	LoadContextStateToShader(gl);
//
//	glGetIntegerv(GL_VIEWPORT, camera->cameraState.m_iViewPort);
//	gl->GetFloatv(GL_PROJECTION_MATRIX,
//			(float*) camera->cameraState.m_ProjectMatrix.m);
//	gl->GetFloatv(GL_MODELVIEW_MATRIX,
//			(float*) camera->cameraState.m_MVMatrix.m);
//	action->GetViewportCuller()->SetViewPortInfo(
//			camera->cameraState.m_ProjectMatrix,
//			camera->cameraState.m_iViewPort);
//	for (int i = 0; i < camera->Size(); i++)
//	{
//		camera->GetChild(i)->Draw(action);
//	}
//	gl->PopMatrix();
}

void GLDrawer20::DrawSection(SceneNode* sceneNode, RenderAction* action)
{

	Section* sectionNode = (Section*) sceneNode;
	//	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//	glClipPlanef(GL_CLIP_PLANE0, sectionNode->m_position);
	glEnable(GL_CLIP_PLANE0);
}

void GLDrawer20::LogGLState()
{
	LOGI("log gl state start.");
//		LOGI("GL_FOG:%s",glIsEnabled(GL_FOG)?"true":"false");
//		LOGI("GL_LIGHTING:%s",glIsEnabled(GL_LIGHTING)?"true":"false");
//		LOGI("GL_TEXTURE_2D:%s",glIsEnabled(GL_TEXTURE_2D)?"true":"false");
//		LOGI("GL_CULL_FACE:%s",glIsEnabled(GL_CULL_FACE)?"true":"false");
//		LOGI("GL_ALPHA_TEST:%s",glIsEnabled(GL_ALPHA_TEST)?"true":"false");
//		LOGI("GL_BLEND:%s",glIsEnabled(GL_BLEND)?"true":"false");
//		LOGI("GL_COLOR_LOGIC_OP:%s",glIsEnabled(GL_COLOR_LOGIC_OP)?"true":"false");
//		LOGI("GL_DITHER:%s",glIsEnabled(GL_DITHER)?"true":"false");
//		LOGI("GL_STENCIL_TEST:%s",glIsEnabled(GL_STENCIL_TEST)?"true":"false");
//		LOGI("GL_DEPTH_TEST:%s",glIsEnabled(GL_DEPTH_TEST)?"true":"false");
//		LOGI("GL_POINT_SMOOTH:%s",glIsEnabled(GL_POINT_SMOOTH)?"true":"false");
//		LOGI("GL_LINE_SMOOTH:%s",glIsEnabled(GL_LINE_SMOOTH)?"true":"false");
//		LOGI("GL_SCISSOR_TEST:%s",glIsEnabled(GL_SCISSOR_TEST)?"true":"false");
//		LOGI("GL_COLOR_MATERIAL:%s",glIsEnabled(GL_COLOR_MATERIAL)?"true":"false");
//		LOGI("GL_NORMALIZE:%s",glIsEnabled(GL_NORMALIZE)?"true":"false");
//		LOGI("GL_RESCALE_NORMAL:%s",glIsEnabled(GL_RESCALE_NORMAL)?"true":"false");
//		LOGI("GL_POLYGON_OFFSET_FILL:%s",glIsEnabled(GL_POLYGON_OFFSET_FILL)?"true":"false");
//		LOGI("GL_VERTEX_ARRAY:%s",glIsEnabled(GL_VERTEX_ARRAY)?"true":"false");
//		LOGI("GL_NORMAL_ARRAY:%s",glIsEnabled(GL_NORMAL_ARRAY)?"true":"false");
//		LOGI("GL_COLOR_ARRAY:%s",glIsEnabled(GL_COLOR_ARRAY)?"true":"false");
//		LOGI("GL_TEXTURE_COORD_ARRAY:%s",glIsEnabled(GL_TEXTURE_COORD_ARRAY)?"true":"false");
//		LOGI("GL_MULTISAMPLE:%s",glIsEnabled(GL_MULTISAMPLE)?"true":"false");
//		LOGI("GL_SAMPLE_ALPHA_TO_COVERAGE:%s",glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE)?"true":"false");
//		LOGI("GL_SAMPLE_ALPHA_TO_ONE:%s",glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE)?"true":"false");
//		LOGI("GL_SAMPLE_COVERAGE:%s",glIsEnabled(GL_SAMPLE_COVERAGE)?"true":"false");

	if (glIsEnabled(GL_FOG))
		LOGE("GL_FOG");
	if (glIsEnabled(GL_LIGHTING))
		LOGE("GL_LIGHTING");
	if (glIsEnabled(GL_TEXTURE_2D))
		LOGE("GL_TEXTURE_2D");
	if (glIsEnabled(GL_CULL_FACE))
		LOGE("GL_CULL_FACE");
	if (glIsEnabled(GL_ALPHA_TEST))
		LOGE("GL_ALPHA_TEST");
	if (glIsEnabled(GL_BLEND))
		LOGE("GL_BLEND");
	if (glIsEnabled(GL_COLOR_LOGIC_OP))
		LOGE("GL_COLOR_LOGIC_OP");
	if (glIsEnabled(GL_DITHER))
		LOGE("GL_DITHER");
	if (glIsEnabled(GL_STENCIL_TEST))
		LOGE("GL_STENCIL_TEST");
	if (glIsEnabled(GL_DEPTH_TEST))
		LOGE("GL_DEPTH_TEST");
	if (glIsEnabled(GL_POINT_SMOOTH))
		LOGE("GL_POINT_SMOOTH");
	if (glIsEnabled(GL_LINE_SMOOTH))
		LOGE("GL_LINE_SMOOTH");
	if (glIsEnabled(GL_SCISSOR_TEST))
		LOGE("GL_SCISSOR_TEST");
	if (glIsEnabled(GL_COLOR_MATERIAL))
		LOGE("GL_COLOR_MATERIAL");
	if (glIsEnabled(GL_NORMALIZE))
		LOGE("GL_NORMALIZE");
	if (glIsEnabled(GL_RESCALE_NORMAL))
		LOGE("GL_RESCALE_NORMAL");
	if (glIsEnabled(GL_POLYGON_OFFSET_FILL))
		LOGE("GL_POLYGON_OFFSET_FILL");
	if (glIsEnabled(GL_VERTEX_ARRAY))
		LOGE("GL_VERTEX_ARRAY");
	if (glIsEnabled(GL_NORMAL_ARRAY))
		LOGE("GL_NORMAL_ARRAY");
	if (glIsEnabled(GL_COLOR_ARRAY))
		LOGE("GL_COLOR_ARRAY");
	if (glIsEnabled(GL_TEXTURE_COORD_ARRAY))
		LOGE("GL_TEXTURE_COORD_ARRAY");
	if (glIsEnabled(GL_MULTISAMPLE))
		LOGE("GL_MULTISAMPLE");
	if (glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE))
		LOGE("GL_SAMPLE_ALPHA_TO_COVERAGE");
	if (glIsEnabled(GL_SAMPLE_ALPHA_TO_ONE))
		LOGE("GL_SAMPLE_ALPHA_TO_ONE");
	if (glIsEnabled(GL_SAMPLE_COVERAGE))
		LOGE("GL_SAMPLE_COVERAGE");

	LOGI("log gl state end.");
}
void GLDrawer20::checkGlError(const char* op)
{
	for (GLint error = glGetError(); error; error = glGetError())
	{
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}
GLuint GLDrawer20::createProgram(const char* pVertexSource,
		const char* pFragmentSource)
{
	// Load the vertex/fragment shaders
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader)
	{
		return 0;
	}
	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader)
	{
		return 0;
	}
	// Create the program object
	GLuint program = glCreateProgram();
	if (program)
	{
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		// Bind  attribute
		glBindAttribLocation(program, 0, "vPosition");
		glBindAttribLocation(program, 1, "a_color");
		glBindAttribLocation(program, 2, "modelView");
		glBindAttribLocation(program, 3, "projection");
		// Link the program
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		// Check the link status
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength > 1)
			{
				char* buf = (char*) malloc(sizeof(char) * bufLength);
				if (buf)
				{
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}
GLuint GLDrawer20::loadShader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader)
	{
		// Load the shader source
		glShaderSource(shader, 1, &pSource, NULL);

		// Compile the shader
		glCompileShader(shader);
		GLint compiled = 0;
		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1)
			{
				char* buf = (char*) malloc(sizeof(char) * infoLen);
				if (buf)
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}
bool GLDrawer20::InitShaders()
{

	if (!gProgram)
	{
		gProgram = createProgram(gVertexShader, gFragmentShader);
		if (!gProgram)
		{
			LOGE("Could not create program.");
			return false;
		}
		// Use the program object
		glUseProgram(gProgram);
		checkGlError("glUseProgram");
	}

	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);
	gvColorHandle = glGetAttribLocation(gProgram, "a_color");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"a_color\") = %d\n", gvColorHandle);
	gvModelViewHandle = glGetUniformLocation(gProgram, "modelView");
	checkGlError("glGetUniformLocation");
	LOGI("glGetUniformLocation(\"modelView\") = %d\n", gvModelViewHandle);
	gvProjectionHandle = glGetUniformLocation(gProgram, "projection");
	checkGlError("glGetUniformLocation");
	LOGI("glGetUniformLocation(\"projection\") = %d\n", gvProjectionHandle);

	return true;
}
}

