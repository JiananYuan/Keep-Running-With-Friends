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
	// ��ʼ�����λ��
	eye = vec4(4.90028, 1, -7.40046, 1);
	// һ��ʼ�ӵ�ָ��ԭ��
	at = vec4(4.64146, 1, -6.43454, 1);
	up = vec4(0, 1, 0, 0);
	// front���������ڿ��Ƶ�һ�˳��ӽǹ۲�  �ο���https://learnopengl.com/Getting-started/Camera
	front = vec4(0, 0, -1, 0);
	// isFree����ָʾ��ǰ����Ƿ�����������
	isFree = false;
	// updateCamera();
};
Camera::~Camera() {}

mat4 Camera::getViewMatrix()
{
	if (isFree) {
		// ����ǵ�һ�˳�ģʽ��
		// �����ӵ��������λ�ñ�����Ծ�ֹ�Ĺ�ϵ��
		// ��Ҫ���ӵ���Ҫ����eye��λ�����ı�
		at = eye + front;
	}
	return this->lookAt(eye, at, up);
}

mat4 Camera::getProjectionMatrix(bool isOrtho)
{
	if (isOrtho) {
		// ��������ͶӰ����
		return this->ortho(-scale, scale, -scale, scale, this->near, this->far);
	}
	else {
		// ����͸��ͶӰ����
		// return this->perspective(fov*20, aspect, this->near, this->far);
		return this->perspective(zoom*fov, aspect, this->near, this->far);
	}
}

mat4 Camera::lookAt(const vec4& eye, const vec4& at, const vec4& up)
{
	// ����۲����ļ���
	vec4 n = normalize(eye - at);					// normalize():��һ������
	vec4 u = normalize(vec4(cross(up, n), 0.0));	// cross()����������
	vec4 v = normalize(vec4(cross(n, u), 0.0));
	vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
	mat4 c = mat4(u, v, n, t);
	if (isSkyBox)
		// �������պУ���Ҫ�Ƴ�λ�Ʋ���
		return c;
	else
		return c * Translate(-eye);						// ���������Ҫ��-eye����ƽ��
}

mat4 Camera::ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// ����ͶӰ����ļ���
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
	// ͸��ͶӰ����ļ���
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
	// ������׶�����
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

 // �����������
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
	// �������λ�úͷ���
	if (!isFree) {
		float eyex, eyey, eyez;
		eyex = radius * cos(upAngle * M_PI / 180.0) * sin(rotateAngle * M_PI / 180.0) + at.x;
		eyey = radius * sin(upAngle * M_PI / 180.0) + at.y;
		eyez = radius * cos(upAngle * M_PI / 180.0) * cos(rotateAngle * M_PI / 180.0) + at.z;
		eye = vec4(eyex, eyey, eyez, 1.0);
	}
}

void Camera::animation(int op) {
	// Ǧ���۲�
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


// �����¼�����
void Camera::keyboard(int key)
{
	float cameraSpeed = 0.2f; // adjust accordingly
	// FPS�������Բ�ܹ۲�
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
	// FPS�������
	if (key == GLFW_KEY_S) {
		if (isFree)
			eye -= cameraSpeed * front;
	}
	// FPS���ǰ��
	if (key == GLFW_KEY_W) {
		if (isFree)
			eye += cameraSpeed * front;
	}
	// FPS�������Բ�ܹ۲�
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
	// ˮƽ���ƹ۲�
	if (key == GLFW_KEY_U) {
		if (!isFree) {
			rotateAngle += 5.0;
			updateCamera();
		}
	}
	// ˮƽ���ƹ۲� ������
	if (key == GLFW_KEY_J) {
		if (!isFree) {
			rotateAngle -= 5.0;
			updateCamera();
		}
	}
	// ��ֱ���ƹ۲�
	if (key == GLFW_KEY_I) {
		if (!isFree) {
			upAngle += 5.0;
			upAngle = min(180.0, upAngle);
			updateCamera();
		}
	}
	// ��ֱ���ƹ۲� ������
	if (key == GLFW_KEY_K) {
		if (!isFree) {
			upAngle -= 5.0;
			upAngle = max(0.0, upAngle);
			updateCamera();
		}
	}
	// ��Զ
	if (key == GLFW_KEY_O) {
		if (!isFree) {
			radius += 0.1;
			updateCamera();
		}
	}
	// ����
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

	// ���ñ仯������
	float sensitivity = 0.5;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw -= xoffset;
	pitch += yoffset;

	// �����ǲ�Ҫ����30��
	if (pitch > 30.0f)
		pitch = 30.0f;
	if (pitch < -30.0f)
		pitch = -30.0f;
	// �����ƫ������Ҫ����������Ч�����ÿ�
	if (op == 1)
		pitch = 0;

	front.x = cos(yaw * M_PI / 180.0) * cos(pitch * M_PI / 180.0);
	front.y = -sin(pitch * M_PI / 180.0);
	front.z = sin(yaw * M_PI / 180.0) * cos(pitch * M_PI / 180.0);
	front = normalize(front);
}
