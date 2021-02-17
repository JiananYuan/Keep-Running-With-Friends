#pragma once
#include "Angel.h"
#include "Camera.h"
#include <vector>
using namespace std;

struct openGLObject_sky
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

class SkyBox {
protected:
    vector<const GLchar*> faces;
    openGLObject_sky skyboxObject;
    GLfloat skyboxVertices[108] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

public:
    SkyBox();
    ~SkyBox();
    GLuint loadCubemap();
    void initSkyBox();
    void drawSkyBox(float screenWidth, float screenHeight, Camera* camera);
    void keyboard(int key);
};