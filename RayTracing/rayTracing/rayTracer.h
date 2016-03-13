#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "common.h"
#include "scene.h"

//光线跟踪器
class Tracer
{
public:
	Tracer();
	~Tracer();
	
	//跟踪函数，遍历场景中的所有物体，返回交点处的颜色
	Vector3 trace(Ray ray,int depth);
	void render();//绘制
	Scene* m_scn;//场景
	Vector3 m_eye;//眼睛位置	
	Color screen_color[VIEW_PIXEL_WIDTH][VIEW_PIXEL_HEIGHT];//存储屏幕上像素点（x,y）处的颜色
	unsigned char buffer[VIEW_PIXEL_WIDTH][VIEW_PIXEL_HEIGHT][3];
	void save_jpg();
};

#endif 