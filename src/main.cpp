#define UNICODE
#define _UNICODE

#define BLOCK_STORY

#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>
#include <fstream>

#include <exception>
#include <typeinfo>
#include <stdexcept>

#define GLEW_STATIC
#include <GL/glew.h>

//#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <SOIL/SOIL.h>
	
#include "Rendering/Shader.h"
#include "Rendering/Batch.h"
#include "Rendering/Object.h"
#include "TestObjects/Label.h"
#include "Test/SimpleObj.h"
#include "Test/MultiTexObj.h"

#include "MyGraphicsExtensions.h"

#include "chunk.h"
#include "camera.h"
#include "raycast.h"
#include "world.h"

std::ostream& operator<<(std::ostream& os, const glm::vec3& value){
	os << value.x << " " << value.y << " " << value.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::ivec3& value){
	os << value.x << " " << value.y << " " << value.z;
	return os;
}

#ifndef media
#define media "media/"
#endif

//#define CHECK_COLLISION(p1, p2) CheckCollision(chunk, (p1).x - 0.3f + (p2).x, (p1).y - 1.4f + (p2).y, (p1).z - 0.3f + (p2).z, (p1).x + 0.3f + (p2).x, (p1).y + 0.4f + (p2).y, (p1).z + 0.3f + (p2).z)
//#define CHECK_COLLISION1(p1) CheckCollision(chunk, (p1).x - 0.3f, (p1).y - 1.4f, (p1).z - 0.3f, (p1).x + 0.3f, (p1).y + 0.4f, (p1).z + 0.3f)

#define CHECK_COLLISION(p1, p2) world.CheckCollision((p1).x - 0.3f + (p2).x, (p1).y - 1.4f + (p2).y, (p1).z - 0.3f + (p2).z, (p1).x + 0.3f + (p2).x, (p1).y + 0.4f + (p2).y, (p1).z + 0.3f + (p2).z)
#define CHECK_COLLISION1(p1) world.CheckCollision((p1).x - 0.3f, (p1).y - 1.4f, (p1).z - 0.3f, (p1).x + 0.3f, (p1).y + 0.4f, (p1).z + 0.3f)

#ifdef _WIN32

extern "C" {
    __declspec(dllimport) int __stdcall MultiByteToWideChar(
        unsigned int CodePage,
        unsigned int dwFlags,
        const char* lpMultiByteStr,
        int cbMultiByte,
        wchar_t* lpWideCharStr,
        int cchWideChar
    );

    __declspec(dllimport) int __stdcall WideCharToMultiByte(
        unsigned int CodePage,
        unsigned int dwFlags,
        const wchar_t* lpWideCharStr,
        int cchWideChar,
        char* lpMultiByteStr,
        int cbMultiByte,
        const char* lpDefaultChar,
        int* lpUsedDefaultChar
    );
}

constexpr unsigned int CP_UTF8 = 65001;

std::wstring utf8_to_wstring(const std::string& utf8_str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, wstr, len);
    std::wstring result(wstr);
    delete[] wstr;
    return result;
}

std::string wstring_to_cp1251(const std::wstring& wstr) {
    int len = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    char* str = new char[len];
    WideCharToMultiByte(1251, 0, wstr.c_str(), -1, str, len, NULL, NULL);
    std::string result(str);
    delete[] str;
    return result;
}

std::string toCp(const std::string& utf8_str){
	return wstring_to_cp1251(utf8_to_wstring(utf8_str));
	//return utf8_to_cp1251(utf8_to_wstring(utf8_str));
}

#endif //_WIN32

#include "input.h"

void debugCallback(unsigned int source, unsigned int type, unsigned int id,
 unsigned int severity, int length,
 const GLchar * message, const void * param) {

	std::cout << "msg: " << source << " " << type << " " << id << " " << severity << " " << length << " " << message << "\n";
 //printf("%s:%s[%s](%d): %s\n", source, type, severity, id, message);
}

struct ST_STRUCT{
	Rendering::Shader sObjShaderMVP;// = Rendering::Shader(media "shaders/rockMVP.vert", media "shaders/rock.frag");
	Rendering::Shader circleShader;//(media "shaders/rockMVP.vert", media "shaders/rockCircle.frag");
	Rendering::Shader sObjShaderMVPAlpha;//(media "shaders/rockMVP.vert", media "shaders/rockA.frag");
	Rendering::Shader outlineShader;//(media "shaders/outline.vert", media "shaders/outline.frag");
	Rendering::Shader exp_no_tex_Shader;//(media "shaders/exp_no_tex.vert", media "shaders/exp_no_tex.frag");
	//Rendering::Shader label_shader(media "shaders/label.vert", media "shaders/label_uColor.frag");
	Rendering::Shader label_shader;//(media "shaders/label.vert", media "shaders/label_uColor_uHighLightColor.frag");
	Rendering::Shader wallShader;//(media "shaders/wall.vert", media "shaders/wall.frag");
	Rendering::Shader wallColorShader;//(media "shaders/wall.vert", media "shaders/wall_color.frag");
	//Rendering::Shader wallShader(media "shaders/wall_displacement_uTime.vert", media "shaders/wall.frag");
	Rendering::Shader colorfulShader;//(media "shaders/colorful.vert", media "shaders/colorful.frag");
	Rendering::Shader highlightShader;//(media "shaders/rockMVP.vert", media "shaders/color_highlight.frag");
	//Rendering::Shader displacementShader(media "shaders/displacement.vert", media "shaders/color_highlight.frag");
	Rendering::Shader displacementShader;//(media "shaders/displacement_uTime.vert", media "shaders/color_highlight.frag");
	Rendering::Shader dotProductTestShader;//(media "shaders/posOut.vert", media "shaders/dotProductTest.frag");
	Rendering::Shader wallVertColorShader;//(media "shaders/wallXYRGB.vert", media "shaders/wall_aColor.frag");
	Rendering::Shader wallCircleShader;//(media "shaders/wallTex.vert", media "shaders/wallTex.frag");
	Rendering::Shader wallRBShader;
	Rendering::Shader shaderXYZUV;
	Rendering::Shader shaderXYZUV_Transparent;
	Rendering::Shader shaderXYZUV_TransparentStripe;
	Rendering::Shader shaderXYZUVS;
	Rendering::Shader shaderRayDebug;

	Rendering::Texture blackTexF;// = Rendering::Texture(media "textures/black_f.png", SOIL_LOAD_RGB);
	Rendering::Texture whiteTexF;// = Rendering::Texture(media "textures/white_f.png", SOIL_LOAD_RGB);
	Rendering::Texture blackTex;// = Rendering::Texture(media "textures/black.png", SOIL_LOAD_RGB); // EDITED FROM black.png TO black_color.png
	Rendering::Texture whiteTex;// = Rendering::Texture(media "textures/white.png", SOIL_LOAD_RGB);
	Rendering::Texture saveTex;// = Rendering::Texture(media "textures/save.png", SOIL_LOAD_RGB);
	Rendering::Texture loadTex;// = Rendering::Texture(media "textures/load.png", SOIL_LOAD_RGB);
	Rendering::Texture reloadTex;// = Rendering::Texture(media "textures/reload.png", SOIL_LOAD_RGB);
	Rendering::Texture captionWhiteMoveTex;// = Rendering::Texture(media "textures/captionWhiteMove.png", SOIL_LOAD_RGBA);
	Rendering::Texture captionBlackMoveTex;// = Rendering::Texture(media "textures/captionBlackMove.png", SOIL_LOAD_RGBA);
	Rendering::Texture captionWhiteWinsTex;// = Rendering::Texture(media "textures/captionWhiteWins.png", SOIL_LOAD_RGBA);
	Rendering::Texture captionBlackWinsTex;// = Rendering::Texture(media "textures/captionBlackWins.png", SOIL_LOAD_RGBA);
	Rendering::Texture blackHighlightedTex;// = Rendering::Texture(media "textures/black_highlighted.png", SOIL_LOAD_RGB);
	Rendering::Texture whiteHighlightedTex;// = Rendering::Texture(media "textures/white_highlighted.png", SOIL_LOAD_RGB);
	Rendering::Texture findPath;// = Rendering::Texture(media "textures/findPath.png", SOIL_LOAD_RGB);
	Rendering::Texture deleteVert;// = Rendering::Texture(media "textures/deleteSelected.png", SOIL_LOAD_RGB);
	Rendering::Texture paintVertsTex;//= Rendering::Texture(media "textures/paintVerts.png", SOIL_LOAD_RGB);
	Rendering::Texture paintLinksTex;// = Rendering::Texture(media "textures/paintLinks.png", SOIL_LOAD_RGB);
	Rendering::Texture paintAllTex;// = Rendering::Texture(media "textures/paintAll.png", SOIL_LOAD_RGB);
	Rendering::Texture clearTex;// = Rendering::Texture(media "textures/clearPaint.png", SOIL_LOAD_RGB);
	Rendering::Texture captionMouse;// = Rendering::Texture(media "textures/captionMouse.png", SOIL_LOAD_RGBA);
	Rendering::Texture emptyTex;// = Rendering::Texture(media "textures/empty.png", SOIL_LOAD_RGBA);
	Rendering::Texture textTex;// = Rendering::Texture(media "textures/symbols.png", SOIL_LOAD_RGBA);
	//Rendering::Texture textTex(media "textures/sussyText.png", SOIL_LOAD_RGBA);
	Rendering::Texture digit_tex;// = Rendering::Texture(media "textures/digits2.png", SOIL_LOAD_RGBA);

	Rendering::Texture taskTableTex;// = Rendering::Texture(media "textures/taskTable.png", SOIL_LOAD_RGBA);
	Rendering::Texture taskFlowTex;// = Rendering::Texture(media "textures/taskFlow.png", SOIL_LOAD_RGBA);
	Rendering::Texture tableMaxTex;// = Rendering::Texture(media "textures/tableMax.png", SOIL_LOAD_RGBA);
	Rendering::Texture tableMinTex;

	Rendering::Texture nextTex;// = Rendering::Texture(media "textures/tableMax.png", SOIL_LOAD_RGBA);
	Rendering::Texture prevTex;

	Rendering::Texture blockAtlas;

	//2D
	glm::mat4 ortho_view;
	float zFar;
	float zNear;
	glm::mat4 orth_proj;
	glm::mat4 ortho_projView;
	glm::mat4 const_ortho_projView;

	//3D
	glm::mat4 view;
	glm::mat4 proj;

	glm::vec3 cameraPos;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;

	glm::mat3 rotRight;

	glm::mat3 rotDown;

	glm::mat3 rotLeft;

	glm::mat3 rotUp;

	Camera camera;
};

std::string f_to_s(float value, int precision){
	int q = std::floor(value);
	int b = value * precision - q * precision;
	if (b > 9)
		return std::to_string(q) + "." + std::to_string(b);
	return std::to_string(q) + ".0" + std::to_string(b);
}

void main_process(const std::string& worldName){
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(input::window_width, input::window_height, "Endless Cavess", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	// V-SYNC
	//glfwSwapInterval(1);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(-1);
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback( debugCallback, NULL );
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	glfwSetKeyCallback(window, input::key_callback);
	glfwSetMouseButtonCallback(window, input::mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, input::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, input::mouse_cursor_callback);
	glfwSetScrollCallback(window, input::scroll_callback);

	glfwGetFramebufferSize(window, &input::window_width, &input::window_height);
	glViewport(0, 0, input::window_width, input::window_height);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	ST_STRUCT ST;

	ST.ortho_view = glm::lookAt(
		glm::vec3(0, 0, 3),
		glm::vec3(0, 0, 2),
		glm::vec3(0, 1, 0)
	);
	ST.zFar = 10000;
	ST.zNear = 0.001f;
	ST.orth_proj = glm::ortho<float>(-1, 1, -1, 1, ST.zNear, ST.zFar);
	ST.ortho_projView = ST.orth_proj * ST.ortho_view;
	ST.const_ortho_projView = ST.orth_proj * ST.ortho_view;

	ST.view = glm::lookAt(
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 1, 0)
	);
	ST.proj = glm::perspective(glm::radians(45.0f), input::window_width / ((float)input::window_height), ST.zNear, ST.zFar);

	ST.cameraPos = glm::vec3(0, 0, 0);
	ST.cameraForward = glm::vec3(0, 0, -1);
	ST.cameraUp = glm::vec3(0, 1, 0);

	float angleRot = 0.004;
	float cosA = glm::cos(angleRot);
	float sinA = glm::sin(angleRot);

	ST.rotRight = {
		cosA, 0, sinA,
		0, 1, 0,
		-sinA, 0, cosA,
	};

	ST.rotDown = {
		1, 0, 0,
		0, cosA, -sinA,
		0, sinA, cosA,
	};

	cosA = glm::cos(-angleRot);
	sinA = glm::sin(-angleRot);

	ST.rotLeft = {
		cosA, 0, sinA,
		0, 1, 0,
		-sinA, 0, cosA,
	};

	ST.rotUp = {
		1, 0, 0,
		0, cosA, -sinA,
		0, sinA, cosA,
	};

	// Shaders
	ST.sObjShaderMVP = Rendering::Shader(media "shaders/rockMVP.vert", media "shaders/rock.frag");
	ST.circleShader = Rendering::Shader(media "shaders/rockMVP.vert", media "shaders/rockCircle.frag");
	ST.sObjShaderMVPAlpha = Rendering::Shader(media "shaders/rockMVP.vert", media "shaders/rockA.frag");
	ST.outlineShader = Rendering::Shader(media "shaders/outline.vert", media "shaders/outline.frag");
	ST.exp_no_tex_Shader = Rendering::Shader(media "shaders/exp_no_tex.vert", media "shaders/exp_no_tex.frag");
	//Rendering::Shader label_shader(media "shaders/label.vert", media "shaders/label_uColor.frag");
	ST.label_shader = Rendering::Shader(media "shaders/label.vert", media "shaders/label_uColor_uHighLightColor.frag");
	ST.wallShader = Rendering::Shader(media "shaders/wall.vert", media "shaders/wall.frag");
	ST.wallColorShader = Rendering::Shader(media "shaders/wall.vert", media "shaders/wall_color.frag");
	//Rendering::Shader wallShader(media "shaders/wall_displacement_uTime.vert", media "shaders/wall.frag");
	ST.colorfulShader = Rendering::Shader(media "shaders/colorful.vert", media "shaders/colorful.frag");
	ST.highlightShader = Rendering::Shader(media "shaders/rockMVP.vert", media "shaders/color_highlight.frag");
	//Rendering::Shader displacementShader(media "shaders/displacement.vert", media "shaders/color_highlight.frag");
	ST.displacementShader = Rendering::Shader(media "shaders/displacement_uTime.vert", media "shaders/color_highlight.frag");
	ST.dotProductTestShader = Rendering::Shader(media "shaders/posOut.vert", media "shaders/dotProductTest.frag");
	ST.wallVertColorShader = Rendering::Shader(media "shaders/wallXYRGB.vert", media "shaders/wall_aColor.frag");
	ST.wallCircleShader = Rendering::Shader(media "shaders/wallTex.vert", media "shaders/wallTex.frag");
	ST.wallRBShader = Rendering::Shader(media "shaders/wallRB.vert", media "shaders/wallRB.frag");
	//Rendering::Shader editableLabelShader(media "shaders/label.vert", media "shaders/color_highlight.frag")
	ST.shaderXYZUV = Rendering::Shader(media "shaders/rockXYZUV.vert", media "shaders/rock.frag");
	ST.shaderXYZUV_Transparent = Rendering::Shader(media "shaders/rockXYZUV.vert", media "shaders/rockTransparent.frag");
	ST.shaderRayDebug = Rendering::Shader(media "shaders/debug_ray.vert", media "shaders/debug_ray.frag");
	ST.shaderXYZUV_TransparentStripe = Rendering::Shader(media "shaders/rockXYZUV.vert", media "shaders/rockTransparentStripe.frag");
	ST.shaderXYZUVS = Rendering::Shader(media "shaders/rockXYZUVS.vert", media "shaders/rockShadow.frag");

	// Textures
	ST.blackTexF = Rendering::Texture(media "textures/black_f.png", SOIL_LOAD_RGB);
	ST.whiteTexF = Rendering::Texture(media "textures/white_f.png", SOIL_LOAD_RGB);
	ST.blackTex = Rendering::Texture(media "textures/black.png", SOIL_LOAD_RGB); // EDITED FROM black.png TO black_color.png
	ST.whiteTex = Rendering::Texture(media "textures/white.png", SOIL_LOAD_RGB);
	ST.saveTex = Rendering::Texture(media "textures/save.png", SOIL_LOAD_RGB);
	ST.loadTex = Rendering::Texture(media "textures/load.png", SOIL_LOAD_RGB);
	ST.reloadTex = Rendering::Texture(media "textures/reload.png", SOIL_LOAD_RGB);
	ST.captionWhiteMoveTex = Rendering::Texture(media "textures/captionWhiteMove.png", SOIL_LOAD_RGBA);
	ST.captionBlackMoveTex = Rendering::Texture(media "textures/captionBlackMove.png", SOIL_LOAD_RGBA);
	ST.captionWhiteWinsTex = Rendering::Texture(media "textures/captionWhiteWins.png", SOIL_LOAD_RGBA);
	ST.captionBlackWinsTex = Rendering::Texture(media "textures/captionBlackWins.png", SOIL_LOAD_RGBA);
	ST.blackHighlightedTex = Rendering::Texture(media "textures/black_highlighted.png", SOIL_LOAD_RGB);
	ST.whiteHighlightedTex = Rendering::Texture(media "textures/white_highlighted.png", SOIL_LOAD_RGB);
	ST.findPath = Rendering::Texture(media "textures/findPath.png", SOIL_LOAD_RGB);
	ST.deleteVert = Rendering::Texture(media "textures/deleteSelected.png", SOIL_LOAD_RGB);
	ST.paintVertsTex = Rendering::Texture(media "textures/paintVerts.png", SOIL_LOAD_RGB);
	ST.paintLinksTex = Rendering::Texture(media "textures/paintLinks.png", SOIL_LOAD_RGB);
	ST.paintAllTex = Rendering::Texture(media "textures/paintAll.png", SOIL_LOAD_RGB);
	ST.clearTex = Rendering::Texture(media "textures/clearPaint.png", SOIL_LOAD_RGB);
	ST.captionMouse = Rendering::Texture(media "textures/captionMouse.png", SOIL_LOAD_RGBA);
	ST.emptyTex = Rendering::Texture(media "textures/empty.png", SOIL_LOAD_RGBA);
	ST.textTex = Rendering::Texture(media "textures/symbols.png", SOIL_LOAD_RGBA);
	//Rendering::Texture textTex(media "textures/sussyText.png", SOIL_LOAD_RGBA);
	ST.digit_tex = Rendering::Texture(media "textures/digits2.png", SOIL_LOAD_RGBA);

	ST.taskTableTex = Rendering::Texture(media "textures/taskTable.png", SOIL_LOAD_RGBA);
	ST.taskFlowTex = Rendering::Texture(media "textures/taskFlow.png", SOIL_LOAD_RGBA);
	ST.tableMaxTex = Rendering::Texture(media "textures/tableMax.png", SOIL_LOAD_RGBA);
	ST.tableMinTex = Rendering::Texture(media "textures/tableMin.png", SOIL_LOAD_RGBA);

	ST.nextTex = Rendering::Texture(media "textures/next.png", SOIL_LOAD_RGBA);
	ST.prevTex = Rendering::Texture(media "textures/prev.png", SOIL_LOAD_RGBA);

#ifdef BLOCK_STORY
	//ST.blockAtlas = Rendering::Texture(media "textures/sus_tex.png", SOIL_LOAD_RGB);
	ST.blockAtlas = Rendering::Texture(media "textures/blockStoryAtlas.png", SOIL_LOAD_RGBA);
#else
	//ST.blockAtlas = Rendering::Texture(media "textures/sus_tex.png", SOIL_LOAD_RGB);
	ST.blockAtlas = Rendering::Texture(media "textures/minecraftAtlas.png", SOIL_LOAD_RGBA);
#endif

	// SET FILTER MODE FOR BLOCKS TEXTURE ATLAS
	glBindTexture(GL_TEXTURE_2D, ST.blockAtlas.textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	srand(time(0));
	//srand(3323);
	float s_scale = 0.1f;

	glm::mat4 vert_label_transform = 
	{
		s_scale * 0.2f, 0, 0, 0,
		0, s_scale * 0.2f, 0, 0,
		0, 0, s_scale * 0.2f, 0,
		-0.3f, 0, 0.2f, 1.0f
	}; 

	ST.camera = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), -90.0f, 0.0f);
	
	// MAIN LOOP VARIABLES
	float _time = 0;

	// deltaTime
	double prevFrameTime = glfwGetTime();
	double deltaTime = 0;

	// MOUSE HOLD TIME
	double mouse_hold_time = 0.2;
	float mouse_move_error = 4.0f;
	double mouse_double_click_time = 0.166;

	double mouse_left_cur_time = 0;
	double mouse_right_cur_time = 0;

	double mouse_left_cursor_start_x = 0;
	double mouse_left_cursor_start_y = 0;
	double mouse_right_cursor_start_x = 0;
	double mouse_right_cursor_start_y = 0;
	
	bool pressed_left = false;
	bool pressed_right = false;

	bool left_start_hold = false;
	bool left_end_hold = false;
	bool right_start_hold = false;
	bool right_end_hold = false;

	double mouse_left_last_click_time = 0;

	EditableLabel editableLabelRez;
	editableLabelRez.Init(10);
	editableLabelRez.SetValue("");
	EditableLabel editableLabel;
	editableLabel.Init(10);
	editableLabel.SetValue("");

	EditableLabel editableLabelFloyd;
	editableLabelFloyd.Init(10);
	editableLabelFloyd.SetValue("");

	EditableLabel editableLabelD;
	editableLabelD.Init(10);
	editableLabelD.SetValue("");

	glm::mat4 editableLabelMatrix = {
		0.03, 0, 0, 0,
		0, 0.03, 0, 0,
		0, 0, 0.03, 0,
		-0.9, -0.8, 0.3, 1
	};
	glm::mat4 editableLabelRezMatrix = {
		0.03, 0, 0, 0,
		0, 0.03, 0, 0,
		0, 0, 0.03, 0,
		-0.9, -0.9, 0.3, 1
	};

	glm::mat4 editableLabelFloydMatrix = {
		0.03, 0, 0, 0,
		0, 0.03, 0, 0,
		0, 0, 0.03, 0,
		-0.9, -0.6, 0.3, 1
	};

	glm::mat4 editableLabelDMatrix = {
		0.03, 0, 0, 0,
		0, 0.03, 0, 0,
		0, 0, 0.03, 0,
		-0.9, -0.7, 0.3, 1
	};

	SetUniform(ST.label_shader, "uColor", glm::vec3(1.0, 1.0, 1.0));

	TextMultiLine multiLine(-0.9, -0.2, 0.1, 0.03);
	std::vector<std::vector<int>> table = 
	{
		{21, 16, 11, 12, 18},
		{9, 11, 13, 19, 13},
		{13, 15, 16, 17, 10},
		{9, 12, 15, 14, 11},
		{7, 10, 14, 17, 9}
	};

	WallMesh backlightMesh;
	glm::mat4 backlightMatrix = {
		1.0, 0, 0, 0,
		0, 1.0, 0, 0,
		0, 0, 1.0, 0,
		-0.92, 0.18, -1.0, 1.0
	};

/*==========================================================================================
                                     	############
                                     ###################
                                 ######             ########
                               #####					#####
                              #####              		 #####
                              #####						 #####
                              #####						 #####
                              #####						 #####
                              #####						 #####
                               #####					#####
                                 #####				  #####
                                 	#####################
                                 		############
//=========================================================================================*/

	

	float scroll_speed = 0.05f;

	EditableLabel treeLabel;
	treeLabel.Init(4);
	treeLabel.SetValue("-249");

	DataGridView dgv1x1(1, 1, 0.1, 0.05);
	dgv1x1.offsetX = 0.2;
	dgv1x1.offsetY = 0.06;
	dgv1x1.labelModel = &editableLabel;
	dgv1x1.SetPosition(gifi47::vec2(0.7, 0.9));

	EditableLabel fpsLabel;
	fpsLabel.Init(6);
	fpsLabel.SetValue("");

	glm::mat4 fpslabelMatrix = {
		0.03, 0, 0, 0,
		0, 0.03, 0, 0,
		0, 0, 0.03, 0,
		-0.9, 0.94, 0.3, 1
	};

	EditableLabel coordsLabel;
	coordsLabel.Init(30);
	coordsLabel.SetValue("");

	glm::mat4 coordsLabelMatrix = {
		0.03, 0, 0, 0,
		0, 0.03, 0, 0,
		0, 0, 0.03, 0,
		-0.9, 0.94, 0.3, 1
	};

	gifi47::vec2 prevPos;
	gifi47::vec2 prevPosRight;

	//ENDLESS CAVESS

	Chunk chunk;
	Chunk chunk2;
	chunk2.x = 1;
	chunk2.y = 0;
	chunk2.z = 0;
	for (int _x = 0; _x < CHUNK_SIZE_X; _x++){
		for (int _y = 0; _y < CHUNK_SIZE_Y; _y++){
			for (int _z = 0; _z < CHUNK_SIZE_Z; _z++){
				/*if (chunk.blocks[_x][_y][_z] != 0){
					chunk.blocks[_x][_y][_z] = 0xAB;
				}*/
				//chunk.blocks[_x][_y][_z] = 0xBC;
				
				int block = (std::rand() % 132);
				int block2 = (std::rand() % 132);
				if (block > 15) {block = 0;block2=0;} 
				if (block2 > 15) {block=0;block2 = 0;}
				chunk.blocks[_x][_y][_z] = block + (block2 << 4);
				int block3 = (std::rand() % 132);
				if (block3 > 15) {block3=0;block2 = 0;}
				if (block2 > 15) {block3=0;block2 = 0;}
				chunk2.blocks[_x][_y][_z] = block2 + (block3 << 4);
			}
		}
	}

	std::vector<float> chunkMesh = chunk.CreateMesh();
	ObjectXYZUV chunkObj(chunkMesh);

	chunkMesh = chunk2.CreateMesh();
	ObjectXYZUV chunkObj2(chunkMesh);
	/*for (int i = 0; i < 6 * 4; i++){
		std::cout <<  chunkObj.indexes[i] << "\n";
	}*/

	tst::SimpleObj tmp_figure(media "models/bishop.obj", ST.sObjShaderMVP, ST.blackTexF, 5);
	
	//ST.cameraPos = glm::vec3(0, 17, 0);

	ST.camera.position = glm::vec3(17, 10, 17);

	glm::mat4 f_mat = glm::translate(glm::mat4(1), glm::vec3((-3.5f + 1.0f * 4) * 0.2f, (3.5f - 1.0f * 4) * 0.2f, 0.0f));
	f_mat = glm::scale(f_mat, glm::vec3(0.05f, 0.05f, 0.05f));

	chunkMesh = std::vector<float>(0);
	AddCube(chunkMesh, 0, 0, 0, 0, 0.51f);
	ObjectXYZUV temp_cube(chunkMesh);

	float speed = 3.0f * 3.0f;

	double prev_mouse_cursor_x = input::mouse_cursor_x;
	double prev_mouse_cursor_y = input::mouse_cursor_y;

	bool left_click = false;
	bool right_click = false;
	bool update_chunk = false;
	bool place_block = false;
	glm::ivec3 place_block_pos(0, 0, 0);
	bool destroy_block = false;
	glm::ivec3 destroy_block_pos(0, 0, 0);

	std::vector<glm::ivec3> blocks;

	int block_to_place = 1;

	RayDebugDrawer rayDrawer;

	chunkMesh = std::vector<float>(0);
	AddCube(chunkMesh, 1, 0, 0, 0, 0.1f);
	ObjectXYZUV cube_to_place(chunkMesh);

	glm::mat4 const_projView = ST.proj * ST.view;
	glm::mat4 cube_to_place_model = glm::mat4(1.0f);
	cube_to_place_model = glm::translate(cube_to_place_model, glm::vec3(-0.2f, -0.2f, 0.5f));

	World world(worldName);

	world.LoadChunk(0, 0, 0);
	world.LoadChunk(0, 0, 1);
	world.LoadChunk(0, 1, 0);
	world.LoadChunk(0, 1, 1);
	world.LoadChunk(1, 0, 0);
	world.LoadChunk(1, 0, 1);
	world.LoadChunk(1, 1, 0);
	world.LoadChunk(1, 1, 1);

	bool buttonUpPressed = false;

	EditableLabel chunksLabel;
	chunksLabel.Init(30);
	chunksLabel.SetValue("");

	glm::mat4 chunksLabelMatrix = {
		0.03, 0, 0, 0,
		0, 0.0435, 0, 0,
		0, 0, 0.03, 0,
		-0.9, 0.85, 0.3, 1
	};

	// MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// MOUSE LEFT BUTTON PRESS
		if (input::mouse_button_left){
			// ON START PRESS BUTTON
			if (!pressed_left){
				mouse_left_cursor_start_x = input::mouse_cursor_x;
				mouse_left_cursor_start_y = input::mouse_cursor_y;
			}
			pressed_left = true;

			if (mouse_left_cur_time > mouse_hold_time || 
				mouse_move_error < gifi47::vec2::distance
				(
					gifi47::vec2(input::mouse_cursor_x, input::mouse_cursor_y),
					gifi47::vec2(mouse_left_cursor_start_x, mouse_left_cursor_start_y)
				))
			{
				if (!input::mouse_button_left_hold){
					input::mouse_button_left_hold = true;
					left_start_hold = true;
				}
			} 
			else
				mouse_left_cur_time += deltaTime;
		}
// MOUSE LEFT BUTTON END PRESS
		if (pressed_left && !input::mouse_button_left){
			if (input::mouse_button_left_hold){ 
				left_end_hold = true;
				input::mouse_button_left_hold = false;
			}
			else if (mouse_left_cur_time <= mouse_hold_time){
				input::mouse_button_left_click = true;
			}
			mouse_left_cur_time = 0;
			pressed_left = false;
		}

// MOUSE RIGHT BUTTON PRESS
		if (input::mouse_button_right){
			if (!pressed_right){
				mouse_right_cursor_start_x = input::mouse_cursor_x;
				mouse_right_cursor_start_y = input::mouse_cursor_y;
			}
			pressed_right = true;
			if (mouse_right_cur_time > mouse_hold_time ||
				mouse_move_error < gifi47::vec2::distance
				(
					gifi47::vec2(input::mouse_cursor_x, input::mouse_cursor_y),
					gifi47::vec2(mouse_right_cursor_start_x, mouse_right_cursor_start_y)
				))
			{
				if (!input::mouse_button_right_hold){
					input::mouse_button_right_hold = true;
					right_start_hold = true;
				}
			} 
			else
				mouse_right_cur_time += deltaTime;
		}

// MOUSE RIGHT BUTTON END PRESS
		if (pressed_right && !input::mouse_button_right){
			if (input::mouse_button_right_hold) {
				input::mouse_button_right_hold = false;
				right_end_hold = true;
			}
			else if (mouse_right_cur_time <= mouse_hold_time){
				input::mouse_button_right_click = true;
			}
			mouse_right_cur_time = 0;
			pressed_right = false;
		}

// RESOLUTION UPDATE
		if (input::is_resolution_changed){
			input::is_resolution_changed = false;
			ST.proj = glm::perspective(glm::radians(90.0f), input::window_width / ((float)input::window_height), ST.zNear, ST.zFar);
			//std::cin >> ST.camera.position.y;
		}

// MOUSE CALLBACKS

// MOUSE LB CLICK CALLBACK
		if (input::mouse_button_left_click){
			input::mouse_button_left_click = false;
			left_click = true;
			if (input::is_mouse_in_button(4, 2)){
				int val = std::stoi(dgv1x1.values[0][0]);
				
			} else if (input::is_mouse_in_button(5, 2)){
				
			} else if (input::is_mouse_in_button(1, 2)){
				
			}
			//gifi47::vec2 mouse_pos = mouse_to_viewport(mouse_left_cursor_start_x, mouse_left_cursor_start_y);
			//std::cout << "CLICK TABLE ON " << mouse_pos.x << " " << mouse_pos.y << "\n";

			gifi47::vec2 mouse_pos = input::mouse_to_viewport_const(mouse_left_cursor_start_x, mouse_left_cursor_start_y);
				dgv1x1.TrySelect(mouse_pos);
			//std::cout << "CLICK TABLE ON " << mouse_pos.x << " " << mouse_pos.y << "\n";
		}
		

// MOUSE LB START HOLD CALLBACK
		if (left_start_hold){
			left_start_hold = false;
			prevPos = input::mouse_to_viewport_const(mouse_left_cursor_start_x, mouse_left_cursor_start_y);
		}

// MOUSE LB ON HOLD
		if (input::mouse_button_left_hold){
			gifi47::vec2 mouse_pos = input::mouse_to_viewport_const(input::mouse_cursor_x, input::mouse_cursor_y);

			input::currentX += 0.5f * deltaTime * input::screen_scale * (prevPos.x - mouse_pos.x) * 300.0f;
			input::currentY += 0.5f * deltaTime * input::screen_scale * (prevPos.y - mouse_pos.y) * 300.0f;
			
			ST.orth_proj = glm::ortho<float>(input::currentX - input::screen_scale, input::currentX + input::screen_scale, input::currentY - input::screen_scale, input::currentY + input::screen_scale, ST.zNear, ST.zFar);
			ST.ortho_projView = ST.orth_proj * ST.ortho_view;

			prevPos = mouse_pos;
		}

// MOUSE LB END HOLD CALLBACK
		if (left_end_hold){
			left_end_hold = false;
			
		}

// MOUSE RB CLICK CALLBACK
		if (input::mouse_button_right_click){
			input::mouse_button_right_click = false;
			right_click = true;
		}

// MOUSE RB START HOLD CALLBACK
		if (right_start_hold){
			right_start_hold = false;
			prevPosRight = input::mouse_to_viewport_const(input::mouse_cursor_x, input::mouse_cursor_y);
			
		}

// MOUSE RB ON HOLD
		if (input::mouse_button_right_hold){
			gifi47::vec2 mouse_pos = input::mouse_to_viewport_const(input::mouse_cursor_x, input::mouse_cursor_y);
			float ttt= (prevPosRight.y - mouse_pos.y);
			input::screen_scale += -ttt * -scroll_speed * std::min(input::screen_scale, std::max(1.0f, std::log(input::screen_scale * input::screen_scale)));
			ST.orth_proj = glm::ortho<float>(input::currentX - input::screen_scale, input::currentX + input::screen_scale, input::currentY - input::screen_scale, input::currentY + input::screen_scale, ST.zNear, ST.zFar);
			ST.ortho_projView = ST.orth_proj * ST.ortho_view;
			input::mouse_scroll_y = 0;

			editableLabelD.SetValue(std::to_string(input::screen_scale));
		}

// MOUSE RB END HOLD CALLBACK
		if (right_end_hold){
			right_end_hold = false;
		}

// BUTTON ARROW LEFT
		if (input::buttonLeft){
			input::buttonLeft = false;
			block_to_place--;
			if (block_to_place < 1) block_to_place = 0xFF;
			
			chunkMesh = std::vector<float>(0);
			AddCube(chunkMesh, block_to_place, 0, 0, 0, 0.1f);
			cube_to_place.ChangeMesh(chunkMesh);
		}

// BUTTON ARROW RIGHT
		if (input::buttonRight){
			input::buttonRight = false;
			block_to_place++;
			if (block_to_place > 0xFF) block_to_place = 1;

			chunkMesh = std::vector<float>(0);
			AddCube(chunkMesh, block_to_place, 0, 0, 0, 0.1f);
			cube_to_place.ChangeMesh(chunkMesh);
		}

// BUTTON ARROW UP
		if (input::buttonUp && !buttonUpPressed){
			buttonUpPressed = true;
		}

// BUTTON ARROW DOWN
		if (input::buttonDown){
			input::buttonDown = false;
			//world.LoadNearbyChunks(ST.camera.position.x, ST.camera.position.y, ST.camera.position.z, 25, 2, 25);
			world.LoadNearbyChunks(ST.camera.position.x, ST.camera.position.y, ST.camera.position.z, 5, 0, 6);
		}

		int selectedDigit = -1;
		for (int i = 0; i < input::buttonDigit.size(); i++){
			if (input::buttonDigit[i]){
				selectedDigit = i;
				input::buttonDigit[i] = false;
				break;
			}
		}
		if (selectedDigit != -1)
			dgv1x1.AddDigit(selectedDigit);

		if (input::buttonBackspace){
			input::buttonBackspace = false;
			dgv1x1.BackSpace();
		}

		if (input::buttonMinus){
			input::buttonMinus = false;
			world.UnloadHiddenChunks(ST.camera.position.x, ST.camera.position.y, ST.camera.position.z);
			world.UnloadEmptyChunks();
			//dgv1x1.Minus();
		}

		float offsetX = (float)(-prev_mouse_cursor_x + input::mouse_cursor_x);
		prev_mouse_cursor_x = input::mouse_cursor_x;
		float offsetY = (float)(prev_mouse_cursor_y - input::mouse_cursor_y);
		prev_mouse_cursor_y = input::mouse_cursor_y;

		glm::vec3 newpos = ST.camera.position + ST.camera.forward * speed * static_cast<float>(deltaTime) * static_cast<float>(input::forward);
		newpos += ST.camera.right * speed * static_cast<float>(deltaTime) * static_cast<float>(input::side);

		glm::vec3 diffpos = newpos - ST.camera.position;

		bool all_c = CHECK_COLLISION(ST.camera.position, diffpos);

		if (all_c){

			bool x_c = CHECK_COLLISION(ST.camera.position, glm::vec3(diffpos.x, 0, 0));
			if (!x_c){
				ST.camera.position += glm::vec3(diffpos.x, 0, 0);
			}

			bool y_c = CHECK_COLLISION(ST.camera.position, glm::vec3(0, diffpos.y, 0));
			if (!y_c){
				ST.camera.position += glm::vec3(0, diffpos.y, 0);
			}

			bool z_c = CHECK_COLLISION(ST.camera.position, glm::vec3(0, 0, diffpos.z));
			if (!z_c){
				ST.camera.position += glm::vec3(0, 0, diffpos.z);
			}

		} else {
			ST.camera.position += diffpos;
		}
		


		ST.camera.ProcessMouseMovement(offsetX, offsetY);

		RaycastResult result = Raycast(
		    ST.camera.position,  // Позиция глаз игрока
		    ST.camera.forward,  // Направление взгляда
		    10.0f,  // Максимальная дистанция
		    world//chunk  // Чанк, в котором ищем
		);

		if (right_click){
			//right_click = false;
			//update_chunk = true;
			/*rayDrawer.UpdateRay(ST.camera.position, ST.camera.position + ST.camera.forward * 10.0f);
			Raycast2(
				ST.camera.position,  // Позиция глаз игрока
		    	ST.camera.forward,  // Направление взгляда
		    	10.0f,  // Максимальная дистанция
		    	chunk,
		    	blocks
		    );
		    std::cout << result.faceNormal << "\n";*/

		}

		if (buttonUpPressed){
			buttonUpPressed = false;
			if (result.hit){
				block_to_place = world.GetBlock(result.blockPos.x, result.blockPos.y, result.blockPos.z);
				chunkMesh = std::vector<float>(0);
				AddCube(chunkMesh, block_to_place, 0, 0, 0, 0.1f);
				cube_to_place.ChangeMesh(chunkMesh);
			}
		}


		if (result.hit) {
		    editableLabelD.SetValue(std::to_string(result.blockPos.x) + " " + std::to_string(result.blockPos.y) + " " + std::to_string(result.blockPos.z));
		} else {
			editableLabelD.SetValue("FALSE");
		}

		if (update_chunk){
			update_chunk = false;
			/*if (destroy_block){
				destroy_block = false;
				chunk.blocks[destroy_block_pos.x][destroy_block_pos.y][destroy_block_pos.z] = 0;
			}
			if (place_block){
				place_block = false;
				chunk.blocks[place_block_pos.x][place_block_pos.y][place_block_pos.z] = block_to_place;
			}
			chunkMesh = chunk.CreateMesh();
			chunkObj.ChangeMesh(chunkMesh);*/
			world.UpdateChunks();
		}

		//float g_y = deltaTime * 0.5f;
		//if (CheckCollision(chunk, ST.camera.position.x - 0.3, ST.camera.position.y - 1.4 - g_y, ST.camera.position.z - 0.3, ST.camera.position.x + 0.3, ST.camera.position.y + 0.4 - g_y, ST.camera.position.z + 0.3)){
		/*if (CHECK_COLLISION(ST.camera.position, glm::vec3(0, -g_y, 0)))
		} else {
			ST.camera.position.y -= g_y;
		}*/

		ST.view = ST.camera.GetViewMatrix();
		glm::mat4 projView = ST.proj * ST.view;


		//dgv1x1.Render(ST.label_shader, ST.textTex, ST.const_ortho_projView);


		editableLabelD.Render(ST.label_shader, ST.textTex, ST.const_ortho_projView * editableLabelDMatrix);

		glm::mat4 current_pos_matrix = projView * f_mat;
		SimpleObjRenderMVP(tmp_figure, ST.blackTexF, current_pos_matrix);

		//chunkObj.RenderMVP(ST.shaderXYZUV, ST.blockAtlas, projView);
		//chunkObj2.RenderMVP(ST.shaderXYZUV, ST.blockAtlas, projView);

		world.RenderChunks(ST.shaderXYZUVS, ST.blockAtlas, projView);

		for (int i = 0; i < blocks.size(); i++){
			glm::mat4 _model = glm::mat4(1.0f); 
			_model = glm::translate(_model, glm::vec3(blocks[i].x, blocks[i].y, blocks[i].z));
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			temp_cube.RenderMVP(ST.shaderXYZUV_TransparentStripe, ST.whiteHighlightedTex, projView * _model, [_time](Rendering::Shader& shader){
				glUniform1f(glGetUniformLocation(shader.programId, "time"), _time);
			});
			glDisable(GL_BLEND);
		}
		rayDrawer.Draw(ST.shaderRayDebug, glm::vec3(1.0f, 0.0f, 0.0f), projView);
		if (result.hit) {
			glm::mat4 _model = glm::mat4(1.0f);
			_model = glm::translate(_model, glm::vec3(result.blockPos.x, result.blockPos.y, result.blockPos.z));
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//temp_cube.RenderMVP(ST.shaderXYZUV_Transparent, ST.whiteHighlightedTex, projView * _model);
			temp_cube.RenderMVP(ST.shaderXYZUV_TransparentStripe, ST.whiteHighlightedTex, projView * _model, [_time](Rendering::Shader& shader){
				glUniform1f(glGetUniformLocation(shader.programId, "time"), _time);
			});
			glDisable(GL_BLEND);
		}

		{
			//glm::mat4 _model = glm::mat4(1.0f); 
			//_model = glm::translate(_model, ST.camera.position + ST.camera.forward + ST.camera.up * -0.3f + ST.camera.right * 0.32f);

			cube_to_place_model = glm::rotate(cube_to_place_model, glm::radians(std::sin(_time * 0.23f) * 2.9f * static_cast<float>(deltaTime)), glm::vec3(0.0f, 1.0f, 0.0f));
			cube_to_place.RenderMVP(ST.shaderXYZUV, ST.blockAtlas, const_projView * cube_to_place_model);
		}

		if (left_click){
			left_click = false;

			if (result.hit) {
				update_chunk = true;
				destroy_block = true;
				destroy_block_pos = result.blockPos;
				world.SetBlock(0, destroy_block_pos.x, destroy_block_pos.y, destroy_block_pos.z);
			}
		}
		if (right_click){
			right_click = false;

			if (result.hit) {
				update_chunk = true;
				place_block = true;
				place_block_pos = result.blockPos + result.faceNormal;
				world.SetBlock(block_to_place, place_block_pos.x, place_block_pos.y, place_block_pos.z);
			}
		}


		//glEnable();

		//rayDrawer.Draw(ST.shaderRayDebug, glm::vec3(1.0f, 0.0f, 0.0f), projView);

		coordsLabel.SetValue("x" + f_to_s(ST.camera.position.x, 100) + " y" + f_to_s(ST.camera.position.y, 100) + " z" + f_to_s(ST.camera.position.z, 100));
		coordsLabel.Render(ST.label_shader, ST.textTex, ST.const_ortho_projView * coordsLabelMatrix);

		chunksLabel.SetValue("chunks: " + std::to_string(world.chunks.loadedChunks));
		chunksLabel.Render(ST.label_shader, ST.textTex, ST.const_ortho_projView * chunksLabelMatrix);

		fpsLabel.SetValue(std::to_string((int)std::round(1.0 / deltaTime)));
		fpsLabel.Render(ST.label_shader, ST.textTex, ST.const_ortho_projView * fpslabelMatrix);

		_time += 0.02f;

		glfwSwapBuffers(window);

		double currentTime = glfwGetTime();
		deltaTime = currentTime - prevFrameTime;
		prevFrameTime = currentTime;
	}


	world.SaveAllChunks();

/*
	// LABEL SHADER & TEXTURE DISPOSE
	ST.digit_tex.Dispose();
	ST.label_shader.Dispose();

	// SHADERS DISPOSE
	ST.sObjShaderMVP.Dispose();
	ST.outlineShader.Dispose();
	ST.sObjShaderMVPAlpha.Dispose();
	ST.exp_no_tex_Shader.Dispose();

	// TEXTURES DISPOSE

	ST.whiteTex.Dispose();
	ST.whiteTexF.Dispose();
	ST.whiteHighlightedTex.Dispose();
	ST.captionWhiteMoveTex.Dispose();
	ST.captionWhiteWinsTex.Dispose();

	ST.blackTex.Dispose();
	ST.blackTexF.Dispose();
	ST.blackHighlightedTex.Dispose();
	ST.captionBlackMoveTex.Dispose();
	ST.captionBlackWinsTex.Dispose();
*/
	DisposeAll();
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int argc, char* argv[]){
	std::string worldName = "world2";
	if (argc > 1) worldName = argv[1];
	std::cout << "HELLO WORLD: " << worldName;
	//try{
		main_process(worldName);
	/*}catch (...){
	    std::cout << "An exception occurred. Exception Nr. " << '\n';
	    std::exception_ptr p = std::current_exception();
        std::clog << "exception: " <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
	}
*/
    std::cout << "END!";
    return 0;
}