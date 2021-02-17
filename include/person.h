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
	int		_index;  // ջ�����
	int		_size;   // ջ��С
	mat4* _matrices; // ����

public:
	MatrixStack(int numMatrices = 100) :_index(0), _size(numMatrices) {
		_matrices = new mat4[numMatrices];
	}
	~MatrixStack() {
		delete[]_matrices;
	}
	// ��ջ
	void push(const mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;
	}
	// ��ջ
	mat4& pop() {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};

struct openGLObject_person
{
    // �����������
    GLuint vao;
    // ���㻺�����
    GLuint vbo;

    // ��ɫ������
    GLuint program;
    // ��ɫ���ļ�
    std::string vshader;
    std::string fshader;
    // ��ɫ������
    GLuint pLocation;
    GLuint cLocation;
    GLuint nLocation;
    GLuint tLocation;

    // ����
    std::string texture_image;
    GLuint texture;

    // ͶӰ�任����
    GLuint modelLocation;
    GLuint viewLocation;
    GLuint projectionLocation;

    // ��Ӱ����
    GLuint shadowLocation;
};

class person {
public:
	// �ؽڴ�С
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

	
	
	int op_Torso = 0;			// ����
	int op_Head = 1;			// ͷ��
	int op_RightUpperArm = 2;	// �Ҵ��
	int op_RightLowerArm = 3;	// ��С��
	int op_LeftUpperArm = 4;	// ����
	int op_LeftLowerArm = 5;	// ��С��
	int op_RightUpperLeg = 6;	// �Ҵ���
	int op_RightLowerLeg = 7;	// ��С��
	int op_LeftUpperLeg = 8;	// �����
	int op_LeftLowerLeg = 9;	// ��С��

	// ��̬
	int step = 5;
	float tmp1 = 0, tmp2 = 0;
	// ��ǰ��ɫ����
	float direction = 0; // ��ǰ���ﳯ��λ��(��Z�Ḻ����н�)
	// (cos, 0, sin)��Ϊǰ������
	// λ��������ԭ��Ϊ�ο��㣩
	vec3 Pos = vec3(0, 0, 0);
	vec3 front = vec3(0, 0, -1);
	float speed = 0.10f;
	float lastX = 0;
	float yaw = 0;
	float camera_person_distance = 3;

	// ���︩���
	float downTheta = 0;
	// �������ڱ��ܵ�״̬����
	bool isBeginRunning = true;
	bool isSitDown = true;
	

	// �ؽڽǴ�С
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
	// �ؽڽǿ���
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