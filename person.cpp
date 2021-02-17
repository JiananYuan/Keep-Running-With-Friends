# include "person.h"
#include "stb_image.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
using namespace std;


void load_texture_STBImage_person(const std::string& file_name, GLuint& texture);


// 包含一个人的部位，TriMesh组件
person::person() {
	// 随机种子，主要用于人物运动时引起场景的轻微抖动
	srand((unsigned)time(NULL));

	// 生成人物各部位
	Torso = new TriMesh(false);
	Head = new TriMesh(false);
	RightUpperArm = new TriMesh(false);
	RightLowerArm = new TriMesh(false);
	LeftUpperArm = new TriMesh(false);
	LeftLowerArm = new TriMesh(false);
	RightUpperLeg = new TriMesh(false);
	RightLowerLeg = new TriMesh(false);
	LeftUpperLeg = new TriMesh(false);
	LeftLowerLeg = new TriMesh(false);
}

// 析构人
person::~person() {
	delete Torso;
	delete Head;
	delete RightUpperArm;
	delete RightLowerArm;
	delete LeftUpperArm;
	delete LeftLowerArm;
	delete RightUpperLeg;
	delete RightLowerLeg;
	delete LeftUpperLeg;
	delete LeftLowerLeg;
}

void person::setPos(vec3 pos) {
	Pos = pos;
}

// 传递给GPU绘制的数据
void person::drawPerson(mat4 modelMatrix, TriMesh* mesh, Camera* camera, Light* light, openGLObject_person object) {
	// 相机矩阵计算
	// camera->updateCamera();
	camera->viewMatrix = camera->getViewMatrix();
	// 获取透视投影矩阵
	camera->projMatrix = camera->getProjectionMatrix(false);

	#ifdef __APPLE__	// for MacOS
		glBindVertexArrayAPPLE(object.vao);
	#else
		glBindVertexArray(object.vao);
	#endif
	glUseProgram(object.program);
	// 传递立方体贴图数据

	// 父节点矩阵 * 本节点局部变换矩阵
	glUniformMatrix4fv(object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(object.viewLocation, 1, GL_TRUE, &camera->viewMatrix[0][0]);
	glUniformMatrix4fv(object.projectionLocation, 1, GL_TRUE, &camera->projMatrix[0][0]);
	glUniform1i(object.shadowLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object.texture);// 该语句必须，否则将只使用同一个纹理进行绘制
	// glUniform1i(glGetUniformLocation(object.program, "texture2"), 0);
	glUniform1i(glGetUniformLocation(object.program, "texture"), 0);
	bindLightAndMaterial(mesh, object, light, camera);
	glDrawArrays(GL_TRIANGLES, 0, mesh->getPoints().size());

	// 将材质和光源数据传递给着色器
	// 绘制
	// glDrawArrays(GL_TRIANGLES, 0, mesh->getPoints().size());

	// 根据光源位置，计算阴影投影矩阵
	mat4 sha = mat4(
		vec4(-light->getTranslation().y, light->getTranslation().x, 0, 0),
		vec4(0, 0, 0, 0),
		vec4(0, light->getTranslation().z, -light->getTranslation().y, 0),
		vec4(0, 1, 0, -light->getTranslation().y)
	);
	// 计算阴影的模型变换矩阵
	modelMatrix = sha * modelMatrix;
	glUniformMatrix4fv(object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(object.viewLocation, 1, GL_TRUE, &camera->viewMatrix[0][0]);
	glUniformMatrix4fv(object.projectionLocation, 1, GL_TRUE, &camera->projMatrix[0][0]);
	// 绘制阴影
	glUniform1i(object.shadowLocation, 1);
	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, mesh->getPoints().size());

	#ifdef __APPLE__	// for MacOS
		glBindVertexArrayAPPLE(0);
	#else
		glBindVertexArray(0);
	#endif
	glUseProgram(0);
}

// 躯体
void person::torso(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 节点局部变换矩阵
	mat4 translate = Translate(0.0, 0.5 * TORSO_HEIGHT, 0);
	mat4 scale = Scale(TORSO_WIDTH, TORSO_HEIGHT, 0.5*TORSO_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, Torso, camera, light, TorsoObject);
}

// 头部
void person::head(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, 0.5 * HEAD_HEIGHT, 0.0);
	mat4 scale = Scale(HEAD_WIDTH, HEAD_HEIGHT, 0.8*HEAD_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, Head, camera, light, HeadObject);
}


// 左大臂
void person::left_upper_arm(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * UPPER_ARM_HEIGHT, 0.0);
	mat4 scale = Scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, LeftUpperArm, camera, light, LeftUpperArmObject);
}


// @TODO: 左小臂
void person::left_lower_arm(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * LOWER_ARM_HEIGHT, 0.0);
	mat4 scale = Scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, LeftLowerArm, camera, light, LeftLowerArmObject);
}

// @TODO: 右大臂
void person::right_upper_arm(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * UPPER_ARM_HEIGHT, 0.0);
	mat4 scale = Scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, RightUpperArm, camera, light, RightUpperArmObject);
}

// @TODO: 右小臂
void person::right_lower_arm(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * LOWER_ARM_HEIGHT, 0.0);
	mat4 scale = Scale(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, RightLowerArm, camera, light, RightLowerArmObject);
}

// @TODO: 左大腿
void person::left_upper_leg(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * UPPER_LEG_HEIGHT, 0.0);
	mat4 scale = Scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, LeftUpperLeg, camera, light, LeftUpperLegObject);
}

// @TODO: 左小腿
void person::left_lower_leg(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * LOWER_LEG_HEIGHT, 0.0);
	mat4 scale = Scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, LeftLowerLeg, camera, light, LeftLowerLegObject);
}

// @TODO: 右大腿
void person::right_upper_leg(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * UPPER_LEG_HEIGHT, 0.0);
	mat4 scale = Scale(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, RightUpperLeg, camera, light, RightUpperLegObject);
}

// @TODO: 右小腿
void person::right_lower_leg(mat4 modelMatrix, Camera* camera, Light* light)
{
	// 本节点局部变换矩阵
	mat4 translate = Translate(0.0, -0.5 * LOWER_LEG_HEIGHT, 0.0);
	mat4 scale = Scale(LOWER_LEG_WIDTH, LOWER_LEG_HEIGHT, LOWER_LEG_WIDTH);
	mat4 instance = translate * scale;
	// 乘以来自父物体的模型变换矩阵，绘制当前物体
	drawPerson(modelMatrix * instance, RightLowerLeg, camera, light, RightLowerLegObject);
}

// 初始化各部位并绑定数据
void person::init1() {

	std::string vshader = "shaders/cube_vshader.glsl";
	std::string fshader = "shaders/cube_fshader.glsl";

	// 设置物体的大小（初始的旋转和位移都为0）
	Torso->readObj("./assets/obj2/torso/torso.obj");
	Head->readObj("./assets/obj2/head/head.obj");
	RightUpperArm->readObj("./assets/obj2/upper_arm/upper_arm.obj");
	LeftUpperArm->readObj("./assets/obj2/upper_arm/upper_arm.obj");
	RightUpperLeg->readObj("./assets/obj2/upper_leg/upper_leg.obj");
	LeftUpperLeg->readObj("./assets/obj2/upper_leg/upper_leg.obj");
	RightLowerArm->readObj("./assets/obj2/lower_arm/lower_arm.obj");
	LeftLowerArm->readObj("./assets/obj2/lower_arm/lower_arm.obj");
	RightLowerLeg->readObj("./assets/obj2/lower_leg/lower_leg.obj");
	LeftLowerLeg->readObj("./assets/obj2/lower_leg/lower_leg.obj");
	// setPos(vec3(5.0, 0.46, -3));
	setPos(vec3(5.0, 0.23, -3));

	// 将物体的顶点数据传递
	bindObjectAndData(Torso, TorsoObject, "./assets/obj2/torso/torso.png", vshader, fshader);
	bindObjectAndData(Head, HeadObject, "./assets/obj2/head/head.png", vshader, fshader);
	bindObjectAndData(RightUpperArm, RightUpperArmObject, "./assets/obj2/upper_arm/upper_arm.png", vshader, fshader);
	bindObjectAndData(LeftUpperArm, LeftUpperArmObject, "./assets/obj2/upper_arm/upper_arm.png", vshader, fshader);
	bindObjectAndData(RightUpperLeg, RightUpperLegObject, "./assets/obj2/upper_leg/upper_leg.png", vshader, fshader);
	bindObjectAndData(LeftUpperLeg, LeftUpperLegObject, "./assets/obj2/upper_leg/upper_leg.png", vshader, fshader);
	bindObjectAndData(RightLowerArm, RightLowerArmObject, "./assets/obj2/lower_arm/lower_arm.png", vshader, fshader);
	bindObjectAndData(LeftLowerArm, LeftLowerArmObject, "./assets/obj2/lower_arm/lower_arm.png", vshader, fshader);
	bindObjectAndData(RightLowerLeg, RightLowerLegObject, "./assets/obj2/lower_leg/lower_leg.png", vshader, fshader);
	bindObjectAndData(LeftLowerLeg, LeftLowerLegObject, "./assets/obj2/lower_leg/lower_leg.png", vshader, fshader);
}

// 初始化各部位并绑定数据
void person::init2() {

	std::string vshader = "shaders/cube_vshader.glsl";
	std::string fshader = "shaders/cube_fshader.glsl";

	// 设置物体的大小（初始的旋转和位移都为0）
	Torso->readObj("./assets/obj2/Steve/torso/torso.obj");
	Head->readObj("./assets/obj2/Steve/head/head.obj");
	RightUpperArm->readObj("./assets/obj2/Steve/upper_arm/upper_arm.obj");
	LeftUpperArm->readObj("./assets/obj2/Steve/upper_arm/upper_arm.obj");
	RightUpperLeg->readObj("./assets/obj2/Steve/upper_leg/upper_leg.obj");
	LeftUpperLeg->readObj("./assets/obj2/Steve/upper_leg/upper_leg.obj");
	RightLowerArm->readObj("./assets/obj2/Steve/lower_arm/lower_arm.obj");
	LeftLowerArm->readObj("./assets/obj2/Steve/lower_arm/lower_arm.obj");
	RightLowerLeg->readObj("./assets/obj2/Steve/lower_leg/lower_leg.obj");
	LeftLowerLeg->readObj("./assets/obj2/Steve/lower_leg/lower_leg.obj");
	// setPos(vec3(0, 0.46, -0.4));
	// setPos(vec3(0, 0.46, 0));
	// setPos(vec3(4.0, 0.01, -4));
	setPos(vec3(4.0, 0.23, -4));


	// 将物体的顶点数据传递
	bindObjectAndData(Torso, TorsoObject, "./assets/obj2/Steve/torso/torso.png", vshader, fshader);
	bindObjectAndData(Head, HeadObject, "./assets/obj2/Steve/head/head.png", vshader, fshader);
	bindObjectAndData(RightUpperArm, RightUpperArmObject, "./assets/obj2/Steve/upper_arm/upper_arm.png", vshader, fshader);
	bindObjectAndData(LeftUpperArm, LeftUpperArmObject, "./assets/obj2/Steve/upper_arm/upper_arm.png", vshader, fshader);
	bindObjectAndData(RightUpperLeg, RightUpperLegObject, "./assets/obj2/Steve/upper_leg/upper_leg.png", vshader, fshader);
	bindObjectAndData(LeftUpperLeg, LeftUpperLegObject, "./assets/obj2/Steve/upper_leg/upper_leg.png", vshader, fshader);
	bindObjectAndData(RightLowerArm, RightLowerArmObject, "./assets/obj2/Steve/lower_arm/lower_arm.png", vshader, fshader);
	bindObjectAndData(LeftLowerArm, LeftLowerArmObject, "./assets/obj2/Steve/lower_arm/lower_arm.png", vshader, fshader);
	bindObjectAndData(RightLowerLeg, RightLowerLegObject, "./assets/obj2/Steve/lower_leg/lower_leg.png", vshader, fshader);
	bindObjectAndData(LeftLowerLeg, LeftLowerLegObject, "./assets/obj2/Steve/lower_leg/lower_leg.png", vshader, fshader);

	setSitDown();

	// glClearColor(1.0, 1.0, 1.0, 1.0);
}

void person::setSitDown() {
	theta[0] = 20;  // Torso
	theta[1] = 0;   // Head
	theta[2] = 0;  // RightUpperArm
	theta[3] = 15;   // RightLowerArm
	theta[4] = -30;  // LeftUpperArm
	theta[5] = 0;   // LeftLowerArm
	theta[6] = 100; // RightUpperLeg
	theta[7] = -30;  // RightLowerLeg
	theta[8] = 130; // LeftUpperLeg
	theta[9] = -70;  // LeftLowerLeg
	Pos.y = 0.01;
	downTheta = 15.0;
}



void person::display_person(Camera* camera, Light* light) {
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	// 相机矩阵计算
	// camera->updateCamera();
	camera->viewMatrix = camera->getViewMatrix();
	camera->projMatrix = camera->getProjectionMatrix(true);
	// direction = camera->getTheta();


	// 物体的变换矩阵
	mat4 modelMatrix = mat4(1.0);

	// 保存变换矩阵的栈
	MatrixStack mstack;

	// 躯干（这里叠加了位移和转角）
	modelMatrix = Translate(Pos) * RotateY(direction-90) * RotateX(downTheta)
		* RotateY(theta[op_Torso]);
	torso(modelMatrix, camera, light);

	mstack.push(modelMatrix); // 保存变换矩阵
	// 头部（这里我们希望机器人的头部只绕Y轴旋转，所以只计算了RotateY）
	modelMatrix *= Translate(0.0, TORSO_HEIGHT, 0.0)
		* RotateY(theta[op_Head]);
	head(modelMatrix, camera, light);
	modelMatrix = mstack.pop(); // 恢复变换矩阵

	// =========== 左臂 ===========
	mstack.push(modelMatrix);   // 保存变换矩阵
	// 左大臂（这里我们希望机器人的左大臂只绕Z轴旋转，所以只计算了RotateZ，后面同理）
	modelMatrix *= Translate(-0.5 * TORSO_WIDTH - 0.5 * UPPER_ARM_WIDTH, 0.88 * TORSO_HEIGHT, 0.0)
		* RotateX(theta[op_LeftUpperArm]);
	left_upper_arm(modelMatrix, camera, light);

	// 左小臂
	modelMatrix *= Translate(0, -0.8 * UPPER_ARM_HEIGHT, 0.0)
		* RotateX(theta[op_LeftLowerArm]);
	left_lower_arm(modelMatrix, camera, light);
	modelMatrix = mstack.pop(); // 恢复变换矩阵


	// =========== 右臂 ===========
	// @TODO: 右大臂
	mstack.push(modelMatrix);   // 保存变换矩阵
	// 左大臂（这里我们希望机器人的左大臂只绕Z轴旋转，所以只计算了RotateZ，后面同理）
	modelMatrix *= Translate(0.5 * TORSO_WIDTH + 0.5 * UPPER_ARM_WIDTH, 0.88 * TORSO_HEIGHT, 0.0)
		* RotateX(theta[op_RightUpperArm]);
	right_upper_arm(modelMatrix, camera, light);

	// @TODO: 右小臂
	modelMatrix *= Translate(0, -0.8 * UPPER_ARM_HEIGHT, 0.0)
		* RotateX(theta[op_RightLowerArm]);
	right_lower_arm(modelMatrix, camera, light);
	modelMatrix = mstack.pop(); // 恢复变换矩阵

	// =========== 左腿 ===========
	// @TODO: 左大腿
	mstack.push(modelMatrix);   // 保存变换矩阵
	modelMatrix *= Translate(-0.7 * UPPER_LEG_WIDTH, 0, 0.0)
		* RotateX(theta[op_LeftUpperLeg]);
	left_upper_leg(modelMatrix, camera, light);

	// @TODO: 左小腿
	modelMatrix *= Translate(0, -0.8 * UPPER_LEG_HEIGHT, 0.0)
		* RotateX(theta[op_LeftLowerLeg]);
	left_lower_leg(modelMatrix, camera, light);
	modelMatrix = mstack.pop(); // 恢复变换矩阵

	// =========== 右腿 ===========
	// @TODO: 右大腿
	mstack.push(modelMatrix);   // 保存变换矩阵
	modelMatrix *= Translate(0.7 * UPPER_LEG_WIDTH, 0, 0.0)
		* RotateX(theta[op_RightUpperLeg]);
	right_upper_leg(modelMatrix, camera, light);

	// @TODO: 右小腿
	modelMatrix *= Translate(0, -0.8 * UPPER_LEG_HEIGHT, 0.0)
		* RotateX(theta[op_RightLowerLeg]);
	right_lower_leg(modelMatrix, camera, light);
	modelMatrix = mstack.pop(); // 恢复变换矩阵


	//glutSwapBuffers();
}

void person::bindObjectAndData(TriMesh* mesh, openGLObject_person& object, std::string texture_image, const std::string& vshader, const std::string& fshader) {

	// 创建顶点数组对象
	#ifdef __APPLE__	// for MacOS
		glGenVertexArraysAPPLE(1, &object.vao);		// 分配1个顶点数组对象
		glBindVertexArrayAPPLE(object.vao);		// 绑定顶点数组对象
	#else				// for Windows
		glGenVertexArrays(1, &object.vao);  	// 分配1个顶点数组对象
		glBindVertexArray(object.vao);  	// 绑定顶点数组对象
	#endif

	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBufferData(GL_ARRAY_BUFFER,
		(mesh->getPoints().size() + mesh->getColors().size() + mesh->getNormals().size()) * sizeof(vec3) + mesh->getTextures().size() * sizeof(vec2),
		NULL,
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getPoints().size() * sizeof(vec3), &mesh->getPoints()[0]);
	glBufferSubData(GL_ARRAY_BUFFER, mesh->getPoints().size() * sizeof(vec3), mesh->getColors().size() * sizeof(vec3), &mesh->getColors()[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (mesh->getPoints().size() + mesh->getColors().size()) * sizeof(vec3), mesh->getNormals().size() * sizeof(vec3), &mesh->getNormals()[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (mesh->getPoints().size() + mesh->getColors().size() + mesh->getNormals().size()) * sizeof(vec3), mesh->getTextures().size() * sizeof(vec2), &mesh->getTextures()[0]);

	object.vshader = vshader;
	object.fshader = fshader;
	object.program = InitShader(object.vshader.c_str(), object.fshader.c_str());

	// 从顶点着色器中初始化顶点的坐标
	object.pLocation = glGetAttribLocation(object.program, "vPosition");
	glEnableVertexAttribArray(object.pLocation);
	glVertexAttribPointer(object.pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// 从顶点着色器中初始化顶点的颜色
	object.cLocation = glGetAttribLocation(object.program, "vColor");
	glEnableVertexAttribArray(object.cLocation);
	glVertexAttribPointer(object.cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mesh->getPoints().size() * sizeof(vec3)));

	// @TODO: Task1 从顶点着色器中初始化顶点的法向量
	object.nLocation = glGetAttribLocation(object.program, "vNormal");
	glEnableVertexAttribArray(object.nLocation);
	glVertexAttribPointer(object.nLocation, 3,
		GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET((mesh->getPoints().size() + mesh->getColors().size()) * sizeof(vec3)));

	object.tLocation = glGetAttribLocation(object.program, "vTexture");
	glEnableVertexAttribArray(object.tLocation);
	glVertexAttribPointer(object.tLocation, 2,
		GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET((mesh->getPoints().size() + mesh->getColors().size()+mesh->getNormals().size()) * sizeof(vec3)));

	// 获得矩阵位置
	object.modelLocation = glGetUniformLocation(object.program, "model");
	object.viewLocation = glGetUniformLocation(object.program, "view");
	object.projectionLocation = glGetUniformLocation(object.program, "projection");
	object.shadowLocation = glGetUniformLocation(object.program, "isShadow");

	// 读取纹理图片数
	object.texture_image = texture_image;
	// 创建纹理的缓存对象
	glGenTextures(1, &object.texture);
	// 调用stb_image生成纹理
	load_texture_STBImage_person(object.texture_image, object.texture);

	// Clean up
	#ifdef __APPLE__
		glBindVertexArrayAPPLE(0);
	#else
		glBindVertexArray(0);
	#endif
	glUseProgram(0);
}


void person::bindLightAndMaterial(TriMesh* mesh, openGLObject_person& object, Light* light, Camera* camera) {

	// 传递相机的位置
	glUniform3fv(glGetUniformLocation(object.program, "eye_position"), 1, vec3(camera->eye[0], camera->eye[1], camera->eye[2]));

	// 传递物体的材质
	vec4 meshAmbient = mesh->getAmbient();
	vec4 meshDiffuse = mesh->getDiffuse();
	vec4 meshSpecular = mesh->getSpecular();
	float meshShininess = mesh->getShininess();
	glUniform4fv(glGetUniformLocation(object.program, "material.ambient"), 1, meshAmbient);
	glUniform4fv(glGetUniformLocation(object.program, "material.diffuse"), 1, meshDiffuse);
	glUniform4fv(glGetUniformLocation(object.program, "material.specular"), 1, meshSpecular);
	glUniform1f(glGetUniformLocation(object.program, "material.shininess"), meshShininess);

	// 传递光源信息
	vec4 lightAmbient = light->getAmbient();
	vec4 lightDiffuse = light->getDiffuse();
	vec4 lightSpecular = light->getSpecular();
	vec3 lightPosition = light->getTranslation();

	glUniform4fv(glGetUniformLocation(object.program, "light.ambient"), 1, lightAmbient);
	glUniform4fv(glGetUniformLocation(object.program, "light.diffuse"), 1, lightDiffuse);
	glUniform4fv(glGetUniformLocation(object.program, "light.specular"), 1, lightSpecular);
	glUniform3fv(glGetUniformLocation(object.program, "light.position"), 1, lightPosition);
}

// 初始化关节角
void person::initTheta() {
	for (int i = 0; i < 10; ++i) {
		theta[i] = 0.0;
	}
	theta[3] = 70.0f;
	theta[5] = 70.0f;
}

// 记录关节角快照
void person::recordTheta() {
	for (int i = 0; i < 10; ++i) {
		tmp_theta[i] = theta[i];
	}
}

// 恢复关节角快照
void person::resetTheta() {
	for (int i = 0; i < 10; ++i) {
		theta[i] = tmp_theta[i];
	}
}

// 重置关节角
void person::resetTheta0() {
	for (int i = 0; i < 10; ++i) {
		theta[i] = 0.0;
	}
	downTheta = 0;
}

// 控制人物运动动作，以及更新人物位置朝向信息
void person::keyboard(int key, Camera* camera, double rate) {
	// 前进：人物跨腿
	if (key == GLFW_KEY_UP || key == GLFW_KEY_H) {
		// PlaySound(TEXT("./assets/bgm.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if (isBeginRunning == true) {
			initTheta();
			isBeginRunning = false;
		}
		downTheta = 0;
		Pos.y = 0.18f;
		// 更新人物位移
		Pos += speed * vec3(cos(direction * M_PI / 180.0), 0, -sin(direction * M_PI / 180.0));
		// 前进时的腿部动作
		if (fabs(theta[op_LeftUpperLeg]) <= 30) {
			//左腿向前
			if (fabs(theta[op_LeftUpperLeg]) <= 0.001 && step > 0) {
				// 左小腿进入向前迈进的状态
				tmp1 = 2 * step;
			}
			else if (fabs(theta[op_LeftUpperLeg]) <= 0.001 && step < 0) {
				// 左小腿进入后退状态
				tmp1 = 0;
			}
			theta[op_LeftUpperLeg] += step*rate;
			theta[op_LeftLowerLeg] -= tmp1*rate;

			//右腿向后
			if (fabs(theta[op_RightUpperLeg]) <= 0.001 && step > 0) {
				// 右小腿进入后退的状态
				tmp2 = 0;
			}
			else if (fabs(theta[op_RightUpperLeg]) <= 0.001 && step < 0) {
				tmp2 = 2 * step;
			}
			theta[op_RightUpperLeg] -= step*rate;
			theta[op_RightLowerLeg] += tmp2*rate;

			theta[op_LeftUpperArm] -= step*rate;
			theta[op_RightUpperArm] += step*rate;
		}
		else {
			step = -step;
			tmp1 = -tmp1;
			tmp2 = -tmp2;
			theta[op_LeftUpperLeg] += step*rate;
			theta[op_LeftLowerLeg] -= tmp1*rate;
			theta[op_RightUpperLeg] -= step*rate;
			theta[op_RightLowerLeg] += tmp2*rate;
			theta[op_LeftUpperArm] -= 8 * step*rate;
			theta[op_RightUpperArm] += 8 * step*rate;
		}
	}
	// 注意这里同样还是前进，只是人是以俯冲的姿势前进
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_N) {
		// 更新
		Pos += speed * vec3(cos(direction * M_PI / 180.0), 0, -sin(direction * M_PI / 180.0));
		// theta[op_Torso] = 30;
		downTheta = 75;
		theta[op_LeftUpperArm] = 180;
		theta[op_RightUpperArm] = 180;
		// Pos.y = 0.3;
		Pos.y = 0.05;
	}
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_B) {
		// 人物朝向角+5
		direction += 5*rate*0.35;
	}
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_M) {
		// 人物朝向角-5
		direction -= 5*rate*0.35;
	}
	//// 固定模式即TPS模式（跟在人物后面）
	//if (!camera->isFree) {
	//	// 根据人物位置反推相机位置
	//	camera->eye = Pos - camera_person_distance * vec3(cos(direction * M_PI / 180), 0, -sin(direction * M_PI / 180));
	//	// 相机视点位置是人物位置，这样可以保持相对静止
	//	camera->at = Pos;
	//	// camera->at.y = 0.46f;
	//}
}

void load_texture_STBImage_person(const std::string& file_name, GLuint& texture) {
	// 读取纹理图片，并将其传递给着色器

	int width, height, channels = 0;
	unsigned char* pixels = NULL;
	// 读取图片的时候先翻转一下图片，如果不设置的话显示出来是反过来的图片
	stbi_set_flip_vertically_on_load(true);
	// 读取图片数据
	pixels = stbi_load(file_name.c_str(), &width, &height, &channels, 0);

	// 调整行对齐格式
	if (width * channels % 4 != 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLenum format = GL_RGB;
	// 设置通道格式
	switch (channels)
	{
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		format = GL_RGB;
		break;
	}

	// 绑定纹理对象
	// glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 指定纹理的放大，缩小滤波，使用线性方式，即当图片放大的时候插值方式
	// 将图片的rgb数据上传给opengl
	glTexImage2D(
		GL_TEXTURE_2D,    // 指定目标纹理，这个值必须是GL_TEXTURE_2D
		0,                // 执行细节级别，0是最基本的图像级别，n表示第N级贴图细化级别
		format,           // 纹理数据的颜色格式(GPU显存)
		width,            // 宽度。早期的显卡不支持不规则的纹理，则宽度和高度必须是2^n
		height,           // 高度。早期的显卡不支持不规则的纹理，则宽度和高度必须是2^n
		0,                // 指定边框的宽度。必须为0
		format,           // 像素数据的颜色格式(CPU内存)
		GL_UNSIGNED_BYTE, // 指定像素数据的数据类型
		pixels            // 指定内存中指向图像数据的指针
	);

	// 生成多级渐远纹理，多消耗1/3的显存，较小分辨率时获得更好的效果
	glGenerateMipmap(GL_TEXTURE_2D);

	// 指定插值方法
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 恢复初始对齐格式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// 释放图形内存
	stbi_image_free(pixels);
}

// 设置部位高光系数
void person::setShininess(float shininess) {
	Torso->setShininess(shininess);
	Head->setShininess(shininess);
	RightUpperArm->setShininess(shininess);
	RightLowerArm->setShininess(shininess);
	LeftUpperArm->setShininess(shininess);
	LeftLowerArm->setShininess(shininess);
	RightUpperLeg->setShininess(shininess);
	RightLowerLeg->setShininess(shininess);
	LeftUpperLeg->setShininess(shininess);
	LeftLowerLeg->setShininess(shininess);
}