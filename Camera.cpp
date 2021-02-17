#include "Camera.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

float min(float _x, float _y) {
	return _x < _y ? _x : _y;
}

float max(float _x, float _y) {
	return _x > _y ? _x : _y;
}


Camera::Camera()
{ 
	// 初始摄像机位置
	eye = vec4(4.90028, 1, -7.40046, 1);
	// 一开始视点指向原点
	at = vec4(4.64146, 1, -6.43454, 1);
	up = vec4(0, 1, 0, 0);
	// front向量，用于控制第一人称视角观察  参考：https://learnopengl.com/Getting-started/Camera
	front = vec4(0, 0, -1, 0);
	// isFree变量指示当前相机是否跟在人物身后
	isFree = false;
	// updateCamera();
};
Camera::~Camera() {}

mat4 Camera::getViewMatrix()
{
	if (isFree) {
		// 如果是第一人称模式，
		// 根据视点与摄像机位置保持相对静止的关系，
		// 需要将视点需要根据eye的位置做改变
		at = eye + front;
	}
	return this->lookAt(eye, at, up);
}

mat4 Camera::getProjectionMatrix(bool isOrtho)
{
	if (isOrtho) {
		// 返回正交投影矩阵
		return this->ortho(-scale, scale, -scale, scale, this->near, this->far);
	}
	else {
		// 返回透视投影矩阵
		// return this->perspective(fov*20, aspect, this->near, this->far);
		return this->perspective(zoom*fov, aspect, this->near, this->far);
	}
}

mat4 Camera::lookAt(const vec4& eye, const vec4& at, const vec4& up)
{
	// 相机观察矩阵的计算
	vec4 n = normalize(eye - at);					// normalize():归一化函数
	vec4 u = normalize(vec4(cross(up, n), 0.0));	// cross()计算向量积
	vec4 v = normalize(vec4(cross(n, u), 0.0));
	vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
	mat4 c = mat4(u, v, n, t);
	if (isSkyBox)
		// 如果是天空盒，需要移除位移部分
		return c;
	else
		return c * Translate(-eye);						// 计算最后需要沿-eye方向平移
}

mat4 Camera::ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// 正交投影矩阵的计算
	mat4 c;
	c[0][0] = 2.0 / (right - left);
	c[1][1] = 2.0 / (top - bottom);
	c[2][2] = -2.0 / (zFar - zNear);
	c[3][3] = 1.0;
	c[0][3] = -(right + left) / (right - left);
	c[1][3] = -(top + bottom) / (top - bottom);
	c[2][3] = -(zFar + zNear) / (zFar - zNear);
	return c;
}

mat4 Camera::perspective(const GLfloat fovy, const GLfloat aspect,
	const GLfloat zNear, const GLfloat zFar)
{
	// 透视投影矩阵的计算
	GLfloat top = tan(fovy * M_PI / 180 / 2) * zNear;
	GLfloat right = top * aspect;

	mat4 c;
	c[0][0] = zNear / right;
	c[1][1] = zNear / top;
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -(2.0 * zFar * zNear) / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	return c;
}

mat4 Camera::frustum(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// 任意视锥体矩阵
	mat4 c;
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	return c;
}

 // 重置相机参数
void Camera::reset() {
	radius = 2.0;
	rotateAngle = 0.0;
	upAngle = 0.0;
	fov = 45.0;
	aspect = 1.0;
	scale = 1.5;
	at = vec4(0, 0, 0, 1);
}


void Camera::updateCamera()
{
	// 设置相机位置和方向
	if (!isFree) {
		float eyex, eyey, eyez;
		eyex = radius * cos(upAngle * M_PI / 180.0) * sin(rotateAngle * M_PI / 180.0) + at.x;
		eyey = radius * sin(upAngle * M_PI / 180.0) + at.y;
		eyez = radius * cos(upAngle * M_PI / 180.0) * cos(rotateAngle * M_PI / 180.0) + at.z;
		eye = vec4(eyex, eyey, eyez, 1.0);
	}
}

void Camera::animation(int op) {
	// 铅锤观察
	if (op == 1) {
		rotateAngle = 0;
		upAngle += 3;
	}
	if (op == 2) {
		upAngle = 180;
		rotateAngle += 1;
	}
	updateCamera();
}


// 键盘事件处理
void Camera::keyboard(int key)
{
	float cameraSpeed = 0.2f; // adjust accordingly
	// FPS相机向左圆周观察
	if (key == GLFW_KEY_A) {
		if (isFree) {
			lastX = direction;
			direction += 5;
			float xoffset = direction - lastX;
			float sensitivity = 0.5;
			xoffset *= sensitivity;
			yaw -= xoffset;
			front.x = cos(yaw * M_PI / 180.0);
			front.y = 0;
			front.z = sin(yaw * M_PI / 180.0);
			front = normalize(front);
		}
	}
	// FPS相机后退
	if (key == GLFW_KEY_S) {
		if (isFree)
			eye -= cameraSpeed * front;
	}
	// FPS相机前进
	if (key == GLFW_KEY_W) {
		if (isFree)
			eye += cameraSpeed * front;
	}
	// FPS相机向右圆周观察
	if (key == GLFW_KEY_D) {
		if (isFree) {
			// eye += normalize(cross(front, up)) * cameraSpeed;
			lastX = direction;
			direction -= 5;
			float xoffset = direction - lastX;
			float sensitivity = 0.5;
			xoffset *= sensitivity;
			yaw -= xoffset;
			front.x = cos(yaw * M_PI / 180.0);
			front.y = 0;
			front.z = sin(yaw * M_PI / 180.0);
			front = normalize(front);
		}
	}
	// 水平环绕观察
	if (key == GLFW_KEY_U) {
		if (!isFree) {
			rotateAngle += 5.0;
			updateCamera();
		}
	}
	// 水平环绕观察 反方向
	if (key == GLFW_KEY_J) {
		if (!isFree) {
			rotateAngle -= 5.0;
			updateCamera();
		}
	}
	// 垂直环绕观察
	if (key == GLFW_KEY_I) {
		if (!isFree) {
			upAngle += 5.0;
			upAngle = min(180.0, upAngle);
			updateCamera();
		}
	}
	// 垂直环绕观察 反方向
	if (key == GLFW_KEY_K) {
		if (!isFree) {
			upAngle -= 5.0;
			upAngle = max(0.0, upAngle);
			updateCamera();
		}
	}
	// 拉远
	if (key == GLFW_KEY_O) {
		if (!isFree) {
			radius += 0.1;
			updateCamera();
		}
	}
	// 拉近
	if (key == GLFW_KEY_L) {
		if (!isFree) {
			radius -= 0.1;
			updateCamera();
		}
	}
	///*case GLFW_KEY_SPACE:
	//	radius = 2.0;
	//	rotateAngle = 0.0;
	//	upAngle = 0.0;
	//	fov = 45.0;
	//	aspect = 1.0;
	//	scale = 1.5;
	//	eye = vec4(0, 0, 2, 1);
	//	at = vec4(0, 0, 0, 1);
	//	up = vec4(0,1,0,0);
	//	break;
}

void Camera::mouse(double xpos, double ypos, int op) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	// 设置变化灵敏度
	float sensitivity = 0.5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw -= xoffset;
	pitch += yoffset;

	// 俯仰角不要超过30度
	if (pitch > 30.0f)
		pitch = 30.0f;
	if (pitch < -30.0f)
		pitch = -30.0f;
	// 如果是偏航，则不要俯仰，否则效果不好看
	if (op == 1)
		pitch = 0;

	front.x = cos(yaw * M_PI / 180.0) * cos(pitch * M_PI / 180.0);
	front.y = -sin(pitch * M_PI / 180.0);
	front.z = sin(yaw * M_PI / 180.0) * cos(pitch * M_PI / 180.0);
	front = normalize(front);
}
