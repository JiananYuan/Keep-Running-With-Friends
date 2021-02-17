#pragma once
#include <assert.h>
#include <mat.h>
# include "TriMesh.h"
# include "Camera.h"

#define White	vec3(1.0, 1.0, 1.0)
#define Yellow	vec3(1.0, 1.0, 0.0)
#define Green	vec3(0.0, 1.0, 0.0)
#define Cyan	vec3(0.0, 1.0, 1.0)
#define Magenta	vec3(1.0, 0.0, 1.0)
#define Red		vec3(1.0, 0.0, 0.0)
#define Black	vec3(0.0, 0.0, 0.0)
#define Blue	vec3(0.0, 0.0, 1.0)
#define Brown	vec3(0.5, 0.5, 0.5)

class MatrixStack {
	int		_index;  // 栈顶编号
	int		_size;   // 栈大小
	mat4* _matrices; // 数据

public:
	MatrixStack(int numMatrices = 100) :_index(0), _size(numMatrices) {
		_matrices = new mat4[numMatrices];
	}
	~MatrixStack() {
		delete[]_matrices;
	}
	// 入栈
	void push(const mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;
	}
	// 出栈
	mat4& pop() {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};

struct openGLObject_person
{
    // 顶点数组对象
    GLuint vao;
    // 顶点缓存对象
    GLuint vbo;

    // 着色器程序
    GLuint program;
    // 着色器文件
    std::string vshader;
    std::string fshader;
    // 着色器变量
    GLuint pLocation;
    GLuint cLocation;
    GLuint nLocation;
    GLuint tLocation;

    // 纹理
    std::string texture_image;
    GLuint texture;

    // 投影变换变量
    GLuint modelLocation;
    GLuint viewLocation;
    GLuint projectionLocation;

    // 阴影变量
    GLuint shadowLocation;
};

class person {
public:
	// 关节大小
	/*float TORSO_HEIGHT = 0.4;
	float TORSO_WIDTH = 0.3;
	float UPPER_ARM_HEIGHT = 0.2;
	float LOWER_ARM_HEIGHT = 0.2;
	float UPPER_ARM_WIDTH = 0.13;
	float LOWER_ARM_WIDTH = 0.1;
	float UPPER_LEG_HEIGHT = 0.2;
	float LOWER_LEG_HEIGHT = 0.3;
	float UPPER_LEG_WIDTH = 0.1;
	float LOWER_LEG_WIDTH = 0.1;
	float HEAD_HEIGHT = 0.2;
	float HEAD_WIDTH = 0.2;*/
	float TORSO_HEIGHT = 0.2;
	float TORSO_WIDTH = 0.15;
	float UPPER_ARM_HEIGHT = 0.1;
	float LOWER_ARM_HEIGHT = 0.1;
	float UPPER_ARM_WIDTH = 0.065;
	float LOWER_ARM_WIDTH = 0.05;
	float UPPER_LEG_HEIGHT = 0.1;
	float LOWER_LEG_HEIGHT = 0.1;
	float UPPER_LEG_WIDTH = 0.05;
	float LOWER_LEG_WIDTH = 0.05;
	float HEAD_HEIGHT = 0.1;
	float HEAD_WIDTH = 0.1;

	
	
	int op_Torso = 0;			// 躯干
	int op_Head = 1;			// 头部
	int op_RightUpperArm = 2;	// 右大臂
	int op_RightLowerArm = 3;	// 右小臂
	int op_LeftUpperArm = 4;	// 左大臂
	int op_LeftLowerArm = 5;	// 左小臂
	int op_RightUpperLeg = 6;	// 右大腿
	int op_RightLowerLeg = 7;	// 右小腿
	int op_LeftUpperLeg = 8;	// 左大腿
	int op_LeftLowerLeg = 9;	// 左小腿

	// 步态
	int step = 5;
	float tmp1 = 0, tmp2 = 0;
	// 当前角色方向
	float direction = 0; // 当前人物朝向方位角(与Z轴负方向夹角)
	// (cos, 0, sin)即为前进方向
	// 位移量（以原点为参考点）
	vec3 Pos = vec3(0, 0, 0);
	vec3 front = vec3(0, 0, -1);
	float speed = 0.10f;
	float lastX = 0;
	float yaw = 0;
	float camera_person_distance = 3;

	// 人物俯冲角
	float downTheta = 0;
	// 人物正在奔跑的状态变量
	bool isBeginRunning = true;
	bool isSitDown = true;
	

	// 关节角大小
	GLfloat theta[10] = {
		0.0,    // Torso
		0.0,    // Head
		0.0,    // RightUpperArm
		70.0,    // RightLowerArm
		0.0,    // LeftUpperArm
		70.0,    // LeftLowerArm
		0.0,    // RightUpperLeg
		0.0,    // RightLowerLeg
		0.0,    // LeftUpperLeg
		0.0     // LeftLowerLeg
	};
	// 关节角快照
	GLfloat tmp_theta[10];

	TriMesh* Torso;
	TriMesh* Head;
	TriMesh* RightUpperArm;
	TriMesh* RightLowerArm;
	TriMesh* LeftUpperArm;
	TriMesh* LeftLowerArm;
	TriMesh* RightUpperLeg;
	TriMesh* RightLowerLeg;
	TriMesh* LeftUpperLeg;
	TriMesh* LeftLowerLeg;
	openGLObject_person TorsoObject;
	openGLObject_person HeadObject;
	openGLObject_person RightUpperArmObject;
	openGLObject_person RightLowerArmObject;
	openGLObject_person LeftUpperArmObject;
	openGLObject_person LeftLowerArmObject;
	openGLObject_person RightUpperLegObject;
	openGLObject_person RightLowerLegObject;
	openGLObject_person LeftUpperLegObject;
	openGLObject_person LeftLowerLegObject;

	person();
	~person();
	void drawPerson(mat4 modelMatrix, TriMesh* mesh, Camera* camera, Light* light, openGLObject_person object);
	void init1();
	void init2();
	void display_person(Camera* camera, Light* light);
	void torso(mat4 modelMatrix, Camera* camera, Light* light);
	void head(mat4 modelMatrix, Camera* camera, Light* light);
	void left_upper_arm(mat4 modelMatrix, Camera* camera, Light* light);
	void left_lower_arm(mat4 modelMatrix, Camera* camera, Light* light);
	void right_upper_arm(mat4 modelMatrix, Camera* camera, Light* light);
	void right_lower_arm(mat4 modelMatrix, Camera* camera, Light* light);
	void left_upper_leg(mat4 modelMatrix, Camera* camera, Light* light);
	void left_lower_leg(mat4 modelMatrix, Camera* camera, Light* light);
	void right_upper_leg(mat4 modelMatrix, Camera* camera, Light* light);
	void right_lower_leg(mat4 modelMatrix, Camera* camera, Light* light);
	void bindObjectAndData(TriMesh* mesh, openGLObject_person& object, std::string texture_image, const std::string& vshader, const std::string& fshader);
	void bindLightAndMaterial(TriMesh* mesh, openGLObject_person& object, Light* light, Camera* camera);
	void keyboard(int key, Camera* camera, double rate);
	void setShininess(float);
	void setPos(vec3);
	void recordTheta();
	void resetTheta();
	void resetTheta0();
	void initTheta();
	void setSitDown();
};