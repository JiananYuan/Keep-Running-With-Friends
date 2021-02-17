#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Angel.h"

class Camera
{
public:
	Camera();
	~Camera();

	mat4 getViewMatrix();
	mat4 getProjectionMatrix( bool isOrtho);

	mat4 lookAt(const vec4& eye, const vec4& at, const vec4& up);

	mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	mat4 perspective(const GLfloat fovy, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar);

	mat4 frustum(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	// 每次更改相机参数后更新一下相关的数值
	void updateCamera();
	// 处理相机的键盘操作
	void keyboard(int key);
	// 处理相机的鼠标操作
	void mouse(double x, double y, int op);

	void animation(int op);

	void reset();

	// 模视矩阵
	mat4 viewMatrix;
	mat4 projMatrix;

	// 相机位置参数
	float radius = 3.0f;
	float rotateAngle = 0.0;
	float upAngle = 0.0;
	vec4 eye;
	vec4 at;
	vec4 up;

	// 投影参数
	#undef near
	#undef far
	// float near = 0.1;
	float near = 0.01;
	float far = 100.0;
	// 透视投影参数
	float fov = 45.0;
	float aspect = 1.0;
	// 正交投影参数
	float scale = 1.5;

	// 自由移动 初始关闭
	bool isFree;
	// 绘制如果是天空盒子则设为true
	bool isSkyBox = false;

	vec4 front;
	// 鼠标控制方向
	float lastX = 0, lastY = 0;
	float yaw = 0;  // 偏航角
	float pitch; // 俯仰角 
	bool firstMouse = true;
	float direction = 0;

	// 用于处理人物运动时的随机抖动
	float rand_vibration = 1.0f;
	// 运动引起的视角变化
	float zoom = 1.0f;
};
#endif
