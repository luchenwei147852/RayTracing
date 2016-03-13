#include "common.h"
#include "math.h"

/*Ĭ�Ϲ��캯��*/
Vector3::Vector3()
{
	x=0;
	y=0;
	z=0;
}

Vector3::Vector3(float a_x, float a_y, float a_z)
{
	x=a_x; 
	y=a_y; 
	z=a_z;
}

Vector3::Vector3(Vector3 &a_v)
{
	x=a_v.x; 
	y=a_v.y; 
	z=a_v.z;
}

/*��Ա����*/

/*�޸�������Ա����ֵ*/
void Vector3::set(float a_x, float a_y, float a_z)
{
	x=a_x;
	y=a_y;
	z=a_z;
}

void Vector3::set(Vector3 &a_v)
{
	x=a_v.x;
	y=a_v.y;
	z=a_v.z;
}

/*������λ��*/
void Vector3::normalize()
{
	float l=1.0f/sqrt(x*x+y*y+z*z);
	x*=l;
	y*=l;
	z*=l;
}

float Vector3::abs(){
	return sqrt(x*x + y*y + z*z);
}

float Vector3::distance(Vector3 &p){
	return (*this - p).abs();
}
/*���������*/
float Vector3::dot(Vector3 a_v)
{
	return x*a_v.x+y*a_v.y+z*a_v.z;
}
/*���������*/
Vector3 Vector3::cross(Vector3 a_v)
{
	return Vector3(y*a_v.z-z*a_v.y, z*a_v.x-x*a_v.z, x*a_v.y-y*a_v.x);
}

/*���ز�����*/

/*���ز�����-����*/
Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

/*��Ԫ����*/

/*�������ӷ�*/
Vector3 operator + (const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/*����������*/
Vector3 operator- (const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/*�����˸������������ĸ������ֱ�˸�����*/
Vector3 operator* (const Vector3& v, float f)
{
	return Vector3(v.x * f, v.y * f, v.z * f);
}

/*�������������������ĸ������ֱ�˸�����*/
Vector3 operator* (float f, const Vector3& v)
{
	return Vector3(v.x * f, v.y * f, v.z * f);
}

/*���������˷��������ĸ�������Ӧ���*/
Vector3 operator* (const Vector3& v1, Vector3& v2)
{
	return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

/*�������Ƚ�*/
bool operator== (const Vector3& v1, const Vector3& v2)
{
	if (v1.x == v2.x&&v1.y == v2.y&&v1.z == v2.z)
		return true;
	else
		return false;
}