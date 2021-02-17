#pragma once
#include "Angel.h"
#include "mat.h"
#include "vec.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"
#include "SkyBox.h"
#include <vector>
#include <string>
#include "person.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <math.h>



// 相机模式，全局变量
enum MODE {
	FREE,  // 0 自由摄影模式
	CENTER // 1 围绕中心旋转模式
};

MODE CAMERA_MODE = CENTER;

// 窗体长宽
int WIDTH = 600;
int HEIGHT = 600;

// 主窗口标识符
// int mainWindow;
GLFWwindow* mainWindow = nullptr;

// 相机
Camera* camera = new Camera();

// 光源
Light* light = new Light(true);

// openGL绘制方法合集
MeshPainter *painter = new MeshPainter();

// 天空盒
SkyBox* skybox;

// 场景中人物
person* robot;
person* robot2;

TriMesh* p1;  // 恋人
TriMesh* p2;  // 草原公路
TriMesh* p3;  // 屋子1
TriMesh* p4;  // 水边木屋
TriMesh* p5;  // 行道树

// 回收和删除我们创建的物体对象
std::vector<TriMesh *> meshList;

void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
	#ifdef __APPLE__	// for MacOS
		vshader = "shaders/vshader_mac.glsl";
		fshader = "shaders/fshader_mac.glsl";
	#else				// for Windows
		vshader = "shaders/vshader_win.glsl";
		fshader = "shaders/fshader_win.glsl";
	#endif

	// 设置光源位置
	light->setTranslation(vec3(-5, 7, -5));
	light->setAmbient(vec4(0.7, 0.7, 0.7, 1.0)); // 环境光
	light->setDiffuse(vec4(0.85, 0.85, 0.85, 1.0)); // 漫反射
	light->setSpecular(vec4(0.1, 0.1, 0.1, 1.0)); // 镜面反射
	light->setAttenuation(1.0, 0.045, 0.0075); // 衰减系数
	
	// 天空盒
	skybox = new SkyBox();
	skybox->initSkyBox();

	// 恋人
	p1 = new TriMesh(true);
	p1->readObj("./assets/obj2/love/love.obj");
	p1->setTranslation(vec3(3.8, 0.269, -2));
	p1->setRotation(vec3(0, -60, 0));
	p1->setScale(vec3(1, 1, 1));
	painter->addMesh(p1, "love", "./assets/obj2/love/love.png", vshader, fshader);
	meshList.push_back(p1);

	// 草原公路地面
	p2 = new TriMesh(true);
	p2->readObj("./assets/obj2/road/road.obj");
	p2->setTranslation(vec3(0, -0.001, 0));
	p2->setRotation(vec3(0, 0, 0));
	p2->setScale(vec3(200, 200, 200));
	painter->addMesh(p2, "road", "./assets/obj2/road/road.png", vshader, fshader);
	meshList.push_back(p2);

	// 屋子1
	p3 = new TriMesh(true);
	p3->readObj("./assets/obj2/house1/house.obj");
	/*p3->setTranslation(vec3(0, 2.13, -5));*/
	p3->setTranslation(vec3(0, 2.13, -6));
	p3->setRotation(vec3(0, 90, 0));
	p3->setScale(vec3(8, 8, 8));
	painter->addMesh(p3, "house1", "./assets/obj2/house1/house.png", vshader, fshader);
	meshList.push_back(p3);

	// 水边木屋
	p4 = new TriMesh(true);
	p4->readObj("./assets/obj2/house2/woodhouse.obj");
	p4->setTranslation(vec3(2, 1.6, -1.9));
	p4->setRotation(vec3(0, 30, 0));
	p4->setScale(vec3(7, 7, 7));
	painter->addMesh(p4, "house2", "./assets/obj2/house2/woodhouse.png", vshader, fshader);
	meshList.push_back(p4);

	// 行道树
	p5 = new TriMesh(true);
	p5->readObj("./assets/obj2/tree/tree.obj");
	p5->setTranslation(vec3(-17.4653, 2.15, 7.14257));
	p5->setRotation(vec3(0, 45, 0));
	p5->setScale(vec3(8, 8, 8));
	painter->addMesh(p5, "tree", "./assets/obj2/tree/tree.png", vshader, fshader);
	meshList.push_back(p5);

	// 对模型设置明暗绘制系数
	p1->setAmbient(vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 环境光
	p1->setDiffuse(vec4(0.1f, 0.0f, 0.1f, 1.0f));    // 漫反射
	p1->setSpecular(vec4(0.7f, 0.6f, 0.6f, 1.0f));   // 镜面反射
	p1->setShininess(3.0);                           //高光系数

	p2->setAmbient(vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 环境光
	p2->setDiffuse(vec4(0.02f, 0.02f, 0.02f, 1.0f));    // 漫反射
	p2->setSpecular(vec4(0, 0, 0, 1.0f));   // 镜面反射
	p2->setShininess(3.0);                           //高光系数

	p3->setAmbient(vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 环境光
	p3->setDiffuse(vec4(0.1f, 0.0f, 0.1f, 1.0f));    // 漫反射
	p3->setSpecular(vec4(0.7f, 0.6f, 0.6f, 1.0f));   // 镜面反射
	p3->setShininess(3.0);                           //高光系数

	p4->setAmbient(vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 环境光
	p4->setDiffuse(vec4(0.1f, 0.0f, 0.1f, 1.0f));    // 漫反射
	p4->setSpecular(vec4(0.7f, 0.6f, 0.6f, 1.0f));   // 镜面反射
	p4->setShininess(3.0);                           //高光系数

	p5->setAmbient(vec4(0.1f, 0.1f, 0.1f, 1.0f)); // 环境光
	p5->setDiffuse(vec4(0.1f, 0.0f, 0.1f, 1.0f));    // 漫反射
	p5->setSpecular(vec4(0.7f, 0.6f, 0.6f, 1.0f));   // 镜面反射
	p5->setShininess(3.0);                           //高光系数

	// 场景人物
	robot = new person();
	robot->init1();

	robot2 = new person();
	robot2->init2();

	// 设置人物材质的高光系数
	robot->setShininess(3.0);                           //高光系数
	robot2->setShininess(3.0);                          //高光系数
}



void display()
{
	// #ifdef __APPLE__ // 解决 macOS 10.15 显示画面缩小问题
	// 	glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
	// #endif
	// glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 绘制场景中的TriMesh
	painter->drawMeshes(light, camera);

	// 绘制场景人物
	robot->display_person(camera, light);
	robot2->display_person(camera, light);

	// 最后绘制天空盒
	skybox->drawSkyBox(HEIGHT, WIDTH, camera);
	// glutSwapBuffers();

	glfwSwapBuffers(mainWindow);
	// std::cout << camera->eye << std::endl;
	// std::cout << camera->at << std::endl;
}


void printHelp()
{
	std::cout << "=====================================================================" << std::endl;
	std::cout << "               计算机图形学_Keep Running With Friends" << std::endl;
	std::cout << "=====================================================================" << std::endl << std::endl;

	std::cout << "<GLFW框架下，实验机器启动Release版本的时间约3秒>" << std::endl << std::endl;	

	std::cout << "使用方法：" << std::endl;
	std::cout <<
		"[窗口]" << std::endl <<
		"ESC:		Exit" << std::endl <<
		"p:		Print help message" << std::endl <<

		std::endl <<
		"[相机]" << std::endl <<
		"c:		切换相机观察模式(两种模式：TPS/FPS)，下面使用需要切换到相应模式" << std::endl <<
		"w:		FPS模式下，向前移动相机" << std::endl <<
		"s:		FPS模式下，向后移动相机" << std::endl <<
		"a:		FPS模式下，向左移动相机" << std::endl <<
		"d:		FPS模式下，向右移动相机" << std::endl <<
		"u/j: 		TPS模式下，水平顺逆时针移动相机，360度全景浏览" << std::endl <<
		"i/k: 		TPS模式下，垂轴顺逆时针移动相机，限定地平面以上浏览" << std::endl <<
		"o/l: 		TPS模式下，控制相机拍摄半径" << std::endl <<
		"鼠标(左键)移动:	TPS模式下，水平方向旋转相机" << std::endl <<
		"鼠标右键拖拽:	TPS模式下，铅直方向旋转相机" << std::endl <<

		std::endl << 
		"[人物]" << std::endl <<
		"↑:		TPS模式下，两个人物协同跑步前进" << std::endl <<
		"↓:		TPS模式下，两个人物协同俯倾前进" << std::endl <<
		"←:		TPS模式下，两个人物协同左转向" << std::endl <<
		"→:		TPS模式下，两个人物协同右转向" << std::endl <<
		"h:		TPS模式下，2号人物跑步前进" << std::endl <<
		"n:		TPS模式下，2号人物俯倾前进" << std::endl <<
		"b:		TPS模式下，2号人物左转向" << std::endl <<
		"m:		TPS模式下，2号人物右转向" << std::endl <<
		"空格键:		TPS模式下，2号人物坐下" << std::endl <<

		std::endl <<
		"[静态模型]" << std::endl <<
		"+/-:		控制场景中两个模型的缩放(不建议过度放大/缩小，因为会导致界面不美观)" << std::endl <<
		"中央回车键:	复原场景中两个模型的大小" << std::endl <<

		std::endl <<
		"[背景]" << std::endl <<
		"1~5:		切换天空盒背景" << std::endl <<

		std::endl << std::endl;
}

static int BUTTON = 0;
// 辅助鼠标函数
void _mouse(GLFWwindow* window, int button, int action, int mods) {
	BUTTON = button;
}

// 鼠标响应函数
void mouse(GLFWwindow* window, double x, double y)
{
	// 左键控制偏航
	if (BUTTON == GLFW_MOUSE_BUTTON_LEFT)
		camera->mouse(x, y, 1);
	// 右键控制俯仰
	if (BUTTON == GLFW_MOUSE_BUTTON_RIGHT)
		camera->mouse(x, y, 2);
}

// 滑轮控制光源位置
void mouseWheel(GLFWwindow* window, double x, double y)
{
	vec3 pos = light->getTranslation();
	if (y > 0) {
		pos.z += 0.1;
	}
	else {
		pos.z -= 0.1;
	}
	light->setTranslation(pos);
}

// 键盘响应函数
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key)
		{
			// ESC键退出游戏
			case GLFW_KEY_ESCAPE:
				exit(EXIT_SUCCESS);
				return;
			// 打印提示信息
			case GLFW_KEY_P:
				printHelp();
				return;
			// 切换相机状态
			case GLFW_KEY_C:
				camera->isFree = !camera->isFree;
				return;
			// 1-6 更改天空盒背景
			case GLFW_KEY_1:
			case GLFW_KEY_2:
			case GLFW_KEY_3:
			case GLFW_KEY_4:
			case GLFW_KEY_5:
				skybox->keyboard(key);
				return;
		}
	}
	 bool tag = false;
	 person* tmp = nullptr;
	// 2号自主运动
	if (key == GLFW_KEY_B || key == GLFW_KEY_N || key == GLFW_KEY_M) {
		robot2->keyboard(key, camera, 1);
		// 2号人物不在坐的状态，设置状态位为0
		robot2->isSitDown = false;
		tag = true;
		tmp = robot2;
	}
	// 1号、2号人物协同运动
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
		// 统一
		robot2->direction = robot->direction;
		// 1号人物跑路向前，俯冲向前，左转，右转 响应
		robot->keyboard(key, camera, 1);
		// 1号人物前进的时候，2号人物跟在旁边一同前进（协同运动）
		robot2->setPos(vec3(robot->Pos.x, robot->Pos.y, robot->Pos.z+0.8));
		// 2号人物的动作响应与1号人物相同
		robot2->keyboard(key, camera, 1);
		// 2号人物不在坐的状态，设置状态位为0
		robot2->isSitDown = false;
		tag = true;
		tmp = robot;
	}
	if (tag) {
		// 固定模式即TPS模式（跟在人物后面）
		if (!camera->isFree) {
			// 根据人物位置反推相机位置
			camera->eye = tmp->Pos - tmp->camera_person_distance * vec3(
				cos(tmp->direction * M_PI / 180), 0, -sin(tmp->direction * M_PI / 180));
			// 相机视点位置是人物位置，这样可以保持相对静止
			camera->at = tmp->Pos;
			// camera->at.y = 0.46f;
		}
		// 在FPS模式下，每次相机视角都递变0.2
		if (!camera->isFree && (key == GLFW_KEY_UP || key == GLFW_KEY_H) ) {
			camera->zoom += 0.2;
			// 但最大不超过1.5f
			if (camera->zoom > 1.5f) {
				camera->zoom = 1.5f;
			}
		}
	}
	// 如果是按下空格键，2号人物坐下
	if (key == GLFW_KEY_SPACE) {
		robot2->setSitDown();
		robot2->isSitDown = true;
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_W || 
		key == GLFW_KEY_U || key == GLFW_KEY_I || key == GLFW_KEY_O || key == GLFW_KEY_J ||
		key == GLFW_KEY_K || key == GLFW_KEY_L) {
		// 相机的移动 响应
		camera->keyboard(key);
	}
	if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_KP_SUBTRACT) {
		p1->keyboard(key);
		p3->keyboard(key);
	}
	if (key == GLFW_KEY_ENTER) {
		p1->setScale(vec3(1, 1, 1));
		p3->setScale(vec3(8, 8, 8));
	}
}

void handleInput() {
	bool tag = false;
	person* tmp = nullptr;
	int key = 0;
	const double rate = 0.30;
	// 2号自主运动
	if (glfwGetKey(mainWindow, GLFW_KEY_H) == GLFW_PRESS) {
		robot2->keyboard(GLFW_KEY_H, camera, rate);
		key = GLFW_KEY_H;
		if (glfwGetKey(mainWindow, GLFW_KEY_B) == GLFW_PRESS) {
			robot2->keyboard(GLFW_KEY_B, camera, rate);
		}
		if (glfwGetKey(mainWindow, GLFW_KEY_M) == GLFW_PRESS) {
			robot2->keyboard(GLFW_KEY_M, camera, rate);
		}
		// 2号人物不在坐的状态，设置状态位为0
		robot2->isSitDown = false;
		tag = true;
		tmp = robot2;
	}
	// 1号、2号人物协同运动
	if (glfwGetKey(mainWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		// 统一
		robot2->direction = robot->direction;
		// 1号人物跑路向前，俯冲向前，左转，右转 响应
		robot->keyboard(GLFW_KEY_UP, camera, rate);
		// 1号人物前进的时候，2号人物跟在旁边一同前进（协同运动）
		robot2->setPos(vec3(robot->Pos.x, robot->Pos.y, robot->Pos.z + 0.8));
		// 2号人物的动作响应与1号人物相同
		robot2->keyboard(GLFW_KEY_UP, camera, rate);
		if (glfwGetKey(mainWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
			// 统一
			robot2->direction = robot->direction;
			// 1号人物跑路向前，俯冲向前，左转，右转 响应
			robot->keyboard(GLFW_KEY_LEFT, camera, rate);
			// 1号人物前进的时候，2号人物跟在旁边一同前进（协同运动）
			robot2->setPos(vec3(robot->Pos.x, robot->Pos.y, robot->Pos.z + 0.8));
			// 2号人物的动作响应与1号人物相同
			robot2->keyboard(GLFW_KEY_LEFT, camera, rate);
		}
		if (glfwGetKey(mainWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			// 统一
			robot2->direction = robot->direction;
			// 1号人物跑路向前，俯冲向前，左转，右转 响应
			robot->keyboard(GLFW_KEY_RIGHT, camera, rate);
			// 1号人物前进的时候，2号人物跟在旁边一同前进（协同运动）
			robot2->setPos(vec3(robot->Pos.x, robot->Pos.y, robot->Pos.z + 0.8));
			// 2号人物的动作响应与1号人物相同
			robot2->keyboard(GLFW_KEY_RIGHT, camera, rate);
		}
		// 2号人物不在坐的状态，设置状态位为0
		robot2->isSitDown = false;
		tag = true;
		tmp = robot;
	}
	if (tag) {
		// 固定模式即TPS模式（跟在人物后面）
		if (!camera->isFree) {
			// 根据人物位置反推相机位置
			camera->eye = tmp->Pos - tmp->camera_person_distance * vec3(
				cos(tmp->direction * M_PI / 180), 0, -sin(tmp->direction * M_PI / 180));
			// 相机视点位置是人物位置，这样可以保持相对静止
			camera->at = tmp->Pos;
			// camera->at.y = 0.46f;
		}
		// 在FPS模式下，每次相机视角都递变0.2
		if (!camera->isFree && (key == GLFW_KEY_UP || key == GLFW_KEY_H)) {
			camera->zoom += 0.2;
			// 但最大不超过1.5f
			if (camera->zoom > 1.5f) {
				camera->zoom = 1.5f;
			}
		}
	}
}

void cleanData() {
	// 释放内存
	
	delete camera;
	camera = NULL;

	delete light;
	light = NULL;

	painter->cleanMeshes();

	delete painter;
	painter = NULL;
	
	for (int i=0; i<meshList.size(); i++) {
		delete meshList[i];
	}
	meshList.clear();
}

void resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
	// 使控制台正常显示中文
	system("chcp 65001 && cls");
	// #pragma execution_character_set("gbk")

	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Keep_Running_With_Friends", nullptr, nullptr);
	glfwMakeContextCurrent(mainWindow);
	
	#ifdef __APPLE__
	#else
		glewExperimental = GL_TRUE;
		glewInit();
	#endif
	// 可变化窗口大小
	glfwSetFramebufferSizeCallback(mainWindow, resize);
	
	init();

	// 注册响应
	glfwSetKeyCallback(mainWindow, keyboard);
	glfwSetScrollCallback(mainWindow, mouseWheel);
	glfwSetMouseButtonCallback(mainWindow, _mouse);
	glfwSetCursorPosCallback(mainWindow, mouse);
	// glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 开局旋转动画
	double init_time = glfwGetTime();
	while (glfwGetTime() < init_time + 3) {
		camera->animation(2);
		display();
	}

	// srand((unsigned)time(NULL));
	while (!glfwWindowShouldClose(mainWindow))
	{
		handleInput();
		// 人物从俯倾恢复跑路
		if (glfwGetKey(mainWindow, GLFW_KEY_DOWN) == GLFW_RELEASE) {
			// 1号人物
			robot->Pos.y = 0.18f;
			if (robot->isBeginRunning == true) {
				robot->resetTheta0();
			}
			robot->downTheta = 0;

			// 2号人物
			if (!robot2->isSitDown) {
				// 没继续前进，先恢复跑路姿势
				if (robot2->isBeginRunning == false && glfwGetKey(mainWindow, GLFW_KEY_UP) == GLFW_RELEASE && 
					glfwGetKey(mainWindow, GLFW_KEY_H) == GLFW_RELEASE && glfwGetKey(mainWindow, GLFW_KEY_N) == GLFW_RELEASE) {
					robot2->resetTheta0();
					robot2->downTheta = 0;
					robot2->Pos.y = 0.18f;
				}
			}
		}
		// 2号人物从俯倾恢复跑路
		if (glfwGetKey(mainWindow, GLFW_KEY_N) == GLFW_RELEASE) {
			// 2号人物
			if (robot2->isBeginRunning == true && !robot->isSitDown) {
				robot2->resetTheta0();
				robot2->downTheta = 0;
				robot2->Pos.y = 0.18f;
			}
		}

		// 两个人物停止运动，所有关节角都归零
		if (glfwGetKey(mainWindow, GLFW_KEY_UP) == GLFW_RELEASE
			&& glfwGetKey(mainWindow, GLFW_KEY_LEFT) == GLFW_RELEASE
			&& glfwGetKey(mainWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE
			&& glfwGetKey(mainWindow, GLFW_KEY_DOWN) == GLFW_RELEASE) {
			// 关节角都恢复为0
			robot->resetTheta0();
			// 人物运动状态处于就绪状态（就是还没跑动）
			robot -> isBeginRunning = true;
			robot->Pos.y = 0.18f;
		}
		// 2号人物停止运动，所有关节角都归零
		if (glfwGetKey(mainWindow, GLFW_KEY_B) == GLFW_RELEASE
			&& glfwGetKey(mainWindow, GLFW_KEY_H) == GLFW_RELEASE
			&& glfwGetKey(mainWindow, GLFW_KEY_N) == GLFW_RELEASE
			&& glfwGetKey(mainWindow, GLFW_KEY_M) == GLFW_RELEASE) {
			if (!robot2->isSitDown && robot2->isBeginRunning) {
				// 关节角都恢复为0
				robot2->resetTheta0();
				// 人物运动状态处于就绪状态（就是还没跑动）
				robot2->isBeginRunning = true;
				robot2->Pos.y = 0.18f;
			}
		}
		// 视野变化
		if (glfwGetKey(mainWindow, GLFW_KEY_UP) == GLFW_RELEASE && 
			glfwGetKey(mainWindow, GLFW_KEY_H) == GLFW_RELEASE) {
			// FPS模式下复原
			if (!camera->isFree) {
				// 每次相机视角都递变0.1
				camera->zoom -= 0.1;
				// 但最大不少于1.0f
				if (camera->zoom < 1.0f) {
					camera->zoom = 1.0f;
				}
			}
		}
		display();
		glfwPollEvents();
	}

	cleanData();
	glfwTerminate();

	return 0;
}
