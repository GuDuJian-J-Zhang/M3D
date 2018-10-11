#include "m3d/renderer/gl20/ShaderSrcCode.h"

namespace M3D
{
#define ENT "\n"
#define TOSTR(a) #a

	ShaderSrcCode::ShaderSrcCode()
	{
		// TODO Auto-generated constructor stub
	}

	ShaderSrcCode::~ShaderSrcCode()
	{
		// TODO Auto-generated destructor stub
	}
	// WIN32
#pragma win32
#ifdef WIN32
	const char *ShaderSrcCode::BackgroundVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec4 a_color;\n"
			"uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_MVPMat * vec4(a_position,1.0);\n"
			"v_color = a_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::BackgroundFrag()
	{
		return "//precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char* ShaderSrcCode::XRayVert()
	{
		return	
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat3 u_normalMat;\n"
			"uniform vec4 u_Color;\n"
			"varying vec3 N;\n"
			"varying vec3 I;\n"
			"varying vec4 Cs;\n"
			"void main()\n"
			"{\n"
			"vec4 P = u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"I = P.xyz - vec3(0);\n"
			"N = normalize(u_normalMat * a_normal);\n"
			"Cs = u_Color;\n"
			"gl_Position = u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"};\n";	
	}

	const char* ShaderSrcCode::XRayFrag()
	{
		return
			"varying vec3 N;\n"
			"varying vec3 I;\n"
			"varying vec4 Cs;\n"
			// globals
			"uniform float edgefalloff;\n"
			"void main()\n"
			"{\n"
			"float opac = dot(normalize(N), normalize(-I));\n"
			"opac = abs(opac);\n"
			"opac = 1.0 - pow(opac, edgefalloff);\n"
			"gl_FragColor = opac * Cs;\n"
			"gl_FragColor.a = opac;\n"
			"}\n";
	}

	const char* ShaderSrcCode::DraggerVert()
	{
		return
			"//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform vec4 u_color;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"varying vec4 v_color;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"gl_Position = u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_color = u_color;\n"
			"gl_Position.z = -gl_Position.w;\n"//gl_Position.z - 0.001f;\n"
											   //"v_normal = mat3(transpose(inverse(u_modelMat))) * a_normal;\n"
			"v_normal = vec3(u_normalMat*vec4(a_normal,0));\n"
			"v_position = vec3(gl_Position);\n"
			"}\n";
	}


	const char* ShaderSrcCode::DraggerFrag()
	{
		return
			//"precision mediump  float;\n"
			"varying vec4 v_color;\n"
			//"varying vec3 v_normal;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			// ambient
			//"float ambientStrength = 0.1;\n"
			//"vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f);\n"
			"const vec3 lightPos =  vec3(10.0,10.0,10.0);\n"
			"const vec3 lightColor =  vec3(1.0,1.0,1.0);\n"
			// diffuse 
			//"vec3 norm = normalize(v_normal);\n"
			//"vec3 lightDir = normalize(lightPos - FragPos);\n"
			//"float diff = max(dot(norm, lightDir), 0.0);\n"
			//"vec3 diffuse = diff * lightColor;\n"
			//"vec3 result = (ambient) * v_color;\n"
			//"gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
			"vec3 norm = normalize(v_normal);\n"
			"vec3 lightDir = normalize(lightPos - v_position);\n"
			"float diff = max(dot(norm, lightDir), 0.0);\n"
			"vec3 diffuse = (diff +0.5) * lightColor;\n"
			"vec4 result = vec4(diffuse.xyz,1.0) * v_color;\n"
			"gl_FragColor =vec4(result.xyzw);\n"
			"}\n";
	}

	const char* ShaderSrcCode::DraggerEdgeVert()
	{
		return
			"//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform vec4 u_color;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_color = u_color;\n"
			"gl_Position.z = -gl_Position.w;\n"//gl_Position.z - 0.001f;\n"
			"}\n";
	}

	const char* ShaderSrcCode::DraggerEdgeFrag()
	{
		return
		//"precision mediump  float;\n"
		"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor =v_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::ModelBaseVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"v_texCoords = tempCoords;\n"
			"v_viewDirection  = normalize(u_eyePosition -fvObjectPosition.xyz);\n"
			"v_lightDirection = normalize(u_lightPosition-fvObjectPosition.xyz);\n"
			" v_normal         = normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"}\n";
	}



	const char *ShaderSrcCode::ModelBaseFrag()
	{
		return "#extension GL_OES_standard_derivatives:enable \n"

			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_specular;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform bool u_useTex;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3){\n"
			"float m;\n"
			"vec3 r;\n vec3 u;\n"
			"u = normalize(ecPosition3);\n"
			"r = reflect(u, normal);\n"
			"m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"const vec4 ambientColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"const vec4 diffuseColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"const float shiness = 30.0;\n"
			"const vec4 specularColor = vec4(0.5,0.5,0.5,1.0);\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"void main() { \n"
			"float fNDotL =0.0;\n"
			"vec3 	halfvector;\n"
			"float fNDotH;"
			" vec3  fvNormal ;\n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"if(gl_FrontFacing){\n"
			" fvNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" fvNormal         = normalize( -v_normal );\n"
			"}\n"
			"vec2 sphereCoords = sphereMap(fvNormal,v_position);\n"
			"//switchColor = switchColor * texture2D(u_sampler1,sphereCoords.xy);\n"
			" fNDotL           = clamp(dot( fvNormal, v_lightDirection ),0.001,1.0);\n"
			"halfvector        =normalize(v_lightDirection+ v_viewDirection );\n"
			" fNDotH            = dot(fvNormal,halfvector);\n"
			"if(u_useTex){\n"
			"switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"}\n"
			"vec4  fvTotalAmbient   = u_ambient * ambientColor;\n"
			"vec4  fvTotalDiffuse   = switchColor*u_diffuse * fNDotL * diffuseColor; \n"
			"vec4  fvTotalSpecular  = u_specular * specularColor *( pow( max(fNDotH,0.0), shiness ) );\n"
			"vec4 colorLinear = fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular;\n"
			"gl_FragColor = vec4(colorLinear.xyz,u_diffuse.a);\n"
			"}\n";
	}

	const char *ShaderSrcCode::EdgeVert()
	{
		return "// precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"#ifdef DEPTH_EDGE\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#ifdef DEPTH_EDGE\n"
			"v_projectPos = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#endif\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"gl_PointSize = 3.0;\n"
			"}\n";
	}
	const char *ShaderSrcCode::EdgeFrag()
	{
		return "//precision highp  float;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"#ifdef DEPTH_EDGE\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"#ifdef DEPTH_EDGE\n"
			"vec3 proj =v_projectPos.xyz/v_projectPos.w;\n "
			"   proj = proj *0.5+0.5;\n"
			"float priDepth = texture2D(u_sampler0,proj.xy).x;\n"
			"if(priDepth<proj.z) discard;\n"
			"#endif\n"
			"gl_FragColor = u_diffuse;\n"
			"}\n";
	}

	const char *ShaderSrcCode::EdgeFragMediump()
	{
		return "precision mediump  float;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"#ifdef DEPTH_EDGE\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"#ifdef DEPTH_EDGE\n"
			"vec3 proj =v_projectPos.xyz/v_projectPos.w;\n "
			"   proj = proj *0.5+0.5;\n"
			"float priDepth = texture2D(u_sampler0,proj.xy).x;\n"
			"if(priDepth<proj.z) discard;\n"
			"#endif\n"
			"gl_FragColor = u_diffuse;\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteEdgeVert()
	{
		return "// precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"#ifdef DEPTH_EDGE\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#ifdef DEPTH_EDGE\n"
			"v_projectPos = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#endif\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"gl_PointSize = 3.0;\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteEdgeFrag()
	{
		return "//precision highp  float;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useMinDepth;\n"
			"#ifdef DEPTH_EDGE\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"#ifdef DEPTH_EDGE\n"
			"vec3 proj =v_projectPos.xyz/v_projectPos.w;\n "
			"   proj = proj *0.5+0.5;\n"
			"float priDepth = texture2D(u_sampler0,proj.xy).x;\n"
			"if(priDepth<proj.z) discard;\n"
			"#endif\n"
			"if(u_useMinDepth == true)gl_FragDepth = 0.0;\n"
			"gl_FragColor = u_diffuse;\n"
			"}\n";
	}

	const char *ShaderSrcCode::WireframeVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_center;\n"
			"void main(){\n"
			"v_center = a_normal;\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"}\n";
	}
	const char *ShaderSrcCode::WireframeFrag()
	{
		return "#extension GL_OES_standard_derivatives : enable\n"

			"uniform vec4 u_diffuse;\n"
			"varying vec3 v_center;\n"
			"float edgeFactorTri() {\n"
			"vec3 d = fwidth( v_center.xyz );\n"
			"vec3 a3 = smoothstep( vec3( 0.0 ), d * 1.5, v_center.xyz );\n"
			"return min( min( a3.x, a3.y ), a3.z );\n"
			"}\n"
			"void main() { \n"
			"gl_FragColor.rgb = mix( vec3( 1.0 ), vec3( 0.2 ), edgeFactorTri() );\n"
			"gl_FragColor.a = 1.0;\n"
			"}\n";
	}

	const char *ShaderSrcCode::BrdfVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3    v_position;"
			"varying vec4 v_worldPosition;\n"
			"void main() { \n"
			"gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 v_worldPosition = u_modelMat* vec4(a_position,1.0);\n"
			"vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"v_texCoords = tempCoords;\n"
			"v_position = fvObjectPosition.xyz;\n"
			"v_viewDirection  = normalize( -fvObjectPosition.xyz);\n"
			"v_lightDirection = normalize(u_lightPosition-fvObjectPosition.xyz);\n"
			" v_normal         = normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"}\n";
	}

	const char *ShaderSrcCode::BrdfFrag()
	{
		return "#extension GL_OES_standard_derivatives:enable \n"

			"out vec4 FragColor;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_specular;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform bool u_useTex;\n"
			"uniform float u_fresnel0;\n"
			"uniform float u_roughness;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3    v_position;"
			"varying vec4 v_worldPosition;\n"
			"const vec3 ambientColor = vec3(1.0, 1.0, 1.0);\n"
			"const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);\n"
			"const float shiness = 65.0;\n"
			"const vec4 specularColor = vec4(0.5,0.5,0.5,1.0);\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"float fresnel(float f0, vec3 n, vec3 l){\n"
			"return f0 + (1.0-f0) * pow(1.0- dot(n, l), 5.0);\n"
			"}\n"
			"float distribution(vec3 n, vec3 h, float roughness){\n"
			"float m_Sq= roughness * roughness;\n"
			"float NdotH_Sq= max(dot(n, h), 0.01);\n"
			"NdotH_Sq= NdotH_Sq * NdotH_Sq;\n"
			"return exp( (NdotH_Sq - 1.0)/(m_Sq*NdotH_Sq) )/ (3.14159265 * m_Sq * NdotH_Sq * NdotH_Sq) ;\n"
			"}\n"
			"float geometry(vec3 n, vec3 h, vec3 v, vec3 l, float roughness){\n"
			"float NdotH= dot(n, h);\n"
			"float NdotL= dot(n, l);\n"
			"float NdotV= dot(n, v);\n"
			"float VdotH= dot(v, h);\n"
			"float NdotL_clamped= max(NdotL, 0.01);\n"
			"float NdotV_clamped= max(NdotV, 0.01);\n"
			"return min( min( 2.0 * NdotH * NdotV_clamped / VdotH, 2.0 * NdotH * NdotL_clamped / VdotH), 1.0);\n"
			"}\n"
			"float diffuseEnergyRatio(float f0, vec3 n, vec3 l){\n"
			" return 1.0 - fresnel(f0, n, l);\n"
			"  }\n"
			"void main() { \n"
			"vec3  fvLightDirection;\n"
			" vec3  fvNormal ;\n"
			" vec3 normal;\n "
			"vec3 view;\n"
			" vec3 halfVec;\n"
			"float NdotL;\n"
			"float NdotV;\n"
			"float NdotL_clamped;\n"
			"float NdotV_clamped;\n"
			"float brdf_spec;\n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"if(gl_FrontFacing){\n"
			" fvNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" fvNormal         = normalize( -v_normal );\n"
			"}\n"
			"fvLightDirection = normalize( v_lightDirection );\n"
			" normal =  normalize(fvNormal);\n"
			"view   = normalize(v_viewDirection);\n"
			" halfVec=  normalize(fvLightDirection + view);\n"
			"NdotL= dot(normal, fvLightDirection);\n"
			"NdotV= dot(normal, view);\n"
			"NdotL_clamped= max(NdotL, 0.01);\n"
			"NdotV_clamped= max(NdotV, 0.01);\n"
			"brdf_spec= fresnel(u_fresnel0, halfVec, fvLightDirection) * \n"
			" geometry(normal, halfVec, view, fvLightDirection, u_roughness) * \n"
			"distribution(normal, halfVec, u_roughness) / (4.0 * NdotL_clamped * NdotV_clamped);\n"
			"if(u_useTex){\n"
			"switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"}\n"
			"vec3 color_spec = NdotL_clamped * brdf_spec * diffuseColor;\n"
			"vec3 color_diff = switchColor.xyz*NdotL_clamped * diffuseEnergyRatio(u_fresnel0, normal, fvLightDirection) * vec3(u_diffuse.xyz) * diffuseColor;\n"
			" gl_FragColor = vec4( color_diff + color_spec, u_diffuse.a)  ; \n"
			"}\n";
	}

	const char *ShaderSrcCode::AxisVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform vec4 u_color;\n"
			"uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_MVPMat * vec4(a_position,1.0);\n"
			"v_color = u_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::AxisFrag()
	{
		return "//precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::ImageVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ImageFrag()
	{
		return "//precision mediump  float;\n"
			"//precision mediump sampler2D;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"vec4 pcolor = texture2D(u_sampler0,v_texCoords);\n"
			"vec4 tcolor = pcolor*u_diffuse;\n"
			"gl_FragColor = vec4(tcolor.xyz,pcolor.a);\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteImageVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteImageFrag()
	{
		return "//precision mediump  float;\n"
			"//precision mediump sampler2D;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useMinDepth;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"vec4 pcolor = texture2D(u_sampler0,v_texCoords);\n"
			"vec4 tcolor = pcolor*u_diffuse;\n"
			"if(u_useMinDepth == true)gl_FragDepth = 0.0;\n"
			"gl_FragColor = vec4(tcolor.xyz,pcolor.a);\n"
			"}\n";
	}

	const char *ShaderSrcCode::CubeMapVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_cubeTexCoords = a_position;\n"
			"}\n";
	}
	const char *ShaderSrcCode::CubeMapFrag()
	{
		return "//precision mediump  float;\n"
			"//precision mediump sampler2D;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"void main() { \n"
			"vec4 pcolor = textureCube(u_samplerCube0,v_cubeTexCoords);\n"
			"gl_FragColor = pcolor;\n"
			"}\n";
	}

	const char *ShaderSrcCode::ClipPlaneVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform vec4 u_color;\n"
			"uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_MVPMat * vec4(a_position,1.0);\n"
			"v_color = u_color;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ClipPlaneFrag()
	{
		return "//precision highp  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ClipPlaneFragMediump()
	{
		return "precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::MultilightVert()
	{
		return "//precision highp  float;\n"
			"//#define FBO \n"
			"struct Light\n"
			"{\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    vec4 position;\n"
			"    vec3 intensity;\n"
			"    vec3 spotDirection;\n"
			"    float spotExponent;\n"
			"    float spotCutoff; // (range: [0.0,90.0], 180.0)\n"
			"    float spotCosCutoff; // (range: [1.0,0.0],-1.0)\n"
			"    float constant;\n"
			"    float linear;\n"
			"    float quadratic;\n"
			"};\n"
			"\n"
			"struct LightModel\n"
			"{\n"
			"    vec4 ambient;    // Acs\n"
			"};\n"
			"\n"
			"struct Materials\n"
			"{\n"
			"    vec4 emission;\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    float shininess;\n"
			"};\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"#ifdef FBO \n"
			"uniform mat4 u_lightMat;\n"
			"uniform mat4 u_lightProject;\n"
			"#endif \n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"\n"
			"//varying\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_diffuseColor;\n"
			"varying vec3 v_specularColor;\n"
			"varying vec3 v_diffuseColorN;\n"
			"varying vec3 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"uniform vec3 u_shadowLPos;\n"
			"#endif \n"
			"//scene ambient\n"
			"uniform LightModel u_lightModel ;\n"
			"const int numberOfLights = 1;\n"
			"uniform Light u_lights[1];\n"
			"uniform int u_enbleLights[1];\n"
			"//Materials\n"
			"uniform Materials u_materials;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"    vec3 totalLighting = vec3(u_materials.emission)+ vec3(u_lightModel.ambient) * vec3(u_materials.ambient);\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalAmbientBack= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuseBack = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecularBack = vec3(0.0,0.0,0.0);\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"#ifdef FBO \n"
			"    v_lightSpacePos = u_lightProject*u_lightMat *u_modelMat* vec4(a_position,1.0);\n"
			"    v_worldNormal = a_normal;\n"
			"#endif \n"
			"    vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_texCoords = tempCoords;\n"
			"    vec3 vNormal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    v_normal = vNormal;\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    v_cubeTexCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(a_normal));\n"
			"#ifdef FBO\n"
			"   v_worldPos = worldPosT;\n"
			"#endif\n"
			"    for(int index = 0;index<numberOfLights;index++)\n"
			"    {\n"
			"        if(u_enbleLights[index] == 1)\n"
			"        {\n"
			"\n"
			"            if (0.0 == u_lights[index].position.w) // directional light?\n"
			"            {\n"
			"                attenuation = 1.0; // no attenuation\n"
			"                lightDirection = normalize(vec3(u_lights[index].position));\n"
			"            }\n"
			"            else // point light or spotlight (or other kind of light)\n"
			"            {\n"
			"                vec3 positionToLightSource = vec3(u_lights[index].position) - fvObjectPosition.xyz ;\n"
			"                float distance = length(positionToLightSource);\n"
			"                lightDirection = normalize(positionToLightSource);\n"
			"                float tempA= u_lights[index].constant + u_lights[index].linear * distance + u_lights[index].quadratic * distance * distance;\n"
			"                attenuation = 1.0 / tempA;\n"
			"                if (u_lights[index].spotCutoff <= 90.0) // spotlight?\n"
			"                {\n"
			"                    float clampedCosine = max(0.0001, dot(lightDirection, -normalize(u_lights[index].spotDirection)));\n"
			"                    if (clampedCosine < u_lights[index].spotCosCutoff) // outside of spotlight cone?\n"
			"                    {\n"
			"                        attenuation = 0.0;\n"
			"                    }\n"
			"                    else\n"
			"                    {\n"
			"                        attenuation = attenuation * pow(clampedCosine, u_lights[index].spotExponent);\n"
			"//attenuation *= clamp((clampedCosine-cos(radians(15.0)) ) / (cos(radians(10.0)) - cos(radians(15.0))), 0.0, 1.0);\n"
			"                    }\n"
			"                }\n"
			"            }\n"
			"\n"
			"         //a side\n"
			"        float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"        vec3 diffuseReflection = attenuation\n"
			"            * vec3(u_lights[index].diffuse) * vec3(u_materials.diffuse)\n"
			"            * fNDotL;\n"
			"        //the othor\n"
			"        fNDotL= clamp(abs(dot( -vNormal, lightDirection )),0.3,1.0);\n"
			"        vec3 diffuseReflectionBack = attenuation\n"
			"            * vec3(u_lights[index].diffuse) * vec3(u_materials.diffuse)\n"
			"            * fNDotL;\n"
			"\n"
			"         totalAmbient =vec3(totalAmbient) + vec3(u_lights[index].ambient) * vec3(u_materials.ambient);\n"
			"		  float intensity = u_lights[index].intensity.x;\n"
			"         totalDiffuse = totalDiffuse + diffuseReflection*intensity;\n"
			"         totalDiffuseBack = totalDiffuseBack + diffuseReflectionBack*intensity;\n"
			"\n"
			"        vec3 eyeDirection = normalize(- fvObjectPosition.xyz );\n"
			"        vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"\n"
			"		vec3 tempsp = vec3(u_lights[index].specular);\n"
			"		float spv =  tempsp.x*tempsp.x+tempsp.y*tempsp.y+tempsp.z*tempsp.z;\n"
			"			if(spv>0.0){\n"
			"        //a side\n"
			"        float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"        vec3 specularReflection = attenuation * vec3(u_lights[index].specular) * vec3(u_materials.specular)\n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"        //the other\n"
			"        fNDotH = max(0.0, abs(dot(-vNormal,halfVector)));\n"
			"        vec3 specularReflectionBack = attenuation * vec3(u_lights[index].specular) * vec3(u_materials.specular)\n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"\n"
			"         totalSpecular = totalSpecular + specularReflection*intensity;\n"
			"         totalSpecularBack = totalSpecularBack + specularReflectionBack*intensity;\n"
			"			}\n"
			"        }\n"
			"    }\n"
			"    v_diffuseColor = min(totalLighting+totalAmbient+totalDiffuse,vec3(1.0));\n"
			"    v_specularColor = min(totalSpecular,vec3(1.0));\n"
			"\n"
			"    v_diffuseColorN = min(totalLighting+totalAmbient+totalDiffuseBack,vec3(1.0));\n"
			"    v_specularColorN = min(totalSpecularBack,vec3(1.0));\n"
			" }\n"

			;
	}
	const char *ShaderSrcCode::MultilightFrag()
	{
		return "//precision highp  float;\n"
			"//#define FBO \n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform bool u_useAmbientTex;\n"
			"uniform bool u_useCubeMapTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_diffuseColor;\n"
			"varying vec3 v_specularColor;\n"
			"varying vec3 v_diffuseColorN;\n"
			"varying vec3 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"uniform vec3 u_shadowLPos;\n"
			"uniform float u_xPixelOffset;\n"
			"uniform float u_yPixelOffset;\n"
			"#endif \n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0);\n"
			"#ifdef FBO \n"
			"float ShadowCalculate(sampler2D sampler,vec4 fragPosLightSpace)\n"
			"{\n"
			"   float shadow = 1.0;\n"
			"	vec3 proj =fragPosLightSpace.xyz/fragPosLightSpace.w;\n"
			"   proj = proj *0.5+0.5;\n"
			"   float currentDepth = proj.z;\n"
			"   vec3 normal = normalize(v_worldNormal);\n"
			"   vec3 lightDir = normalize(u_shadowLPos-v_worldPos);\n"
			"   float bias = max(0.05*(1.0-dot(normal,lightDir)),0.005);\n"
			"   for(int y=-1; y<=1;y++)\n"
			"   {\n"
			"       for(int x=-1; x<=1;x++)\n"
			"        {\n"
			"  				 float closeDepth = texture2D(sampler,(proj+vec3(float(x)*u_xPixelOffset,float(y)*u_yPixelOffset,0.05)).xy).x;\n "
			"                shadow += (closeDepth<(currentDepth-bias))?0.5:1.0;\n"
			"         }\n"
			"    }\n"
			"   shadow /=16.0;\n"
			"   shadow += 0.2;\n"
			"   return shadow;\n"
			"}\n"
			"#endif \n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			"    if(u_useTex)\n"
			"    {\n"
			"        switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"    }\n"
			"    if(u_useAmbientTex)\n"
			"    {\n"
			"        vec2 sphereCoords = sphereMap(v_normal,v_position);\n"
			"        environment = texture2D(u_sampler1,sphereCoords.xy);\n"
			"    }\n"
			"    if(u_useCubeMapTex)\n"
			"    {\n"
			"        environment = textureCube(u_samplerCube0,v_cubeTexCoords.xyz);\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment;\n"
			" 	float shadow = 1.0;\n"
			"#ifdef FBO \n"
			"	 shadow = ShadowCalculate(u_sampler2,v_lightSpacePos);\n"
			"#endif \n"
			"	float alpha = u_diffuse.a;\n"
			"    if(gl_FrontFacing)\n"
			"    {\n"
			"        gl_FragColor = vec4(vec3(texColor)*v_diffuseColor*(shadow)*vec3(u_selectColor)+vec3(v_specularColor)*(shadow),alpha);\n"
			"    }\n"
			"    else\n"
			"    {\n"
			"        gl_FragColor = vec4(vec3(texColor)*v_diffuseColorN*(shadow)*vec3(u_selectColor)+vec3(v_specularColorN)*(shadow),alpha);\n"
			"    }\n"
			"}\n"
			"\n"

			;
	}

	const char *ShaderSrcCode::BaseVertexVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_specular;\n"
			"vec3 vviewDirection;\n"
			"vec3 vlightDirection;\n"
			"vec3 vnormal;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec4 v_diffuseColor;\n"
			"varying vec4 v_specularColor;\n"
			"varying vec4 v_diffuseColorN;\n"
			"varying vec4 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"const vec4 ambientColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"const vec4 diffuseColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"uniform float u_shininess;\n"
			"const vec4 specularColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"void main() {\n"
			"gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"v_texCoords = tempCoords;\n"
			"vviewDirection  = normalize( -fvObjectPosition.xyz);\n"
			"vlightDirection = normalize(u_lightPosition-fvObjectPosition.xyz);\n"
			"vnormal         = normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"v_normal = vnormal;\n"
			"float fNDotL =0.0;\n"
			"vec3 halfvector;\n"
			"float fNDotH;\n"
			"fNDotL           = clamp(abs(dot( vnormal, vlightDirection )),0.3,1.0);\n"
			"halfvector        =normalize(vlightDirection+ vviewDirection );\n"
			"fNDotH            = dot(vnormal,halfvector);\n"
			"vec4  fvTotalAmbient   = u_ambient * ambientColor;\n"
			"vec4  fvTotalDiffuse   = switchColor*u_diffuse * fNDotL * diffuseColor;\n"
			"vec4  fvTotalSpecular  = u_specular * specularColor *( pow( max(fNDotH,0.0), u_shininess ) );\n"
			"v_diffuseColor=min(fvTotalAmbient + fvTotalDiffuse,vec4(1.0));\n"
			"v_specularColor = min(fvTotalSpecular,vec4(1.0));\n"
			"\n"
			"fNDotL           = clamp(abs(dot( -vnormal, vlightDirection )),0.3,1.0);\n"
			"fNDotH            = dot(-vnormal,halfvector);\n"
			"fvTotalDiffuse   = switchColor*u_diffuse * fNDotL * diffuseColor;\n"
			"fvTotalSpecular  = u_specular * specularColor *( pow( max(fNDotH,0.0), u_shininess ) );\n"
			"v_diffuseColorN=min(fvTotalAmbient + fvTotalDiffuse,vec4(1.0));\n"
			"v_specularColorN = min(fvTotalSpecular,vec4(1.0));\n"
			""
			"}\n"
			"\n"

			;
	}
	const char *ShaderSrcCode::BaseVertexFrag()
	{
		return

			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec4 v_diffuseColor;\n"
			"varying vec4 v_specularColor;\n"
			"varying vec4 v_diffuseColorN;\n"
			"varying vec4 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0);\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			"    if(u_useTex){\n"
			"          switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"                }\n"
			"	vec4 texColor = switchColor *environment;\n"
			"    if(gl_FrontFacing){\n"
			"    	  gl_FragColor = vec4(vec3(texColor*v_diffuseColor)*vec3(u_selectColor)+vec3(v_specularColor),u_diffuse.a+1.0);\n"
			"    }else{\n"
			"         gl_FragColor = vec4(vec3(texColor*v_diffuseColorN)*vec3(u_selectColor)+vec3(v_specularColorN),u_diffuse.a+1.0);\n"
			"    }\n"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::ShadowMapVert()
	{
		return "attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ShadowMapFrag()
	{
		return

			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"gl_FragColor = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1.0);\n"
			"}\n";
	}

	const char *ShaderSrcCode::FBODebugVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}
	const char *ShaderSrcCode::FBODebugFrag()
	{
		return "//precision highp  float;\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"vec4 pcolor = texture2D(u_sampler0,v_texCoords);\n"
			"vec4 tcolor = pcolor;\n"
			"gl_FragColor = tcolor;\n"
			"gl_FragColor = vec4(vec3(tcolor),1.0);\n"
			"}\n";
	}

	const char *ShaderSrcCode::PlaneShadowVert()
	{
		return

			"#define FBO \n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"#ifdef FBO \n"
			"uniform mat4 u_lightMat;\n"
			"uniform mat4 u_lightProject;\n"
			"#endif \n"
			"\n"
			"varying vec3 v_position;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"#endif \n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec3 lightDirection;\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"#ifdef FBO \n"
			"    v_lightSpacePos = u_lightProject*u_lightMat *u_modelMat* vec4(a_position,1.0);\n"
			"    v_worldNormal = a_normal;\n"
			"#endif \n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    v_worldPos = worldPos.xyz;\n"
			" }\n";
	}
	const char *ShaderSrcCode::PlaneShadowFrag()
	{
		return

			"#define FBO \n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"varying vec3 v_position;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform vec3 u_shadowLPos;\n"
			"uniform float u_xPixelOffset;\n"
			"uniform float u_yPixelOffset;\n"
			"#endif \n"
			"float ShadowCalculate(sampler2D sampler,vec4 fragPosLightSpace)\n"
			"{\n"
			"   float shadow = 1.0;\n"
			"	vec3 proj =fragPosLightSpace.xyz/fragPosLightSpace.w;\n"
			"   proj = proj *0.5+0.5;\n"
			"  if(proj.x>1.0 || proj.x<0.0 || proj.y>1.0 || proj.y<0.0) return 1.0;\n"
			"   float currentDepth = proj.z;\n"
			"   vec3 normal = normalize(v_worldNormal);\n"
			"   vec3 lightDir = normalize(u_shadowLPos-v_worldPos);\n"
			"   float bias = max(0.05*(1.0-dot(normal,lightDir)),0.005);\n"
			"   for(int y=-1; y<=1;y++)\n"
			"   {\n"
			"       for(int x=-1; x<=1;x++)\n"
			"        {\n"
			"  				 float closeDepth = texture2D(sampler,(proj+vec3(float(x)*u_xPixelOffset,float(y)*u_yPixelOffset,0.05)).xy).x;\n "
			"                shadow += (closeDepth<(currentDepth-bias+bias*0.99))?0.5:1.0;\n"
			"         }\n"
			"    }\n"
			"   shadow /=16.0;\n"
			"   return shadow;\n"
			"}\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			" 	float shadow = 1.0;\n"
			"#ifdef FBO \n"
			"	 shadow = ShadowCalculate(u_sampler0,v_lightSpacePos);\n"
			"#endif \n"
			"float alpha = 1.0;\n"
			"if(shadow < 1.0 && shadow>0.62) alpha = 0.0;\n"
			"    gl_FragColor = vec4(u_diffuse.rgb*shadow,u_diffuse.a *(1.0-shadow)*alpha);\n"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::BlurVert()
	{
		return

			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}
	const char *ShaderSrcCode::BlurFrag()
	{
		return "#extension GL_OES_standard_derivatives : enable\n"

			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"varying vec2 v_texCoords;\n"
			"const float v = 1.0/254.0;\n"
			" float weights[39] ; \n"
			"float samples[26] ;\n"
			"void main(void)\n"
			"{\n"
			" weights[0] = 0.250441;\n"
			" weights[1] = 0.437000;\n weights[2] =  0.635000;\n"
			" weights[3] =0.096356;\n weights[4] =0.064487;\n  weights[5] =0.039097;\n"
			" weights[6] =0.136575;\n weights[7] = 0.103222;\n weights[8] = 0.064912;\n"
			"weights[9] = 0.074844;\n  weights[10] =0.086388;\n weights[11] =0.062272;\n"
			" weights[12] =0.131798;\n  weights[13] =0.151695;\n weights[14] =0.103676;\n"
			" weights[15] = 0.035690;\n  weights[16] =0.042728;\n weights[17] =0.033003;\n"
			"  weights[18] =0.048593;\n  weights[19] =0.064740;\n  weights[20] =0.046131;\n"
			" weights[21] =0.048092;\n  weights[22] =0.003042;\n weights[23] =0.000400;\n"
			"weights[24] =0.028845;\n  weights[25] =0.005406;\n  weights[26] =0.001222;\n"
			"  weights[27] =0.031322;\n  weights[28] =0.006034;\n  weights[29] =0.001420;\n"
			" weights[30] =0.031428;\n weights[31] =0.009152;\n weights[32] =0.002511;\n"
			"  weights[33] =0.010936;\n weights[34] =0.002868;\n  weights[35] =0.000652;\n"
			" weights[36] =0.033588;\n weights[37] =0.023239;\n  weights[38] =0.009703;\n"
			" samples[0] =0.000000;\n  samples[1] =0.000000;\n"
			" samples[2] =1.633992;\n samples[3] =0.036795;\n"
			"  samples[4] =0.177801;\n  samples[5] =1.717593;\n"
			" samples[6] =-0.194986;\n  samples[7] =0.091894;\n"
			" samples[8] =-0.239737;\n  samples[9] =-0.220217;\n"
			"samples[10] =0.003530;\n  samples[11] =-0.118219;\n"
			" samples[12] =1.32010;\n  samples[13] =-0.181542;\n"
			"samples[14] =5.970690;\n  samples[15] =0.253378;\n"
			"samples[16] =-1.089250;\n samples[17] = 4.958349;\n"
			" samples[18] = -4.015465;\n samples[19] = 4.156699;\n"
			"samples[20] =-4.063099;\n  samples[21] =-4.110150;\n"
			" samples[22] =-0.638605;\n  samples[23] =-6.297663;\n"
			" samples[24] =2.542348;\n  samples[25] =-3.245901;\n"
			"    vec3 sum = vec3( 0.0 );\n"
			"    sum.r += texture2D( u_sampler0, vec2( v_texCoords.s + samples[0] * v, v_texCoords.t + samples[1] * v ) ).r * weights[0];\n"
			"    sum.g += texture2D( u_sampler0, vec2( v_texCoords.s + samples[0] * v, v_texCoords.t + samples[1] * v ) ).g * weights[1];\n"
			"    sum.b += texture2D( u_sampler0, vec2( v_texCoords.s + samples[0] * v, v_texCoords.t + samples[1] * v ) ).b * weights[2];\n"
			"    for(int i = 1;i< 13 ;i++)\n"
			"    {\n"
			"        vec2 samplesTmp = vec2(samples[2*i],samples[2*i+1]);\n"
			"        vec3 weightsTmp = vec3(weights[3*i],weights[3*i+1],weights[3*i+2]);\n"
			"        float u_sampler1 = texture2D(u_sampler1, vec2( v_texCoords.s + samplesTmp.x * v, v_texCoords.t + samplesTmp.y * v ) ).r;\n"
			"        float dx = 1.0 - dFdx(u_sampler1);\n"
			"        float dy = 1.0 - dFdy(u_sampler1);\n"
			"        sum.r += texture2D( u_sampler0, vec2( v_texCoords.s + samplesTmp.x * v * dx, v_texCoords.t + samplesTmp.y * v * dy ) ).r * weightsTmp.r;\n"
			"        sum.g += texture2D( u_sampler0, vec2( v_texCoords.s + samplesTmp.x * v * dx, v_texCoords.t + samplesTmp.y * v * dy ) ).g * weightsTmp.g;\n"
			"        sum.b += texture2D( u_sampler0, vec2( v_texCoords.s + samplesTmp.x * v * dx, v_texCoords.t + samplesTmp.y * v * dy ) ).b * weightsTmp.b;\n"
			"    }\n"
			"\n"
			"    gl_FragColor = vec4(vec3(sum),1.0);\n"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::SpecularVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"}\n";
	}
	const char *ShaderSrcCode::SpecularFrag()
	{
		return "#extension GL_OES_standard_derivatives : enable\n"

			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_selectColor;\n"
			"uniform vec3 u_lightPosition;\n"
			"\n"
			"float beckmannDistribution(in float x, in float roughness)\n"
			"{\n"
			"    float NdotH = max(x, 0.0001);\n"
			"    float cos2Alpha = NdotH * NdotH;\n"
			"    float tan2Alpha = (cos2Alpha - 1.0) / cos2Alpha;\n"
			"    float roughness2 = roughness * roughness;\n"
			"    float denom = 3.1415926 * roughness2 * cos2Alpha * cos2Alpha *0.05;\n"
			"    return exp(tan2Alpha / roughness2) / denom;\n"
			"}\n"
			"\n"
			"float cookTorranceSpecular(in vec3 lightDirection, in vec3 viewDirection, in vec3 surfaceNormal, in float roughness, in float fresnel)\n"
			"{\n"
			"    float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);\n"
			"    float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);\n"
			"    vec3 H = normalize(lightDirection + viewDirection);\n"
			"    float NdotH = max(dot(surfaceNormal, H), 0.0);\n"
			"    float VdotH = max(dot(viewDirection, H), 0.000001);\n"
			"    float LdotH = max(dot(lightDirection, H), 0.000001);\n"
			"    float G1 = (2.0 * NdotH * VdotN) / VdotH;\n"
			"    float G2 = (2.0 * NdotH * LdotN) / LdotH;\n"
			"    float G = min(1.0, min(G1, G2));\n"
			"    float D = beckmannDistribution(NdotH, roughness);\n"
			"    float F = fresnel;\n"
			"    return  G * F * D / (3.14159265 *max(VdotN, 0.000001)*max(LdotN, 0.000001));\n"
			"}\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			"if(gl_FrontFacing)"
			"{"
			"    float specular = cookTorranceSpecular(normalize(u_lightPosition), -normalize(v_position), v_normal, 0.027, 0.04);\n"
			"    vec3 diff = vec3(max(dot(v_normal,normalize(u_lightPosition)),0.0));\n"
			"    gl_FragColor = vec4(mix(u_ambient.xyz,vec3(diff)* u_diffuse.xyz*vec3(u_selectColor),0.95) + vec3(specular), u_diffuse.a);\n"
			"}"
			"else"
			"{"
			"    float specular = cookTorranceSpecular(normalize(u_lightPosition), -normalize(v_position),- v_normal, 0.027, 0.04);\n"
			"    vec3 diff = vec3(max(abs(dot(-v_normal,normalize(u_lightPosition))),0.0));\n"
			"    gl_FragColor = vec4(mix(u_ambient.xyz,vec3(diff)* u_diffuse.xyz*vec3(u_selectColor),0.95) + vec3(specular), u_diffuse.a);\n"
			"}"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::EdgeDetectionVert()
	{
		return "//precision highp  float;"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}
	const char *ShaderSrcCode::EdgeDetectionFrag()
	{
		return "//precision highp  float;"
			"uniform sampler2D u_sampler0;\n"
			"uniform float u_xPixelOffset;\n"
			"uniform float u_yPixelOffset;\n"
			"varying vec2 v_texCoords;\n"
			"void main( void )\n"
			"{\n"
			"float _Size = 1024.0;\n"
			"   vec3 lum = vec3(0.2125,0.7154,0.0721);\n"
			"     float mc00 = dot(texture2D (u_sampler0, v_texCoords-vec2(1.0*u_xPixelOffset,1.0*u_yPixelOffset)).rgb, lum);\n"
			"     float mc10 = dot(texture2D (u_sampler0, v_texCoords-vec2(0.0,1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc20 = dot(texture2D (u_sampler0, v_texCoords-vec2(-1.0,1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc01 = dot(texture2D (u_sampler0, v_texCoords-vec2(1.0,0.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc11mc = dot(texture2D (u_sampler0, v_texCoords).rgb, lum);\n"
			"     float mc21 = dot(texture2D (u_sampler0, v_texCoords-vec2(-1.0,0.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc02 = dot(texture2D (u_sampler0, v_texCoords-vec2(1.0,-1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc12 = dot(texture2D (u_sampler0, v_texCoords-vec2(0.0,-1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc22 = dot(texture2D (u_sampler0, v_texCoords-vec2(-1.0,-1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"\n"
			"     float GX = -1.0 * mc00 + mc20 + -2.0 * mc01 + 2.0 * mc21 - mc02 + mc22;\n"
			"     float GY = mc00 + 2.0 * mc10 + mc20 - mc02 - 2.0 * mc12 - mc22;\n"
			"     vec4 c;\n"
			"     float gratValue =length(vec2(GX,GY));\n "
			"     c = vec4(gratValue,gratValue,gratValue,1.0);\n"
			"\n"
			"      float factor = 1.0-c.x;\n"
			"      if(factor>0.92)\n"
			"      {\n"
			"      factor = 1.0;\n"
			"      }\n"
			"      else\n"
			"      {\n"
			"      factor = 0.0;\n"
			"      }\n"
			"   if(texture2D (u_sampler0, v_texCoords).a<0.33)"
			"{gl_FragColor = vec4(factor,0.0,0.0,1.0);}\n"
			"else"
			"{ gl_FragColor = vec4(factor,factor,factor,1.0);\n}"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::QuadVert()
	{
		return "//precision highp  float;\n"
			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}
	const char *ShaderSrcCode::QuadFrag()
	{
		return "//precision highp  float;\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"gl_FragColor =texture2D(u_sampler0,v_texCoords);\n"
			"}\n";
	}
	const char *ShaderSrcCode::QuadFragMediump()
	{
		return "precision mediump  float;\n"
			"precision mediump sampler2D;"
			"uniform sampler2D u_sampler0;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"gl_FragColor =texture2D(u_sampler0,v_texCoords);\n"
			"}\n";
	}
	const char *ShaderSrcCode::MultilightPerFragVert()
	{
		return

			"//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    v_cubeTexCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(a_normal));\n"
			" }\n";
	}
	const char *ShaderSrcCode::MultilightPerFragFrag()
	{
		return

			"//precision highp  float;\n"
			"//precision highp  int;\n"
			"//precision mediump sampler2D;\n"
			"//precision mediump samplerCube;\n"
			"struct Light\n"
			"{\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    vec4 position;\n"
			"    vec3 intensity;\n"
			"    vec3 spotDirection;\n"
			"    float spotExponent;\n"
			"    float spotCutoff; // (range: [0.0,90.0], 180.0)\n"
			"    float spotCosCutoff; // (range: [1.0,0.0],-1.0)\n"
			"    float constant;\n"
			"    float linear;\n"
			"    float quadratic;\n"
			"};\n"
			"\n"
			"struct LightModel\n"
			"{\n"
			"    vec4 ambient;    // Acs\n"
			"};\n"
			"\n"
			"struct Materials\n"
			"{\n"
			"    vec4 emission;\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    float shininess;\n"
			"};\n"
			"uniform LightModel u_lightModel ;\n"
			"const int numberOfLights = 3;\n"
			"uniform Light u_lights[3];\n"
			"uniform int u_enbleLights[3];\n"
			"uniform Materials u_materials;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform bool u_useAmbientTex;\n"
			"uniform bool u_useCubeMapTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"vec3 OptimizedCineonToneMapping(vec3 color ) {\n"

			// optimized filmic operator by Jim Hejl and Richard Burgess-Dawson
			"  // color *= toneMappingExposure;\n"
			"  color = max(vec3(0.0), color - 0.004);\n"
			"   return pow((color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06), vec3(2.2));\n"

			"}\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"float testF = 0.0;\n"
			"    vec3 testColor= vec3(0.7,0.0,0.0);"
			"    vec3 totalLighting = vec3(u_materials.emission)+ vec3(u_lightModel.ambient) * vec3(u_materials.ambient);\n"
			" vec3 diffuseColor;\n"
			" vec3 specularColor;\n"
			" vec3  vNormal ;\n"
			"if(gl_FrontFacing){\n"
			" vNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" vNormal         = normalize( -v_normal );\n"
			"}\n"

			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"//if (u_enableClip[2] == 1) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"

			"    if(u_useTex)\n"
			"    {\n"
			"        switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"    }\n"
			"    if(u_useAmbientTex)\n"
			"    {\n"
			"        vec2 sphereCoords = sphereMap(v_normal,v_position);\n"
			"        environment = texture2D(u_sampler1,sphereCoords.xy);\n"
			"    }\n"
			"    if(u_useCubeMapTex)\n"
			"    {\n"
			"        environment = textureCube(u_samplerCube0,v_cubeTexCoords.xyz);\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment;\n"
			"	vec3 eyeDirection = (u_projectionMat[3][3] == 0.0)?normalize(fvObjectPosition.xyz):vec3(0.0, 0.0, -1.0);\n"
			"    for(int index = 0;index<numberOfLights;index++)\n"
			"    {\n"
			"        if(u_enbleLights[index] == 1)\n"
			"        {\n"

			"            if (0.0 == u_lights[index].position.w) // directional light?\n"
			"            {\n"

			"                attenuation = 1.0; // no attenuation\n"
			"                lightDirection = normalize(vec3(u_lights[index].position));\n"
			"            }\n"
			"			else{\n"
			"					vec3 d = u_lights[index].position.xyz -fvObjectPosition.xyz;\n "
			"testColor = normalize(d);\n"
			"					lightDirection = normalize(d);\n"
			"					if (u_lights[index].spotCutoff<90.0)\n"
			"					{\n"
			"						float cosine = max(dot(lightDirection, -u_lights[index].spotDirection), 0.0);\n"
			"						attenuation = pow(cosine, u_lights[index].spotExponent);\n"
			"						attenuation *= step(u_lights[index].spotCosCutoff, cosine);\n"
			"					}\n"
			"					float distance = length(d);\n"
			"testF = distance;\n"
			"					attenuation /= u_lights[index].constant +\n"
			"						u_lights[index].linear * distance +\n"
			"						u_lights[index].quadratic * distance * distance;\n"
			"				}\n"
			"        float fNDotL= clamp((dot( vNormal, lightDirection )),0.0,1.0);\n"
			"        vec3 diffuseReflection = attenuation\n"
			"            * vec3(u_lights[index].diffuse) \n"
			"            * fNDotL;\n"
			"	totalDiffuse += diffuseReflection;\n"
			"		  float intensity = u_lights[0].intensity.x;\n"
			"        vec3 halfVector = normalize(lightDirection- eyeDirection);\n"
			"        float fNDotH = max(0.00001, (dot(vNormal,halfVector)));\n"
			"        vec3 specularReflection = attenuation * vec3(u_lights[index].specular) \n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"         totalSpecular = totalSpecular + specularReflection;\n"
			"        }\n"
			"    }\n"
			"vec3 scenecolor = vec3(0.2, 0.2, 0.2);\n"
			"         totalAmbient =vec3(totalAmbient) + (vec3(u_lights[0].ambient*0.00001)+scenecolor) * vec3(u_materials.diffuse);\n"
			"    diffuseColor = min(totalLighting+totalAmbient+totalDiffuse* vec3(u_materials.diffuse),vec3(1.0));\n"
			"    specularColor= min(totalSpecular* vec3(u_materials.specular),vec3(1.0));\n"
			"    float alpha = u_diffuse.a;"
			"if(u_enbleLights[1] == 1) testF = 1.0;\n else\n  testF = 0.0;\n"
			"    gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)/**0.0000001+vec3(testF,0.0,0.0)*/+vec3(specularColor),alpha);\n"

			//			"   gl_FragColor.rgb = OptimizedCineonToneMapping(gl_FragColor.rgb);\n"
			" }\n";
	}

	const char *ShaderSrcCode::MultilightPerFragFragMediump()
	{
		return

			"precision mediump  float;\n"
			"precision mediump  int;\n"
			"precision mediump sampler2D;\n"
			"precision mediump samplerCube;\n"
			"struct Light\n"
			"{\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    vec4 position;\n"
			"    vec3 intensity;\n"
			"    vec3 spotDirection;\n"
			"    float spotExponent;\n"
			"    float spotCutoff; // (range: [0.0,90.0], 180.0)\n"
			"    float spotCosCutoff; // (range: [1.0,0.0],-1.0)\n"
			"    float constant;\n"
			"    float linear;\n"
			"    float quadratic;\n"
			"};\n"
			"\n"
			"struct LightModel\n"
			"{\n"
			"    vec4 ambient;    // Acs\n"
			"};\n"
			"\n"
			"struct Materials\n"
			"{\n"
			"    vec4 emission;\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    float shininess;\n"
			"};\n"
			"uniform LightModel u_lightModel ;\n"
			"const int numberOfLights = 1;\n"
			"uniform Light u_lights[2];\n"
			"uniform int u_enbleLights[1];\n"
			"uniform Materials u_materials;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform bool u_useAmbientTex;\n"
			"uniform bool u_useCubeMapTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"    vec3 testColor= vec3(0.7,0.0,0.0);"
			"    vec3 totalLighting = vec3(u_materials.emission)+ vec3(u_lightModel.ambient) * vec3(u_materials.ambient);\n"
			" vec3 diffuseColor;\n"
			" vec3 specularColor;\n"
			" vec3  vNormal ;\n"
			"if(gl_FrontFacing){\n"
			" vNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" vNormal         = normalize( -v_normal );\n"
			"}\n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"

			"    if(u_useTex)\n"
			"    {\n"
			"        switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"    }\n"
			"    if(u_useAmbientTex)\n"
			"    {\n"
			"        vec2 sphereCoords = sphereMap(v_normal,v_position);\n"
			"        environment = texture2D(u_sampler1,sphereCoords.xy);\n"
			"    }\n"
			"    if(u_useCubeMapTex)\n"
			"    {\n"
			"        environment = textureCube(u_samplerCube0,v_cubeTexCoords.xyz);\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment;\n"
			"    for(int index = 0;index<numberOfLights;index++)\n"
			"    {\n"
			"        if(u_enbleLights[index] == 1)\n"
			"        {\n"

			"            if (0.0 == u_lights[index].position.w) // directional light?\n"
			"            {\n"

			"                attenuation = 1.0; // no attenuation\n"
			"                lightDirection = normalize(vec3(u_lights[index].position)/*- fvObjectPosition.xyz*/);\n"
			"            }\n"
			"        float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.2,1.0);\n"
			"        vec3 diffuseReflection = attenuation\n"
			"            * vec3(u_lights[index].diffuse) * vec3(u_materials.diffuse)\n"
			"            * fNDotL;\n"
			"         totalAmbient =vec3(totalAmbient) + vec3(u_lights[index].ambient) * vec3(u_materials.ambient);\n"
			"		  float intensity = u_lights[index].intensity.x;\n"
			"         totalDiffuse = totalDiffuse + diffuseReflection*intensity;\n"
			"        vec3 eyeDirection =vec3(0,0,1); /*normalize(- fvObjectPosition.xyz );*/\n"
			"        vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"        float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"        vec3 specularReflection = attenuation * vec3(u_lights[index].specular) * vec3(u_materials.specular)\n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"         totalSpecular = totalSpecular + specularReflection*intensity;\n"
			"        }\n"
			"    }\n"
			"    diffuseColor = min(totalLighting+totalAmbient+totalDiffuse,vec3(1.0));\n"
			"    specularColor= min(totalSpecular,vec3(1.0));\n"
			"    gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);\n"
			" }\n";
	}

	const char *ShaderSrcCode::CapPlaneVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 fvObjectPosition;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			" }\n";
	}

	const char *ShaderSrcCode::CapPlaneFrag()
	{
		return "uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"

			"uniform mat4 u_projectionMat;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 fvObjectPosition;\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	vec3 diffuseColor;\n"
			"	vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal         = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal         = normalize( -v_normal );\n"
			"	}\n"

			"	if (u_reverseClip) {\n"
			"		int iEnableNum = 0; \n"
			"		int iClipNum = 0; \n"
			"	for (int i = 0; i < 3; i++) {\n"
			"		if (u_enableClips[i]) {\n"
			"			iEnableNum++; \n"
			"			float dPara = u_clipPlanes[i].w; \n"
			"			if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"				iClipNum++; \n"
			"			}\n"
			"		}\n"
			"	}\n"
			"	if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"		discard; \n"
			"	}\n"
			"}\n"
			"else {\n"
			"	for (int i = 0; i < 3; i++) {\n"
			"		if (u_enableClips[i]) {\n"
			"			float dPara = u_clipPlanes[i].w; \n"
			"				if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"					discard; \n"
			"				}\n"
			"			}\n"
			"		}\n"
			"}\n"

			"	vec3 eyeDirection = (u_projectionMat[3][3] == 0.0)?normalize(fvObjectPosition.xyz):vec3(0.0, 0.0, -1.0);\n"
			"   attenuation = 1.0; // no attenuation\n"
			"   lightDirection = normalize(vec3(0.0,0.0,1.0));\n"
			"   float fNDotL= clamp((dot( vNormal, lightDirection )),0.2,1.0);\n"
			"   vec3 diffuseReflection = attenuation\n"
			"            * fNDotL*vec3(1.0,1.0,1.0);\n"
			"	totalDiffuse += diffuseReflection;\n"
			" //  vec3 halfVector = normalize(lightDirection- eyeDirection);\n"
			" //  float fNDotH = max(0.00001, (dot(vNormal,halfVector)));\n"
			"//   vec3 specularReflection = attenuation  \n"
			"  //             * pow(fNDotH, 60.0);\n"
			" //  totalSpecular = totalSpecular + specularReflection;\n"
			"	//vec3 scenecolor = vec3(0.2, 0.2, 0.2);\n"
			"    diffuseColor = min(totalDiffuse* vec3(u_diffuse),vec3(1.0));\n"
			"   // specularColor= min(totalSpecular* vec3(1.0,1.0,1.0),vec3(1.0));\n"
			"    float alpha = u_diffuse.a;"
			"    gl_FragColor = vec4(diffuseColor/*+vec3(specularColor)*/,alpha);\n"
			" }\n";
	}


	//zhubao 
	const char * ShaderSrcCode::JewelFrontVert()
	{
		return 	"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform mat4 u_worldNormalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz;\n"
			"    v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    eyeWorldDirection =normalize( u_eyePosition -worldPosT);\n "
			"    lightWorldDirection =normalize( u_lightPosition -worldPosT);\n "
			" vec3 worldNormal = vec3(u_worldNormalMat*vec4(a_normal,0.0));\n"
			"	v_worldNormal = worldNormal;\n"
			"    v_cubeRefractCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(worldNormal));\n"
			" }\n";
	}
	const char * ShaderSrcCode::JewelFrontFrag()
	{
		return

			"uniform bool u_useFrontTexture;\n"
			"uniform bool u_useHighlightTexture;\n"
			"uniform bool u_useFrontCubeTexture;\n"
			"uniform sampler2D frontTexture;\n"
			"uniform sampler2D highlightTexture;\n"
			"uniform samplerCube frontCubeTexture;\n"
			"uniform bool u_keepDirection;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment1 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment2 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 highLightColor = vec4(0.0,0.0,0.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal = normalize( -v_normal );\n"
			"	}\n"
			"   if(u_useFrontTexture == true)\n"
			"    {\n"
			"       vec2 sphereCoords = vec2(1.0,1.0);\n"
			"       vec2 matCapCoords = vec2(1.0,1.0);\n"
			"       if(u_keepDirection == true){"
			"         sphereCoords = sphereMap(normalize(v_worldNormal),v_position);\n"
			"		 matCapCoords = v_worldNormal.xy;}\n"
			"          else{"
			"         sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			" matCapCoords = v_normal.xy;}\n"
			"         matCapCoords = matCapCoords*0.5+0.5;"
			"        environment1 =texture2D(frontTexture,matCapCoords.xy);\n"
			"    }\n"
			"	if(u_useHighlightTexture == true)"
			"   {"
			"        vec2 sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			"        highLightColor =texture2D(highlightTexture,sphereCoords.xy);\n"
			"    }"
			"    if(u_useFrontCubeTexture == true)\n"
			"    {\n"
			"        environment2 = textureCube(frontCubeTexture,v_cubeRefractCoords.xyz);\n"
			"		 environment2.rbg = pow(environment2.rbg,vec3(1.0/1.7));\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment1*environment2;\n"
			"    lightDirection = lightWorldDirection;\n"
			"    float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"    vec3 diffuseReflection = \n"
			"              vec3(1.0,1.0,1.0)\n"
			"            * fNDotL;\n"
			"    totalDiffuse =  diffuseReflection;\n"
			"    vec3 eyeDirection =eyeWorldDirection;\n"
			"    vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"    float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"    vec3 specularReflection =  vec3(1.0)\n"
			"               * pow(fNDotH,10.0);\n"
			"vec3 lightDirection1 = normalize(vec3(vec4(1.0,0.0,0.0,0.0)) - v_position);\n"
			"vec3 lightDirection2 = normalize(vec3(vec4(0.0,-1.0,0.0,0.0))-v_position);\n"
			"vec3 lightDirection3 = normalize(vec3(vec4(0.0,0.0,1.0,0.0)) - v_position);\n"
			"    vec3 eyeDirection1 =vec3(0.0,0.0,0.0)-v_position;\n"
			"    vec3 halfVector1 = normalize(lightDirection1+ eyeDirection1);\n"
			"    float fNDotH1 = max(0.0, (dot(vNormal,halfVector1)));\n"
			"    vec3 specularReflection1 =  vec3(1.0)\n"
			"               * pow(fNDotH1,50.0);\n"
			"    vec3 eyeDirection2 =eyeDirection1;\n"
			"    vec3 halfVector2 = normalize(lightDirection2+ eyeDirection2);\n"
			"    float fNDotH2 = max(0.0, (dot(vNormal,halfVector2)));\n"
			"    vec3 specularReflection2 =  vec3(1.0)\n"
			"               * pow(fNDotH2,50.0);\n"
			"    vec3 eyeDirection3 =eyeDirection1;\n"
			"    vec3 halfVector3 = normalize(lightDirection3+ eyeDirection3);\n"
			"    float fNDotH3 = max(0.0, (dot(vNormal,halfVector3)));\n"
			"    vec3 specularReflection3 =  vec3(1.0)\n"
			"               * pow(fNDotH3,50.0);\n"
			"    totalSpecular = totalSpecular + (specularReflection+specularReflection1+specularReflection2+specularReflection3);\n"
			"    diffuseColor = min(totalDiffuse,vec3(1.0));\n"
			"    specularColor= min(totalSpecular,vec3(1.0));\n"
			"	if(u_useFrontTexture||u_useFrontCubeTexture)"
			"	 {"
			"       if(u_useHighlightTexture == true){"
			"		gl_FragColor = vec4(vec3(texColor)*u_diffuse.xyz*vec3(u_selectColor)+highLightColor.rgb,u_diffuse.a);\n"
			"       }"
			"   else{"
			"		gl_FragColor = vec4(vec3(texColor)*u_diffuse.xyz*vec3(u_selectColor),u_diffuse.a);"
			"       }\n"
			"		 gl_FragColor.rbg = pow(gl_FragColor.rbg,vec3(1.0/1.2));\n"
			"	}\n"
			"    else {gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);}\n"
			"	}\n";
	}

	const char * ShaderSrcCode::JewelBackVert()
	{
		return 	"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform mat4 u_worldNormalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz;\n"
			"    v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    eyeWorldDirection =normalize( u_eyePosition -worldPosT);\n "
			"    lightWorldDirection =normalize( u_lightPosition -worldPosT);\n "
			" vec3 worldNormal = vec3(u_worldNormalMat*vec4(a_normal,0.0));\n"
			"	v_worldNormal = worldNormal;\n"
			"    v_cubeRefractCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(worldNormal));\n"
			" }\n";
	}
	const char * ShaderSrcCode::JewelBackFrag()
	{
		return

			"uniform bool u_useBackTexture;\n"
			"uniform bool u_useHighlightTexture;\n"
			"uniform bool u_useBackCubeTexture;\n"
			"uniform sampler2D backTexture;\n"
			"uniform sampler2D highlightTexture;\n"
			"uniform samplerCube backCubeTexture;\n"
			"uniform bool u_keepDirection;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment1 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment2 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 highLightColor = vec4(0.0,0.0,0.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal = normalize( -v_normal );\n"
			"	}\n"
			"   if(u_useBackTexture == true)\n"
			"    {\n"
			"       vec2 sphereCoords = vec2(1.0,1.0);\n"
			"       vec2 matCapCoords = vec2(1.0,1.0);\n"
			"       if(u_keepDirection == true){"
			"         sphereCoords = sphereMap(normalize(v_worldNormal),v_position);\n"
			"		 matCapCoords = v_worldNormal.xy;}\n"
			"          else{"
			"         sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			" matCapCoords = v_normal.xy;}\n"
			"         matCapCoords = matCapCoords*0.5+0.5;"
			"        environment1 =texture2D(backTexture,matCapCoords.xy);\n"
			"    }\n"
			"	if(u_useHighlightTexture == true)"
			"   {"
			"        vec2 sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			"        highLightColor =texture2D(highlightTexture,sphereCoords.xy);\n"
			"    }"
			"    if(u_useBackCubeTexture == true)\n"
			"    {\n"
			"        environment2 = textureCube(backCubeTexture,v_cubeRefractCoords.xyz);\n"
			"		 environment2.rbg = pow(environment2.rbg,vec3(1.0/1.7));\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment1*environment2;\n"
			"    lightDirection = lightWorldDirection;\n"
			"    float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"    vec3 diffuseReflection = \n"
			"              vec3(1.0,1.0,1.0)\n"
			"            * fNDotL;\n"
			"    totalDiffuse =  diffuseReflection;\n"
			"    vec3 eyeDirection =eyeWorldDirection;\n"
			"    vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"    float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"    vec3 specularReflection =  vec3(1.0)\n"
			"               * pow(fNDotH,10.0);\n"
			"vec3 lightDirection1 = normalize(vec3(vec4(1.0,0.0,0.0,0.0)) - v_position);\n"
			"vec3 lightDirection2 = normalize(vec3(vec4(0.0,-1.0,0.0,0.0))-v_position);\n"
			"vec3 lightDirection3 = normalize(vec3(vec4(0.0,0.0,1.0,0.0)) - v_position);\n"
			"    vec3 eyeDirection1 =vec3(0.0,0.0,0.0)-v_position;\n"
			"    vec3 halfVector1 = normalize(lightDirection1+ eyeDirection1);\n"
			"    float fNDotH1 = max(0.0, (dot(vNormal,halfVector1)));\n"
			"    vec3 specularReflection1 =  vec3(1.0)\n"
			"               * pow(fNDotH1,50.0);\n"
			"    vec3 eyeDirection2 =eyeDirection1;\n"
			"    vec3 halfVector2 = normalize(lightDirection2+ eyeDirection2);\n"
			"    float fNDotH2 = max(0.0, (dot(vNormal,halfVector2)));\n"
			"    vec3 specularReflection2 =  vec3(1.0)\n"
			"               * pow(fNDotH2,50.0);\n"
			"    vec3 eyeDirection3 =eyeDirection1;\n"
			"    vec3 halfVector3 = normalize(lightDirection3+ eyeDirection3);\n"
			"    float fNDotH3 = max(0.0, (dot(vNormal,halfVector3)));\n"
			"    vec3 specularReflection3 =  vec3(1.0)\n"
			"               * pow(fNDotH3,50.0);\n"
			"    totalSpecular = totalSpecular + (specularReflection+specularReflection1+specularReflection2+specularReflection3);\n"
			"    diffuseColor = min(totalDiffuse,vec3(1.0));\n"
			"    specularColor= min(totalSpecular,vec3(1.0));\n"
			"	if(u_useBackTexture||u_useBackCubeTexture)"
			"	 {"
			"       if(u_useHighlightTexture == true){"
			"		gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor)+highLightColor.rgb,u_diffuse.a);\n"
			"       }"
			"   else{"
			"		gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor),u_diffuse.a);"
			"       }\n"
			"		 gl_FragColor.rbg = pow(gl_FragColor.rbg,vec3(1.0/1.2));\n"
			"	}\n"
			"    else {gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);}\n"
			"	}\n";
	}

	const char * ShaderSrcCode::RingVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform mat4 u_worldNormalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			//"varying vec3 v_cubeRefractCoords;\n"
			"varying vec3 v_worldPos;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 tempCoords = vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz;\n"
			"    v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    v_worldPos = worldPosT;\n"
			"    eyeWorldDirection =normalize( u_eyePosition -worldPosT);\n "
			"    lightWorldDirection =normalize( u_lightPosition -worldPosT);\n "
			" vec3 worldNormal = vec3(u_worldNormalMat*vec4(a_normal,0.0));\n"
			"	v_worldNormal = worldNormal;\n"
			//"    v_cubeRefractCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(worldNormal));\n"
			" }\n";
	}

	const char * ShaderSrcCode::RingFrag()
	{
		return

			"uniform bool u_useFrontTexture;\n"
			"uniform bool u_useFrontCubeTexture;\n"
			"uniform sampler2D frontTexture;\n"
			"uniform samplerCube frontCubeTexture;\n"
			"uniform bool u_useHighlightTexture;\n"
			"uniform sampler2D highlightTexture;\n"
			"uniform sampler2D normalMap;\n"
			"uniform bool u_useBumpMap;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			//"varying vec3 v_cubeRefractCoords;\n"
			"varying vec3 v_worldPos;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment1 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment2 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 highLightColor = vec4(0.0,0.0,0.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"vec2 dHdxy_fwd(in vec2 Texcoord ,in sampler2D bumpMap) {\n"
			"		vec2 dSTdx = dFdx( Texcoord );"
			"		vec2 dSTdy = dFdy( Texcoord );"
			"		float bumpScale = 0.45;\n"
			"		float Hll = bumpScale * texture2D( bumpMap, Texcoord ).x;"
			"		float dBx = bumpScale * texture2D( bumpMap, Texcoord + dSTdx ).x - Hll;"
			"		float dBy = bumpScale * texture2D( bumpMap, Texcoord + dSTdy ).x - Hll;"
			"		return vec2( dBx, dBy );"
			"		}\n"
			"vec3 perturbNormalArb( vec3 surf_pos, vec3 surf_norm, vec2 dHdxy ) {"
			"		vec3 vSigmaX = dFdx( surf_pos );"
			"		vec3 vSigmaY = dFdy( surf_pos );"
			"		vec3 vN = surf_norm;"
			"		vec3 R1 = cross( vSigmaY, vN );"
			"		vec3 R2 = cross( vN, vSigmaX );"
			"		float fDet = dot( vSigmaX, R1 );"
			"		fDet *= ( float( gl_FrontFacing ) * 2.0 - 1.0 );"
			"		vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );"
			"		return normalize( abs( fDet ) * surf_norm - vGrad );"
			"		}	"
			"\n"
			"vec3 perturbNormal2Arb(vec3 eye_pos, vec3 surf_norm,in vec2 Texcoord,in sampler2D normalMap) {\n"

			"vec3 q0 = dFdx(eye_pos.xyz);\n"
			"vec3 q1 = dFdy(eye_pos.xyz);\n"
			"vec2 st0 = dFdx(Texcoord.st);\n"
			"vec2 st1 = dFdy(Texcoord.st);\n"

			"vec3 S = normalize(q0 * st1.t - q1 * st0.t);\n"
			"vec3 T = normalize(-q0 * st1.s + q1 * st0.s);\n"
			"vec3 N = normalize(surf_norm);\n"

			"vec3 mapN = texture2D(normalMap, Texcoord).xyz * 2.0 - 1.0;\n"
			"mapN.xy =/* normalScale*/ vec2(1.0)* mapN.xy;\n"
			"mat3 tsn = mat3(S, T, N);\n"
			"return normalize(tsn * mapN);\n"

			"}\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			//如果使用金属工艺效果的话，按照法线贴图计算normal
			"   if(u_useBumpMap == true && u_useFrontTexture == true)\n"
			"   {\n"
			"		vec3 bumpNormal = vec3(0.0,0.0,0.0);\n"
			//"		bumpNormal = normalize(  perturbNormalArb( v_position, normalize( v_normal ), dHdxy_fwd(v_texCoords.xy*4.0,normalMap) ) );\n"
			"		bumpNormal = normalize(  perturbNormal2Arb( v_position, normalize( v_normal ), v_texCoords.xy*6.0,normalMap ) );\n"
			"		vNormal = bumpNormal;\n"
			"   }\n"
			"   else if(u_useBumpMap == true && u_useFrontCubeTexture == true)\n"
			"   {\n"
			"		vec3 bumpNormal = vec3(0.0,0.0,0.0);\n"
			//"		bumpNormal = normalize(  perturbNormalArb( v_position, normalize( v_normal ), dHdxy_fwd(v_texCoords.xy*4.0,normalMap) ) );\n"
			"		bumpNormal = normalize(  perturbNormal2Arb( v_worldPos, normalize( v_worldNormal ), v_texCoords.xy*6.0,normalMap ) );\n"
			"		vNormal = bumpNormal;\n"
			"   }\n"
			//使用球形映射,使用相机中的normal
			"   else if(u_useFrontTexture == true) \n"
			"   {\n"
			"		if(gl_FrontFacing){\n"
			"			vNormal = normalize( v_normal );\n"
			"		}\n"
			"		else {\n"
			"			vNormal = normalize( -v_normal );\n"
			"		}\n"
			"   }\n"
				//使用立方体映射，使用world中的normal
			"   else if(u_useFrontCubeTexture == true) \n"
			"   {\n"
			"		if(gl_FrontFacing){\n"
			"			vNormal = normalize( v_worldNormal );\n"
			"		}\n"
			"		else {\n"
			"			vNormal = normalize( -v_worldNormal );\n"
			"		}\n"
			"   }\n"
			"   if(u_useFrontTexture == true)\n"
			"    {\n"
			"		vec2 matCapCoords = vec2(1.0, 1.0); \n"
			"		matCapCoords = vNormal.xy;\n"
			"       matCapCoords = matCapCoords*0.5+0.5;"
			"       environment1 =texture2D(frontTexture,matCapCoords.xy);\n"
			"    }\n"
			"    if(u_useFrontCubeTexture == true)\n"
			"    {\n"
			"		 vec3 v_cubeRefractCoords =reflect(normalize(v_worldPos -u_eyePosition), normalize(vNormal));\n"
			"        environment2 = textureCube(frontCubeTexture,v_cubeRefractCoords.xyz);\n"
			"		 environment2.rbg = pow(environment2.rbg,vec3(1.0/1.7));\n"
			"    }\n"
			"	if(u_useHighlightTexture == true)"
			"   {"
			"		vec2 matCapCoords = vec2(1.0, 1.0); \n"
			"		matCapCoords = vNormal.xy;\n"
			"       matCapCoords = matCapCoords*0.5+0.5;"
			"        highLightColor =texture2D(highlightTexture,matCapCoords.xy);\n"
			"    }"
			"    vec4 texColor = switchColor *environment1*environment2;\n"


				"    lightDirection = lightWorldDirection;\n"
				"    float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
				"    vec3 diffuseReflection = \n"
				"              vec3(1.0,1.0,1.0)\n"
				"            * fNDotL;\n"
				"    totalDiffuse =  diffuseReflection;\n"				
				"    diffuseColor = min(totalDiffuse,vec3(1.0));\n"		


			"	if(u_useFrontCubeTexture||u_useFrontTexture)"
			"	 {"
			"       if(u_useHighlightTexture == true){"
				"			gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor)/**vec3(totalDiffuse)*/+highLightColor.rgb,u_diffuse.a);"
			"		}\n"
			"		else{\n"
			"		gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor)/**vec3(totalDiffuse)*/,u_diffuse.a);"
			"		}\n"
				//提亮颜色
			"		 gl_FragColor.rbg = pow(gl_FragColor.rbg,vec3(1.0/1.2));\n"
			"	}\n"
			"    else {gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);}\n"
			"	}\n";
	}

	const char * ShaderSrcCode::JewelTypeVert()
	{
		return
			"attribute  vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"     gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelTypeFrag()
	{
		return
			//该层用来存储钻石、玉石front和back层混合系数。
			"uniform float mixFactor;\n"
			"void main(void)\n"
			"{\n"
			"    gl_FragColor = vec4(0.0,0.0,0.0,mixFactor);\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelHighLightVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			" }\n"
			;
	}

	const char * ShaderSrcCode::JewelHighLightFrag()
	{
		return
			//获取钻石、玉石的高光
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"uniform vec3 u_specular;\n"
			"vec3 lightPoss[4];\n"
			"vec3 lightSpecular[4];\n"
			"void main() { \n"
			"lightPoss[0] = normalize(vec3(-1.0,0.0,1.0));\n "
			"lightPoss[1] = normalize(vec3(0.0,1.0,0.0));\n "
			"lightPoss[2] = normalize(vec3(1.0,0.0,1.0));\n "
			"lightPoss[3] = normalize(vec3(0.0,-1.0,0.0));\n "
			"lightSpecular[0] = vec3(0.9,0.9,0.9);\n"
			"lightSpecular[1] = vec3(0.9,0.9,0.9);\n"
			"lightSpecular[2] = vec3(0.9,0.9,0.9);\n"
			"lightSpecular[3] = vec3(0.9,0.9,0.9);\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal = normalize( -v_normal );\n"
			"	}\n"
			"  vec3 viewDir = normalize(v_position);\n"
			"   for(int i =0 ;i<4;i++)\n"
			"  {\n"
			"   vec3 H = normalize(lightPoss[i]-viewDir);\n"
			"   float specular_bsdf = pow(max(dot(vNormal, H), 0.0), 30.0);"
			"   totalSpecular += lightSpecular[i] * specular_bsdf;"
			"  }"
			"  vec3 fragcolor = vec3(0.0);\n"
			"   fragcolor= totalSpecular*u_specular.rgb;\n"
			"  gl_FragColor = vec4(fragcolor,1.0);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::JewelBlendQuadVert()
	{
		return

			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelBlendQuadFrag()
	{
		return
			//合并front层，back层，jewelType层，高光层
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform sampler2D u_sampler3;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"  vec4 frontColor = texture2D(u_sampler0,v_texCoords);\n"
			"  vec4 backColor = texture2D(u_sampler1,v_texCoords);\n"
			"  vec4 jewelType = texture2D(u_sampler2,v_texCoords);\n"
			"  vec4 highlight = texture2D(u_sampler3,v_texCoords);\n"
			"  vec4 FragColor = vec4(0.0);"
			"  FragColor = vec4( mix(frontColor.rgb,backColor.rgb,jewelType.a)+highlight.rgb*0.40,1.0);\n "
			"  gl_FragColor = FragColor;\n"
			"}\n";
	}

	const char * ShaderSrcCode::JewelFinalQuadVert()
	{
		return
			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelFinalQuadFrag()
	{
		return

			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform sampler2D u_sampler3;\n"
			"//uniform sampler2D u_sampler4;\n"
			"//uniform sampler2D u_sampler5;\n"
			"//uniform sampler2D u_sampler6;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			//根据深度纹理来合并上几步生成的纹理，最终合成输出结果
			"  vec4 blendColor = texture2D(u_sampler0,v_texCoords);\n"
			"  vec4 jewelDepth = texture2D(u_sampler1,v_texCoords);\n"
			"  vec4 ringArmColor = texture2D(u_sampler2,v_texCoords);\n"
			"  vec4 ringArmDepth = texture2D(u_sampler3,v_texCoords);\n"
			"  //vec4 jewelStarColor= texture2D(u_sampler4,v_texCoords);\n"
			" // vec4 jewelStarDepth= texture2D(u_sampler5,v_texCoords);\n"
			" // vec4 jewelNoteColor = texture2D(u_sampler6,v_texCoords);\n"
			"   vec4 FragColor = vec4(0.0);"
			"	if(jewelDepth.r<ringArmDepth.r)\n"
			"	{\n"
			"      FragColor = vec4(blendColor.rgb,1.0);\n "
			"   }\n"
			"   else\n"
			"   {\n"
			"      FragColor = vec4(ringArmColor.rgb,1.0);\n"
			"   }\n"
			"    gl_FragColor = vec4(FragColor.rgb,1.0);\n"
			"}\n";
	}

	const char * ShaderSrcCode::OutlineVert()
	{
		return
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"void main() {\n "
			"   gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"   vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"   v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"}\n";
	}

	const char * ShaderSrcCode::OutlineFrag()
	{
		return 		"uniform vec4 u_diffuse;\n"
			"varying vec3 v_position;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"void main() { \n"
			"if (u_reverseClip)\n"
			"{"
			"	int iEnableNum = 0;\n"
			"	int iClipNum = 0;\n"
			"	for (int i = 0; i < 3; i++)\n"
			"	{\n"
			"		if (u_enableClips[i])\n"
			"		{\n"
			"			iEnableNum++;\n"
			"			float dPara = u_clipPlanes[i].w;\n"
			"			if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0)\n"
			"			{\n"
			"				iClipNum++;\n"
			"			}\n"
			"		}\n"
			"	}\n"
			"	if (iEnableNum == iClipNum && iEnableNum != 0)\n"
			"	{\n"
			"		//if (u_enableClip[2] == 1) {\n"
			"		discard;\n"
			"	}\n"
			"}\n"
			"else\n"
			"{\n"
			"	for (int i = 0; i < 3; i++)\n"
			"	{\n"
			"		if (u_enableClips[i])\n"
			"		{\n"
			"			float dPara = u_clipPlanes[i].w;\n"
			"			if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0)\n"
			"			{\n"
			"				discard;\n"
			"			}\n"
			"		}\n"
			"	}\n"
			"}\n"
			"   gl_FragColor =u_diffuse;\n"
			"}\n";;
	}

	const char* ShaderSrcCode::GaussianBlurVert()
	{
		return
			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}";
	}

	const char* ShaderSrcCode::GaussianBlurFrag()
	{
		return

			"\n"
			"varying vec2 v_texCoords;\n"
			"\n"
			"uniform sampler2D image;\n"
			"\n"
			"uniform bool horizontal;\n"
			"\n"
			"uniform float tex_offset;\n"
			"\n"
			"void main()\n"
			"{\n"
			"float weight[5];\n"
			"weight[0] = 0.227027;\n"
			"weight[1] = 0.1945946;\n"
			"weight[2] = 0.1216216;\n"
			"weight[3] = 0.054054;\n"
			"weight[4] = 0.016216;\n"
			"   \n"
			"    vec4 result = texture2D(image, v_texCoords) * weight[0]; // current fragment's contribution\n"
			"    if (horizontal)\n"
			"    {\n"
			"\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 1.0, 0.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords - vec2(tex_offset * 1.0, 0.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 2.0, 0.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords - vec2(tex_offset * 2.0, 0.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 3.0, 0.0)) * weight[3];\n"
			"     result += texture2D(image, v_texCoords - vec2(tex_offset * 3.0, 0.0)) * weight[3];\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 4.0, 0.0)) * weight[4];\n"
			"      result += texture2D(image, v_texCoords - vec2(tex_offset * 4.0, 0.0)) * weight[4];\n"
			"\n"
			"    }\n"
			"    else\n"
			"    {\n"
			"\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 1.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 1.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 2.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 2.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 3.0)) * weight[3];\n"
			"     result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 3.0)) * weight[3];\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 4.0)) * weight[4];\n"
			"     result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 4.0)) * weight[4];\n"
			"        \n"
			"    }\n"
			"    gl_FragColor = vec4(result);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::GaussianBlurOutlineVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}";
	}

	const char * ShaderSrcCode::GaussianBlurOutlineFrag()
	{
		return

			"uniform sampler2D height;\n"
			"uniform sampler2D model;\n"
			"varying  vec2 v_texCoords;\n"
			"void main()\n"
			"{\n"
			"   vec4 gaussianColor = texture2D(height, v_texCoords);\n"
			"   vec4 modelColor = texture2D(model, v_texCoords);\n"
			"  vec4 final = (gaussianColor - modelColor);\n"
			"  if(final.r<0.0) final.r = 0.0;\n"
			"  if(final.g<0.0) final.g = 0.0;\n"
			"  if(final.b<0.0) final.b = 0.0;\n"
			"  if(final.a<0.0) final.a = 0.0;\n"
			" //final = pow(final,vec3(0.5));\n"
			"	gl_FragColor = vec4(final*4.0);\n"
			"}";
	}

	const char * ShaderSrcCode::CombineOutlineVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords; \n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::CombineOutlineFrag()
	{
		return
			"uniform sampler2D u_sampler0;\n"
			"varying  vec2 v_texCoords;\n"
			"void main()\n"
			"{\n"
			"   vec4 gaussianColor = texture2D(u_sampler0,v_texCoords);\n"
			"  float alpha = 1.0;\n"
			" //  if(gaussianColor.r<=0.0 &&gaussianColor.g<=0.0&&gaussianColor.b<=0.0 ) alpha = 0.0;     \n"
			"	gl_FragColor =gaussianColor;\n"
			"}\n";
	}

	const char * ShaderSrcCode::SsaoEffectVert()
	{
		return
			""
			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords; \n"
			"varying  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char * ShaderSrcCode::SsaoEffectFrag()
	{
		return
			"uniform bool isPerspective;\n"
			"uniform float cameraNear;\n"
			"uniform float cameraFar;\n"
			"uniform float radius;\n"
			"uniform vec2 size;\n"
			"uniform float aoClamp;\n"

			"uniform float g_SSAO_Bias;\n"
			"uniform float g_Aear;\n"
			"uniform float g_Displace;\n"
			"uniform float g_LumInfluence;\n" //- is how strong ambient light affects on ssao.The brighter light in the room the less ssao you will see;
			"uniform float g_SSAOContrast;\n"// is how strong the ssao effect (how dark is ssao shadows);
			"uniform sampler2D depthMap;\n"

			"#define DL 2.399963229728653\n" // PI * ( 3.0 - sqrt( 5.0 ) )
			"#define EULER 2.718281828459045\n"

			"varying  vec2 v_texCoords;\n"

			"const int samples = 64;\n"     // ao sample count

			"const bool useNoise = true;\n"      // use noise instead of pattern for sample dithering
			"const float noiseAmount = 0.001;\n" // dithering amount

		//	"const float g_SSAO_Bias = 0.4;\n"   // is ssao 'accuracy'. Too accurate ssao can cause artifacts (shadows) on flat surfaces. This variable is needed to reduce such artifacts;
		//	"const float g_Displace = 0.4;\n"  // gauss bell center
			"vec2 rand( const vec2 coord ) {\n"

			"	vec2 noise;\n"

			"	if ( useNoise ) {\n"

			"		float nx = dot ( coord, vec2( 12.9898, 78.233 ) );\n"
			"		float ny = dot ( coord, vec2( 12.9898, 78.233 ) * 2.0 );\n"

			"		noise = clamp( fract ( 43758.5453 * sin( vec2( nx, ny ) ) ), 0.0, 1.0 );\n"

			"	} else {\n"

			"		float ff = fract( 1.0 - coord.s * ( size.x / 2.0 ) );\n"
			"		float gg = fract( coord.t * ( size.y / 2.0 ) );\n"

			"		noise = vec2( 0.25, 0.75 ) * vec2( ff ) + vec2( 0.75, 0.25 ) * gg;\n"

			"	}\n"

			"	return ( noise * 2.0  - 1.0 ) * noiseAmount;\n"

			"}\n"

			"float readDepth( const in vec2 coord,in bool isPerspective) {\n"
			"	float z =  texture2D( depthMap, coord ).r;\n"
			"	if(isPerspective == true)\n"
			"	{\n"
			"		"
			"		float cameraFarPlusNear = cameraFar + cameraNear;\n"
			"		float cameraFarMinusNear = cameraFar - cameraNear;\n"
			"		float cameraCoef = 2.0 * cameraNear;\n"
			"		return cameraCoef / ( cameraFarPlusNear - z * cameraFarMinusNear );\n"//转换成线性
			"	}\n"
			"   return z;\n"
			"}\n"

			"float compareDepths( const in float depth1, const in float depth2, inout int far ) {\n"

			"	float garea = g_Aear;\n"                         // gauss bell width
			"	float diff = ( depth1 - depth2 ) * 100.0;\n"  // depth difference (0-100)

			// reduce left bell width to avoid self-shadowing

			"	if ( diff < g_Displace ) {\n"

			"		garea = g_SSAO_Bias;\n"

			"	} else {\n"

			"		far = 1;\n"

			"	}\n"

			"	float dd = diff - g_Displace;\n"
			"	float gauss = pow( EULER, -2.0 * ( dd * dd ) / ( garea * garea ) );\n"
			"	return gauss;\n"

			"}\n"
			"float calcAO( float depth, float dw, float dh ) {\n"

			"	vec2 vv = vec2( dw, dh );\n"

			"	vec2 coord1 = v_texCoords + radius * vv;\n"
			"	vec2 coord2 = v_texCoords - radius * vv;\n"

			"	float temp1 = 0.0;\n"
			"	float temp2 = 0.0;\n"

			"	int far = 0;\n"
			"	temp1 = compareDepths( depth, readDepth( coord1,isPerspective ), far );\n"
			//"float rangeCheck = smoothstep(0.0, 1.0, radius / (abs(depth - readDepth( coord1,isPerspective ))*10000.0) );\n"
			// DEPTH EXTRAPOLATION

			"	if ( far > 0 ) {\n"

			"		temp2 = compareDepths( readDepth( coord2,isPerspective ), depth, far );\n"
			"		temp1 += ( 1.0 - temp1 ) * temp2;\n"

			"	}\n"

			"	return temp1;\n"

			"}\n"
			"void main() {\n"

			"	vec2 noise = rand( v_texCoords );\n"
			"	float depth = readDepth( v_texCoords ,isPerspective);\n"

			"	float tt = clamp( depth, aoClamp, 1.0 );\n"

			"	float w = ( 1.0 / size.x ) / tt + ( noise.x * ( 1.0 - noise.x ) );\n"
			"	float h = ( 1.0 / size.y ) / tt + ( noise.y * ( 1.0 - noise.y ) );\n"

			"	float ao = 0.0;\n"

			"	float dz = 1.0 / float( samples );\n"
			"	float l = 0.0;\n"
			"	float z = 1.0 - dz / 2.0;\n"

			"	for ( int i = 0; i <= samples; i ++ ) {\n"

			"		float r = sqrt( 1.0 - z );\n"

			"		float pw = cos( l ) * r;\n"
			"		float ph = sin( l ) * r;\n"
			"		ao += calcAO( depth, pw * w, ph * h );\n"
			"		z = z - dz;\n"
			"		l = l + DL;\n"

			"	}\n"

			"	ao /= float( samples );\n"
			"   ao*=g_SSAOContrast;\n"
			"	ao = 1.0 - ao;\n"

			//"vec3 color = texture2D( depthMap, v_texCoords ).rgb;\n"		
			"	vec3 lumcoeff = vec3( 0.2126, 0.7152, 0.0722 );\n"
			"	float lum = dot( vec3(depth), lumcoeff );\n"
			"	vec3 luminance = vec3( lum );\n"



			"	vec3 final = vec3( mix( vec3( ao ), vec3( 1.0 ), luminance *g_LumInfluence) );\n"  // ambient occlusion only
			//"  final = ((final - 0.5)*max(g_SSAOContrast,0.0))+0.5;\n"
			//"	vec3 final = vec3(ao);\n"
			"	gl_FragColor = vec4( final, 1.0 );\n"

			"}"
			;
	}

	const char* ShaderSrcCode::SsaoBlurVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords; \n"
			"varying  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::SsaoBlurFrag()
	{
		return
			""
			"varying  vec2 v_texCoords;\n"
			"uniform vec2 size;\n"
			"uniform sampler2D ssaoInput;\n"

			"void main()\n"
			"{\n"
			"	vec2 texelSize = 1.0 / size;\n"
			"	float result = 0.0;\n"
			"	for (int x = -2; x < 2; ++x)\n"
			"	{\n"
			"		for (int y = -2; y < 2; ++y)\n"
			"		{\n"
			"			vec2 offset = vec2(float(x), float(y)) * texelSize;\n"
			"			result += texture2D(ssaoInput, v_texCoords + offset).r;\n"
			"		}\n"
			"	}\n"
			"	gl_FragColor = vec4(vec3(result / (4.0 * 4.0)),1.0);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::SaoReconstructCSZVert()
	{
		return
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"out  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//	"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char * ShaderSrcCode::SaoReconstructCSZFrag()
	{
		return
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"#if __VERSION__ == 120\n"
			//  Compatibility with older versions of GLSL
			"#   define texelFetch texelFetch2D\n"
			"#   define result gl_FragColor.r\n"
			"#else\n"
			"			out float     result;\n"
			"#endif\n"
			"uniform sampler2D depthMap;\n"
			"uniform float near;\n"
			"uniform float far;\n"
			"uniform bool isPerspective;\n"
			"float getOrthographicZ(float z)\n"
			"{\n"
			"   float ret;\n"
			"   ret =  z*(far - near)+near;\n"
			"   ret = -ret;\n"
			"   return ret;\n"
			"}\n"
			"float getPerspectiveZ( float z)\n"
			"{\n"
			"   float ret;\n"
			"   ret =  near * far / (far +z * ( near - far));\n"
			"   ret = -ret;\n"
			"   return ret;\n"
			"}\n"
			"float reconstructCSZ( float z)\n"
			"{\n"
			"   if(isPerspective == true)\n"
			"     return getPerspectiveZ(z);\n"
			"   else\n"
			"     return getOrthographicZ(z);\n"
			"}\n"
			"void main() {\n"
			"	result = reconstructCSZ(texelFetch(depthMap, ivec2(gl_FragCoord.xy), 0).r);\n"
			"}\n"
			;
	}

	const char* ShaderSrcCode::SaoMinifyCSZVert()
	{
		return
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"out  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//	"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::SaoMinifyCSZFrag()
	{
		return
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"#if __VERSION__ == 120\n"
			//  Compatibility with older versions of GLSL
			"#   define texelFetch texelFetch2D\n"
			"#   define textureSize  textureSize2D\n"
			"#endif\n"
			"uniform sampler2D cszBuffer;\n"
			"uniform int       previousMIPNumber;\n"
			"void main() {\n"
			"ivec2 ssP = ivec2(gl_FragCoord.xy);\n"
			// Rotated grid subsampling to avoid XY directional bias or Z precision bias while downsampling.
			// On DX9, the bit-and can be implemented with floating-point modulo
			"gl_FragColor = texelFetch2D(cszBuffer, clamp(ssP * 2 + ivec2(ssP.y & 1, ssP.x & 1), ivec2(0), textureSize(cszBuffer, previousMIPNumber) - ivec2(1)), previousMIPNumber);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::SaoBlurVert()
	{
		return
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"out  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//	"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::SaoBlurFrag()
	{
		return
			"#version 120 // -*- c++ -*-\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"#line 4\n"
			/**
			\file SAO_blur.pix
			\author Morgan McGuire and Michael Mara, NVIDIA Research

			\brief 7-tap 1D cross-bilateral blur using a packed depth key

			DX11 HLSL port by Leonardo Zide, Treyarch

			Open Source under the "BSD" license: http://www.opensource.org/licenses/bsd-license.php

			Copyright (c) 2011-2012, NVIDIA
			All rights reserved.

			Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

			Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
			Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
			THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
			*/

			//////////////////////////////////////////////////////////////////////////////////////////////
			// Tunable Parameters:

			/** Increase to make depth edges crisper. Decrease to reduce flicker. */
			"#define EDGE_SHARPNESS     (1.0)\n"

			/** Step in 2-pixel intervals since we already blurred against neighbors in the
			first AO pass.  This constant can be increased while R decreases to improve
			performance at the expense of some dithering artifacts.

			Morgan found that a scale of 3 left a 1-pixel checkerboard grid that was
			unobjectionable after shading was applied but eliminated most temporal incoherence
			from using small numbers of sample taps.
			*/
			"#define SCALE               (2)\n"

			/** Filter radius in pixels. This will be multiplied by SCALE. */
			"#define R                   (4)\n"


			//////////////////////////////////////////////////////////////////////////////////////////////

			/** Type of data to read from source.  This macro allows
			the same blur shader to be used on different kinds of input data. */
			"#define VALUE_TYPE        float\n"

			/** Swizzle to use to extract the channels of source. This macro allows
			the same blur shader to be used on different kinds of input data. */
			"#define VALUE_COMPONENTS   r\n"

			"#define VALUE_IS_KEY       0\n"

			/** Channel encoding the bilateral key value (which must not be the same as VALUE_COMPONENTS) */
			"#define KEY_COMPONENTS     gb\n"


			"#if __VERSION__ >= 330\n"
			// Gaussian coefficients
			"			const float gaussian[R + 1] = \n"
			//    float[](0.356642, 0.239400, 0.072410, 0.009869);
			//    float[](0.398943, 0.241971, 0.053991, 0.004432, 0.000134);  // stddev = 1.0
			"			float[](0.153170, 0.144893, 0.122649, 0.092902, 0.062970);  // stddev = 2.0\n"
			//      float[](0.111220, 0.107798, 0.098151, 0.083953, 0.067458, 0.050920, 0.036108); // stddev = 3.0
			"#endif\n"

			"uniform sampler2D   source;\n"

			/** (1, 0) or (0, 1)*/
			"uniform vec2       axis;\n"

			"#if __VERSION__ == 120\n"
			"#   define          texelFetch texelFetch2D\n"
			"#else\n"
			"		out vec3            gl_FragColor;\n"
			"#endif\n"

			"#define  result         gl_FragColor.VALUE_COMPONENTS\n"
			"#define  keyPassThrough gl_FragColor.KEY_COMPONENTS\n"

			/** Returns a number on (0, 1) */
			"float unpackKey(vec2 p) {\n"
			"			return p.x * (256.0 / 257.0) + p.y * (1.0 / 257.0);\n"
			"		}\n"


			"		void main() {\n"
			"#   if __VERSION__ < 330\n"
			"			float gaussian[R + 1];\n"
			"#       if R == 3\n"
			"			gaussian[0] = 0.153170; gaussian[1] = 0.144893; gaussian[2] = 0.122649; gaussian[3] = 0.092902;  // stddev = 2.0\n"
			"#       elif R == 4\n"
			"			gaussian[0] = 0.153170; gaussian[1] = 0.144893; gaussian[2] = 0.122649; gaussian[3] = 0.092902; gaussian[4] = 0.062970;  // stddev = 2.0\n"
			"#       elif R == 6\n"
			"			gaussian[0] = 0.111220; gaussian[1] = 0.107798; gaussian[2] = 0.098151; gaussian[3] = 0.083953; gaussian[4] = 0.067458; gaussian[5] = 0.050920; gaussian[6] = 0.036108;\n"
			"#       endif\n"
			"#   endif\n"

			"			ivec2 ssC = ivec2(gl_FragCoord.xy);\n"

			"			vec4 temp = texelFetch(source, ssC, 0);\n"

			"			keyPassThrough = temp.KEY_COMPONENTS;\n"
			"			float key = unpackKey(keyPassThrough);\n"
			"			VALUE_TYPE sum = temp.VALUE_COMPONENTS;\n"

			//"			if (key == 1.0) {\n"
			//// Sky pixel (if you aren't using depth keying, disable this test)
			//"				result = sum;\n"
			//"				return;\n"
			//"			}\n"

			// Base weight for depth falloff.  Increase this for more blurriness,
			// decrease it for better edge discrimination
			"			float BASE = gaussian[0];\n"
			"			float totalWeight = BASE;\n"
			"			sum *= totalWeight;\n"


			"			for (int r = -R; r <= R; ++r) {\n"
			// We already handled the zero case above.  This loop should be unrolled and the static branch optimized out,
			// so the IF statement has no runtime cost
			"				if (r != 0) {\n"
			"					temp = texelFetch(source, ssC + ivec2(int(axis.x),int(axis.y)) * (r * SCALE), 0);\n"
			"					float      tapKey = unpackKey(temp.KEY_COMPONENTS);\n"
			"					VALUE_TYPE value = temp.VALUE_COMPONENTS;\n"

			// spatial domain: offset gaussian tap
			"					float weight = 0.3 + gaussian[abs(r)];\n"

			// range domain (the "bilateral" weight). As depth difference increases, decrease weight.
			"					weight *= max(0.0, 1.0\n"
			"						- (EDGE_SHARPNESS * 2000.0) * abs(tapKey - key)\n"
			"					);\n"

			"					sum += value * weight;\n"
			"					totalWeight += weight;\n"
			"				}\n"
			"			}\n"

			"			const float epsilon = 0.0001;\n"
			"			result = sum / (totalWeight + epsilon);\n"
			"		}\n"
			;
	}

	const char * ShaderSrcCode::SaoEffectVert()
	{
		return
			""
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"varying  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char * ShaderSrcCode::SaoEffectFrag()
	{
		return
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"uniform sampler2D CS_Z_buffer;\n"
			"uniform float near;\n"
			"uniform float far;\n"
			" uniform vec2 resolution;\n"
			"uniform bool isPerspective;\n"


			// Total number of direct samples to take at each pixel
			"#define NUM_SAMPLES (21)\n"

			// If using depth mip levels, the log of the maximum pixel offset before we need to switch to a lower
			// miplevel to maintain reasonable spatial locality in the cache
			// If this number is too small (< 3), too many taps will land in the same pixel, and we'll get bad variance that manifests as flashing.
			// If it is too high (> 5), we'll get bad performance because we're not using the MIP levels effectively
			"#define LOG_MAX_OFFSET 3\n"

			// This must be less than or equal to the MAX_MIP_LEVEL defined in SSAO.cpp
			"#define MAX_MIP_LEVEL 5\n"

			/** Used for preventing AO computation on the sky (at infinite depth) and defining the CS Z to bilateral depth key scaling.
			This need not match the real far plane*/
			"#define FAR_PLANE_Z (300.0)\n"

			// This is the number of turns around the circle that the spiral pattern makes.  This should be prime to prevent
			// taps from lining up.  This particular choice was tuned for NUM_SAMPLES == 9
			"#define NUM_SPIRAL_TURNS (7)\n"

			//////////////////////////////////////////////////

			/** The height in pixels of a 1m object if viewed from 1m away.
			You can compute it from your projection matrix.  The actual value is just
			a scale factor on radius; you can simply hardcode this to a constant (~500)
			and make your radius value unitless (...but resolution dependent.)  */
			"uniform float projScale ;\n"

			"uniform vec4 saoData;\n"
			/** World-space AO radius in scene units (r).  e.g., 1.0m */
			"   float radius = saoData.x;\n"

			/** Bias to avoid AO in smooth corners, e.g., 0.01m */
			" float bias = saoData.y;\n "

			/** Darkending factor, e.g., 1.0 */
			"float intensity = saoData.z;\n"
			" float intensityDivR6 = saoData.w;\n"
			/**  vec4(-2.0f / (width*P[0][0]),
			-2.0f / (height*P[1][1]),
			( 1.0f - P[0][2]) / P[0][0],
			( 1.0f + P[1][2]) / P[1][1])

			where P is the projection matrix that maps camera space points
			to [-1, 1] x [-1, 1].  That is, GCamera::getProjectUnit(). */
			"uniform vec4 projInfo;\n"

			" float radius2 = radius * radius;\n"

			// Compatibility with future versions of GLSL: the shader still works if you change the
			// version line at the top to something like #version 330 compatibility.
			"#if __VERSION__ == 120\n"
			"#define texelFetch texelFetch2D\n"
			"#define textureSize textureSize2D\n"
			"#else\n"
			"   out vec3 gl_FragColor;\n"
			"#endif\n"
			"#define visibility gl_FragColor.r\n"
			"#define bilateralKey gl_FragColor.gb\n"

			/////////////////////////////////////////////////////////

			/** Reconstructs screen-space unit normal from screen-space position */

			"vec3 reconstructNonUnitCSFaceNormal(vec3 C)\n"
			"{\n"
			"    return cross(dFdy(C), dFdx(C));\n"
			"}\n"

			/** Reconstruct camera-space P.xyz from screen-space S = (x, y) in
			pixels and camera-space z < 0.  Assumes that the upper-left pixel center
			is at (0.5, 0.5) [but that need not be the location at which the sample tap
			was placed!]

			Costs 3 MADD.  Error is on the order of 10^3 at the far plane, partly due to z precision.
			*/
			"vec3 getPerspectiveCSPosition(vec2 S, float z)\n"
			"{\n"
			"    return vec3((S.xy * projInfo.xy + projInfo.zw) * z, z);\n"
			"}\n"
			"vec3 getOrthographicCSPosition(vec2 S, float z)\n"
			"{\n"
			"    return vec3((S.xy * projInfo.xy + projInfo.zw), z);\n"
			"}\n"
			"vec3 reconstructCSPosition( vec2 S, float z)\n"
			"{\n"
			"   if(isPerspective == true)\n"
			"     return getPerspectiveCSPosition(S,z);\n"
			"   else\n"
			"     return getOrthographicCSPosition(S,z);\n"
			"}\n"

			/** Reconstructs screen-space unit normal from screen-space position */
			"vec3 reconstructCSFaceNormal(vec3 C)\n"
			"{\n"
			"    return normalize(cross(dFdy(C), dFdx(C)));\n"
			"}\n"
			//-----------------------------------------------------------------------------------------------------------
			/** Read the camera-space position of the point at screen-space pixel ssP */
			"vec3 getPosition(ivec2 ssP, sampler2D cszBuffer)\n"
			"{\n"
			"   vec3 P;\n"
			"   P.z = texelFetch(cszBuffer, ssP, 0).r;\n"
			// Offset to pixel center
			"   P = reconstructCSPosition(vec2(ssP) + vec2(0.5), P.z);\n"
			"   return P;\n"
			"}\n"
			"	int getMipLevel(float ssR) {\n"
			// Derivation:
			//  mipLevel = floor(log(ssR / MAX_OFFSET));
			"	return clamp(int(floor(log2(ssR))) - LOG_MAX_OFFSET, 0, MAX_MIP_LEVEL);\n"
			"}\n"

			/** Read the camera-space position of the point at screen-space pixel ssP + unitOffset * ssR.  Assumes length(unitOffset) == 1 */
			"vec3 getOffsetPosition(ivec2 ssC, vec2 unitOffset, float ssR, sampler2D cszBuffer)\n"
			"{\n"
			// Derivation:
			//  mipLevel = floor(log(ssR / MAX_OFFSET));
			"    int mipLevel =getMipLevel(ssR);\n"

			"    ivec2 ssP = ivec2(ssR * unitOffset) + ssC;\n"

			"    vec3 P;\n"

			// We need to divide by 2^mipLevel to read the appropriately scaled coordinate from a MIP-map.
			// Manually clamp to the texture size because texelFetch bypasses the texture unit
			"   ivec2 mipP = clamp(ssP >> mipLevel, ivec2(0), textureSize(CS_Z_buffer, mipLevel) - ivec2(1));\n"

			//read from depth buffer
			"   P.z = texelFetch(cszBuffer, mipP,0).r;\n"
			// Offset to pixel center
			"    P = reconstructCSPosition(vec2(ssP) + vec2(0.5), P.z);\n"

			"   return P;\n"
			"}\n"

			"float fallOffFunction(float vv, float vn, float epsilon) {\n"
			// A: From the HPG12 paper
			// Note large epsilon to avoid overdarkening within cracks
			//  Assumes the desired result is intensity/radius^6 in main()
			// return float(vv < radius2) * max((vn - bias) / (epsilon + vv), 0.0) * radius2 * 0.6;

			// B: Smoother transition to zero (lowers contrast, smoothing out corners). [Recommended]
			"#   if HIGH_QUALITY\n"
			// Epsilon inside the sqrt for rsqrt operation
			"float f = max(1.0 - vv * invRadius2, 0.0); return f * max((vn - bias) * rsqrt(epsilon + vv), 0.0);\n"
			"#   else\n"
			// Avoid the square root from above.
			//  Assumes the desired result is intensity/radius^6 in main()
			"				float f = max(radius2 - vv, 0.0); return f * f * f * max((vn - bias) / (epsilon + vv), 0.0);\n"
			"#   endif\n"

			// C: Medium contrast (which looks better at high radii), no division.  Note that the 
			// contribution still falls off with radius^2, but we've adjusted the rate in a way that is
			// more computationally efficient and happens to be aesthetically pleasing.  Assumes 
			// division by radius^6 in main()
			// return 4.0 * max(1.0 - vv * invRadius2, 0.0) * max(vn - bias, 0.0);

			// D: Low contrast, no division operation
			//return 2.0 * float(vv < radius * radius) * max(vn - bias, 0.0);
			"}\n"
			/** Compute the occlusion due to sample point \a Q about camera-space point \a C with unit normal \a n_C */
			"float aoValueFromPositionsAndNormal(vec3 C, vec3 n_C, vec3 Q) {\n"
			"vec3 v = Q - C;\n"
			"float vv = dot(v, v);\n"
			"float vn = dot(v, n_C);\n"
			"const float epsilon = 0.001;\n"

			// Without the angular adjustment term, surfaces seen head on have less AO
			"return fallOffFunction(vv, vn, epsilon) * mix(1.0, max(0.0, 1.5 * n_C.z), 0.35);\n"
			"}\n"

			/** Returns a unit vector and a screen-space radius for the tap on a unit disk (the caller should scale by the actual disk radius) */
			"vec2  tapLocation(int sampleNumber, float spinAngle, out float ssR)\n"
			"{\n"
			// Radius relative to ssR
			"    float alpha = float(sampleNumber + 0.5) * (1.0 / NUM_SAMPLES);\n"
			"   float angle = alpha * (NUM_SPIRAL_TURNS * 6.28) + spinAngle;\n"

			"    ssR = alpha;\n"
			"   return vec2(cos(angle), sin(angle));\n"
			"}\n"
			/** Compute the occlusion due to sample with index \a i about the pixel at \a uv that corresponds
			to camera-space point \a C with unit normal \a n_C, using maximum screen-space sampling radius \a ssDiskRadius */
			"float sampleAO(ivec2 ssC, vec3 C, vec3 n_C, float ssDiskRadius, int tapIndex, float randomPatternRotationAngle, in sampler2D cszBuffer)\n"
			"{\n"
			// Offset on the unit disk, spun for this pixel
			"   float ssR;\n"
			"   vec2 unitOffset = tapLocation(tapIndex, randomPatternRotationAngle, ssR);\n"
			"   ssR =  ssR * ssDiskRadius;\n"
			// The occluding point in camera space
			"  vec3 Q = getOffsetPosition(ssC, unitOffset, ssR,cszBuffer);\n"

			" return aoValueFromPositionsAndNormal(C, n_C, Q);\n"
			"}\n"

			"float CSZToKey(float z)\n"
			"{\n"
			"   return clamp(z * (-1.0 / far), 0.0, 1.0);\n"
			"}\n"

			/** Used for packing Z into the GB channels */
			"void  packKey(float key, out vec2 p)\n"
			"{\n"
			// Round to the nearest 1/256.0
			"    float temp = floor(key * 256.0);\n"

			// Integer part
			"    p.x = temp * (1.0 / 256.0);\n"

			// Fractional part
			"    p.y = key * 256.0 - temp;\n"
			"}\n"
			"const float MIN_RADIUS = 3.0;\n"
			"void main()\n"
			"{\n"
			// Pixel being shaded
			"   ivec2 ssC = ivec2(gl_FragCoord.xy);\n"

			"   vec3 C = getPosition(ssC,CS_Z_buffer);\n"

			"    packKey(CSZToKey(C.z), bilateralKey);\n"
			// Hash function used in the HPG12 AlchemyAO paper
			"    float randomPatternRotationAngle = ((3 * ssC.x) ^ (ssC.y + ssC.x * ssC.y)) * 10;\n"

			"   vec3 n_C = reconstructNonUnitCSFaceNormal(C);\n"
			"	if (dot(n_C, n_C) > ((C.z * C.z * 0.00006)*(C.z * C.z * 0.00006))){\n"
			"		visibility = 1.0; \n"
			"		return;\n"
			"	} else {\n"
			"		n_C = normalize(n_C);\n"
			"	}\n"

			// Choose the screen-space sample radius
			// proportional to the projected area of the sphere
			"    float ssDiskRadius = projScale * radius / C.z;\n"
			//"if (ssDiskRadius <= 3.0) {\n"
			//	"visibility = 1.0;\n"
			//	" return;\n"
			//	" }\n"

			"    float sum = 0.0;\n"
			"   for (int i = 0; i < NUM_SAMPLES; ++i)\n"
			"   sum += sampleAO(ssC, C, n_C, ssDiskRadius, i, randomPatternRotationAngle,CS_Z_buffer);\n"

			"#   if HIGH_QUALITY\n"
			"				float A = pow(max(0.0, 1.0 - sqrt(sum * (3.0 / NUM_SAMPLES))), intensity);\n"
			"#   else\n"
			"				float A = max(0.0, 1.0 - sum * intensityDivR6 * (5.0 / NUM_SAMPLES));\n"
			// Anti-tone map to reduce contrast and drag dark region farther
			// (x^0.2 + 1.2 * x^4)/2.2
			"			A = (pow(A, 0.2) + 1.2 * A*A*A*A) / 2.2;\n"
			"#   endif\n"
			// Bilateral box-filter over a quad for free, respecting depth edges
			// (the difference that this makes is subtle)
			"   if (abs(dFdx(C.z)) < 0.02)\n"
			"   {\n"
			"    A -= dFdx(A) * (float(ssC.x & 1) - 0.5);\n"
			"   }\n"
			"   if (abs(dFdy(C.z)) < 0.02)\n"
			"   {\n"
			"    A -= dFdy(A) * (float(ssC.y & 1) - 0.5);\n"
			"   }\n"
				"  visibility = A;\n"
	//		"     visibility = mix(1.0, A, clamp(ssDiskRadius - MIN_RADIUS,0.0,1.0));\n"
			//				"gl_FragColor = vec4(vec3(A),1.0);\n"//ao debug
			//		"gl_FragColor = vec4(1.0+A*0.0000001,0.0,0.0,1.0);\n"//ao debug
			"}\n"
			;
	}

	const char *ShaderSrcCode::SceneGroundVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec4 a_color;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"//uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position =  u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_color = a_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::SceneGroundFrag()
	{
		return "//precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char* ShaderSrcCode::MatCapMaterialVert()
	{
		return
			"#define PHONG\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <uv_pars_vertex>\n"
			"#include <displacementmap_pars_vertex>\n"
			"#include <shadowmap_pars_vertex>\n"
			"#include <clip_pars_vertex>\n"
			"void main(){\n"
			"  #include <uv_vertex>\n"
			"  #include <beginnormal_vertex>\n"
			"  #include <defaultnormal_vertex>\n"
			"vNormal = normalize( transformedNormal );\n"
			"  #include <begin_vertex>\n"
			"#include <displacementmap_vertex>\n"
			"  #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"  #include <worldpos_vertex>\n"
			"#include <shadowmap_vertex>\n"
			"}"
			;
	}

	const char* ShaderSrcCode::MatCapMaterialFrag()
	{
		return
			"#define PHONG\n"
			"uniform vec4 diffuse;\n"
			"uniform vec4 emissive;\n"
			"uniform vec4 specular;\n"
			"uniform float shininess;\n"
			"uniform float opacity;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"#include <uv_pars_fragment>\n"
			"#include <map_pars_fragment>\n"
			"#include <ssao_par_fragment>\n"
			"#include <bsdfs>\n"
			"#include <lights_pars>\n"
			"#include <matcap_pars_fragment>\n"
			"#include <lights_phong_pars_fragment>\n"
			"#include <normalmap_pars_fragment>\n"
			"#include <specularmap_pars_fragment>\n"
			"#include <shadowmap_pars_fragment>\n"
			"void main(){\n"
			"   #include <clip_fragment>\n"
			"   vec4 diffuseColor = vec4( diffuse.xyz, diffuse.a );\n"
			"   ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );\n"
			"vec3 totalEmissiveRadiance = emissive.rgb;\n"
			"   #include <map_fragment>\n"
			"   #include <specularmap_fragment>\n"
			"   #include <normal_fragment>\n"
			"	#include <lights_phong_fragment>\n"
			"   #include <lights_template>\n"
			"	float ssaoFactor = 1.0f;\n"
			"	#include <ssao_fragment>\n"
			"	vec3 outgoingLight = ssaoFactor*(reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular)+ totalEmissiveRadiance  ;\n"
			"   gl_FragColor = vec4(outgoingLight, opacity );\n"
			"#include <encodings_fragment>"
			"gl_FragColor.rgb = gl_FragColor.rgb*u_selectColor.rgb;\n"

			"}"
			;
	}

	const char* ShaderSrcCode::DepthMaterialVert()
	{
		return
			"varying vec3 vViewPosition;\n"
			
			"#include <common>\n"
			"#include <clip_pars_vertex>\n"
			"void main()\n"
			"{\n"
			"    #include <begin_vertex>\n"
			"    #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"}\n"
			;
	}

	const char* ShaderSrcCode::DepthMaterialFrag()
	{
		return
			"varying vec3 vViewPosition;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"\n"
			"void main()\n"
			"{\n"
			"   #include <clip_fragment>\n"
			"    gl_FragColor = packDepthToRGBA(gl_FragCoord.z);\n"
			//"    gl_FragColor =vec4(1.0,0.0,0.0,1.0);",
			"}\n"
			;
	}

	const char * ShaderSrcCode::PhongMaterialVert()
	{
		return
			"#define PHONG\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <uv_pars_vertex>\n"
			"#include <displacementmap_pars_vertex>\n"
			"#include <shadowmap_pars_vertex>\n"
			"#include <clip_pars_vertex>\n"
			"void main(){\n"
			"  #include <uv_vertex>\n"
			"  #include <beginnormal_vertex>\n"
			"  #include <defaultnormal_vertex>\n"
			"vNormal = normalize( transformedNormal );\n"
			"  #include <begin_vertex>\n"
			"#include <displacementmap_vertex>\n"
			"  #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"  #include <worldpos_vertex>\n"
			"#include <shadowmap_vertex>\n"
			"}"
			;
	}

	const char * ShaderSrcCode::PhongMaterialFrag()
	{
		return
			"#define PHONG\n"
			"uniform vec4 diffuse;\n"
			"uniform vec4 emissive;\n"
			"uniform vec4 specular;\n"
			"uniform float shininess;\n"
			"uniform float opacity;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"#include <uv_pars_fragment>\n"
			"#include <map_pars_fragment>\n"
			"#include <ssao_par_fragment>\n"
			"#include <emissivemap_pars_fragment>\n"
			"#include <bsdfs>\n"
			"#include <lights_pars>\n"
			"#include <matcap_pars_fragment>\n"
			"#include <lights_phong_pars_fragment>\n"
			"#include <normalmap_pars_fragment>\n"
			"#include <specularmap_pars_fragment>\n"
			"#include <shadowmap_pars_fragment>\n"
			"void main(){\n"
			"   #include <clip_fragment>\n"
			"   vec4 diffuseColor = vec4( diffuse.xyz, diffuse.a );\n"
			"   ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );\n"
			"vec3 totalEmissiveRadiance = emissive.rgb;\n"
			"   #include <map_fragment>\n"
			"   #include <specularmap_fragment>\n"
			"   #include <normal_fragment>\n"
			"#include <emissivemap_fragment>\n"
			"	#include <lights_phong_fragment>\n"
			"   #include <lights_template>\n"
			"	float ssaoFactor = 1.0f;\n"
			"	#include <ssao_fragment>\n"
			"	vec3 outgoingLight = ssaoFactor*(reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular)+ totalEmissiveRadiance  ;\n"
			"   gl_FragColor = vec4(outgoingLight, opacity );\n"
			"#include <encodings_fragment>\n"
			"gl_FragColor.rgb = gl_FragColor.rgb+u_selectColor.rgb*0.0000001;\n"

			"}"
			;
	}

	const char* ShaderSrcCode::PbrMaterialVert()
	{
		return "#define PHYSICAL\n"
			"#define USE_LOCAL_CUBEMAP\n"
			"uniform float gamma;\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <uv_pars_vertex>\n"
			"#include <parallax_corrected_cubemap_pars_vertex>\n"
			"#include <displacementmap_pars_vertex>\n"
			"#include <shadowmap_pars_vertex>\n"
			"#include <clip_pars_vertex>\n"
			"#include <z_depth_pars_vert>\n"
			"void main(){\n"
			"  #include <uv_vertex>\n"
			"  #include <beginnormal_vertex>\n"
			"  #include <defaultnormal_vertex>\n"
			"vNormal = normalize( transformedNormal );\n"
			"  #include <begin_vertex>\n"
			"#include <displacementmap_vertex>\n"
			"  #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"  #include <worldpos_vertex>\n"
			" #include <parallax_corrected_cubemap_vertex>\n"
			"   #include <shadowmap_vertex>\n"
			"   #include <z_depth_vert>\n"
			"}"
			;
	}

	const char* ShaderSrcCode::PbrMaterialFrag()
	{
		return
			"#define PHYSICAL\n"
			"#define USE_LOCAL_CUBEMAP\n"
			"#define TONE_MAPPING\n"
			"uniform vec4 diffuse;\n"
			"uniform vec4 emissive;\n"
			"uniform float roughness;\n"
			"uniform float metalness;\n"
			"uniform float opacity;\n"
			"uniform float gamma;\n"
			"uniform sampler2D lut;\n"
			"vec3 test = vec3(1.0,1.0,1.0);\n"
			"#ifndef STANDARD\n"
			"uniform float clearCoat;\n"
			"uniform float clearCoatRoughness;\n"
			"#endif\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"#include <uv_pars_fragment>\n"
			"#include <map_pars_fragment>\n"
			"#include <aomap_pars_fragment>\n"
			"#include <ssao_par_fragment>\n"
			"#include <emissivemap_pars_fragment>\n"
			"#include <parallax_corrected_cubemap_pars_fragment>\n"
			"#include <envmap_pars_fragment>\n"
			"#include <bsdfs>\n"
			"#include <lights_pars>\n"
			"#include <lights_physical_pars_fragment>\n"
			"#include <normalmap_pars_fragment>\n"
			"#include <shadowmap_pars_fragment>\n"
			"#include <z_depth_pars_frag>\n"
			"#include <tonemapping_pars_fragment>\n"
			"#include <metallicRoughness_pars_fragment>\n"
			"void main(){\n"
			"#include <z_depth_frag>\n"
			"   #include <clip_fragment>\n"
			"   vec4 diffuseColor = vec4( diffuse.xyz, diffuse.a );\n"
			"   ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );\n"
			"vec3 totalEmissiveRadiance = emissive.rgb;\n"
			"   #include <map_fragment>\n"
			"   #include <metallicRoughness_fragment>\n"
			"   #include <normal_fragment>\n"
			"#include <emissivemap_fragment>\n"

			"	#include <lights_physical_fragment>\n"
			"   #include <lights_template>\n"

			"#include <aomap_fragment>\n"
			"float ssaoFactor = 1.0f;\n"
			"#include <ssao_fragment>\n"

			"	vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular+ totalEmissiveRadiance ;\n"
			//"	vec3 lumcoeff = vec3( 0.299, 0.587, 0.114 );\n"
			//"	float lum = dot( outgoingLight.rgb, lumcoeff );\n"
			//"	vec3 luminance = vec3( lum );\n"

			//"	outgoingLight = vec3( outgoingLight * mix( vec3( ssaoFactor ), vec3( 1.0 ), luminance * 1.0f ) );\n"  // mix( color * ao, white, luminance )

			"//   outgoingLight =OptimizedCineonToneMapping(outgoingLight);\n"
			// "   gl_FragColor = vec4(pow(outgoingLight, vec3(1.0 / GAMMA_FACTOR)), diffuseColor.a );\n"
			"   gl_FragColor = vec4(outgoingLight, opacity );\n"
			"#include <tonemapping_fragment>\n"
			//"gl_FragColor.rgb = pow(gl_FragColor.rgb*u_selectColor.rgb, vec3(1.0 / gamma));\n"
			"#include <encodings_fragment>\n"
			"gl_FragColor.rgb = gl_FragColor.rgb*u_selectColor.rgb;\n"
			"}"
			;
	}

#else
	const char *ShaderSrcCode::BackgroundVert()
	{
		return "precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec4 a_color;\n"
			"uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_MVPMat * vec4(a_position,1.0);\n"
			"v_color = a_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::BackgroundFrag()
	{
		return "precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}
    const char* ShaderSrcCode::XRayVert()
    {
        return
        "attribute vec3 a_position;\n"
        "attribute vec3 a_normal;\n"
        "uniform mat4 u_modelMat;\n"
        "uniform mat4 u_viewMat;\n"
        "uniform mat4 u_projectionMat;\n"
        "uniform mat3 u_normalMat;\n"
        "uniform vec4 u_Color;\n"
        "varying vec3 N;\n"
        "varying vec3 I;\n"
        "varying vec4 Cs;\n"
        "void main()\n"
        "{\n"
        "vec4 P = u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
        "I = P.xyz - vec3(0);\n"
        "N = normalize(u_normalMat * a_normal);\n"
        "Cs = u_Color;\n"
        "gl_Position = u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
        "};\n";
    }
    const char* ShaderSrcCode::XRayFrag()
    {
        return
        "varying vec3 N;\n"
        "varying vec3 I;\n"
        "varying vec4 Cs;\n"
        // globals
        "uniform float edgefalloff;\n"
        "void main()\n"
        "{\n"
        "float opac = dot(normalize(N), normalize(-I));\n"
        "opac = abs(opac);\n"
        "opac = 1.0 - pow(opac, edgefalloff);\n"
        "gl_FragColor = opac * Cs;\n"
        "gl_FragColor.a = opac;\n"
        "}\n";
    }
    const char* ShaderSrcCode::DraggerVert()
    {
        return
        "//precision highp  float;\n"
        "attribute vec3 a_position;\n"
        "attribute vec3 a_normal;\n"
        "uniform vec4 u_color;\n"
        "uniform mat4 u_modelMat;\n"
        "uniform mat4 u_viewMat;\n"
        "uniform mat4 u_projectionMat;\n"
        "uniform mat4 u_normalMat;\n"
        "varying vec4 v_color;\n"
        "varying vec3 v_normal;\n"
        "varying vec3 v_position;\n"
        "void main() { \n"
        "gl_Position = u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
        "v_color = u_color;\n"
        "gl_Position.z = -gl_Position.w;\n"//gl_Position.z - 0.001f;\n"
        //"v_normal = mat3(transpose(inverse(u_modelMat))) * a_normal;\n"
        "v_normal = vec3(u_normalMat*vec4(a_normal,0));\n"
        "v_position = vec3(gl_Position);\n"
        "}\n";
    }
    
    
    const char* ShaderSrcCode::DraggerFrag()
    {
        return
        //"precision mediump  float;\n"
        "varying vec4 v_color;\n"
        //"varying vec3 v_normal;\n"
        "varying vec3 v_normal;\n"
        "varying vec3 v_position;\n"
        "void main() { \n"
        // ambient
        //"float ambientStrength = 0.1;\n"
        //"vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f);\n"
        "const vec3 lightPos =  vec3(10.0,10.0,10.0);\n"
        "const vec3 lightColor =  vec3(1.0,1.0,1.0);\n"
        // diffuse
        //"vec3 norm = normalize(v_normal);\n"
        //"vec3 lightDir = normalize(lightPos - FragPos);\n"
        //"float diff = max(dot(norm, lightDir), 0.0);\n"
        //"vec3 diffuse = diff * lightColor;\n"
        //"vec3 result = (ambient) * v_color;\n"
        //"gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
        "vec3 norm = normalize(v_normal);\n"
        "vec3 lightDir = normalize(lightPos - v_position);\n"
        "float diff = max(dot(norm, lightDir), 0.0);\n"
        "vec3 diffuse = (diff +0.5) * lightColor;\n"
        "vec4 result = vec4(diffuse.xyz,1.0) * v_color;\n"
        "gl_FragColor =vec4(result.xyzw);\n"
        "}\n";
    }
    
    const char* ShaderSrcCode::DraggerEdgeVert()
    {
        return
        "//precision highp  float;\n"
        "attribute vec3 a_position;\n"
        "uniform vec4 u_color;\n"
        "uniform mat4 u_modelMat;\n"
        "uniform mat4 u_viewMat;\n"
        "uniform mat4 u_projectionMat;\n"
        "varying vec4 v_color;\n"
        "void main() { \n"
        "gl_Position = u_projectionMat*u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
        "v_color = u_color;\n"
        "gl_Position.z = -gl_Position.w;\n"//gl_Position.z - 0.001f;\n"
        "}\n";
    }
    
    const char* ShaderSrcCode::DraggerEdgeFrag()
    {
        return
        //"precision mediump  float;\n"
        "varying vec4 v_color;\n"
        "void main() { \n"
        "gl_FragColor =v_color;\n"
        "}\n";
    }
	const char *ShaderSrcCode::ModelBaseVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"v_texCoords = tempCoords;\n"
			"v_viewDirection  = normalize(u_eyePosition -fvObjectPosition.xyz);\n"
			"v_lightDirection = normalize(u_lightPosition-fvObjectPosition.xyz);\n"
			" v_normal         = normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"}\n";
	}
	const char *ShaderSrcCode::ModelBaseFrag()
	{
		return "#extension GL_OES_standard_derivatives:enable \n"
			"precision mediump  float;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_specular;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform bool u_useTex;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3){\n"
			"float m;\n"
			"vec3 r;\n vec3 u;\n"
			"u = normalize(ecPosition3);\n"
			"r = reflect(u, normal);\n"
			"m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"const vec4 ambientColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"const vec4 diffuseColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"const float shiness = 30.0;\n"
			"const vec4 specularColor = vec4(0.5,0.5,0.5,1.0);\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"void main() { \n"
			"float fNDotL =0.0;\n"
			"vec3 	halfvector;\n"
			"float fNDotH;"
			" vec3  fvNormal ;\n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"if(gl_FrontFacing){\n"
			" fvNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" fvNormal         = normalize( -v_normal );\n"
			"}\n"
			"vec2 sphereCoords = sphereMap(fvNormal,v_position);\n"
			"//switchColor = switchColor * texture2D(u_sampler1,sphereCoords.xy);\n"
			" fNDotL           = clamp(dot( fvNormal, v_lightDirection ),0.001,1.0);\n"
			"halfvector        =normalize(v_lightDirection+ v_viewDirection );\n"
			" fNDotH            = dot(fvNormal,halfvector);\n"
			"if(u_useTex){\n"
			"switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"}\n"
			"vec4  fvTotalAmbient   = u_ambient * ambientColor;\n"
			"vec4  fvTotalDiffuse   = switchColor*u_diffuse * fNDotL * diffuseColor; \n"
			"vec4  fvTotalSpecular  = u_specular * specularColor *( pow( max(fNDotH,0.0), shiness ) );\n"
			"vec4 colorLinear = fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular;\n"
			"gl_FragColor = vec4(colorLinear.xyz,u_diffuse.a);\n"
			"}\n";
	}

	const char *ShaderSrcCode::EdgeVert()
	{
		return " precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"#ifdef DEPTH_EDGE\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#ifdef DEPTH_EDGE\n"
			"v_projectPos = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#endif\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"gl_PointSize = 3.0;\n"
			"}\n";
	}
	const char *ShaderSrcCode::EdgeFrag()
	{
		return "precision highp  float;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"#ifdef DEPTH_EDGE\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"#ifdef DEPTH_EDGE\n"
			"vec3 proj =v_projectPos.xyz/v_projectPos.w;\n "
			"   proj = proj *0.5+0.5;\n"
			"float priDepth = texture2D(u_sampler0,proj.xy).x;\n"
			"if(priDepth<proj.z) discard;\n"
			"#endif\n"
			"gl_FragColor = u_diffuse;\n"
			"}\n";
	}

	const char *ShaderSrcCode::EdgeFragMediump()
	{
		return "precision mediump  float;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"#ifdef DEPTH_EDGE\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"#ifdef DEPTH_EDGE\n"
			"vec3 proj =v_projectPos.xyz/v_projectPos.w;\n "
			"   proj = proj *0.5+0.5;\n"
			"float priDepth = texture2D(u_sampler0,proj.xy).x;\n"
			"if(priDepth<proj.z) discard;\n"
			"#endif\n"
			"gl_FragColor = u_diffuse;\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteEdgeVert()
	{
		return " precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"#ifdef DEPTH_EDGE\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#ifdef DEPTH_EDGE\n"
			"v_projectPos = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"#endif\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"gl_PointSize = 3.0;\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteEdgeFrag()
	{
		return  //"#extension GL_EXT_frag_depth : enable\n"
			"precision highp  float;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useMinDepth;\n"
			"#ifdef DEPTH_EDGE\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec4 v_projectPos;\n"
			"#endif\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"#ifdef DEPTH_EDGE\n"
			"vec3 proj =v_projectPos.xyz/v_projectPos.w;\n "
			"   proj = proj *0.5+0.5;\n"
			"float priDepth = texture2D(u_sampler0,proj.xy).x;\n"
			"if(priDepth<proj.z) discard;\n"
			"#endif\n"
			//"if(u_useMinDepth == true)gl_FragDepthEXT = 0.0;\n"
			"gl_FragColor = u_diffuse;\n"
			"}\n";
	}

	const char *ShaderSrcCode::WireframeVert()
	{
		return
			"precision mediump  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_center;\n"
			"void main(){\n"
			"v_center = a_normal;\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"}\n";
	}
	const char *ShaderSrcCode::WireframeFrag()
	{
		return "#extension GL_OES_standard_derivatives : enable\n"

			"uniform vec4 u_diffuse;\n"
			"varying vec3 v_center;\n"
			"float edgeFactorTri() {\n"
			"vec3 d = fwidth( v_center.xyz );\n"
			"vec3 a3 = smoothstep( vec3( 0.0 ), d * 1.5, v_center.xyz );\n"
			"return min( min( a3.x, a3.y ), a3.z );\n"
			"}\n"
			"void main() { \n"
			"gl_FragColor.rgb = mix( vec3( 1.0 ), vec3( 0.2 ), edgeFactorTri() );\n"
			"gl_FragColor.a = 1.0;\n"
			"}\n";
	}

	const char *ShaderSrcCode::BrdfVert()
	{
		return
			"precision mediump  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3    v_position;"
			"varying vec4 v_worldPosition;\n"
			"void main() { \n"
			"gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 v_worldPosition = u_modelMat* vec4(a_position,1.0);\n"
			"vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"v_texCoords = tempCoords;\n"
			"v_position = fvObjectPosition.xyz;\n"
			"v_viewDirection  = normalize( -fvObjectPosition.xyz);\n"
			"v_lightDirection = normalize(u_lightPosition-fvObjectPosition.xyz);\n"
			" v_normal         = normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"}\n";
	}

	const char *ShaderSrcCode::BrdfFrag()
	{
		return "#extension GL_OES_standard_derivatives:enable \n"
			"precision mediump  float;\n"
			"out vec4 FragColor;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_specular;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform bool u_useTex;\n"
			"uniform float u_fresnel0;\n"
			"uniform float u_roughness;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_viewDirection;\n"
			"varying vec3 v_lightDirection;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3    v_position;"
			"varying vec4 v_worldPosition;\n"
			"const vec3 ambientColor = vec3(1.0, 1.0, 1.0);\n"
			"const vec3 diffuseColor = vec3(1.0, 1.0, 1.0);\n"
			"const float shiness = 65.0;\n"
			"const vec4 specularColor = vec4(0.5,0.5,0.5,1.0);\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"float fresnel(float f0, vec3 n, vec3 l){\n"
			"return f0 + (1.0-f0) * pow(1.0- dot(n, l), 5.0);\n"
			"}\n"
			"float distribution(vec3 n, vec3 h, float roughness){\n"
			"float m_Sq= roughness * roughness;\n"
			"float NdotH_Sq= max(dot(n, h), 0.01);\n"
			"NdotH_Sq= NdotH_Sq * NdotH_Sq;\n"
			"return exp( (NdotH_Sq - 1.0)/(m_Sq*NdotH_Sq) )/ (3.14159265 * m_Sq * NdotH_Sq * NdotH_Sq) ;\n"
			"}\n"
			"float geometry(vec3 n, vec3 h, vec3 v, vec3 l, float roughness){\n"
			"float NdotH= dot(n, h);\n"
			"float NdotL= dot(n, l);\n"
			"float NdotV= dot(n, v);\n"
			"float VdotH= dot(v, h);\n"
			"float NdotL_clamped= max(NdotL, 0.01);\n"
			"float NdotV_clamped= max(NdotV, 0.01);\n"
			"return min( min( 2.0 * NdotH * NdotV_clamped / VdotH, 2.0 * NdotH * NdotL_clamped / VdotH), 1.0);\n"
			"}\n"
			"float diffuseEnergyRatio(float f0, vec3 n, vec3 l){\n"
			" return 1.0 - fresnel(f0, n, l);\n"
			"  }\n"
			"void main() { \n"
			"vec3  fvLightDirection;\n"
			" vec3  fvNormal ;\n"
			" vec3 normal;\n "
			"vec3 view;\n"
			" vec3 halfVec;\n"
			"float NdotL;\n"
			"float NdotV;\n"
			"float NdotL_clamped;\n"
			"float NdotV_clamped;\n"
			"float brdf_spec;\n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"if(gl_FrontFacing){\n"
			" fvNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" fvNormal         = normalize( -v_normal );\n"
			"}\n"
			"fvLightDirection = normalize( v_lightDirection );\n"
			" normal =  normalize(fvNormal);\n"
			"view   = normalize(v_viewDirection);\n"
			" halfVec=  normalize(fvLightDirection + view);\n"
			"NdotL= dot(normal, fvLightDirection);\n"
			"NdotV= dot(normal, view);\n"
			"NdotL_clamped= max(NdotL, 0.01);\n"
			"NdotV_clamped= max(NdotV, 0.01);\n"
			"brdf_spec= fresnel(u_fresnel0, halfVec, fvLightDirection) * \n"
			" geometry(normal, halfVec, view, fvLightDirection, u_roughness) * \n"
			"distribution(normal, halfVec, u_roughness) / (4.0 * NdotL_clamped * NdotV_clamped);\n"
			"if(u_useTex){\n"
			"switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"}\n"
			"vec3 color_spec = NdotL_clamped * brdf_spec * diffuseColor;\n"
			"vec3 color_diff = switchColor.xyz*NdotL_clamped * diffuseEnergyRatio(u_fresnel0, normal, fvLightDirection) * vec3(u_diffuse.xyz) * diffuseColor;\n"
			" gl_FragColor = vec4( color_diff + color_spec, u_diffuse.a)  ; \n"
			"}\n";
	}

	const char *ShaderSrcCode::AxisVert()
	{
		return "precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform vec4 u_color;\n"
			"uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_MVPMat * vec4(a_position,1.0);\n"
			"v_color = u_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::AxisFrag()
	{
		return "precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::ImageVert()
	{
		return "precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ImageFrag()
	{
		return "precision mediump  float;\n"
			"precision mediump sampler2D;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"vec4 pcolor = texture2D(u_sampler0,v_texCoords);\n"
			"vec4 tcolor = pcolor*u_diffuse;\n"
			"gl_FragColor = vec4(tcolor.xyz,pcolor.a);\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteImageVert()
	{
		return "precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}

	const char *ShaderSrcCode::NoteImageFrag()
	{
		return //"#extension GL_EXT_frag_depth : enable\n"
			"precision mediump  float;\n"
			"precision mediump sampler2D;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useMinDepth;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"vec4 pcolor = texture2D(u_sampler0,v_texCoords);\n"
			"vec4 tcolor = pcolor*u_diffuse;\n"
			//"if(u_useMinDepth == true)gl_FragDepthEXT = 0.0;\n"
			"gl_FragColor = vec4(tcolor.xyz,pcolor.a);\n"
			"}\n";
	}

	const char *ShaderSrcCode::CubeMapVert()
	{
		return "precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_cubeTexCoords = a_position;\n"
			"}\n";
	}
	const char *ShaderSrcCode::CubeMapFrag()
	{
		return "precision mediump  float;\n"
			"precision mediump sampler2D;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"void main() { \n"
			"vec4 pcolor = textureCube(u_samplerCube0,v_cubeTexCoords);\n"
			"gl_FragColor = pcolor;\n"
			"}\n";
	}

	const char *ShaderSrcCode::ClipPlaneVert()
	{
		return "precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"uniform vec4 u_color;\n"
			"uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position = u_MVPMat * vec4(a_position,1.0);\n"
			"v_color = u_color;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ClipPlaneFrag()
	{
		return "precision highp  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ClipPlaneFragMediump()
	{
		return "precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::MultilightVert()
	{
		return "precision highp  float;\n"
			"//#define FBO \n"
			"struct Light\n"
			"{\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    vec4 position;\n"
			"    vec3 intensity;\n"
			"    vec3 spotDirection;\n"
			"    float spotExponent;\n"
			"    float spotCutoff; // (range: [0.0,90.0], 180.0)\n"
			"    float spotCosCutoff; // (range: [1.0,0.0],-1.0)\n"
			"    float constant;\n"
			"    float linear;\n"
			"    float quadratic;\n"
			"};\n"
			"\n"
			"struct LightModel\n"
			"{\n"
			"    vec4 ambient;    // Acs\n"
			"};\n"
			"\n"
			"struct Materials\n"
			"{\n"
			"    vec4 emission;\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    float shininess;\n"
			"};\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"#ifdef FBO \n"
			"uniform mat4 u_lightMat;\n"
			"uniform mat4 u_lightProject;\n"
			"#endif \n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"\n"
			"//varying\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_diffuseColor;\n"
			"varying vec3 v_specularColor;\n"
			"varying vec3 v_diffuseColorN;\n"
			"varying vec3 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"uniform vec3 u_shadowLPos;\n"
			"#endif \n"
			"//scene ambient\n"
			"uniform LightModel u_lightModel ;\n"
			"const int numberOfLights = 1;\n"
			"uniform Light u_lights[1];\n"
			"uniform int u_enbleLights[1];\n"
			"//Materials\n"
			"uniform Materials u_materials;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"    vec3 totalLighting = vec3(u_materials.emission)+ vec3(u_lightModel.ambient) * vec3(u_materials.ambient);\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalAmbientBack= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuseBack = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecularBack = vec3(0.0,0.0,0.0);\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"#ifdef FBO \n"
			"    v_lightSpacePos = u_lightProject*u_lightMat *u_modelMat* vec4(a_position,1.0);\n"
			"    v_worldNormal = a_normal;\n"
			"#endif \n"
			"    vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_texCoords = tempCoords;\n"
			"    vec3 vNormal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    v_normal = vNormal;\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    v_cubeTexCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(a_normal));\n"
			"#ifdef FBO\n"
			"   v_worldPos = worldPosT;\n"
			"#endif\n"
			"    for(int index = 0;index<numberOfLights;index++)\n"
			"    {\n"
			"        if(u_enbleLights[index] == 1)\n"
			"        {\n"
			"\n"
			"            if (0.0 == u_lights[index].position.w) // directional light?\n"
			"            {\n"
			"                attenuation = 1.0; // no attenuation\n"
			"                lightDirection = normalize(vec3(u_lights[index].position));\n"
			"            }\n"
			"            else // point light or spotlight (or other kind of light)\n"
			"            {\n"
			"                vec3 positionToLightSource = vec3(u_lights[index].position) - fvObjectPosition.xyz ;\n"
			"                float distance = length(positionToLightSource);\n"
			"                lightDirection = normalize(positionToLightSource);\n"
			"                float tempA= u_lights[index].constant + u_lights[index].linear * distance + u_lights[index].quadratic * distance * distance;\n"
			"                attenuation = 1.0 / tempA;\n"
			"                if (u_lights[index].spotCutoff <= 90.0) // spotlight?\n"
			"                {\n"
			"                    float clampedCosine = max(0.0001, dot(lightDirection, -normalize(u_lights[index].spotDirection)));\n"
			"                    if (clampedCosine < u_lights[index].spotCosCutoff) // outside of spotlight cone?\n"
			"                    {\n"
			"                        attenuation = 0.0;\n"
			"                    }\n"
			"                    else\n"
			"                    {\n"
			"                        attenuation = attenuation * pow(clampedCosine, u_lights[index].spotExponent);\n"
			"//attenuation *= clamp((clampedCosine-cos(radians(15.0)) ) / (cos(radians(10.0)) - cos(radians(15.0))), 0.0, 1.0);\n"
			"                    }\n"
			"                }\n"
			"            }\n"
			"\n"
			"         //a side\n"
			"        float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"        vec3 diffuseReflection = attenuation\n"
			"            * vec3(u_lights[index].diffuse) * vec3(u_materials.diffuse)\n"
			"            * fNDotL;\n"
			"        //the othor\n"
			"        fNDotL= clamp(abs(dot( -vNormal, lightDirection )),0.3,1.0);\n"
			"        vec3 diffuseReflectionBack = attenuation\n"
			"            * vec3(u_lights[index].diffuse) * vec3(u_materials.diffuse)\n"
			"            * fNDotL;\n"
			"\n"
			"         totalAmbient =vec3(totalAmbient) + vec3(u_lights[index].ambient) * vec3(u_materials.ambient);\n"
			"		  float intensity = u_lights[index].intensity.x;\n"
			"         totalDiffuse = totalDiffuse + diffuseReflection*intensity;\n"
			"         totalDiffuseBack = totalDiffuseBack + diffuseReflectionBack*intensity;\n"
			"\n"
			"        vec3 eyeDirection = normalize(- fvObjectPosition.xyz );\n"
			"        vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"\n"
			"		vec3 tempsp = vec3(u_lights[index].specular);\n"
			"		float spv =  tempsp.x*tempsp.x+tempsp.y*tempsp.y+tempsp.z*tempsp.z;\n"
			"			if(spv>0.0){\n"
			"        //a side\n"
			"        float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"        vec3 specularReflection = attenuation * vec3(u_lights[index].specular) * vec3(u_materials.specular)\n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"        //the other\n"
			"        fNDotH = max(0.0, abs(dot(-vNormal,halfVector)));\n"
			"        vec3 specularReflectionBack = attenuation * vec3(u_lights[index].specular) * vec3(u_materials.specular)\n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"\n"
			"         totalSpecular = totalSpecular + specularReflection*intensity;\n"
			"         totalSpecularBack = totalSpecularBack + specularReflectionBack*intensity;\n"
			"			}\n"
			"        }\n"
			"    }\n"
			"    v_diffuseColor = min(totalLighting+totalAmbient+totalDiffuse,vec3(1.0));\n"
			"    v_specularColor = min(totalSpecular,vec3(1.0));\n"
			"\n"
			"    v_diffuseColorN = min(totalLighting+totalAmbient+totalDiffuseBack,vec3(1.0));\n"
			"    v_specularColorN = min(totalSpecularBack,vec3(1.0));\n"
			" }\n"

			;
	}
	const char *ShaderSrcCode::MultilightFrag()
	{
		return "precision highp  float;\n"
			"//#define FBO \n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform bool u_useAmbientTex;\n"
			"uniform bool u_useCubeMapTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_diffuseColor;\n"
			"varying vec3 v_specularColor;\n"
			"varying vec3 v_diffuseColorN;\n"
			"varying vec3 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"uniform vec3 u_shadowLPos;\n"
			"uniform float u_xPixelOffset;\n"
			"uniform float u_yPixelOffset;\n"
			"#endif \n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0);\n"
			"#ifdef FBO \n"
			"float ShadowCalculate(sampler2D sampler,vec4 fragPosLightSpace)\n"
			"{\n"
			"   float shadow = 1.0;\n"
			"	vec3 proj =fragPosLightSpace.xyz/fragPosLightSpace.w;\n"
			"   proj = proj *0.5+0.5;\n"
			"   float currentDepth = proj.z;\n"
			"   vec3 normal = normalize(v_worldNormal);\n"
			"   vec3 lightDir = normalize(u_shadowLPos-v_worldPos);\n"
			"   float bias = max(0.05*(1.0-dot(normal,lightDir)),0.005);\n"
			"   for(int y=-1; y<=1;y++)\n"
			"   {\n"
			"       for(int x=-1; x<=1;x++)\n"
			"        {\n"
			"  				 float closeDepth = texture2D(sampler,(proj+vec3(float(x)*u_xPixelOffset,float(y)*u_yPixelOffset,0.05)).xy).x;\n "
			"                shadow += (closeDepth<(currentDepth-bias))?0.5:1.0;\n"
			"         }\n"
			"    }\n"
			"   shadow /=16.0;\n"
			"   shadow += 0.2;\n"
			"   return shadow;\n"
			"}\n"
			"#endif \n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			"    if(u_useTex)\n"
			"    {\n"
			"        switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"    }\n"
			"    if(u_useAmbientTex)\n"
			"    {\n"
			"        vec2 sphereCoords = sphereMap(v_normal,v_position);\n"
			"        environment = texture2D(u_sampler1,sphereCoords.xy);\n"
			"    }\n"
			"    if(u_useCubeMapTex)\n"
			"    {\n"
			"        environment = textureCube(u_samplerCube0,v_cubeTexCoords.xyz);\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment;\n"
			" 	float shadow = 1.0;\n"
			"#ifdef FBO \n"
			"	 shadow = ShadowCalculate(u_sampler2,v_lightSpacePos);\n"
			"#endif \n"
			"	float alpha = u_diffuse.a;\n"
			"    if(gl_FrontFacing)\n"
			"    {\n"
			"        gl_FragColor = vec4(vec3(texColor)*v_diffuseColor*(shadow)*vec3(u_selectColor)+vec3(v_specularColor)*(shadow),alpha);\n"
			"    }\n"
			"    else\n"
			"    {\n"
			"        gl_FragColor = vec4(vec3(texColor)*v_diffuseColorN*(shadow)*vec3(u_selectColor)+vec3(v_specularColorN)*(shadow),alpha);\n"
			"    }\n"
			"}\n"
			"\n"

			;
	}

	const char *ShaderSrcCode::BaseVertexVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_specular;\n"
			"vec3 vviewDirection;\n"
			"vec3 vlightDirection;\n"
			"vec3 vnormal;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec4 v_diffuseColor;\n"
			"varying vec4 v_specularColor;\n"
			"varying vec4 v_diffuseColorN;\n"
			"varying vec4 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"const vec4 ambientColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"const vec4 diffuseColor = vec4(1.0, 1.0, 1.0,1.0);\n"
			"uniform float u_shininess;\n"
			"const vec4 specularColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"void main() {\n"
			"gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"v_texCoords = tempCoords;\n"
			"vviewDirection  = normalize( -fvObjectPosition.xyz);\n"
			"vlightDirection = normalize(u_lightPosition-fvObjectPosition.xyz);\n"
			"vnormal         = normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"v_normal = vnormal;\n"
			"float fNDotL =0.0;\n"
			"vec3 halfvector;\n"
			"float fNDotH;\n"
			"fNDotL           = clamp(abs(dot( vnormal, vlightDirection )),0.3,1.0);\n"
			"halfvector        =normalize(vlightDirection+ vviewDirection );\n"
			"fNDotH            = dot(vnormal,halfvector);\n"
			"vec4  fvTotalAmbient   = u_ambient * ambientColor;\n"
			"vec4  fvTotalDiffuse   = switchColor*u_diffuse * fNDotL * diffuseColor;\n"
			"vec4  fvTotalSpecular  = u_specular * specularColor *( pow( max(fNDotH,0.0), u_shininess ) );\n"
			"v_diffuseColor=min(fvTotalAmbient + fvTotalDiffuse,vec4(1.0));\n"
			"v_specularColor = min(fvTotalSpecular,vec4(1.0));\n"
			"\n"
			"fNDotL           = clamp(abs(dot( -vnormal, vlightDirection )),0.3,1.0);\n"
			"fNDotH            = dot(-vnormal,halfvector);\n"
			"fvTotalDiffuse   = switchColor*u_diffuse * fNDotL * diffuseColor;\n"
			"fvTotalSpecular  = u_specular * specularColor *( pow( max(fNDotH,0.0), u_shininess ) );\n"
			"v_diffuseColorN=min(fvTotalAmbient + fvTotalDiffuse,vec4(1.0));\n"
			"v_specularColorN = min(fvTotalSpecular,vec4(1.0));\n"
			""
			"}\n"
			"\n"

			;
	}
	const char *ShaderSrcCode::BaseVertexFrag()
	{
		return
			"precision mediump  float;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec4 v_diffuseColor;\n"
			"varying vec4 v_specularColor;\n"
			"varying vec4 v_diffuseColorN;\n"
			"varying vec4 v_specularColorN;\n"
			"varying vec3 v_normal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0);\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			"    if(u_useTex){\n"
			"          switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"                }\n"
			"	vec4 texColor = switchColor *environment;\n"
			"    if(gl_FrontFacing){\n"
			"    	  gl_FragColor = vec4(vec3(texColor*v_diffuseColor)*vec3(u_selectColor)+vec3(v_specularColor),u_diffuse.a+1.0);\n"
			"    }else{\n"
			"         gl_FragColor = vec4(vec3(texColor*v_diffuseColorN)*vec3(u_selectColor)+vec3(v_specularColorN),u_diffuse.a+1.0);\n"
			"    }\n"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::ShadowMapVert()
	{
		return "attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"}\n";
	}
	const char *ShaderSrcCode::ShadowMapFrag()
	{
		return
			"precision mediump  float;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"
			"gl_FragColor = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1.0);\n"
			"}\n";
	}

	const char *ShaderSrcCode::FBODebugVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}
	const char *ShaderSrcCode::FBODebugFrag()
	{
		return "precision highp  float;\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec2 v_texCoords;\n"
			"varying vec3 v_position;\n"
			"void main() { \n"
			"vec4 pcolor = texture2D(u_sampler0,v_texCoords);\n"
			"vec4 tcolor = pcolor;\n"
			"gl_FragColor = tcolor;\n"
			"}\n";
	}

	const char *ShaderSrcCode::PlaneShadowVert()
	{
		return

			"#define FBO \n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"#ifdef FBO \n"
			"uniform mat4 u_lightMat;\n"
			"uniform mat4 u_lightProject;\n"
			"#endif \n"
			"\n"
			"varying vec3 v_position;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"#endif \n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec3 lightDirection;\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"v_position = fvObjectPosition.xyz;\n"
			"#ifdef FBO \n"
			"    v_lightSpacePos = u_lightProject*u_lightMat *u_modelMat* vec4(a_position,1.0);\n"
			"    v_worldNormal = a_normal;\n"
			"#endif \n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    v_worldPos = worldPos.xyz;\n"
			" }\n";
	}
	const char *ShaderSrcCode::PlaneShadowFrag()
	{
		return
			"precision mediump  float;\n"
			"#define FBO \n"
			"uniform sampler2D u_sampler0;\n"
			"uniform vec4 u_diffuse;\n"
			"varying vec3 v_position;\n"
			"#ifdef FBO \n"
			"varying vec4 v_lightSpacePos;\n"
			"varying vec3 v_worldNormal;\n"
			"varying vec3 v_worldPos;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform vec3 u_shadowLPos;\n"
			"uniform float u_xPixelOffset;\n"
			"uniform float u_yPixelOffset;\n"
			"#endif \n"
			"float ShadowCalculate(sampler2D sampler,vec4 fragPosLightSpace)\n"
			"{\n"
			"   float shadow = 1.0;\n"
			"	vec3 proj =fragPosLightSpace.xyz/fragPosLightSpace.w;\n"
			"   proj = proj *0.5+0.5;\n"
			"  if(proj.x>1.0 || proj.x<0.0 || proj.y>1.0 || proj.y<0.0) return 1.0;\n"
			"   float currentDepth = proj.z;\n"
			"   vec3 normal = normalize(v_worldNormal);\n"
			"   vec3 lightDir = normalize(u_shadowLPos-v_worldPos);\n"
			"   float bias = max(0.05*(1.0-dot(normal,lightDir)),0.005);\n"
			"   for(int y=-1; y<=1;y++)\n"
			"   {\n"
			"       for(int x=-1; x<=1;x++)\n"
			"        {\n"
			"  				 float closeDepth = texture2D(sampler,(proj+vec3(float(x)*u_xPixelOffset,float(y)*u_yPixelOffset,0.05)).xy).x;\n "
			"                shadow += (closeDepth<(currentDepth-bias+bias*0.99))?0.5:1.0;\n"
			"         }\n"
			"    }\n"
			"   shadow /=16.0;\n"
			"   return shadow;\n"
			"}\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			" 	float shadow = 1.0;\n"
			"#ifdef FBO \n"
			"	 shadow = ShadowCalculate(u_sampler0,v_lightSpacePos);\n"
			"#endif \n"
			"float alpha = 1.0;\n"
			"if(shadow < 1.0 && shadow>0.62) alpha = 0.0;\n"
			"    gl_FragColor = vec4(u_diffuse.rgb*shadow,u_diffuse.a *(1.0-shadow)*alpha);\n"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::BlurVert()
	{
		return

			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}
	const char *ShaderSrcCode::BlurFrag()
	{
		return "#extension GL_OES_standard_derivatives : enable\n"
			"precision mediump  float;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"varying vec2 v_texCoords;\n"
			"const float v = 1.0/254.0;\n"
			" float weights[39] ; \n"
			"float samples[26] ;\n"
			"void main(void)\n"
			"{\n"
			" weights[0] = 0.250441;\n"
			" weights[1] = 0.437000;\n weights[2] =  0.635000;\n"
			" weights[3] =0.096356;\n weights[4] =0.064487;\n  weights[5] =0.039097;\n"
			" weights[6] =0.136575;\n weights[7] = 0.103222;\n weights[8] = 0.064912;\n"
			"weights[9] = 0.074844;\n  weights[10] =0.086388;\n weights[11] =0.062272;\n"
			" weights[12] =0.131798;\n  weights[13] =0.151695;\n weights[14] =0.103676;\n"
			" weights[15] = 0.035690;\n  weights[16] =0.042728;\n weights[17] =0.033003;\n"
			"  weights[18] =0.048593;\n  weights[19] =0.064740;\n  weights[20] =0.046131;\n"
			" weights[21] =0.048092;\n  weights[22] =0.003042;\n weights[23] =0.000400;\n"
			"weights[24] =0.028845;\n  weights[25] =0.005406;\n  weights[26] =0.001222;\n"
			"  weights[27] =0.031322;\n  weights[28] =0.006034;\n  weights[29] =0.001420;\n"
			" weights[30] =0.031428;\n weights[31] =0.009152;\n weights[32] =0.002511;\n"
			"  weights[33] =0.010936;\n weights[34] =0.002868;\n  weights[35] =0.000652;\n"
			" weights[36] =0.033588;\n weights[37] =0.023239;\n  weights[38] =0.009703;\n"
			" samples[0] =0.000000;\n  samples[1] =0.000000;\n"
			" samples[2] =1.633992;\n samples[3] =0.036795;\n"
			"  samples[4] =0.177801;\n  samples[5] =1.717593;\n"
			" samples[6] =-0.194986;\n  samples[7] =0.091894;\n"
			" samples[8] =-0.239737;\n  samples[9] =-0.220217;\n"
			"samples[10] =0.003530;\n  samples[11] =-0.118219;\n"
			" samples[12] =1.32010;\n  samples[13] =-0.181542;\n"
			"samples[14] =5.970690;\n  samples[15] =0.253378;\n"
			"samples[16] =-1.089250;\n samples[17] = 4.958349;\n"
			" samples[18] = -4.015465;\n samples[19] = 4.156699;\n"
			"samples[20] =-4.063099;\n  samples[21] =-4.110150;\n"
			" samples[22] =-0.638605;\n  samples[23] =-6.297663;\n"
			" samples[24] =2.542348;\n  samples[25] =-3.245901;\n"
			"    vec3 sum = vec3( 0.0 );\n"
			"    sum.r += texture2D( u_sampler0, vec2( v_texCoords.s + samples[0] * v, v_texCoords.t + samples[1] * v ) ).r * weights[0];\n"
			"    sum.g += texture2D( u_sampler0, vec2( v_texCoords.s + samples[0] * v, v_texCoords.t + samples[1] * v ) ).g * weights[1];\n"
			"    sum.b += texture2D( u_sampler0, vec2( v_texCoords.s + samples[0] * v, v_texCoords.t + samples[1] * v ) ).b * weights[2];\n"
			"    for(int i = 1;i< 13 ;i++)\n"
			"    {\n"
			"        vec2 samplesTmp = vec2(samples[2*i],samples[2*i+1]);\n"
			"        vec3 weightsTmp = vec3(weights[3*i],weights[3*i+1],weights[3*i+2]);\n"
			"        float u_sampler1 = texture2D(u_sampler1, vec2( v_texCoords.s + samplesTmp.x * v, v_texCoords.t + samplesTmp.y * v ) ).r;\n"
			"        float dx = 1.0 - dFdx(u_sampler1);\n"
			"        float dy = 1.0 - dFdy(u_sampler1);\n"
			"        sum.r += texture2D( u_sampler0, vec2( v_texCoords.s + samplesTmp.x * v * dx, v_texCoords.t + samplesTmp.y * v * dy ) ).r * weightsTmp.r;\n"
			"        sum.g += texture2D( u_sampler0, vec2( v_texCoords.s + samplesTmp.x * v * dx, v_texCoords.t + samplesTmp.y * v * dy ) ).g * weightsTmp.g;\n"
			"        sum.b += texture2D( u_sampler0, vec2( v_texCoords.s + samplesTmp.x * v * dx, v_texCoords.t + samplesTmp.y * v * dy ) ).b * weightsTmp.b;\n"
			"    }\n"
			"\n"
			"    gl_FragColor = vec4(vec3(sum),1.0);\n"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::SpecularVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"\n"
			"void main()\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"}\n";
	}
	const char *ShaderSrcCode::SpecularFrag()
	{
		return "#extension GL_OES_standard_derivatives : enable\n"
			"precision mediump  float;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_ambient;\n"
			"uniform vec4 u_selectColor;\n"
			"uniform vec3 u_lightPosition;\n"
			"\n"
			"float beckmannDistribution(in float x, in float roughness)\n"
			"{\n"
			"    float NdotH = max(x, 0.0001);\n"
			"    float cos2Alpha = NdotH * NdotH;\n"
			"    float tan2Alpha = (cos2Alpha - 1.0) / cos2Alpha;\n"
			"    float roughness2 = roughness * roughness;\n"
			"    float denom = 3.1415926 * roughness2 * cos2Alpha * cos2Alpha *0.05;\n"
			"    return exp(tan2Alpha / roughness2) / denom;\n"
			"}\n"
			"\n"
			"float cookTorranceSpecular(in vec3 lightDirection, in vec3 viewDirection, in vec3 surfaceNormal, in float roughness, in float fresnel)\n"
			"{\n"
			"    float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);\n"
			"    float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);\n"
			"    vec3 H = normalize(lightDirection + viewDirection);\n"
			"    float NdotH = max(dot(surfaceNormal, H), 0.0);\n"
			"    float VdotH = max(dot(viewDirection, H), 0.000001);\n"
			"    float LdotH = max(dot(lightDirection, H), 0.000001);\n"
			"    float G1 = (2.0 * NdotH * VdotN) / VdotH;\n"
			"    float G2 = (2.0 * NdotH * LdotN) / LdotH;\n"
			"    float G = min(1.0, min(G1, G2));\n"
			"    float D = beckmannDistribution(NdotH, roughness);\n"
			"    float F = fresnel;\n"
			"    return  G * F * D / (3.14159265 *max(VdotN, 0.000001)*max(LdotN, 0.000001));\n"
			"}\n"
			"void main(void)\n"
			"{\n"
			"    if (u_reverseClip) {\n"
			"        int iEnableNum = 0; \n"
			"        int iClipNum = 0; \n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                iEnableNum++; \n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > -0.0001) {\n"
			"                    iClipNum++; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"        if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"            discard; \n"
			"        }\n"
			"    }\n"
			"    else {\n"
			"        for (int i = 0; i < 3; i++) {\n"
			"            if (u_enableClips[i]) {\n"
			"                float dPara = u_clipPlanes[i].w; \n"
			"                if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0001) {\n"
			"                    discard; \n"
			"                }\n"
			"            }\n"
			"        }\n"
			"    }\n"
			"if(gl_FrontFacing)"
			"{"
			"    float specular = cookTorranceSpecular(normalize(u_lightPosition), -normalize(v_position), v_normal, 0.027, 0.04);\n"
			"    vec3 diff = vec3(max(dot(v_normal,normalize(u_lightPosition)),0.0));\n"
			"    gl_FragColor = vec4(mix(u_ambient.xyz,vec3(diff)* u_diffuse.xyz*vec3(u_selectColor),0.95) + vec3(specular), u_diffuse.a);\n"
			"}"
			"else"
			"{"
			"    float specular = cookTorranceSpecular(normalize(u_lightPosition), -normalize(v_position),- v_normal, 0.027, 0.04);\n"
			"    vec3 diff = vec3(max(abs(dot(-v_normal,normalize(u_lightPosition))),0.0));\n"
			"    gl_FragColor = vec4(mix(u_ambient.xyz,vec3(diff)* u_diffuse.xyz*vec3(u_selectColor),0.95) + vec3(specular), u_diffuse.a);\n"
			"}"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::EdgeDetectionVert()
	{
		return "//precision highp  float;"
			"attribute vec3 a_position;\n"
			"attribute vec2 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec2 v_texCoords;\n"
			"void main(){\n"
			"gl_Position = u_projectionMat *u_viewMat*u_modelMat*vec4(a_position,1.0);\n"
			"v_texCoords = a_texCoords;\n"
			"}\n";
	}
	const char *ShaderSrcCode::EdgeDetectionFrag()
	{
		return "precision highp  float;"
			"uniform sampler2D u_sampler0;\n"
			"uniform float u_xPixelOffset;\n"
			"uniform float u_yPixelOffset;\n"
			"varying vec2 v_texCoords;\n"
			"void main( void )\n"
			"{\n"
			"float _Size = 1024.0;\n"
			"   vec3 lum = vec3(0.2125,0.7154,0.0721);\n"
			"     float mc00 = dot(texture2D (u_sampler0, v_texCoords-vec2(1.0*u_xPixelOffset,1.0*u_yPixelOffset)).rgb, lum);\n"
			"     float mc10 = dot(texture2D (u_sampler0, v_texCoords-vec2(0.0,1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc20 = dot(texture2D (u_sampler0, v_texCoords-vec2(-1.0,1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc01 = dot(texture2D (u_sampler0, v_texCoords-vec2(1.0,0.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc11mc = dot(texture2D (u_sampler0, v_texCoords).rgb, lum);\n"
			"     float mc21 = dot(texture2D (u_sampler0, v_texCoords-vec2(-1.0,0.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc02 = dot(texture2D (u_sampler0, v_texCoords-vec2(1.0,-1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc12 = dot(texture2D (u_sampler0, v_texCoords-vec2(0.0,-1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"     float mc22 = dot(texture2D (u_sampler0, v_texCoords-vec2(-1.0,-1.0)*vec2(u_xPixelOffset,u_yPixelOffset)).rgb, lum);\n"
			"\n"
			"     float GX = -1.0 * mc00 + mc20 + -2.0 * mc01 + 2.0 * mc21 - mc02 + mc22;\n"
			"     float GY = mc00 + 2.0 * mc10 + mc20 - mc02 - 2.0 * mc12 - mc22;\n"
			"     vec4 c;\n"
			"     float gratValue =length(vec2(GX,GY));\n "
			"     c = vec4(gratValue,gratValue,gratValue,1.0);\n"
			"\n"
			"      float factor = 1.0-c.x;\n"
			"      if(factor>0.92)\n"
			"      {\n"
			"      factor = 1.0;\n"
			"      }\n"
			"      else\n"
			"      {\n"
			"      factor = 0.0;\n"
			"      }\n"
			"   if(texture2D (u_sampler0, v_texCoords).a<0.33)"
			"{gl_FragColor = vec4(factor,0.0,0.0,1.0);}\n"
			"else"
			"{ gl_FragColor = vec4(factor,factor,factor,1.0);\n}"
			"}\n"
			"\n";
	}

	const char *ShaderSrcCode::QuadVert()
	{
		return "//precision highp  float;\n"
			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}
	const char *ShaderSrcCode::QuadFrag()
	{
		return "precision highp  float;\n"
			"uniform sampler2D u_sampler0;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"gl_FragColor =texture2D(u_sampler0,v_texCoords);\n"
			"}\n";
	}
	const char *ShaderSrcCode::QuadFragMediump()
	{
		return "precision mediump  float;\n"
			"precision mediump sampler2D;"
			"uniform sampler2D u_sampler0;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"gl_FragColor =texture2D(u_sampler0,v_texCoords);\n"
			"}\n";
	}
	const char *ShaderSrcCode::MultilightPerFragVert()
	{
		return

			"precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    v_cubeTexCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(a_normal));\n"
			" }\n";
	}
	const char *ShaderSrcCode::MultilightPerFragFrag()
	{
		return

			"precision highp  float;\n"
			"precision highp  int;\n"
			"//precision mediump sampler2D;\n"
			"//precision mediump samplerCube;\n"
			"struct Light\n"
			"{\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    vec4 position;\n"
			"    vec3 intensity;\n"
			"    vec3 spotDirection;\n"
			"    float spotExponent;\n"
			"    float spotCutoff; // (range: [0.0,90.0], 180.0)\n"
			"    float spotCosCutoff; // (range: [1.0,0.0],-1.0)\n"
			"    float constant;\n"
			"    float linear;\n"
			"    float quadratic;\n"
			"};\n"
			"\n"
			"struct LightModel\n"
			"{\n"
			"    vec4 ambient;    // Acs\n"
			"};\n"
			"\n"
			"struct Materials\n"
			"{\n"
			"    vec4 emission;\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    float shininess;\n"
			"};\n"
			"uniform LightModel u_lightModel ;\n"
			"const int numberOfLights = 3;\n"
			"uniform Light u_lights[3];\n"
			"uniform int u_enbleLights[3];\n"
			"uniform Materials u_materials;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform bool u_useAmbientTex;\n"
			"uniform bool u_useCubeMapTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"float testF = 0.0;\n"
			"    vec3 testColor= vec3(0.7,0.0,0.0);"
			"    vec3 totalLighting = vec3(u_materials.emission)+ vec3(u_lightModel.ambient) * vec3(u_materials.ambient);\n"
			" vec3 diffuseColor;\n"
			" vec3 specularColor;\n"
			" vec3  vNormal ;\n"
			"if(gl_FrontFacing){\n"
			" vNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" vNormal         = normalize( -v_normal );\n"
			"}\n"

			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"//if (u_enableClip[2] == 1) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"

			"    if(u_useTex)\n"
			"    {\n"
			"        switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"    }\n"
			"    if(u_useAmbientTex)\n"
			"    {\n"
			"        vec2 sphereCoords = sphereMap(v_normal,v_position);\n"
			"        environment = texture2D(u_sampler1,sphereCoords.xy);\n"
			"    }\n"
			"    if(u_useCubeMapTex)\n"
			"    {\n"
			"        environment = textureCube(u_samplerCube0,v_cubeTexCoords.xyz);\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment;\n"
			"	vec3 eyeDirection = (u_projectionMat[3][3] == 0.0)?normalize(fvObjectPosition.xyz):vec3(0.0, 0.0, -1.0);\n"
			"    for(int index = 0;index<numberOfLights;index++)\n"
			"    {\n"
			"        if(u_enbleLights[index] == 1)\n"
			"        {\n"

			"            if (0.0 == u_lights[index].position.w) // directional light?\n"
			"            {\n"

			"                attenuation = 1.0; // no attenuation\n"
			"                lightDirection = normalize(vec3(u_lights[index].position));\n"
			"            }\n"
			"			else{\n"
			"					vec3 d = u_lights[index].position.xyz -fvObjectPosition.xyz;\n "
			"testColor = normalize(d);\n"
			"					lightDirection = normalize(d);\n"
			"					if (u_lights[index].spotCutoff<90.0)\n"
			"					{\n"
			"						float cosine = max(dot(lightDirection, -u_lights[index].spotDirection), 0.0);\n"
			"						attenuation = pow(cosine, u_lights[index].spotExponent);\n"
			"						attenuation *= step(u_lights[index].spotCosCutoff, cosine);\n"
			"					}\n"
			"					float distance = length(d);\n"
			"testF = distance;\n"
			"					attenuation /= u_lights[index].constant +\n"
			"						u_lights[index].linear * distance +\n"
			"						u_lights[index].quadratic * distance * distance;\n"
			"				}\n"
			"        float fNDotL= clamp((dot( vNormal, lightDirection )),0.0,1.0);\n"
			"        vec3 diffuseReflection = attenuation\n"
			"            * vec3(u_lights[index].diffuse) \n"
			"            * fNDotL;\n"
			"	totalDiffuse += diffuseReflection;\n"
			"		  float intensity = u_lights[0].intensity.x;\n"
			"        vec3 halfVector = normalize(lightDirection- eyeDirection);\n"
			"        float fNDotH = max(0.00001, (dot(vNormal,halfVector)));\n"
			"        vec3 specularReflection = attenuation * vec3(u_lights[index].specular) \n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"         totalSpecular = totalSpecular + specularReflection;\n"
			"        }\n"
			"    }\n"
			"vec3 scenecolor = vec3(0.2, 0.2, 0.2);\n"
			"         totalAmbient =vec3(totalAmbient) + (vec3(u_lights[0].ambient*0.00001)+scenecolor) * vec3(u_materials.diffuse);\n"
			"    diffuseColor = min(totalLighting+totalAmbient+totalDiffuse* vec3(u_materials.diffuse),vec3(1.0));\n"
			"    specularColor= min(totalSpecular* vec3(u_materials.specular),vec3(1.0));\n"
			"    float alpha = u_diffuse.a;"
			"if(u_enbleLights[1] == 1) testF = 1.0;\n else\n  testF = 0.0;\n"
			"    gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)/**0.0000001+vec3(testF,0.0,0.0)*/+vec3(specularColor),alpha);\n"
			" }\n";
	}

	const char *ShaderSrcCode::MultilightPerFragFragMediump()
	{
		return

			"precision mediump  float;\n"
			"precision mediump  int;\n"
			"precision mediump sampler2D;\n"
			"precision mediump samplerCube;\n"
			"struct Light\n"
			"{\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    vec4 position;\n"
			"    vec3 intensity;\n"
			"    vec3 spotDirection;\n"
			"    float spotExponent;\n"
			"    float spotCutoff; // (range: [0.0,90.0], 180.0)\n"
			"    float spotCosCutoff; // (range: [1.0,0.0],-1.0)\n"
			"    float constant;\n"
			"    float linear;\n"
			"    float quadratic;\n"
			"};\n"
			"\n"
			"struct LightModel\n"
			"{\n"
			"    vec4 ambient;    // Acs\n"
			"};\n"
			"\n"
			"struct Materials\n"
			"{\n"
			"    vec4 emission;\n"
			"    vec4 ambient;\n"
			"    vec4 diffuse;\n"
			"    vec4 specular;\n"
			"    float shininess;\n"
			"};\n"
			"uniform LightModel u_lightModel ;\n"
			"const int numberOfLights = 1;\n"
			"uniform Light u_lights[2];\n"
			"uniform int u_enbleLights[1];\n"
			"uniform Materials u_materials;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"uniform bool u_useTex;\n"
			"uniform bool u_useAmbientTex;\n"
			"uniform bool u_useCubeMapTex;\n"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform samplerCube u_samplerCube0;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeTexCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"    vec3 testColor= vec3(0.7,0.0,0.0);"
			"    vec3 totalLighting = vec3(u_materials.emission)+ vec3(u_lightModel.ambient) * vec3(u_materials.ambient);\n"
			" vec3 diffuseColor;\n"
			" vec3 specularColor;\n"
			" vec3  vNormal ;\n"
			"if(gl_FrontFacing){\n"
			" vNormal         = normalize( v_normal );\n"
			"}\n"
			"else {\n"
			" vNormal         = normalize( -v_normal );\n"
			"}\n"
			"if (u_reverseClip) {\n"
			"int iEnableNum = 0; \n"
			"int iClipNum = 0; \n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"iEnableNum++; \n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"iClipNum++; \n"
			"}\n"
			"}\n"
			"}\n"
			"if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"else {\n"
			"for (int i = 0; i < 3; i++) {\n"
			"if (u_enableClips[i]) {\n"
			"float dPara = u_clipPlanes[i].w; \n"
			"if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"discard; \n"
			"}\n"
			"}\n"
			"}\n"
			"}\n"

			"    if(u_useTex)\n"
			"    {\n"
			"        switchColor = texture2D( u_sampler0, v_texCoords.xy );\n"
			"    }\n"
			"    if(u_useAmbientTex)\n"
			"    {\n"
			"        vec2 sphereCoords = sphereMap(v_normal,v_position);\n"
			"        environment = texture2D(u_sampler1,sphereCoords.xy);\n"
			"    }\n"
			"    if(u_useCubeMapTex)\n"
			"    {\n"
			"        environment = textureCube(u_samplerCube0,v_cubeTexCoords.xyz);\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment;\n"
			"    for(int index = 0;index<numberOfLights;index++)\n"
			"    {\n"
			"        if(u_enbleLights[index] == 1)\n"
			"        {\n"

			"            if (0.0 == u_lights[index].position.w) // directional light?\n"
			"            {\n"

			"                attenuation = 1.0; // no attenuation\n"
			"                lightDirection = normalize(vec3(u_lights[index].position)/*- fvObjectPosition.xyz*/);\n"
			"            }\n"
			"        float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.2,1.0);\n"
			"        vec3 diffuseReflection = attenuation\n"
			"            * vec3(u_lights[index].diffuse) * vec3(u_materials.diffuse)\n"
			"            * fNDotL;\n"
			"         totalAmbient =vec3(totalAmbient) + vec3(u_lights[index].ambient) * vec3(u_materials.ambient);\n"
			"		  float intensity = u_lights[index].intensity.x;\n"
			"         totalDiffuse = totalDiffuse + diffuseReflection*intensity;\n"
			"        vec3 eyeDirection =vec3(0,0,1); /*normalize(- fvObjectPosition.xyz );*/\n"
			"        vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"        float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"        vec3 specularReflection = attenuation * vec3(u_lights[index].specular) * vec3(u_materials.specular)\n"
			"               * pow(fNDotH, u_materials.shininess);\n"
			"         totalSpecular = totalSpecular + specularReflection*intensity;\n"
			"        }\n"
			"    }\n"
			"    diffuseColor = min(totalLighting+totalAmbient+totalDiffuse,vec3(1.0));\n"
			"    specularColor= min(totalSpecular,vec3(1.0));\n"
			"    gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);\n"
			" }\n";
	}

	const char *ShaderSrcCode::CapPlaneVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 fvObjectPosition;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			" }\n";
	}

	const char *ShaderSrcCode::CapPlaneFrag()
	{
		return
			"precision highp  float;"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"

			"uniform mat4 u_projectionMat;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"varying vec4 fvObjectPosition;\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	vec3 diffuseColor;\n"
			"	vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal         = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal         = normalize( -v_normal );\n"
			"	}\n"

			"	if (u_reverseClip) {\n"
			"		int iEnableNum = 0; \n"
			"		int iClipNum = 0; \n"
			"	for (int i = 0; i < 3; i++) {\n"
			"		if (u_enableClips[i]) {\n"
			"			iEnableNum++; \n"
			"			float dPara = u_clipPlanes[i].w; \n"
			"			if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0) {\n"
			"				iClipNum++; \n"
			"			}\n"
			"		}\n"
			"	}\n"
			"	if (iEnableNum == iClipNum && iEnableNum != 0) {\n"
			"		discard; \n"
			"	}\n"
			"}\n"
			"else {\n"
			"	for (int i = 0; i < 3; i++) {\n"
			"		if (u_enableClips[i]) {\n"
			"			float dPara = u_clipPlanes[i].w; \n"
			"				if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0) {\n"
			"					discard; \n"
			"				}\n"
			"			}\n"
			"		}\n"
			"}\n"

			"	vec3 eyeDirection = (u_projectionMat[3][3] == 0.0)?normalize(fvObjectPosition.xyz):vec3(0.0, 0.0, -1.0);\n"
			"   attenuation = 1.0; // no attenuation\n"
			"   lightDirection = normalize(vec3(0.0,0.0,1.0));\n"
			"   float fNDotL= clamp((dot( vNormal, lightDirection )),0.2,1.0);\n"
			"   vec3 diffuseReflection = attenuation\n"
			"            * fNDotL*vec3(1.0,1.0,1.0);\n"
			"	totalDiffuse += diffuseReflection;\n"
			" //  vec3 halfVector = normalize(lightDirection- eyeDirection);\n"
			" //  float fNDotH = max(0.00001, (dot(vNormal,halfVector)));\n"
			"//   vec3 specularReflection = attenuation  \n"
			"  //             * pow(fNDotH, 60.0);\n"
			" //  totalSpecular = totalSpecular + specularReflection;\n"
			"	//vec3 scenecolor = vec3(0.2, 0.2, 0.2);\n"
			"    diffuseColor = min(totalDiffuse* vec3(u_diffuse),vec3(1.0));\n"
			"   // specularColor= min(totalSpecular* vec3(1.0,1.0,1.0),vec3(1.0));\n"
			"    float alpha = u_diffuse.a;"
			"    gl_FragColor = vec4(diffuseColor/*+vec3(specularColor)*/,alpha);\n"
			" }\n";
	}
	//zhubao 
	const char * ShaderSrcCode::JewelFrontVert()
	{
		return 	"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"//attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"//uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform mat4 u_worldNormalMat;\n"
			"varying vec3 v_position;\n"
			"//varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    //vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz;\n"
			"   // v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    eyeWorldDirection =normalize( u_eyePosition -worldPosT);\n "
			"    lightWorldDirection =normalize( u_lightPosition -worldPosT);\n "
			" vec3 worldNormal = vec3(u_worldNormalMat*vec4(a_normal,0.0));\n"
			"	v_worldNormal = worldNormal;\n"
			"    v_cubeRefractCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(worldNormal));\n"
			" }\n";
	}
	const char * ShaderSrcCode::JewelFrontFrag()
	{
		return
			"precision highp  float;"
			"uniform bool u_useFrontTexture;\n"
			"uniform bool u_useHighlightTexture;\n"
			"uniform bool u_useFrontCubeTexture;\n"
			"uniform sampler2D frontTexture;\n"
			"uniform sampler2D highlightTexture;\n"
			"uniform samplerCube frontCubeTexture;\n"
			"uniform bool u_keepDirection;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"//varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment1 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment2 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 highLightColor = vec4(0.0,0.0,0.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal = normalize( -v_normal );\n"
			"	}\n"
			"   if(u_useFrontTexture == true)\n"
			"    {\n"
			"       vec2 sphereCoords = vec2(1.0,1.0);\n"
			"       vec2 matCapCoords = vec2(1.0,1.0);\n"
			"       if(u_keepDirection == true){"
			"         sphereCoords = sphereMap(normalize(v_worldNormal),v_position);\n"
			"		 matCapCoords = v_worldNormal.xy;}\n"
			"          else{"
			"         sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			" matCapCoords = v_normal.xy;}\n"
			"         matCapCoords = matCapCoords*0.5+0.5;"
			"        environment1 =texture2D(frontTexture,matCapCoords.xy);\n"
			"    }\n"
			"	if(u_useHighlightTexture == true)"
			"   {"
			"        vec2 sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			"        highLightColor =texture2D(highlightTexture,sphereCoords.xy);\n"
			"    }"
			"    if(u_useFrontCubeTexture == true)\n"
			"    {\n"
			"        environment2 = textureCube(frontCubeTexture,v_cubeRefractCoords.xyz);\n"
			"		 environment2.rbg = pow(environment2.rbg,vec3(1.0/1.7));\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment1*environment2;\n"
			"    lightDirection = lightWorldDirection;\n"
			"    float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"    vec3 diffuseReflection = \n"
			"              vec3(1.0,1.0,1.0)\n"
			"            * fNDotL;\n"
			"    totalDiffuse =  diffuseReflection;\n"
			"    vec3 eyeDirection =eyeWorldDirection;\n"
			"    vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"    float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"    vec3 specularReflection =  vec3(1.0)\n"
			"               * pow(fNDotH,10.0);\n"
			"vec3 lightDirection1 = normalize(vec3(vec4(1.0,0.0,0.0,0.0)) - v_position);\n"
			"vec3 lightDirection2 = normalize(vec3(vec4(0.0,-1.0,0.0,0.0))-v_position);\n"
			"vec3 lightDirection3 = normalize(vec3(vec4(0.0,0.0,1.0,0.0)) - v_position);\n"
			"    vec3 eyeDirection1 =vec3(0.0,0.0,0.0)-v_position;\n"
			"    vec3 halfVector1 = normalize(lightDirection1+ eyeDirection1);\n"
			"    float fNDotH1 = max(0.0, (dot(vNormal,halfVector1)));\n"
			"    vec3 specularReflection1 =  vec3(1.0)\n"
			"               * pow(fNDotH1,50.0);\n"
			"    vec3 eyeDirection2 =eyeDirection1;\n"
			"    vec3 halfVector2 = normalize(lightDirection2+ eyeDirection2);\n"
			"    float fNDotH2 = max(0.0, (dot(vNormal,halfVector2)));\n"
			"    vec3 specularReflection2 =  vec3(1.0)\n"
			"               * pow(fNDotH2,50.0);\n"
			"    vec3 eyeDirection3 =eyeDirection1;\n"
			"    vec3 halfVector3 = normalize(lightDirection3+ eyeDirection3);\n"
			"    float fNDotH3 = max(0.0, (dot(vNormal,halfVector3)));\n"
			"    vec3 specularReflection3 =  vec3(1.0)\n"
			"               * pow(fNDotH3,50.0);\n"
			"    totalSpecular = totalSpecular + (specularReflection+specularReflection1+specularReflection2+specularReflection3);\n"
			"    diffuseColor = min(totalDiffuse,vec3(1.0));\n"
			"    specularColor= min(totalSpecular,vec3(1.0));\n"
			"	if(u_useFrontTexture||u_useFrontCubeTexture)"
			"	 {"
			"       if(u_useHighlightTexture == true){"
			"		gl_FragColor = vec4(vec3(texColor)*u_diffuse.xyz*vec3(u_selectColor)+highLightColor.rgb,u_diffuse.a);\n"
			"       }"
			"   else{"
			"		gl_FragColor = vec4(vec3(texColor)*u_diffuse.xyz*vec3(u_selectColor),u_diffuse.a);"
			"       }\n"
			"		 gl_FragColor.rbg = pow(gl_FragColor.rbg,vec3(1.0/1.2));\n"
			"	}\n"
			"    else {gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);}\n"
			"	}\n";
	}

	const char * ShaderSrcCode::JewelBackVert()
	{
		return 	"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"//attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"//uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform mat4 u_worldNormalMat;\n"
			"varying vec3 v_position;\n"
			"//varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"   // vec4 tempCoords = u_textureMat*vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz;\n"
			"   // v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    eyeWorldDirection =normalize( u_eyePosition -worldPosT);\n "
			"    lightWorldDirection =normalize( u_lightPosition -worldPosT);\n "
			" vec3 worldNormal = vec3(u_worldNormalMat*vec4(a_normal,0.0));\n"
			"	v_worldNormal = worldNormal;\n"
			"    v_cubeRefractCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(worldNormal));\n"
			" }\n";
	}
	const char * ShaderSrcCode::JewelBackFrag()
	{
		return
			"precision highp  float;"
			"uniform bool u_useBackTexture;\n"
			"uniform bool u_useHighlightTexture;\n"
			"uniform bool u_useBackCubeTexture;\n"
			"uniform sampler2D backTexture;\n"
			"uniform sampler2D highlightTexture;\n"
			"uniform samplerCube backCubeTexture;\n"
			"uniform bool u_keepDirection;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"//varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			"varying vec3 v_cubeRefractCoords;\n"
			"varying vec4 fvObjectPosition;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment1 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment2 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 highLightColor = vec4(0.0,0.0,0.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal = normalize( -v_normal );\n"
			"	}\n"
			"   if(u_useBackTexture == true)\n"
			"    {\n"
			"       vec2 sphereCoords = vec2(1.0,1.0);\n"
			"       vec2 matCapCoords = vec2(1.0,1.0);\n"
			"       if(u_keepDirection == true){"
			"         sphereCoords = sphereMap(normalize(v_worldNormal),v_position);\n"
			"		 matCapCoords = v_worldNormal.xy;}\n"
			"          else{"
			"         sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			" matCapCoords = v_normal.xy;}\n"
			"         matCapCoords = matCapCoords*0.5+0.5;"
			"        environment1 =texture2D(backTexture,matCapCoords.xy);\n"
			"    }\n"
			"	if(u_useHighlightTexture == true)"
			"   {"
			"        vec2 sphereCoords = sphereMap(normalize(v_normal),v_position);\n"
			"        highLightColor =texture2D(highlightTexture,sphereCoords.xy);\n"
			"    }"
			"    if(u_useBackCubeTexture == true)\n"
			"    {\n"
			"        environment2 = textureCube(backCubeTexture,v_cubeRefractCoords.xyz);\n"
			"		 environment2.rbg = pow(environment2.rbg,vec3(1.0/1.7));\n"
			"    }\n"
			"    vec4 texColor = switchColor *environment1*environment2;\n"
			"    lightDirection = lightWorldDirection;\n"
			"    float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"    vec3 diffuseReflection = \n"
			"              vec3(1.0,1.0,1.0)\n"
			"            * fNDotL;\n"
			"    totalDiffuse =  diffuseReflection;\n"
			"    vec3 eyeDirection =eyeWorldDirection;\n"
			"    vec3 halfVector = normalize(lightDirection+ eyeDirection);\n"
			"    float fNDotH = max(0.0, abs(dot(vNormal,halfVector)));\n"
			"    vec3 specularReflection =  vec3(1.0)\n"
			"               * pow(fNDotH,10.0);\n"
			"vec3 lightDirection1 = normalize(vec3(vec4(1.0,0.0,0.0,0.0)) - v_position);\n"
			"vec3 lightDirection2 = normalize(vec3(vec4(0.0,-1.0,0.0,0.0))-v_position);\n"
			"vec3 lightDirection3 = normalize(vec3(vec4(0.0,0.0,1.0,0.0)) - v_position);\n"
			"    vec3 eyeDirection1 =vec3(0.0,0.0,0.0)-v_position;\n"
			"    vec3 halfVector1 = normalize(lightDirection1+ eyeDirection1);\n"
			"    float fNDotH1 = max(0.0, (dot(vNormal,halfVector1)));\n"
			"    vec3 specularReflection1 =  vec3(1.0)\n"
			"               * pow(fNDotH1,50.0);\n"
			"    vec3 eyeDirection2 =eyeDirection1;\n"
			"    vec3 halfVector2 = normalize(lightDirection2+ eyeDirection2);\n"
			"    float fNDotH2 = max(0.0, (dot(vNormal,halfVector2)));\n"
			"    vec3 specularReflection2 =  vec3(1.0)\n"
			"               * pow(fNDotH2,50.0);\n"
			"    vec3 eyeDirection3 =eyeDirection1;\n"
			"    vec3 halfVector3 = normalize(lightDirection3+ eyeDirection3);\n"
			"    float fNDotH3 = max(0.0, (dot(vNormal,halfVector3)));\n"
			"    vec3 specularReflection3 =  vec3(1.0)\n"
			"               * pow(fNDotH3,50.0);\n"
			"    totalSpecular = totalSpecular + (specularReflection+specularReflection1+specularReflection2+specularReflection3);\n"
			"    diffuseColor = min(totalDiffuse,vec3(1.0));\n"
			"    specularColor= min(totalSpecular,vec3(1.0));\n"
			"	if(u_useBackTexture||u_useBackCubeTexture)"
			"	 {"
			"       if(u_useHighlightTexture == true){"
			"		gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor)+highLightColor.rgb,u_diffuse.a);\n"
			"       }"
			"   else{"
			"		gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor),u_diffuse.a);"
			"       }\n"
			"		 gl_FragColor.rbg = pow(gl_FragColor.rbg,vec3(1.0/1.2));\n"
			"	}\n"
			"    else {gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);}\n"
			"	}\n";
	}

	const char * ShaderSrcCode::RingVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"attribute vec3 a_texCoords;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"uniform mat4 u_textureMat;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec3 u_lightPosition;\n"
			"uniform mat4 u_worldNormalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			//"varying vec3 v_cubeRefractCoords;\n"
			"varying vec3 v_worldPos;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 tempCoords = vec4(a_texCoords.xyz,1.0);\n"
			"    v_position = fvObjectPosition.xyz;\n"
			"    v_texCoords = tempCoords;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			"    vec4 worldPos = u_modelMat * vec4(a_position,1.0);\n"
			"    vec3 worldPosT = worldPos.xyz;\n"
			"    v_worldPos = worldPosT;\n"
			"    eyeWorldDirection =normalize( u_eyePosition -worldPosT);\n "
			"    lightWorldDirection =normalize( u_lightPosition -worldPosT);\n "
			" vec3 worldNormal = vec3(u_worldNormalMat*vec4(a_normal,0.0));\n"
			"	v_worldNormal = worldNormal;\n"
			//"    v_cubeRefractCoords =reflect(normalize(worldPosT -u_eyePosition), normalize(worldNormal));\n"
			" }\n";
	}

	const char * ShaderSrcCode::RingFrag()
	{
		return
            "#extension GL_OES_standard_derivatives : enable\n"
			"precision highp  float;\n"
			"uniform bool u_useFrontTexture;\n"
			"uniform bool u_useFrontCubeTexture;\n"
			"uniform sampler2D frontTexture;\n"
			"uniform samplerCube frontCubeTexture;\n"
			"uniform bool u_useHighlightTexture;\n"
			"uniform sampler2D highlightTexture;\n"
			"uniform sampler2D normalMap;\n"
			"uniform bool u_useBumpMap;\n"
			"uniform vec4 u_diffuse;\n"
			"uniform vec3 u_eyePosition;\n"
			"uniform vec4 u_selectColor;\n"
			"varying vec3 v_position;\n"
			"varying vec4 v_texCoords;\n"
			"varying vec3 v_normal;\n"
			//"varying vec3 v_cubeRefractCoords;\n"
			"varying vec3 v_worldPos;\n"
			"varying vec3 eyeWorldDirection;\n"
			"varying vec3 lightWorldDirection;\n"
			"varying vec3 v_worldNormal;\n"
			"vec4 switchColor = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment1 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 environment2 = vec4(1.0,1.0,1.0,1.0);\n"
			"vec4 highLightColor = vec4(0.0,0.0,0.0,1.0);\n"
			"vec2 sphereMap(vec3 normal, vec3 ecPosition3)\n"
			"{\n"
			"   float m;\n"
			"   vec3 r;\n"
			"   vec3 u;\n"
			"   u = normalize(ecPosition3);\n"
			"   r = reflect(u, normal);\n"
			"   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0))+0.0001;\n"
			"   return vec2 (r.x / m + 0.5, r.y / m + 0.5);\n"
			"}\n"
			"vec2 dHdxy_fwd(in vec2 Texcoord ,in sampler2D bumpMap) {\n"
			"		vec2 dSTdx = dFdx( Texcoord );"
			"		vec2 dSTdy = dFdy( Texcoord );"
			"		float bumpScale = 0.45;\n"
			"		float Hll = bumpScale * texture2D( bumpMap, Texcoord ).x;"
			"		float dBx = bumpScale * texture2D( bumpMap, Texcoord + dSTdx ).x - Hll;"
			"		float dBy = bumpScale * texture2D( bumpMap, Texcoord + dSTdy ).x - Hll;"
			"		return vec2( dBx, dBy );"
			"		}\n"
			"vec3 perturbNormalArb( vec3 surf_pos, vec3 surf_norm, vec2 dHdxy ) {"
			"		vec3 vSigmaX = dFdx( surf_pos );"
			"		vec3 vSigmaY = dFdy( surf_pos );"
			"		vec3 vN = surf_norm;"
			"		vec3 R1 = cross( vSigmaY, vN );"
			"		vec3 R2 = cross( vN, vSigmaX );"
			"		float fDet = dot( vSigmaX, R1 );"
			"		fDet *= ( float( gl_FrontFacing ) * 2.0 - 1.0 );"
			"		vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );"
			"		return normalize( abs( fDet ) * surf_norm - vGrad );"
			"		}	"
			"\n"
			"vec3 perturbNormal2Arb(vec3 eye_pos, vec3 surf_norm,in vec2 Texcoord,in sampler2D normalMap) {\n"

			"vec3 q0 = dFdx(eye_pos.xyz);\n"
			"vec3 q1 = dFdy(eye_pos.xyz);\n"
			"vec2 st0 = dFdx(Texcoord.st);\n"
			"vec2 st1 = dFdy(Texcoord.st);\n"

			"vec3 S = normalize(q0 * st1.t - q1 * st0.t);\n"
			"vec3 T = normalize(-q0 * st1.s + q1 * st0.s);\n"
			"vec3 N = normalize(surf_norm);\n"

			"vec3 mapN = texture2D(normalMap, Texcoord).xyz * 2.0 - 1.0;\n"
			"mapN.xy =/* normalScale*/ vec2(1.0)* mapN.xy;\n"
			"mat3 tsn = mat3(S, T, N);\n"
			"return normalize(tsn * mapN);\n"

			"}\n"
			"void main(void)\n"
			"{\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"

			"   if(u_useBumpMap == true && u_useFrontTexture == true)\n"
			"   {\n"
			"		vec3 bumpNormal = vec3(0.0,0.0,0.0);\n"
			//"		bumpNormal = normalize(  perturbNormalArb( v_position, normalize( v_normal ), dHdxy_fwd(v_texCoords.xy*4.0,normalMap) ) );\n"
			"		bumpNormal = normalize(  perturbNormal2Arb( v_position, normalize( v_normal ), v_texCoords.xy*6.0,normalMap ) );\n"
			"		vNormal = bumpNormal;\n"
			"   }\n"
			"   else if(u_useBumpMap == true && u_useFrontCubeTexture == true)\n"
			"   {\n"
			"		vec3 bumpNormal = vec3(0.0,0.0,0.0);\n"
			//"		bumpNormal = normalize(  perturbNormalArb( v_position, normalize( v_normal ), dHdxy_fwd(v_texCoords.xy*4.0,normalMap) ) );\n"
			"		bumpNormal = normalize(  perturbNormal2Arb( v_worldPos, normalize( v_worldNormal ), v_texCoords.xy*6.0,normalMap ) );\n"
			"		vNormal = bumpNormal;\n"
			"   }\n"
			"   else if(u_useFrontTexture == true) \n"
			"   {\n"
			"		if(gl_FrontFacing){\n"
			"			vNormal = normalize( v_normal );\n"
			"		}\n"
			"		else {\n"
			"			vNormal = normalize( -v_normal );\n"
			"		}\n"
			"   }\n"
			"   else if(u_useFrontCubeTexture == true) \n"
			"   {\n"
			"		if(gl_FrontFacing){\n"
			"			vNormal = normalize( v_worldNormal );\n"
			"		}\n"
			"		else {\n"
			"			vNormal = normalize( -v_worldNormal );\n"
			"		}\n"
			"   }\n"
			"   if(u_useFrontTexture == true)\n"
			"    {\n"
			"		vec2 matCapCoords = vec2(1.0, 1.0); \n"
			"		matCapCoords = vNormal.xy;\n"
			"       matCapCoords = matCapCoords*0.5+0.5;"
			"       environment1 =texture2D(frontTexture,matCapCoords.xy);\n"
			"    }\n"
			"    if(u_useFrontCubeTexture == true)\n"
			"    {\n"
			"		 vec3 v_cubeRefractCoords =reflect(normalize(v_worldPos -u_eyePosition), normalize(vNormal));\n"
			"        environment2 = textureCube(frontCubeTexture,v_cubeRefractCoords.xyz);\n"
			"		 environment2.rbg = pow(environment2.rbg,vec3(1.0/1.7));\n"
			"    }\n"
			"	if(u_useHighlightTexture == true)"
			"   {"
			"		vec2 matCapCoords = vec2(1.0, 1.0); \n"
			"		matCapCoords = vNormal.xy;\n"
			"       matCapCoords = matCapCoords*0.5+0.5;"
			"        highLightColor =texture2D(highlightTexture,matCapCoords.xy);\n"
			"    }"
			"    vec4 texColor = switchColor *environment1*environment2;\n"


			"    lightDirection = lightWorldDirection;\n"
			"    float fNDotL= clamp(abs(dot( vNormal, lightDirection )),0.3,1.0);\n"
			"    vec3 diffuseReflection = \n"
			"              vec3(1.0,1.0,1.0)\n"
			"            * fNDotL;\n"
			"    totalDiffuse =  diffuseReflection;\n"
			"    diffuseColor = min(totalDiffuse,vec3(1.0));\n"

			"	if(u_useFrontCubeTexture||u_useFrontTexture)"
			"	 {"
			"       if(u_useHighlightTexture == true){"
			"			gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor)/**vec3(totalDiffuse)*/+highLightColor.rgb,u_diffuse.a);"
			"		}\n"
			"		else{\n"
			"		gl_FragColor = vec4(vec3(texColor)*vec3(u_diffuse)*vec3(u_selectColor)/**vec3(totalDiffuse)*/,u_diffuse.a);"
			"		}\n"
			"		 gl_FragColor.rbg = pow(gl_FragColor.rbg,vec3(1.0/1.2));\n"
			"	}\n"
			"    else {gl_FragColor = vec4(vec3(texColor)*diffuseColor*vec3(u_selectColor)+vec3(specularColor),u_diffuse.a);}\n"
			"	}\n";
	}

	const char * ShaderSrcCode::JewelTypeVert()
	{
		return
			"attribute  vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"     gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelTypeFrag()
	{
		return
			"precision highp  float;"
			"uniform float mixFactor;\n"
			"void main(void)\n"
			"{\n"
			"    gl_FragColor = vec4(0.0,0.0,0.0,mixFactor);\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelHighLightVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute vec3 a_normal;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"uniform mat4 u_normalMat;\n"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"    v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"    v_normal= normalize(vec3((u_normalMat * vec4(a_normal,0.0))));\n"
			" }\n"
			;
	}

	const char * ShaderSrcCode::JewelHighLightFrag()
	{
		return
			"precision highp  float;"
			"varying vec3 v_position;\n"
			"varying vec3 v_normal;\n"
			"uniform vec3 u_specular;\n"
			"vec3 lightPoss[4];\n"
			"vec3 lightSpecular[4];\n"
			"void main() { \n"
			"lightPoss[0] = normalize(vec3(-1.0,0.0,1.0));\n "
			"lightPoss[1] = normalize(vec3(0.0,1.0,0.0));\n "
			"lightPoss[2] = normalize(vec3(1.0,0.0,1.0));\n "
			"lightPoss[3] = normalize(vec3(0.0,-1.0,0.0));\n "
			"lightSpecular[0] = vec3(0.9,0.9,0.9);\n"
			"lightSpecular[1] = vec3(0.9,0.9,0.9);\n"
			"lightSpecular[2] = vec3(0.9,0.9,0.9);\n"
			"lightSpecular[3] = vec3(0.9,0.9,0.9);\n"
			"    vec3 totalAmbient= vec3(0.0,0.0,0.0);\n"
			"    vec3 totalDiffuse = vec3(0.0,0.0,0.0);\n"
			"    vec3 totalSpecular = vec3(0.0,0.0,0.0);\n"
			"    vec3 lightDirection;\n"
			"    float attenuation=1.0;\n"
			"	 vec3 diffuseColor;\n"
			"	 vec3 specularColor;\n"
			"	 vec3  vNormal ;\n"
			"	if(gl_FrontFacing){\n"
			"		vNormal = normalize( v_normal );\n"
			"	}\n"
			"	else {\n"
			"		vNormal = normalize( -v_normal );\n"
			"	}\n"
			"  vec3 viewDir = normalize(v_position);\n"
			"   for(int i =0 ;i<4;i++)\n"
			"  {\n"
			"   vec3 H = normalize(lightPoss[i]-viewDir);\n"
			"   float specular_bsdf = pow(max(dot(vNormal, H), 0.0), 30.0);"
			"   totalSpecular += lightSpecular[i] * specular_bsdf;"
			"  }"
			"  vec3 fragcolor = vec3(0.0);\n"
			"   fragcolor= totalSpecular*u_specular.rgb;\n"
			"  gl_FragColor = vec4(fragcolor,1.0);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::JewelBlendQuadVert()
	{
		return

			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelBlendQuadFrag()
	{
		return
			"precision highp  float;"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform sampler2D u_sampler3;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"  vec4 frontColor = texture2D(u_sampler0,v_texCoords);\n"
			"  vec4 backColor = texture2D(u_sampler1,v_texCoords);\n"
			"  vec4 jewelType = texture2D(u_sampler2,v_texCoords);\n"
			"  vec4 highlight = texture2D(u_sampler3,v_texCoords);\n"
			"  vec4 FragColor = vec4(0.0);"
			"  FragColor = vec4( mix(frontColor.rgb,backColor.rgb,jewelType.a)+highlight.rgb*0.40,frontColor.a);\n "
			"  gl_FragColor = FragColor;\n"
			"}\n";
	}

	const char * ShaderSrcCode::JewelFinalQuadVert()
	{
		return
			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			"\n";
	}

	const char * ShaderSrcCode::JewelFinalQuadFrag()
	{
		return
			"precision highp  float;"
			"uniform sampler2D u_sampler0;\n"
			"uniform sampler2D u_sampler1;\n"
			"uniform sampler2D u_sampler2;\n"
			"uniform sampler2D u_sampler3;\n"
			"//uniform sampler2D u_sampler4;\n"
			"//uniform sampler2D u_sampler5;\n"
			"//uniform sampler2D u_sampler6;\n"
			"varying vec2 v_texCoords;\n"
			"void main() { \n"
			"  vec4 blendColor = texture2D(u_sampler0,v_texCoords);\n"
			"  vec4 jewelDepth = texture2D(u_sampler1,v_texCoords);\n"
			"  vec4 ringArmColor = texture2D(u_sampler2,v_texCoords);\n"
			"  vec4 ringArmDepth = texture2D(u_sampler3,v_texCoords);\n"
			"  //vec4 jewelStarColor= texture2D(u_sampler4,v_texCoords);\n"
			" // vec4 jewelStarDepth= texture2D(u_sampler5,v_texCoords);\n"
			" // vec4 jewelNoteColor = texture2D(u_sampler6,v_texCoords);\n"
			"   vec4 FragColor = vec4(0.0);"
			"	if(jewelDepth.r<ringArmDepth.r)\n"
			"	{\n"
			"      FragColor = vec4(blendColor.rgba);\n "
			"   }\n"
			"   else\n"
			"   {\n"
			"      FragColor = vec4(ringArmColor.rgba);\n"
			"   }\n"
			"    gl_FragColor = vec4(FragColor.rgba);\n"
			"}\n";
	}

	const char * ShaderSrcCode::OutlineVert()
	{
		return
			"attribute vec3 a_position;\n"
			"uniform mat4 u_modelMat;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"varying vec3 v_position;\n"
			"void main() {\n "
			"   gl_Position = u_projectionMat * u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"   vec4 fvObjectPosition = u_viewMat*u_modelMat* vec4(a_position,1.0);\n"
			"   v_position = fvObjectPosition.xyz/fvObjectPosition.w;\n"
			"}\n";
	}

	const char * ShaderSrcCode::OutlineFrag()
	{
		return
			"precision highp  float;"
			"uniform vec4 u_diffuse;\n"
			"varying vec3 v_position;\n"
			"uniform vec4 u_clipPlanes[3];\n"
			"uniform bool u_enableClips[3];\n"
			"uniform bool u_reverseClip;\n"
			"void main() { \n"
			"if (u_reverseClip)\n"
			"{"
			"	int iEnableNum = 0;\n"
			"	int iClipNum = 0;\n"
			"	for (int i = 0; i < 3; i++)\n"
			"	{\n"
			"		if (u_enableClips[i])\n"
			"		{\n"
			"			iEnableNum++;\n"
			"			float dPara = u_clipPlanes[i].w;\n"
			"			if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) > 0.0)\n"
			"			{\n"
			"				iClipNum++;\n"
			"			}\n"
			"		}\n"
			"	}\n"
			"	if (iEnableNum == iClipNum && iEnableNum != 0)\n"
			"	{\n"
			"		//if (u_enableClip[2] == 1) {\n"
			"		discard;\n"
			"	}\n"
			"}\n"
			"else\n"
			"{\n"
			"	for (int i = 0; i < 3; i++)\n"
			"	{\n"
			"		if (u_enableClips[i])\n"
			"		{\n"
			"			float dPara = u_clipPlanes[i].w;\n"
			"			if ((dot(vec3(v_position.xyz), vec3(u_clipPlanes[i].xyz)) + dPara) < 0.0)\n"
			"			{\n"
			"				discard;\n"
			"			}\n"
			"		}\n"
			"	}\n"
			"}\n"
			"   gl_FragColor =u_diffuse;\n"
			"}\n";;
	}

	const char* ShaderSrcCode::GaussianBlurVert()
	{
		return
			"attribute  vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(a_position,1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}";
	}

	const char* ShaderSrcCode::GaussianBlurFrag()
	{
		return

			"\n"
			"precision highp  float;"
			"varying vec2 v_texCoords;\n"
			"\n"
			"uniform sampler2D image;\n"
			"\n"
			"uniform bool horizontal;\n"
			"\n"
			"uniform float tex_offset;\n"
			"\n"
			"void main()\n"
			"{\n"
			"float weight[5];\n"
			"weight[0] = 0.227027;\n"
			"weight[1] = 0.1945946;\n"
			"weight[2] = 0.1216216;\n"
			"weight[3] = 0.054054;\n"
			"weight[4] = 0.016216;\n"
			"   \n"
			"    vec4 result = texture2D(image, v_texCoords) * weight[0]; // current fragment's contribution\n"
			"    if (horizontal)\n"
			"    {\n"
			"\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 1.0, 0.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords - vec2(tex_offset * 1.0, 0.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 2.0, 0.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords - vec2(tex_offset * 2.0, 0.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 3.0, 0.0)) * weight[3];\n"
			"     result += texture2D(image, v_texCoords - vec2(tex_offset * 3.0, 0.0)) * weight[3];\n"
			"      result += texture2D(image, v_texCoords + vec2(tex_offset * 4.0, 0.0)) * weight[4];\n"
			"      result += texture2D(image, v_texCoords - vec2(tex_offset * 4.0, 0.0)) * weight[4];\n"
			"\n"
			"    }\n"
			"    else\n"
			"    {\n"
			"\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 1.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 1.0)) * weight[1];\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 2.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 2.0)) * weight[2];\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 3.0)) * weight[3];\n"
			"     result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 3.0)) * weight[3];\n"
			"      result += texture2D(image, v_texCoords + vec2(0.0, tex_offset * 4.0)) * weight[4];\n"
			"     result += texture2D(image, v_texCoords - vec2(0.0, tex_offset * 4.0)) * weight[4];\n"
			"        \n"
			"    }\n"
			"    gl_FragColor = vec4(result);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::GaussianBlurOutlineVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords;\n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}";
	}

	const char * ShaderSrcCode::GaussianBlurOutlineFrag()
	{
		return
			"precision highp  float;"
			"uniform sampler2D height;\n"
			"uniform sampler2D model;\n"
			"varying  vec2 v_texCoords;\n"
			"void main()\n"
			"{\n"
			"   vec4 gaussianColor = texture2D(height, v_texCoords);\n"
			"   vec4 modelColor = texture2D(model, v_texCoords);\n"
			"  vec4 final = (gaussianColor - modelColor);\n"
			"  if(final.r<0.0) final.r = 0.0;\n"
			"  if(final.g<0.0) final.g = 0.0;\n"
			"  if(final.b<0.0) final.b = 0.0;\n"
			"  if(final.a<0.0) final.a = 0.0;\n"
			" //final = pow(final,vec3(0.5));\n"
			"	gl_FragColor = vec4(final*4.0);\n"
			"}";
	}

	const char * ShaderSrcCode::CombineOutlineVert()
	{
		return

			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords; \n"
			"varying  vec2 v_texCoords;\n"
			"\n"
			"void main(void)\n"
			"{\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::CombineOutlineFrag()
	{
		return
			"precision highp  float;"
			"uniform sampler2D u_sampler0;\n"
			"varying  vec2 v_texCoords;\n"
			"void main()\n"
			"{\n"
			"   vec4 gaussianColor = texture2D(u_sampler0,v_texCoords);\n"
			"  float alpha = 1.0;\n"
			" //  if(gaussianColor.r<=0.0 &&gaussianColor.g<=0.0&&gaussianColor.b<=0.0 ) alpha = 0.0;     \n"
			"	gl_FragColor =gaussianColor;\n"
			"}\n";
	}

	const char * ShaderSrcCode::SsaoEffectVert()
	{
		return
			""
			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords; \n"
			"varying  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char * ShaderSrcCode::SsaoEffectFrag()
	{
		return
			"uniform bool isPerspective;\n"
			"uniform float cameraNear;\n"
			"uniform float cameraFar;\n"
			"uniform float radius;\n"
			"uniform vec2 size;\n"
			"uniform float aoClamp;\n"

			"uniform float g_SSAO_Bias;\n"
			"uniform float g_Aear;\n"
			"uniform float g_Displace;\n"
			"uniform float g_LumInfluence;\n" //- is how strong ambient light affects on ssao.The brighter light in the room the less ssao you will see;
			"uniform float g_SSAOContrast;\n"// is how strong the ssao effect (how dark is ssao shadows);
			"uniform sampler2D depthMap;\n"

			"#define DL 2.399963229728653\n" // PI * ( 3.0 - sqrt( 5.0 ) )
			"#define EULER 2.718281828459045\n"

			"varying  vec2 v_texCoords;\n"

			"const int samples = 64;\n"     // ao sample count

			"const bool useNoise = true;\n"      // use noise instead of pattern for sample dithering
			"const float noiseAmount = 0.001;\n" // dithering amount

		//	"const float g_SSAO_Bias = 0.4;\n"   // is ssao 'accuracy'. Too accurate ssao can cause artifacts (shadows) on flat surfaces. This variable is needed to reduce such artifacts;
		//	"const float g_Displace = 0.4;\n"  // gauss bell center
			"vec2 rand( const vec2 coord ) {\n"

			"	vec2 noise;\n"

			"	if ( useNoise ) {\n"

			"		float nx = dot ( coord, vec2( 12.9898, 78.233 ) );\n"
			"		float ny = dot ( coord, vec2( 12.9898, 78.233 ) * 2.0 );\n"

			"		noise = clamp( fract ( 43758.5453 * sin( vec2( nx, ny ) ) ), 0.0, 1.0 );\n"

			"	} else {\n"

			"		float ff = fract( 1.0 - coord.s * ( size.x / 2.0 ) );\n"
			"		float gg = fract( coord.t * ( size.y / 2.0 ) );\n"

			"		noise = vec2( 0.25, 0.75 ) * vec2( ff ) + vec2( 0.75, 0.25 ) * gg;\n"

			"	}\n"

			"	return ( noise * 2.0  - 1.0 ) * noiseAmount;\n"

			"}\n"

			"float readDepth( const in vec2 coord,in bool isPerspective) {\n"
			"	float z =  texture2D( depthMap, coord ).r;\n"
			"	if(isPerspective == true)\n"
			"	{\n"
			"		"
			"		float cameraFarPlusNear = cameraFar + cameraNear;\n"
			"		float cameraFarMinusNear = cameraFar - cameraNear;\n"
			"		float cameraCoef = 2.0 * cameraNear;\n"
			"		return cameraCoef / ( cameraFarPlusNear - z * cameraFarMinusNear );\n"//转换成线性
			"	}\n"
			"   return z;\n"
			"}\n"

			"float compareDepths( const in float depth1, const in float depth2, inout int far ) {\n"

			"	float garea = g_Aear;\n"                         // gauss bell width
			"	float diff = ( depth1 - depth2 ) * 100.0;\n"  // depth difference (0-100)

			// reduce left bell width to avoid self-shadowing

			"	if ( diff < g_Displace ) {\n"

			"		garea = g_SSAO_Bias;\n"

			"	} else {\n"

			"		far = 1;\n"

			"	}\n"

			"	float dd = diff - g_Displace;\n"
			"	float gauss = pow( EULER, -2.0 * ( dd * dd ) / ( garea * garea ) );\n"
			"	return gauss;\n"

			"}\n"
			"float calcAO( float depth, float dw, float dh ) {\n"

			"	vec2 vv = vec2( dw, dh );\n"

			"	vec2 coord1 = v_texCoords + radius * vv;\n"
			"	vec2 coord2 = v_texCoords - radius * vv;\n"

			"	float temp1 = 0.0;\n"
			"	float temp2 = 0.0;\n"

			"	int far = 0;\n"
			"	temp1 = compareDepths( depth, readDepth( coord1,isPerspective ), far );\n"
			//"float rangeCheck = smoothstep(0.0, 1.0, radius / (abs(depth - readDepth( coord1,isPerspective ))*10000.0) );\n"
			// DEPTH EXTRAPOLATION

			"	if ( far > 0 ) {\n"

			"		temp2 = compareDepths( readDepth( coord2,isPerspective ), depth, far );\n"
			"		temp1 += ( 1.0 - temp1 ) * temp2;\n"

			"	}\n"

			"	return temp1;\n"

			"}\n"
			"void main() {\n"

			"	vec2 noise = rand( v_texCoords );\n"
			"	float depth = readDepth( v_texCoords ,isPerspective);\n"

			"	float tt = clamp( depth, aoClamp, 1.0 );\n"

			"	float w = ( 1.0 / size.x ) / tt + ( noise.x * ( 1.0 - noise.x ) );\n"
			"	float h = ( 1.0 / size.y ) / tt + ( noise.y * ( 1.0 - noise.y ) );\n"

			"	float ao = 0.0;\n"

			"	float dz = 1.0 / float( samples );\n"
			"	float l = 0.0;\n"
			"	float z = 1.0 - dz / 2.0;\n"

			"	for ( int i = 0; i <= samples; i ++ ) {\n"

			"		float r = sqrt( 1.0 - z );\n"

			"		float pw = cos( l ) * r;\n"
			"		float ph = sin( l ) * r;\n"
			"		ao += calcAO( depth, pw * w, ph * h );\n"
			"		z = z - dz;\n"
			"		l = l + DL;\n"

			"	}\n"

			"	ao /= float( samples );\n"
			"   ao*=g_SSAOContrast;\n"
			"	ao = 1.0 - ao;\n"

			//"vec3 color = texture2D( depthMap, v_texCoords ).rgb;\n"		
			"	vec3 lumcoeff = vec3( 0.2126, 0.7152, 0.0722 );\n"
			"	float lum = dot( vec3(depth), lumcoeff );\n"
			"	vec3 luminance = vec3( lum );\n"



			"	vec3 final = vec3( mix( vec3( ao ), vec3( 1.0 ), luminance *g_LumInfluence) );\n"  // ambient occlusion only
			//"  final = ((final - 0.5)*max(g_SSAOContrast,0.0))+0.5;\n"
			//"	vec3 final = vec3(ao);\n"
			"	gl_FragColor = vec4( final, 1.0 );\n"

			"}"
			;
	}

	const char* ShaderSrcCode::SsaoBlurVert()
	{
		return
			"attribute vec3 a_position;\n"
			"attribute  vec2 a_texCoords; \n"
			"varying  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::SsaoBlurFrag()
	{
		return
			""
			"varying  vec2 v_texCoords;\n"
			"uniform vec2 size;\n"
			"uniform sampler2D ssaoInput;\n"

			"void main()\n"
			"{\n"
			"	vec2 texelSize = 1.0 / size;\n"
			"	float result = 0.0;\n"
			"	for (int x = -2; x < 2; ++x)\n"
			"	{\n"
			"		for (int y = -2; y < 2; ++y)\n"
			"		{\n"
			"			vec2 offset = vec2(float(x), float(y)) * texelSize;\n"
			"			result += texture2D(ssaoInput, v_texCoords + offset).r;\n"
			"		}\n"
			"	}\n"
			"	gl_FragColor = vec4(vec3(result / (4.0 * 4.0)),1.0);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::SaoReconstructCSZVert()
	{
		return
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"out  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//	"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char * ShaderSrcCode::SaoReconstructCSZFrag()
	{
		return
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"#if __VERSION__ == 120\n"
			//  Compatibility with older versions of GLSL
			"#   define texelFetch texelFetch2D\n"
			"#   define result gl_FragColor.r\n"
			"#else\n"
			"			out float     result;\n"
			"#endif\n"
			"uniform sampler2D depthMap;\n"
			"uniform float near;\n"
			"uniform float far;\n"
			"uniform bool isPerspective;\n"
			"float getOrthographicZ(float z)\n"
			"{\n"
			"   float ret;\n"
			"   ret =  z*(far - near)+near;\n"
			"   ret = -ret;\n"
			"   return ret;\n"
			"}\n"
			"float getPerspectiveZ( float z)\n"
			"{\n"
			"   float ret;\n"
			"   ret =  near * far / (far +z * ( near - far));\n"
			"   ret = -ret;\n"
			"   return ret;\n"
			"}\n"
			"float reconstructCSZ( float z)\n"
			"{\n"
			"   if(isPerspective == true)\n"
			"     return getPerspectiveZ(z);\n"
			"   else\n"
			"     return getOrthographicZ(z);\n"
			"}\n"
			"void main() {\n"
			"	result = reconstructCSZ(texelFetch(depthMap, ivec2(gl_FragCoord.xy), 0).r);\n"
			"}\n"
			;
	}

	const char* ShaderSrcCode::SaoMinifyCSZVert()
	{
		return
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"out  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//	"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::SaoMinifyCSZFrag()
	{
		return
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"#if __VERSION__ == 120\n"
			//  Compatibility with older versions of GLSL
			"#   define texelFetch texelFetch2D\n"
			"#   define textureSize  textureSize2D\n"
			"#endif\n"
			"uniform sampler2D cszBuffer;\n"
			"uniform int       previousMIPNumber;\n"
			"void main() {\n"
			"ivec2 ssP = ivec2(gl_FragCoord.xy);\n"
			// Rotated grid subsampling to avoid XY directional bias or Z precision bias while downsampling.
			// On DX9, the bit-and can be implemented with floating-point modulo
			"gl_FragColor = texelFetch2D(cszBuffer, clamp(ssP * 2 + ivec2(ssP.y & 1, ssP.x & 1), ivec2(0), textureSize(cszBuffer, previousMIPNumber) - ivec2(1)), previousMIPNumber);\n"
			"}\n"
			;
	}

	const char * ShaderSrcCode::SaoBlurVert()
	{
		return
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"out  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//	"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::SaoBlurFrag()
	{
		return
			"#version 120 // -*- c++ -*-\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"#line 4\n"
			/**
			\file SAO_blur.pix
			\author Morgan McGuire and Michael Mara, NVIDIA Research

			\brief 7-tap 1D cross-bilateral blur using a packed depth key

			DX11 HLSL port by Leonardo Zide, Treyarch

			Open Source under the "BSD" license: http://www.opensource.org/licenses/bsd-license.php

			Copyright (c) 2011-2012, NVIDIA
			All rights reserved.

			Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

			Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
			Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
			THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
			*/

			//////////////////////////////////////////////////////////////////////////////////////////////
			// Tunable Parameters:

			/** Increase to make depth edges crisper. Decrease to reduce flicker. */
			"#define EDGE_SHARPNESS     (1.0)\n"

			/** Step in 2-pixel intervals since we already blurred against neighbors in the
			first AO pass.  This constant can be increased while R decreases to improve
			performance at the expense of some dithering artifacts.

			Morgan found that a scale of 3 left a 1-pixel checkerboard grid that was
			unobjectionable after shading was applied but eliminated most temporal incoherence
			from using small numbers of sample taps.
			*/
			"#define SCALE               (2)\n"

			/** Filter radius in pixels. This will be multiplied by SCALE. */
			"#define R                   (4)\n"


			//////////////////////////////////////////////////////////////////////////////////////////////

			/** Type of data to read from source.  This macro allows
			the same blur shader to be used on different kinds of input data. */
			"#define VALUE_TYPE        float\n"

			/** Swizzle to use to extract the channels of source. This macro allows
			the same blur shader to be used on different kinds of input data. */
			"#define VALUE_COMPONENTS   r\n"

			"#define VALUE_IS_KEY       0\n"

			/** Channel encoding the bilateral key value (which must not be the same as VALUE_COMPONENTS) */
			"#define KEY_COMPONENTS     gb\n"


			"#if __VERSION__ >= 330\n"
			// Gaussian coefficients
			"			const float gaussian[R + 1] = \n"
			//    float[](0.356642, 0.239400, 0.072410, 0.009869);
			//    float[](0.398943, 0.241971, 0.053991, 0.004432, 0.000134);  // stddev = 1.0
			"			float[](0.153170, 0.144893, 0.122649, 0.092902, 0.062970);  // stddev = 2.0\n"
			//      float[](0.111220, 0.107798, 0.098151, 0.083953, 0.067458, 0.050920, 0.036108); // stddev = 3.0
			"#endif\n"

			"uniform sampler2D   source;\n"

			/** (1, 0) or (0, 1)*/
			"uniform vec2       axis;\n"

			"#if __VERSION__ == 120\n"
			"#   define          texelFetch texelFetch2D\n"
			"#else\n"
			"		out vec3            gl_FragColor;\n"
			"#endif\n"

			"#define  result         gl_FragColor.VALUE_COMPONENTS\n"
			"#define  keyPassThrough gl_FragColor.KEY_COMPONENTS\n"

			/** Returns a number on (0, 1) */
			"float unpackKey(vec2 p) {\n"
			"			return p.x * (256.0 / 257.0) + p.y * (1.0 / 257.0);\n"
			"		}\n"


			"		void main() {\n"
			"#   if __VERSION__ < 330\n"
			"			float gaussian[R + 1];\n"
			"#       if R == 3\n"
			"			gaussian[0] = 0.153170; gaussian[1] = 0.144893; gaussian[2] = 0.122649; gaussian[3] = 0.092902;  // stddev = 2.0\n"
			"#       elif R == 4\n"
			"			gaussian[0] = 0.153170; gaussian[1] = 0.144893; gaussian[2] = 0.122649; gaussian[3] = 0.092902; gaussian[4] = 0.062970;  // stddev = 2.0\n"
			"#       elif R == 6\n"
			"			gaussian[0] = 0.111220; gaussian[1] = 0.107798; gaussian[2] = 0.098151; gaussian[3] = 0.083953; gaussian[4] = 0.067458; gaussian[5] = 0.050920; gaussian[6] = 0.036108;\n"
			"#       endif\n"
			"#   endif\n"

			"			ivec2 ssC = ivec2(gl_FragCoord.xy);\n"

			"			vec4 temp = texelFetch(source, ssC, 0);\n"

			"			keyPassThrough = temp.KEY_COMPONENTS;\n"
			"			float key = unpackKey(keyPassThrough);\n"
			"			VALUE_TYPE sum = temp.VALUE_COMPONENTS;\n"

			//"			if (key == 1.0) {\n"
			//// Sky pixel (if you aren't using depth keying, disable this test)
			//"				result = sum;\n"
			//"				return;\n"
			//"			}\n"

			// Base weight for depth falloff.  Increase this for more blurriness,
			// decrease it for better edge discrimination
			"			float BASE = gaussian[0];\n"
			"			float totalWeight = BASE;\n"
			"			sum *= totalWeight;\n"


			"			for (int r = -R; r <= R; ++r) {\n"
			// We already handled the zero case above.  This loop should be unrolled and the static branch optimized out,
			// so the IF statement has no runtime cost
			"				if (r != 0) {\n"
			"					temp = texelFetch(source, ssC + ivec2(int(axis.x),int(axis.y)) * (r * SCALE), 0);\n"
			"					float      tapKey = unpackKey(temp.KEY_COMPONENTS);\n"
			"					VALUE_TYPE value = temp.VALUE_COMPONENTS;\n"

			// spatial domain: offset gaussian tap
			"					float weight = 0.3 + gaussian[abs(r)];\n"

			// range domain (the "bilateral" weight). As depth difference increases, decrease weight.
			"					weight *= max(0.0, 1.0\n"
			"						- (EDGE_SHARPNESS * 2000.0) * abs(tapKey - key)\n"
			"					);\n"

			"					sum += value * weight;\n"
			"					totalWeight += weight;\n"
			"				}\n"
			"			}\n"

			"			const float epsilon = 0.0001;\n"
			"			result = sum / (totalWeight + epsilon);\n"
			"		}\n"
			;
	}

	const char * ShaderSrcCode::SaoEffectVert()
	{
		return
			""
			"#version 120\n"
			"in vec3 a_position;\n"
			"in  vec2 a_texCoords; \n"
			//"varying  vec2 v_texCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(a_position, 1.0);\n"
			//"    v_texCoords = a_texCoords;\n"
			"}"
			;
	}

	const char * ShaderSrcCode::SaoEffectFrag()
	{
		return
			"#version 120\n"
			"#extension GL_EXT_gpu_shader4 : require\n"
			"uniform sampler2D CS_Z_buffer;\n"
			"uniform float near;\n"
			"uniform float far;\n"
			" uniform vec2 resolution;\n"
			"uniform bool isPerspective;\n"


			// Total number of direct samples to take at each pixel
			"#define NUM_SAMPLES (21)\n"

			// If using depth mip levels, the log of the maximum pixel offset before we need to switch to a lower
			// miplevel to maintain reasonable spatial locality in the cache
			// If this number is too small (< 3), too many taps will land in the same pixel, and we'll get bad variance that manifests as flashing.
			// If it is too high (> 5), we'll get bad performance because we're not using the MIP levels effectively
			"#define LOG_MAX_OFFSET 3\n"

			// This must be less than or equal to the MAX_MIP_LEVEL defined in SSAO.cpp
			"#define MAX_MIP_LEVEL 5\n"

			/** Used for preventing AO computation on the sky (at infinite depth) and defining the CS Z to bilateral depth key scaling.
			This need not match the real far plane*/
			"#define FAR_PLANE_Z (300.0)\n"

			// This is the number of turns around the circle that the spiral pattern makes.  This should be prime to prevent
			// taps from lining up.  This particular choice was tuned for NUM_SAMPLES == 9
			"#define NUM_SPIRAL_TURNS (7)\n"

			//////////////////////////////////////////////////

			/** The height in pixels of a 1m object if viewed from 1m away.
			You can compute it from your projection matrix.  The actual value is just
			a scale factor on radius; you can simply hardcode this to a constant (~500)
			and make your radius value unitless (...but resolution dependent.)  */
			"uniform float projScale ;\n"

			"uniform vec4 saoData;\n"
			/** World-space AO radius in scene units (r).  e.g., 1.0m */
			"   float radius = saoData.x;\n"

			/** Bias to avoid AO in smooth corners, e.g., 0.01m */
			" float bias = saoData.y;\n "

			/** Darkending factor, e.g., 1.0 */
			"float intensity = saoData.z;\n"
			" float intensityDivR6 = saoData.w;\n"
			/**  vec4(-2.0f / (width*P[0][0]),
			-2.0f / (height*P[1][1]),
			( 1.0f - P[0][2]) / P[0][0],
			( 1.0f + P[1][2]) / P[1][1])

			where P is the projection matrix that maps camera space points
			to [-1, 1] x [-1, 1].  That is, GCamera::getProjectUnit(). */
			"uniform vec4 projInfo;\n"

			" float radius2 = radius * radius;\n"

			// Compatibility with future versions of GLSL: the shader still works if you change the
			// version line at the top to something like #version 330 compatibility.
			"#if __VERSION__ == 120\n"
			"#define texelFetch texelFetch2D\n"
			"#define textureSize textureSize2D\n"
			"#else\n"
			"   out vec3 gl_FragColor;\n"
			"#endif\n"
			"#define visibility gl_FragColor.r\n"
			"#define bilateralKey gl_FragColor.gb\n"

			/////////////////////////////////////////////////////////

			/** Reconstructs screen-space unit normal from screen-space position */

			"vec3 reconstructNonUnitCSFaceNormal(vec3 C)\n"
			"{\n"
			"    return cross(dFdy(C), dFdx(C));\n"
			"}\n"

			/** Reconstruct camera-space P.xyz from screen-space S = (x, y) in
			pixels and camera-space z < 0.  Assumes that the upper-left pixel center
			is at (0.5, 0.5) [but that need not be the location at which the sample tap
			was placed!]

			Costs 3 MADD.  Error is on the order of 10^3 at the far plane, partly due to z precision.
			*/
			"vec3 getPerspectiveCSPosition(vec2 S, float z)\n"
			"{\n"
			"    return vec3((S.xy * projInfo.xy + projInfo.zw) * z, z);\n"
			"}\n"
			"vec3 getOrthographicCSPosition(vec2 S, float z)\n"
			"{\n"
			"    return vec3((S.xy * projInfo.xy + projInfo.zw), z);\n"
			"}\n"
			"vec3 reconstructCSPosition( vec2 S, float z)\n"
			"{\n"
			"   if(isPerspective == true)\n"
			"     return getPerspectiveCSPosition(S,z);\n"
			"   else\n"
			"     return getOrthographicCSPosition(S,z);\n"
			"}\n"

			/** Reconstructs screen-space unit normal from screen-space position */
			"vec3 reconstructCSFaceNormal(vec3 C)\n"
			"{\n"
			"    return normalize(cross(dFdy(C), dFdx(C)));\n"
			"}\n"
			//-----------------------------------------------------------------------------------------------------------
			/** Read the camera-space position of the point at screen-space pixel ssP */
			"vec3 getPosition(ivec2 ssP, sampler2D cszBuffer)\n"
			"{\n"
			"   vec3 P;\n"
			"   P.z = texelFetch(cszBuffer, ssP, 0).r;\n"
			// Offset to pixel center
			"   P = reconstructCSPosition(vec2(ssP) + vec2(0.5), P.z);\n"
			"   return P;\n"
			"}\n"
			"	int getMipLevel(float ssR) {\n"
			// Derivation:
			//  mipLevel = floor(log(ssR / MAX_OFFSET));
			"	return clamp(int(floor(log2(ssR))) - LOG_MAX_OFFSET, 0, MAX_MIP_LEVEL);\n"
			"}\n"

			/** Read the camera-space position of the point at screen-space pixel ssP + unitOffset * ssR.  Assumes length(unitOffset) == 1 */
			"vec3 getOffsetPosition(ivec2 ssC, vec2 unitOffset, float ssR, sampler2D cszBuffer)\n"
			"{\n"
			// Derivation:
			//  mipLevel = floor(log(ssR / MAX_OFFSET));
			"    int mipLevel =getMipLevel(ssR);\n"

			"    ivec2 ssP = ivec2(ssR * unitOffset) + ssC;\n"

			"    vec3 P;\n"

			// We need to divide by 2^mipLevel to read the appropriately scaled coordinate from a MIP-map.
			// Manually clamp to the texture size because texelFetch bypasses the texture unit
			"   ivec2 mipP = clamp(ssP >> mipLevel, ivec2(0), textureSize(CS_Z_buffer, mipLevel) - ivec2(1));\n"

			//read from depth buffer
			"   P.z = texelFetch(cszBuffer, mipP,0).r;\n"
			// Offset to pixel center
			"    P = reconstructCSPosition(vec2(ssP) + vec2(0.5), P.z);\n"

			"   return P;\n"
			"}\n"

			"float fallOffFunction(float vv, float vn, float epsilon) {\n"
			// A: From the HPG12 paper
			// Note large epsilon to avoid overdarkening within cracks
			//  Assumes the desired result is intensity/radius^6 in main()
			// return float(vv < radius2) * max((vn - bias) / (epsilon + vv), 0.0) * radius2 * 0.6;

			// B: Smoother transition to zero (lowers contrast, smoothing out corners). [Recommended]
			"#   if HIGH_QUALITY\n"
			// Epsilon inside the sqrt for rsqrt operation
			"float f = max(1.0 - vv * invRadius2, 0.0); return f * max((vn - bias) * rsqrt(epsilon + vv), 0.0);\n"
			"#   else\n"
			// Avoid the square root from above.
			//  Assumes the desired result is intensity/radius^6 in main()
			"				float f = max(radius2 - vv, 0.0); return f * f * f * max((vn - bias) / (epsilon + vv), 0.0);\n"
			"#   endif\n"

			// C: Medium contrast (which looks better at high radii), no division.  Note that the 
			// contribution still falls off with radius^2, but we've adjusted the rate in a way that is
			// more computationally efficient and happens to be aesthetically pleasing.  Assumes 
			// division by radius^6 in main()
			// return 4.0 * max(1.0 - vv * invRadius2, 0.0) * max(vn - bias, 0.0);

			// D: Low contrast, no division operation
			//return 2.0 * float(vv < radius * radius) * max(vn - bias, 0.0);
			"}\n"
			/** Compute the occlusion due to sample point \a Q about camera-space point \a C with unit normal \a n_C */
			"float aoValueFromPositionsAndNormal(vec3 C, vec3 n_C, vec3 Q) {\n"
			"vec3 v = Q - C;\n"
			"float vv = dot(v, v);\n"
			"float vn = dot(v, n_C);\n"
			"const float epsilon = 0.001;\n"

			// Without the angular adjustment term, surfaces seen head on have less AO
			"return fallOffFunction(vv, vn, epsilon) * mix(1.0, max(0.0, 1.5 * n_C.z), 0.35);\n"
			"}\n"

			/** Returns a unit vector and a screen-space radius for the tap on a unit disk (the caller should scale by the actual disk radius) */
			"vec2  tapLocation(int sampleNumber, float spinAngle, out float ssR)\n"
			"{\n"
			// Radius relative to ssR
			"    float alpha = float(sampleNumber + 0.5) * (1.0 / NUM_SAMPLES);\n"
			"   float angle = alpha * (NUM_SPIRAL_TURNS * 6.28) + spinAngle;\n"

			"    ssR = alpha;\n"
			"   return vec2(cos(angle), sin(angle));\n"
			"}\n"
			/** Compute the occlusion due to sample with index \a i about the pixel at \a uv that corresponds
			to camera-space point \a C with unit normal \a n_C, using maximum screen-space sampling radius \a ssDiskRadius */
			"float sampleAO(ivec2 ssC, vec3 C, vec3 n_C, float ssDiskRadius, int tapIndex, float randomPatternRotationAngle, in sampler2D cszBuffer)\n"
			"{\n"
			// Offset on the unit disk, spun for this pixel
			"   float ssR;\n"
			"   vec2 unitOffset = tapLocation(tapIndex, randomPatternRotationAngle, ssR);\n"
			"   ssR =  ssR * ssDiskRadius;\n"
			// The occluding point in camera space
			"  vec3 Q = getOffsetPosition(ssC, unitOffset, ssR,cszBuffer);\n"

			" return aoValueFromPositionsAndNormal(C, n_C, Q);\n"
			"}\n"

			"float CSZToKey(float z)\n"
			"{\n"
			"   return clamp(z * (-1.0 / far), 0.0, 1.0);\n"
			"}\n"

			/** Used for packing Z into the GB channels */
			"void  packKey(float key, out vec2 p)\n"
			"{\n"
			// Round to the nearest 1/256.0
			"    float temp = floor(key * 256.0);\n"

			// Integer part
			"    p.x = temp * (1.0 / 256.0);\n"

			// Fractional part
			"    p.y = key * 256.0 - temp;\n"
			"}\n"
			"const float MIN_RADIUS = 3.0;\n"
			"void main()\n"
			"{\n"
			// Pixel being shaded
			"   ivec2 ssC = ivec2(gl_FragCoord.xy);\n"

			"   vec3 C = getPosition(ssC,CS_Z_buffer);\n"

			"    packKey(CSZToKey(C.z), bilateralKey);\n"
			// Hash function used in the HPG12 AlchemyAO paper
			"    float randomPatternRotationAngle = ((3 * ssC.x) ^ (ssC.y + ssC.x * ssC.y)) * 10;\n"

			"   vec3 n_C = reconstructNonUnitCSFaceNormal(C);\n"
			"	if (dot(n_C, n_C) > ((C.z * C.z * 0.00006)*(C.z * C.z * 0.00006))){\n"
			"		visibility = 1.0; \n"
			"		return;\n"
			"	} else {\n"
			"		n_C = normalize(n_C);\n"
			"	}\n"

			// Choose the screen-space sample radius
			// proportional to the projected area of the sphere
			"    float ssDiskRadius = projScale * radius / C.z;\n"
			//"if (ssDiskRadius <= 3.0) {\n"
			//	"visibility = 1.0;\n"
			//	" return;\n"
			//	" }\n"

			"    float sum = 0.0;\n"
			"   for (int i = 0; i < NUM_SAMPLES; ++i)\n"
			"   sum += sampleAO(ssC, C, n_C, ssDiskRadius, i, randomPatternRotationAngle,CS_Z_buffer);\n"

			"#   if HIGH_QUALITY\n"
			"				float A = pow(max(0.0, 1.0 - sqrt(sum * (3.0 / NUM_SAMPLES))), intensity);\n"
			"#   else\n"
			"				float A = max(0.0, 1.0 - sum * intensityDivR6 * (5.0 / NUM_SAMPLES));\n"
			// Anti-tone map to reduce contrast and drag dark region farther
			// (x^0.2 + 1.2 * x^4)/2.2
			"			A = (pow(A, 0.2) + 1.2 * A*A*A*A) / 2.2;\n"
			"#   endif\n"
			// Bilateral box-filter over a quad for free, respecting depth edges
			// (the difference that this makes is subtle)
			"   if (abs(dFdx(C.z)) < 0.02)\n"
			"   {\n"
			"    A -= dFdx(A) * (float(ssC.x & 1) - 0.5);\n"
			"   }\n"
			"   if (abs(dFdy(C.z)) < 0.02)\n"
			"   {\n"
			"    A -= dFdy(A) * (float(ssC.y & 1) - 0.5);\n"
			"   }\n"
				"  visibility = A;\n"
		//	"     visibility = mix(1.0, A, clamp(ssDiskRadius - MIN_RADIUS,0.0,1.0));\n"
			//				"gl_FragColor = vec4(vec3(A),1.0);\n"//ao debug
			"}\n"
			;
	}

	const char *ShaderSrcCode::SceneGroundVert()
	{
		return "//precision highp  float;\n"
			"attribute vec3 a_position;\n"
			"attribute vec4 a_color;\n"
			"uniform mat4 u_viewMat;\n"
			"uniform mat4 u_projectionMat;\n"
			"//uniform mat4 u_MVPMat;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_Position =  u_projectionMat*u_viewMat*vec4(a_position,1.0);\n"
			"v_color = a_color;\n"
			"}\n";
	}

	const char *ShaderSrcCode::SceneGroundFrag()
	{
		return "//precision mediump  float;\n"
			"varying vec4 v_color;\n"
			"void main() { \n"
			"gl_FragColor = v_color;\n"
			"}\n";
	}

	const char * ShaderSrcCode::PhongMaterialVert()
	{
		return
			"#define PHONG\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <uv_pars_vertex>\n"
			"#include <displacementmap_pars_vertex>\n"
			"#include <shadowmap_pars_vertex>\n"
			"#include <clip_pars_vertex>\n"
			"void main(){\n"
			"  #include <uv_vertex>\n"
			"  #include <beginnormal_vertex>\n"
			"  #include <defaultnormal_vertex>\n"
			"vNormal = normalize( transformedNormal );\n"
			"  #include <begin_vertex>\n"
			"#include <displacementmap_vertex>\n"
			"  #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"  #include <worldpos_vertex>\n"
			"#include <shadowmap_vertex>\n"
			"}"
			;
	}

	const char * ShaderSrcCode::PhongMaterialFrag()
	{
		return
			"#define PHONG\n"
			"uniform vec4 diffuse;\n"
			"uniform vec4 emissive;\n"
			"uniform vec4 specular;\n"
			"uniform float shininess;\n"
			"uniform float opacity;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"#include <uv_pars_fragment>\n"
			"#include <map_pars_fragment>\n"
			"#include <ssao_par_fragment>\n"
			"#include <bsdfs>\n"
			"#include <lights_pars>\n"
			"#include <matcap_pars_fragment>\n"
			"#include <lights_phong_pars_fragment>\n"
			"#include <normalmap_pars_fragment>\n"
			"#include <specularmap_pars_fragment>\n"
			"#include <shadowmap_pars_fragment>\n"
			"void main(){\n"
			"   #include <clip_fragment>\n"
			"   vec4 diffuseColor = vec4( diffuse.xyz, diffuse.a );\n"
			"   ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );\n"
			"vec3 totalEmissiveRadiance = emissive.rgb;\n"
			"   #include <map_fragment>\n"
			"   #include <specularmap_fragment>\n"
			"   #include <normal_fragment>\n"
			"	#include <lights_phong_fragment>\n"
			"   #include <lights_template>\n"
			"	float ssaoFactor = 1.0;\n"
			"	#include <ssao_fragment>\n"
			"	vec3 outgoingLight = ssaoFactor*(reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular)+ totalEmissiveRadiance  ;\n"
			"   gl_FragColor = vec4(outgoingLight, opacity );\n"
			"#include <encodings_fragment>\n"
			"gl_FragColor.rgb = gl_FragColor.rgb*u_selectColor.rgb;\n"

			"}"
			;
	}

	const char* ShaderSrcCode::PbrMaterialVert()
	{
		return "#define PHYSICAL\n"
			"#define USE_LOCAL_CUBEMAP\n"
			"uniform float gamma;\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <uv_pars_vertex>\n"
			"#include <parallax_corrected_cubemap_pars_vertex>\n"
			"#include <displacementmap_pars_vertex>\n"
			"#include <shadowmap_pars_vertex>\n"
			"#include <clip_pars_vertex>\n"
			"#include <z_depth_pars_vert>\n"
			"void main(){\n"
			"  #include <uv_vertex>\n"
			"  #include <beginnormal_vertex>\n"
			"  #include <defaultnormal_vertex>\n"
			"vNormal = normalize( transformedNormal );\n"
			"  #include <begin_vertex>\n"
			"#include <displacementmap_vertex>\n"
			"  #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"  #include <worldpos_vertex>\n"
			" #include <parallax_corrected_cubemap_vertex>\n"
			"   #include <shadowmap_vertex>\n"
			"   #include <z_depth_vert>\n"
			"}"
			;
	}

	const char* ShaderSrcCode::PbrMaterialFrag()
	{
		return
			"#define PHYSICAL\n"
			"#define USE_LOCAL_CUBEMAP\n"
			"#define TONE_MAPPING\n"
			"uniform vec4 diffuse;\n"
			"uniform vec4 emissive;\n"
			"uniform float roughness;\n"
			"uniform float metalness;\n"
			"uniform float opacity;\n"
			"uniform float gamma;\n"
			"uniform sampler2D lut;\n"
			"vec3 test = vec3(1.0,1.0,1.0);\n"
			"#ifndef STANDARD\n"
			"uniform float clearCoat;\n"
			"uniform float clearCoatRoughness;\n"
			"#endif\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"#include <uv_pars_fragment>\n"
			"#include <map_pars_fragment>\n"
			"#include <aomap_pars_fragment>\n"
			"#include <ssao_par_fragment>\n"
			"#include <emissivemap_pars_fragment>\n"
			"#include <parallax_corrected_cubemap_pars_fragment>\n"
			"#include <envmap_pars_fragment>\n"
			"#include <bsdfs>\n"
			"#include <lights_pars>\n"
			"#include <lights_physical_pars_fragment>\n"
			"#include <normalmap_pars_fragment>\n"
			"#include <shadowmap_pars_fragment>\n"
			"#include <z_depth_pars_frag>\n"
			"#include <tonemapping_pars_fragment>\n"
			"#include <metallicRoughness_pars_fragment>\n"
			"void main(){\n"
			"#include <z_depth_frag>\n"
			"   #include <clip_fragment>\n"
			"   vec4 diffuseColor = vec4( diffuse.xyz, diffuse.a );\n"
			"   ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );\n"
			"vec3 totalEmissiveRadiance = emissive.rgb;\n"
			"   #include <map_fragment>\n"
			"   #include <metallicRoughness_fragment>\n"
			"   #include <normal_fragment>\n"
			"#include <emissivemap_fragment>\n"

			"	#include <lights_physical_fragment>\n"
			"   #include <lights_template>\n"

			"#include <aomap_fragment>\n"
			"float ssaoFactor = 1.0;\n"
			"#include <ssao_fragment>\n"

			"	vec3 outgoingLight = reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular+ totalEmissiveRadiance ;\n"
			//"	vec3 lumcoeff = vec3( 0.299, 0.587, 0.114 );\n"
			//"	float lum = dot( outgoingLight.rgb, lumcoeff );\n"
			//"	vec3 luminance = vec3( lum );\n"

			//"	outgoingLight = vec3( outgoingLight * mix( vec3( ssaoFactor ), vec3( 1.0 ), luminance * 1.0f ) );\n"  // mix( color * ao, white, luminance )

			"//   outgoingLight =OptimizedCineonToneMapping(outgoingLight);\n"
			// "   gl_FragColor = vec4(pow(outgoingLight, vec3(1.0 / GAMMA_FACTOR)), diffuseColor.a );\n"
			"   gl_FragColor = vec4(outgoingLight, opacity );\n"
			"#include <tonemapping_fragment>\n"
			//"gl_FragColor.rgb = pow(gl_FragColor.rgb*u_selectColor.rgb, vec3(1.0 / gamma));\n"
			"#include <encodings_fragment>"
			"gl_FragColor.rgb = gl_FragColor.rgb*u_selectColor.rgb;\n"
			"}"
			;
	}

	const char* ShaderSrcCode::MatCapMaterialVert()
	{
		return
			"#define PHONG\n"
			"varying vec3 vViewPosition;\n"
			"varying vec3 vNormal;\n"
			"#include <common>\n"
			"#include <uv_pars_vertex>\n"
			"#include <displacementmap_pars_vertex>\n"
			"#include <shadowmap_pars_vertex>\n"
			"#include <clip_pars_vertex>\n"
			"void main(){\n"
			"  #include <uv_vertex>\n"
			"  #include <beginnormal_vertex>\n"
			"  #include <defaultnormal_vertex>\n"
			"vNormal = normalize( transformedNormal );\n"
			"  #include <begin_vertex>\n"
			"#include <displacementmap_vertex>\n"
			"  #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"  #include <worldpos_vertex>\n"
			"#include <shadowmap_vertex>\n"
			"}"
			;
	}

	const char* ShaderSrcCode::MatCapMaterialFrag()
	{
		return
			"#define PHONG\n"
			"uniform vec4 diffuse;\n"
			"uniform vec4 emissive;\n"
			"uniform vec4 specular;\n"
			"uniform float shininess;\n"
			"uniform float opacity;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"#include <uv_pars_fragment>\n"
			"#include <map_pars_fragment>\n"
			"#include <ssao_par_fragment>\n"
			"#include <bsdfs>\n"
			"#include <lights_pars>\n"
			"#include <matcap_pars_fragment>\n"
			"#include <lights_phong_pars_fragment>\n"
			"#include <normalmap_pars_fragment>\n"
			"#include <specularmap_pars_fragment>\n"
			"#include <shadowmap_pars_fragment>\n"
			"void main(){\n"
			"   #include <clip_fragment>\n"
			"   vec4 diffuseColor = vec4( diffuse.xyz, diffuse.a );\n"
			"   ReflectedLight reflectedLight = ReflectedLight( vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ), vec3( 0.0 ) );\n"
			"vec3 totalEmissiveRadiance = emissive.rgb;\n"
			"   #include <map_fragment>\n"
			"   #include <specularmap_fragment>\n"
			"   #include <normal_fragment>\n"
			"	#include <lights_phong_fragment>\n"
			"   #include <lights_template>\n"
			"	float ssaoFactor = 1.0f;\n"
			"	#include <ssao_fragment>\n"
			"	vec3 outgoingLight = ssaoFactor*(reflectedLight.directDiffuse + reflectedLight.indirectDiffuse + reflectedLight.directSpecular + reflectedLight.indirectSpecular)+ totalEmissiveRadiance  ;\n"
			"   gl_FragColor = vec4(outgoingLight, opacity );\n"
			"#include <encodings_fragment>"
			"gl_FragColor.rgb = gl_FragColor.rgb*u_selectColor.rgb;\n"

			"}"
			;
	}

	const char* ShaderSrcCode::DepthMaterialVert()
	{
		return
			"varying vec3 vViewPosition;\n"

			"#include <common>\n"
			"#include <clip_pars_vertex>\n"
			"void main()\n"
			"{\n"
			"    #include <begin_vertex>\n"
			"    #include <project_vertex>\n"
			"vViewPosition = - mvPosition.xyz;\n"
			"}\n"
			;
	}

	const char* ShaderSrcCode::DepthMaterialFrag()
	{
		return
			"varying vec3 vViewPosition;\n"
			"#include <common>\n"
			"#include <packing>\n"
			"#include <clip_pars_fragment>\n"
			"\n"
			"void main()\n"
			"{\n"
			"   #include <clip_fragment>\n"
			"    gl_FragColor = packDepthToRGBA(gl_FragCoord.z);\n"
			//"    gl_FragColor =vec4(1.0,0.0,0.0,1.0);",
			"}\n"
			;
	}

#endif
}

/* namespace M3D */
