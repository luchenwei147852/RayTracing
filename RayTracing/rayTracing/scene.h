#ifndef _SCENE_H_
#define _SCENE_H_

#include <stdlib.h>
#include "common.h"
#include <iostream>
#include <windows.h>

//射线类
class Ray
{
public:
	Ray();
	Ray(Vector3& org,Vector3& drct);
	
	const Vector3& get_origin();/*读取射线起点*/
	const Vector3& get_direction();/*读取射线方向*/
	void set_origin(Vector3& org);/*设置射线起点*/
	void set_direction(Vector3& drct);/*设置射线方向*/
	Vector3 get_point(float d);/*获取射线上离起点距离d的某点*/

private:
	Vector3 m_origin;/*起点*/
	Vector3 m_direction;/*方向*/
};

/*一般物体类*/
class Object
{
public:
	Object();
	float get_spec();
	float get_refl();
	float get_diffuse();
	void reset(float spec, float refl);/*重新设置成员变量值*/
	virtual Vector3 get_color(Vector3 position) = 0;
	virtual Vector3 get_normal(Vector3 point) = 0;/*获取物体表面一点的法线*/
	virtual INTERSECTION_TYPE is_intersected(Ray ray,float& dst) = 0;/*判断光线是否与物体相交*/
	int style;
protected:
	float m_spec;/*镜面反射强度*/
	float m_refl;/*环境反射强度*/
};

/*球体类*/
class Sphere:public Object
{
public:
	Sphere();
	Sphere(Vector3 cent, float radi);
	Sphere(Vector3 cent, float radi, Vector3 color, float spec, float refl);
	Vector3 get_color(Vector3 position);
	Vector3 get_center();
	float get_radius();
	void reset_c_r(Vector3 cent, float radi, Vector3 color);/*重新设置球心和半径和颜色*/
	Vector3 get_normal(Vector3 point);/*获取球体表面一点的法线*/
	INTERSECTION_TYPE is_intersected(Ray ray,float& dst);/*判断光线是否与球体相交*/

protected:
	Color m_color;/*颜色*/
	Vector3 m_center;/*球心*/
	float m_radius;/*半径*/
};

/*点光源类*/
class PointLight
{
public:
	PointLight();
	Vector3 get_pst();
	Color get_color();
	void reset(Vector3 position, Vector3 color);/*重新设置光源位置和颜色*/
private:
	Vector3 m_position;/*位置*/
	Color m_color;/*颜色*/
};

/*三角形类*/
class Triangle :public Object
{
public:
	Triangle();
	Triangle(Vector3 nor, Vector3 po1, Vector3 po2, Vector3 po3);
	Triangle(Vector3 nor, Vector3 po1, Vector3 po2, Vector3 po3, Vector3 color1, Vector3 color2, Vector3 color3, float spec, float refl);
	Vector3 get_color(Vector3 position);
	float get_distance();
	bool get_equal();
	Vector3 get_normal(Vector3 position);/*获取平板表面一点的法线*/
	bool InTriangle(const Vector3& orig, const Vector3& dir, float* t, float* u, float* v);
	INTERSECTION_TYPE is_intersected(Ray ray, float& dist);/*判断光线是否与平板相交*/

private:
	Color m_color[3];/*颜色*/
	Vector3 position[3];/*平板法向量*/
	Vector3 m_normal;/*平板法向量*/
	bool is_color_equal;/*三个顶点颜色是否一样*/
};

/*场景类*/
class Scene
{
public:
	Scene();
	~Scene();

	void init_scene();/*初始化场景*/
	int get_o_num();/*场景中物体数量*/
	int get_l_num();/*场景中光源数量*/
	void set_amblight(Color amb);/*设置环境光*/
	Color get_amblight();/*获取环境光*/
	Object* get_objects( int idx );/*取出编号为idx的物体*/
	PointLight get_light(int idx);/*取出编号为idx的光源*/
	Object** m_obj;/*物体实例*/
	PointLight* m_light;/*光源*/
	int loadScene(char *filename);
	void parse_check(char *expected, char *found);
	void parse_doubles(FILE*file, char *check, double p[3]);
	void parse_rad(FILE*file, double *r);
	void parse_shi(FILE*file, double *shi);
private:
	int m_obj_num;/*物体数量*/
	int m_lig_num;/*光源数量*/
	Color amb_light;/*环境光*/
};

#endif /*_SCENE_H_*/