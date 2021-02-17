﻿#include "TriMesh.h"
#include <sstream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
using namespace std;

// 一些基础颜色
const vec3 basic_colors[8] = {
	vec3(1.0, 1.0, 1.0), // White
	vec3(1.0, 1.0, 0.0), // Yellow
	vec3(0.0, 1.0, 0.0), // Green
	vec3(0.0, 1.0, 1.0), // Cyan
	vec3(1.0, 0.0, 1.0), // Magenta
	vec3(1.0, 0.0, 0.0), // Red
	vec3(0.0, 0.0, 0.0), // Black
	vec3(0.0, 0.0, 1.0)	 // Blue
};

// 立方体的各个点
const vec3 cube_vertices[8] = {
	vec3(-0.5, -0.5, -0.5),
	vec3(0.5, -0.5, -0.5),
	vec3(-0.5, 0.5, -0.5),
	vec3(0.5, 0.5, -0.5),
	vec3(-0.5, -0.5, 0.5),
	vec3(0.5, -0.5, 0.5),
	vec3(-0.5, 0.5, 0.5),
	vec3(0.5, 0.5, 0.5)};

// 三角形的点
const vec3 triangle_vertices[3] = {
	vec3(-0.5, -0.5, 0.0),
	vec3(0.5, -0.5, 0.0),
	vec3(0.0, 0.5, 0.0)};

// 正方形平面
const vec3 square_vertices[4] = {
	vec3(-0.5, -0.5, 0.0),
	vec3(0.5, -0.5, 0.0),
	vec3(0.5, 0.5, 0.0),
	vec3(-0.5, 0.5, 0.0),
};

TriMesh::TriMesh(bool is_normalize)
{
	do_normalize_size = is_normalize;
	diagonal_length = 1.0;
}

TriMesh::~TriMesh()
{
}

std::vector<vec3> TriMesh::getVertexPositions()
{
	return vertex_positions;
}

std::vector<vec3> TriMesh::getVertexColors()
{
	return vertex_colors;
}

std::vector<vec3> TriMesh::getVertexNormals()
{
	return vertex_normals;
}

std::vector<vec2> TriMesh::getVertexTextures()
{
	return vertex_textures;
}

std::vector<vec3i> TriMesh::getFaces()
{
	return faces;
}

std::vector<vec3> TriMesh::getPoints()
{
	return points;
}

std::vector<vec3> TriMesh::getColors()
{
	return colors;
}

std::vector<vec3> TriMesh::getNormals()
{
	return normals;
}

std::vector<vec2> TriMesh::getTextures()
{
	return textures;
}

void TriMesh::computeTriangleNormals()
{
	face_normals.resize(faces.size());
	for (size_t i = 0; i < faces.size(); i++)
	{
		auto &face = faces[i];
		vec3 v01 = vertex_positions[face.y] - vertex_positions[face.x];
		vec3 v02 = vertex_positions[face.z] - vertex_positions[face.x];
		face_normals[i] = normalize(cross(v01, v02));
	}
}

void TriMesh::computeVertexNormals()
{
	// 计算面片的法向量
	if (face_normals.size() == 0 && faces.size() > 0)
	{
		computeTriangleNormals();
	}
	// 初始化法向量为0
	vertex_normals.resize(vertex_positions.size(), vec3(0, 0, 0));
	for (size_t i = 0; i < faces.size(); i++)
	{
		auto &face = faces[i];
		vertex_normals[face.x] += face_normals[i];
		vertex_normals[face.y] += face_normals[i];
		vertex_normals[face.z] += face_normals[i];
	}
	for (size_t i = 0; i < vertex_normals.size(); i++)
	{
		vertex_normals[i] = normalize(vertex_normals[i]);
	}
	// 球心在原点的球法向量为坐标
	// for (int i = 0; i < vertex_positions.size(); i++)
	// 	vertex_normals.push_back(vertex_positions[i] - vec3(0.0, 0.0, 0.0));
}

void TriMesh::setNormalize(bool do_norm) { do_normalize_size = do_norm; }
bool TriMesh::getNormalize() { return do_normalize_size; }
float TriMesh::getDiagonalLength() { return diagonal_length; }

vec3 TriMesh::getTranslation()
{
	return translation;
}

vec3 TriMesh::getRotation()
{
	return rotation;
}

vec3 TriMesh::getScale()
{
	return scale;
}

mat4 TriMesh::getModelMatrix()
{
	return Translate(getTranslation()) *
		   RotateY(direction) * 
		   RotateZ(getRotation()[2]) *
		   RotateY(getRotation()[1]) *
		   RotateX(getRotation()[0]) *
		   Scale(getScale());
}

void TriMesh::setTranslation(vec3 translation)
{
	this->translation = translation;
}

void TriMesh::setRotation(vec3 rotation)
{
	this->rotation = rotation;
}

void TriMesh::setScale(vec3 scale)
{
	this->scale = scale;
}

vec4 TriMesh::getAmbient() { return ambient; };
vec4 TriMesh::getDiffuse() { return diffuse; };
vec4 TriMesh::getSpecular() { return specular; };
float TriMesh::getShininess() { return shininess; };

void TriMesh::setAmbient(vec4 _ambient) { ambient = _ambient; };
void TriMesh::setDiffuse(vec4 _diffuse) { diffuse = _diffuse; };
void TriMesh::setSpecular(vec4 _specular) { specular = _specular; };
void TriMesh::setShininess(float _shininess) { shininess = _shininess; };

void TriMesh::cleanData()
{
	vertex_positions.clear();
	vertex_colors.clear();
	vertex_normals.clear();
	vertex_textures.clear();

	faces.clear();
	normal_index.clear();
	color_index.clear();
	texture_index.clear();

	face_normals.clear();


	points.clear();
	colors.clear();
	normals.clear();
	textures.clear();
}

void TriMesh::storeFacesPoints()
{
	// 将读取的顶点根据三角面片上的顶点下标逐个加入
	// 要传递给GPU的points等容器内，可以参考之前实验的代码写法进行补全

	// 看是否归一化物体大小，是的话，这里将物体顶点缩放到对角线长度为1的包围盒内
	if (do_normalize_size) {
		// 记录物体包围盒大小，可以用于大小的归一化
		// 先获得包围盒的对角顶点
		float max_x = -FLT_MAX;
		float max_y = -FLT_MAX;
		float max_z = -FLT_MAX;
		float min_x = FLT_MAX;
		float min_y = FLT_MAX;
		float min_z = FLT_MAX;
		for (int i = 0; i < vertex_positions.size(); i++) {
			auto& position = vertex_positions[i];
			if (position.x > max_x) max_x = position.x;
			if (position.y > max_y) max_y = position.y;
			if (position.z > max_z) max_z = position.z;
			if (position.x < min_x) min_x = position.x;
			if (position.y < min_y) min_y = position.y;
			if (position.z < min_z) min_z = position.z;
		}
		up_corner = vec3(max_x, max_y, max_z);
		down_corner = vec3(min_x, min_y, min_z);
		center = vec3((min_x+max_x)/2.0, (min_y+max_y)/2.0, (min_z+max_z)/2.0);
		diagonal_length = length( up_corner - down_corner );
		for (int i = 0; i < vertex_positions.size(); i++) {
			vertex_positions[i] = (vertex_positions[i] - center) / diagonal_length;
		}
	}

	// 计算法向量
	if (vertex_normals.size() == 0)
		computeVertexNormals();

	// 根据每个三角面片的顶点下标存储要传入GPU的数据
	for (int i = 0; i < faces.size(); i++)
	{
		// 坐标
		points.emplace_back(vertex_positions[faces[i].x]);
		points.emplace_back(vertex_positions[faces[i].y]);
		points.emplace_back(vertex_positions[faces[i].z]);
		// 法向量
		if (vertex_normals.size() != 0) {
			normals.emplace_back(vertex_normals[normal_index[i].x]);
			normals.emplace_back(vertex_normals[normal_index[i].y]);
			normals.emplace_back(vertex_normals[normal_index[i].z]);
		}
		// 颜色
		colors.emplace_back(vertex_colors[color_index[i].x]);
		colors.emplace_back(vertex_colors[color_index[i].y]);
		colors.emplace_back(vertex_colors[color_index[i].z]);
		// 纹理
		if (vertex_textures.size() != 0) {
			textures.emplace_back(vertex_textures[texture_index[i].x]);
			textures.emplace_back(vertex_textures[texture_index[i].y]);
			textures.emplace_back(vertex_textures[texture_index[i].z]);
		}
	}
}

// 立方体生成12个三角形的顶点索引
void TriMesh::generateCube()
{
	// 创建顶点前要先把那些vector清空
	cleanData();

	for (int i = 0; i < 8; i++)
	{
		vertex_positions.push_back(cube_vertices[i]);
		vertex_colors.push_back(basic_colors[i]);
	}

	// 每个三角面片的顶点下标
	faces.push_back(vec3i(0, 1, 3));
	faces.push_back(vec3i(0, 3, 2));
	faces.push_back(vec3i(1, 4, 5));
	faces.push_back(vec3i(1, 0, 4));
	faces.push_back(vec3i(4, 0, 2));
	faces.push_back(vec3i(4, 2, 6));
	faces.push_back(vec3i(5, 6, 4));
	faces.push_back(vec3i(5, 7, 6));
	faces.push_back(vec3i(2, 6, 7));
	faces.push_back(vec3i(2, 7, 3));
	faces.push_back(vec3i(1, 5, 7));
	faces.push_back(vec3i(1, 7, 3));

	// 顶点纹理坐标，这里是每个面都用一个正方形图片的情况
	vertex_textures.push_back(vec2(0, 0));
	vertex_textures.push_back(vec2(1, 0));
	vertex_textures.push_back(vec2(0, 1));
	vertex_textures.push_back(vec2(1, 1));
	vertex_textures.push_back(vec2(0, 0));
	vertex_textures.push_back(vec2(1, 0));
	vertex_textures.push_back(vec2(0, 1));
	vertex_textures.push_back(vec2(1, 1));

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();

	// 创建顶点前要先把那些vector清空
	//cleanData();

	//for (int i = 0; i < 8; i++)
	//{
	//	vertex_positions.push_back(cube_vertices[i]);
	//	if (_color[0] == -1) {
	//		vertex_colors.push_back(basic_colors[i]);
	//	}
	//	else {
	//		vertex_colors.push_back(_color);
	//	}
	//}

	//// 每个三角面片的顶点下标

	//// 每个三角面片的顶点下标
	//faces.push_back(vec3i(0, 1, 3));
	//faces.push_back(vec3i(0, 3, 2));
	//faces.push_back(vec3i(1, 4, 5));
	//faces.push_back(vec3i(1, 0, 4));
	//faces.push_back(vec3i(4, 0, 2));
	//faces.push_back(vec3i(4, 2, 6));
	//faces.push_back(vec3i(5, 6, 4));
	//faces.push_back(vec3i(5, 7, 6));
	//faces.push_back(vec3i(2, 6, 7));
	//faces.push_back(vec3i(2, 7, 3));
	//faces.push_back(vec3i(1, 5, 7));
	//faces.push_back(vec3i(1, 7, 3));

	//storeFacesPoints();

	//normals.clear();
	//// 正方形的法向量不能靠之前顶点法向量的方法直接计算，因为每个四边形平面是正交的，不是连续曲面
	//for (int i = 0; i < faces.size(); i++)
	//{
	//	normals.push_back(face_normals[i]);
	//	normals.push_back(face_normals[i]);
	//	normals.push_back(face_normals[i]);
	//}
}

void TriMesh::generateTriangle(vec3 color)
{
	// 创建顶点前要先把那些vector清空
	cleanData();

	for (int i = 0; i < 3; i++)
	{
		vertex_positions.push_back(triangle_vertices[i]);
		vertex_colors.push_back(color);
	}

	// 每个三角面片的顶点下标
	faces.push_back(vec3i(0, 1, 2));

	// 顶点纹理坐标
	vertex_textures.push_back(vec2(0, 0));
	vertex_textures.push_back(vec2(1, 0));
	vertex_textures.push_back(vec2(0.5, 1));

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();
}

void TriMesh::generateSquare(vec3 color)
{
	// 创建顶点前要先把那些vector清空
	cleanData();

	for (int i = 0; i < 4; i++)
	{
		vertex_positions.push_back(square_vertices[i]);
		vertex_colors.push_back(color);
	}

	// 每个三角面片的顶点下标
	faces.push_back(vec3i(0, 1, 2));
	faces.push_back(vec3i(0, 2, 3));

	// 顶点纹理坐标
	vertex_textures.push_back(vec2(0, 0));
	vertex_textures.push_back(vec2(1, 0));
	vertex_textures.push_back(vec2(1, 1));
	vertex_textures.push_back(vec2(0, 1));

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();
}

void TriMesh::generateCylinder(int num_division, float radius, float height)
{

	cleanData();

	int num_samples = num_division;
	float step = 2 * M_PI / num_samples; // 每个切片的弧度

	// 按cos和sin生成x，y坐标，z为负，即得到下表面顶点坐标
	// 顶点， 纹理
	float z = -height;
	for (int i = 0; i < num_samples; i++)
	{
		float r_r_r = i * step;
		float x = radius * cos(r_r_r);
		float y = radius * sin(r_r_r);
		// 添加顶点坐标
		vertex_positions.push_back(vec3(x, y, z));
		vertex_normals.push_back( normalize(vec3(x, y, 0)));
		// 这里颜色和法向量一样
		vertex_colors.push_back( normalize(vec3(x, y, 0)));
	}

	// 按cos和sin生成x，y坐标，z为正，即得到上表面顶点坐标
	z = height;
	for (int i = 0; i < num_samples; i++)
	{
		float r_r_r = i * step;
		float x = radius * cos(r_r_r);
		float y = radius * sin(r_r_r);
		vertex_positions.push_back(vec3(x, y, z));
		vertex_normals.push_back( normalize(vec3(x, y, 0)));
		vertex_colors.push_back( normalize(vec3(x, y, 0)));
	}

	// 面片生成三角面片，每个矩形由两个三角形面片构成
	for (int i = 0; i < num_samples; i++)
	{
		// 面片1
		faces.push_back(vec3i(i, (i + 1) % num_samples, (i) + num_samples));
		// 面片2
		faces.push_back(vec3i((i) + num_samples, (i + 1) % num_samples, (i + num_samples + 1) % (num_samples) + num_samples));

		// 面片1对应的顶点的纹理坐标
		vertex_textures.push_back(vec2(1.0 * i / num_samples, 0.0));
		vertex_textures.push_back(vec2(1.0 * (i+1) / num_samples, 0.0));
		vertex_textures.push_back(vec2(1.0 * i / num_samples, 1.0));
		// 对应的三角面片的纹理坐标的下标
		texture_index.push_back( vec3i( 6*i, 6*i+1, 6*i+2 ) );

		// 面片2对应的顶点的纹理坐标
		vertex_textures.push_back(vec2(1.0 * i / num_samples, 1.0));
		vertex_textures.push_back(vec2(1.0 * (i+1) / num_samples, 0.0));
		vertex_textures.push_back(vec2(1.0 * (i+1) / num_samples, 1.0));
		// 对应的三角面片的纹理坐标的下标
		texture_index.push_back( vec3i( 6*i+3, 6*i+4, 6*i+5 ) );
	}

	// 三角面片的每个顶点的法向量的下标，这里和顶点坐标的下标 faces是一致的，所以我们用faces就行
	normal_index = faces;
	// 三角面片的每个顶点的颜色的下标
	color_index = faces;

	storeFacesPoints();
}

void TriMesh::generateDisk(int num_division, float radius)
{
	cleanData();
	
	int num_samples = num_division;
	float step = 2 * M_PI / num_samples; // 每个切片的弧度

	// 按cos和sin生成x，y坐标，z为负，即得到下表面顶点坐标
	float z = 0;
	for (int i = 0; i < num_samples; i++)
	{
		float r_r_r = i * step;
		float x = radius * cos(r_r_r);
		float y = radius * sin(r_r_r);
		vertex_positions.push_back(vec3(x, y, z));
		vertex_normals.push_back(vec3(0, 0, 1));
		// 这里采用法线来生成颜色，可以自定义自己的颜色生成方式
		vertex_colors.push_back(vec3(0, 0, 1));
	}
	// 中心点
	vertex_positions.push_back(vec3(0, 0, 0));
	vertex_normals.push_back(vec3(0, 0, 1));
	vertex_colors.push_back(vec3(0, 0, 1));

	// 生成三角面片，每个矩形由两个三角形面片构成
	for (int i = 0; i < num_samples; i++)
	{
		// 面片1
		faces.push_back(vec3i(i, (i + 1) % num_samples, num_samples));

		// 将0-360度映射到UV坐标的0-1
		for (int j = 0; j < 2; j++)
		{
			float r_r_r = (i + j) * step;
			float x = cos(r_r_r) / 2.0 + 0.5;
			float y = sin(r_r_r) / 2.0 + 0.5;
			// 纹理坐标
			vertex_textures.push_back(vec2(x, y));
		}
		// 纹理坐标 中心
		vertex_textures.push_back(vec2(0.5, 0.5));

		// 对应的三角面片的每个顶点的纹理坐标的下标
		texture_index.push_back(vec3i(3 * i, 3 * i + 1, 3 * i + 2));
	}

	// 三角面片的每个顶点的法向量的下标，这里和顶点坐标的下标 faces是一致的，所以我们用faces就行
	normal_index = faces;
	// 三角面片的每个顶点的颜色的下标
	color_index = faces;

	
	storeFacesPoints();
}

void TriMesh::generateCone(int num_division, float radius, float height)
{
	cleanData();

	int num_samples = num_division;

	float step = 2 * M_PI / num_samples;

	float z = 0;
	for (int i = 0; i < num_samples; i++)
	{
		float r_r_r = i * step;
		float x = radius * cos(r_r_r);
		float y = radius * sin(r_r_r);

		vertex_positions.push_back(vec3(x, y, z));
		vertex_normals.push_back( normalize(vec3(x, y, 0)) );
		vertex_colors.push_back( normalize(vec3(x, y, 0)) );
	}

	vertex_positions.push_back(vec3(0, 0, height));
	vertex_normals.push_back(vec3(0, 0, 1));
	vertex_colors.push_back(vec3(0, 0, 1));

	for (int i = 0; i < (num_samples); i++)
	{
		// 三角面片
		faces.push_back( vec3i( num_samples, (i) % (num_samples), (i + 1) % (num_samples) ) );
		// 该面片每个顶点的纹理坐标
		vertex_textures.push_back(vec2(0.5, 1-0));
		vertex_textures.push_back(vec2( 1.0 * (i) / num_samples , 1-1));
		vertex_textures.push_back(vec2( 1.0 * (i + 1) / num_samples , 1-1));
		// 该三角面片的每个顶点的纹理坐标的下标
		texture_index.push_back(vec3i( 3*i, 3*i+1, 3*i+2 ));
	}


	// 三角面片的每个顶点的法向量的下标，这里和顶点坐标的下标 faces是一致的，所以我们用faces就行
	normal_index = faces;
	// 三角面片的每个顶点的颜色的下标
	color_index = faces;

	storeFacesPoints();
}

void TriMesh::readOff(const std::string &filename)
{
	// fin打开文件读取文件信息
	if (filename.empty())
	{
		return;
	}
	std::ifstream fin;
	fin.open(filename);
	if (!fin)
	{
		printf("File on error\n");
		return;
	}
	else
	{
		printf("File open success\n");

		cleanData();

		int nVertices, nFaces, nEdges;

		// 读取OFF字符串
		std::string str;
		fin >> str;
		// 读取文件中顶点数、面片数、边数
		fin >> nVertices >> nFaces >> nEdges;
		// 根据顶点数，循环读取每个顶点坐标
		for (int i = 0; i < nVertices; i++)
		{
			vec3 tmp_node;
			fin >> tmp_node.x >> tmp_node.y >> tmp_node.z;
			vertex_positions.push_back(tmp_node);
			vertex_colors.push_back(tmp_node);
		}
		// 根据面片数，循环读取每个面片信息，并用构建的vec3i结构体保存
		for (int i = 0; i < nFaces; i++)
		{
			int num, a, b, c;
			// num记录此面片由几个顶点构成，a、b、c为构成该面片顶点序号
			fin >> num >> a >> b >> c;
			faces.push_back(vec3i(a, b, c));
		}
	}
	fin.close();

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();
};

void TriMesh::readObj(const std::string& filename)
{
	std::ifstream fin(filename);
	std::string line;
	if (!fin) {
		std::cout << "ERROR: cannot open the file: " << filename << std::endl;
		exit(0);	// 退出程序
	}
	cleanData();
	// 逐行读取obj文件
	while (std::getline(fin, line))
	{
		std::istringstream sin(line);
		std::string type;
		GLfloat _x, _y, _z;
		int a0 = 0, b0 = 0, c0 = 0;
		int a1 = 0, b1 = 0, c1 = 0;
		int a2 = 0, b2 = 0, c2 = 0;
		char slash;
		sin >> type;  // 读取数据类型(v & vt & vn & f)
		// 顶点坐标
		if (type == "v") {
			sin >> _x >> _y >> _z;
			vertex_positions.emplace_back(vec3(_x, _y, _z));
		}
		// 顶点法向量
		else if (type == "vn") {
			sin >> _x >> _y >> _z;
			vertex_normals.emplace_back(vec3(_x, _y, _z));
		}
		// 顶点纹理坐标	
		else if (type == "vt") {
			sin >> _x >> _y;
			vertex_textures.emplace_back(vec2(_x, _y));
		}
		// faces (针对三角面片)
		else  if (type == "f") {
			sin >> a0 >> slash >> b0 >> slash >> c0;
			sin >> a1 >> slash >> b1 >> slash >> c1;
			sin >> a2 >> slash >> b2 >> slash >> c2;

			// 存储面片对应的顶点下标
			faces.emplace_back(vec3i(a0 - 1, a1 - 1, a2 - 1));
			// 存储纹理映射对应的下标
			texture_index.emplace_back(vec3i(b0 - 1, b1 - 1, b2 - 1));
			// 存储法向量对应的顶点下标
			normal_index.emplace_back(vec3i(c0 - 1, c1 - 1, c2 - 1));
		}
	}

	// 其中vertex_color和color_index可以用法向量的数值赋值
	vertex_colors = vertex_normals;
	color_index = normal_index;

	storeFacesPoints();
}

// 接受键盘响应
void TriMesh::keyboard(int key) {
	if (key == GLFW_KEY_KP_ADD) {
		setScale(getScale() + vec3(DELTA_DELTA, DELTA_DELTA, DELTA_DELTA));
	}
	if (key == GLFW_KEY_KP_SUBTRACT) {
		// 放缩不要是负数
		if (getScale().x > DELTA_DELTA && getScale().y > DELTA_DELTA && getScale().z > DELTA_DELTA) {
			setScale(getScale() - vec3(DELTA_DELTA, DELTA_DELTA, DELTA_DELTA));
		}
	}
}


// Light
mat4 Light::getShadowProjectionMatrix()
{
	// 这里只实现了Y=0平面上的阴影投影矩阵，其他情况自己补充
	float lx, ly, lz;

	mat4 modelMatrix = this->getModelMatrix();
	vec4 light_position = modelMatrix * this->translation;

	lx = light_position[0];
	ly = light_position[1];
	lz = light_position[2];

	return mat4(
		-ly, 0.0, 0.0, 0.0,
		lx, 0.0, lz, 1.0,
		0.0, 0.0, -ly, 0.0,
		0.0, 0.0, 0.0, -ly);
}

Light::Light(bool is_normalize):TriMesh(is_normalize) {
	
}

// 设置衰减系数的参数
void Light::setAttenuation(float _constant, float _linear, float _quadratic)
{
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}

float Light::getConstant() { return constant; };
float Light::getLinear() { return linear; };
float Light::getQuadratic() { return quadratic; };
