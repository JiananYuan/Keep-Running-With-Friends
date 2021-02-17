#version 330 core

// ������ɫ��
in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;
in vec2 vTexture;

// ����ƬԪ��ɫ���ı���
out vec3 position;
out vec3 normal;
out vec2 texCoord;

// ģ�ͱ任��������۲����ͶӰ����
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	vec4 v1 = model * vec4(vPosition, 1.0);  
	// ����model�����п���Ϊ��Ӱ����Ϊ�˵õ���ȷλ�ã�������Ҫ��һ��͸�ӳ���
	vec4 v2 = vec4(v1.xyz / v1.w, 1.0);
	// ���������ͶӰ
	vec4 v3 = projection* view * v2;

	gl_Position = v3;

    position = vec3(v2.xyz);
    normal = vec3( (model *  vec4(vNormal, 0.0)).xyz );
	texCoord = vTexture;

}