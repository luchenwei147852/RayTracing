#ifndef _SCENE_H_
#define _SCENE_H_

#include <stdlib.h>
#include "common.h"
#include <iostream>
#include <windows.h>

//������
class Ray
{
public:
	Ray();
	Ray(Vector3& org,Vector3& drct);
	
	const Vector3& get_origin();/*��ȡ�������*/
	const Vector3& get_direction();/*��ȡ���߷���*/
	void set_origin(Vector3& org);/*�����������*/
	void set_direction(Vector3& drct);/*�������߷���*/
	Vector3 get_point(float d);/*��ȡ��������������d��ĳ��*/

private:
	Vector3 m_origin;/*���*/
	Vector3 m_direction;/*����*/
};

/*һ��������*/
class Object
{
public:
	Object();
	float get_spec();
	float get_refl();
	float get_diffuse();
	void reset(float spec, float refl);/*�������ó�Ա����ֵ*/
	virtual Vector3 get_color(Vector3 position) = 0;
	virtual Vector3 get_normal(Vector3 point) = 0;/*��ȡ�������һ��ķ���*/
	virtual INTERSECTION_TYPE is_intersected(Ray ray,float& dst) = 0;/*�жϹ����Ƿ��������ཻ*/
	int style;
protected:
	float m_spec;/*���淴��ǿ��*/
	float m_refl;/*��������ǿ��*/
};

/*������*/
class Sphere:public Object
{
public:
	Sphere();
	Sphere(Vector3 cent, float radi);
	Sphere(Vector3 cent, float radi, Vector3 color, float spec, float refl);
	Vector3 get_color(Vector3 position);
	Vector3 get_center();
	float get_radius();
	void reset_c_r(Vector3 cent, float radi, Vector3 color);/*�����������ĺͰ뾶����ɫ*/
	Vector3 get_normal(Vector3 point);/*��ȡ�������һ��ķ���*/
	INTERSECTION_TYPE is_intersected(Ray ray,float& dst);/*�жϹ����Ƿ��������ཻ*/

protected:
	Color m_color;/*��ɫ*/
	Vector3 m_center;/*����*/
	float m_radius;/*�뾶*/
};

/*���Դ��*/
class PointLight
{
public:
	PointLight();
	Vector3 get_pst();
	Color get_color();
	void reset(Vector3 position, Vector3 color);/*�������ù�Դλ�ú���ɫ*/
private:
	Vector3 m_position;/*λ��*/
	Color m_color;/*��ɫ*/
};

/*��������*/
class Triangle :public Object
{
public:
	Triangle();
	Triangle(Vector3 nor, Vector3 po1, Vector3 po2, Vector3 po3);
	Triangle(Vector3 nor, Vector3 po1, Vector3 po2, Vector3 po3, Vector3 color1, Vector3 color2, Vector3 color3, float spec, float refl);
	Vector3 get_color(Vector3 position);
	float get_distance();
	bool get_equal();
	Vector3 get_normal(Vector3 position);/*��ȡƽ�����һ��ķ���*/
	bool InTriangle(const Vector3& orig, const Vector3& dir, float* t, float* u, float* v);
	INTERSECTION_TYPE is_intersected(Ray ray, float& dist);/*�жϹ����Ƿ���ƽ���ཻ*/

private:
	Color m_color[3];/*��ɫ*/
	Vector3 position[3];/*ƽ�巨����*/
	Vector3 m_normal;/*ƽ�巨����*/
	bool is_color_equal;/*����������ɫ�Ƿ�һ��*/
};

/*������*/
class Scene
{
public:
	Scene();
	~Scene();

	void init_scene();/*��ʼ������*/
	int get_o_num();/*��������������*/
	int get_l_num();/*�����й�Դ����*/
	void set_amblight(Color amb);/*���û�����*/
	Color get_amblight();/*��ȡ������*/
	Object* get_objects( int idx );/*ȡ�����Ϊidx������*/
	PointLight get_light(int idx);/*ȡ�����Ϊidx�Ĺ�Դ*/
	Object** m_obj;/*����ʵ��*/
	PointLight* m_light;/*��Դ*/
	int loadScene(char *filename);
	void parse_check(char *expected, char *found);
	void parse_doubles(FILE*file, char *check, double p[3]);
	void parse_rad(FILE*file, double *r);
	void parse_shi(FILE*file, double *shi);
private:
	int m_obj_num;/*��������*/
	int m_lig_num;/*��Դ����*/
	Color amb_light;/*������*/
};

#endif /*_SCENE_H_*/