#pragma once
#include "SkyBox.h"
#include "stb_image.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>



SkyBox::SkyBox() {
    // 初始化天空盒着色器
    skyboxObject.vshader = "shaders/skybox_vshader.glsl";
    skyboxObject.fshader = "shaders/skybox_fshader.glsl";
    skyboxObject.program = InitShader(skyboxObject.vshader.c_str(), skyboxObject.fshader.c_str());
    skyboxObject.viewLocation = glGetUniformLocation(skyboxObject.program, "view");
    skyboxObject.projectionLocation = glGetUniformLocation(skyboxObject.program, "projection");
    // 初始化纹理贴图路径
    faces.push_back("./assets/skybox/6/right.jpg");
    faces.push_back("./assets/skybox/6/left.jpg");
    faces.push_back("./assets/skybox/6/top.jpg");
    faces.push_back("./assets/skybox/6/bottom.jpg");
    faces.push_back("./assets/skybox/6/back.jpg");
    faces.push_back("./assets/skybox/6/front.jpg");
}

SkyBox::~SkyBox() {
    faces.clear();
}

void SkyBox::initSkyBox() {
    // 创建顶点数组对象
    #ifdef __APPLE__	// for MacOS
        glGenVertexArraysAPPLE(1, &skyboxObject.vao);		// 分配1个顶点数组对象
        glBindVertexArrayAPPLE(skyboxObject.vao);		// 绑定顶点数组对象
    #else				// for Windows
        glGenVertexArrays(1, &skyboxObject.vao);
        glBindVertexArray(skyboxObject.vao);
    #endif
    
    // 创建并初始化顶点缓存对象
    glGenBuffers(1, &skyboxObject.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxObject.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    skyboxObject.texture = loadCubemap();

    #ifdef __APPLE__
        glBindVertexArrayAPPLE(0);
    #else
        glBindVertexArray(0);
    #endif
    glUseProgram(0);
}

// 加载天空盒纹理
// 参考：LearnOpenGL
GLuint SkyBox::loadCubemap() {
    int width, height, channels = 0;
    unsigned char* image;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(true);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = stbi_load(faces[i], &width, &height, &channels, 0);
        if (image) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
            );
        }
        else {
            std::cout << "Fail to read image!" << std::endl;
        }
        // 释放图形内存
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void SkyBox::drawSkyBox(float screenWidth, float screenHeight, Camera* camera) {
    // Draw skybox first
    glDepthFunc(GL_LEQUAL); // Remember to turn depth writing off
    glUseProgram(skyboxObject.program);
    glUniform1i(glGetUniformLocation(skyboxObject.program, "skybox"), 0);
    // 相机矩阵计算
    camera->isSkyBox = true;
    // camera->updateCamera();
    camera->viewMatrix = camera->getViewMatrix();
    camera->projMatrix = camera->getProjectionMatrix(true);
    camera->isSkyBox = false;
    glUniformMatrix4fv(skyboxObject.viewLocation, 1, GL_FALSE, &camera->viewMatrix[0][0]);
    glUniformMatrix4fv(skyboxObject.projectionLocation, 1, GL_FALSE, &camera->projMatrix[0][0]);
    // skybox cube
    glBindVertexArray(skyboxObject.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxObject.texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
    glUseProgram(0);
}

// 按键切换天空盒背景
void SkyBox::keyboard(int key) {
    faces.clear();
    switch (key) {
    case GLFW_KEY_5:
        faces.push_back("./assets/skybox/6/right.jpg");
        faces.push_back("./assets/skybox/6/left.jpg");
        faces.push_back("./assets/skybox/6/top.jpg");
        faces.push_back("./assets/skybox/6/bottom.jpg");
        faces.push_back("./assets/skybox/6/back.jpg");
        faces.push_back("./assets/skybox/6/front.jpg");
        break;
    case GLFW_KEY_4:
        faces.push_back("./assets/skybox/5/right.jpg");
        faces.push_back("./assets/skybox/5/left.jpg");
        faces.push_back("./assets/skybox/5/top.jpg");
        faces.push_back("./assets/skybox/5/bottom.jpg");
        faces.push_back("./assets/skybox/5/back.jpg");
        faces.push_back("./assets/skybox/5/front.jpg");
        break;
    case GLFW_KEY_3:
        faces.push_back("./assets/skybox/4/right.jpg");
        faces.push_back("./assets/skybox/4/left.jpg");
        faces.push_back("./assets/skybox/4/top.jpg");
        faces.push_back("./assets/skybox/4/bottom.jpg");
        faces.push_back("./assets/skybox/4/back.jpg");
        faces.push_back("./assets/skybox/4/front.jpg");
        break;
    case GLFW_KEY_2:
        faces.push_back("./assets/skybox/2/right.jpg");
        faces.push_back("./assets/skybox/2/left.jpg");
        faces.push_back("./assets/skybox/2/top.jpg");
        faces.push_back("./assets/skybox/2/bottom.jpg");
        faces.push_back("./assets/skybox/2/back.jpg");
        faces.push_back("./assets/skybox/2/front.jpg");
        break;
    case GLFW_KEY_1:
        faces.push_back("./assets/skybox/1/right.jpg");
        faces.push_back("./assets/skybox/1/left.jpg");
        faces.push_back("./assets/skybox/1/top.jpg");
        faces.push_back("./assets/skybox/1/bottom.jpg");
        faces.push_back("./assets/skybox/1/back.jpg");
        faces.push_back("./assets/skybox/1/front.jpg");
        break;
    }
    initSkyBox();  
}