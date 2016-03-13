#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "common.h"
#include "scene.h"

//���߸�����
class Tracer
{
public:
	Tracer();
	~Tracer();
	
	//���ٺ��������������е��������壬���ؽ��㴦����ɫ
	Vector3 trace(Ray ray,int depth);
	void render();//����
	Scene* m_scn;//����
	Vector3 m_eye;//�۾�λ��	
	Color screen_color[VIEW_PIXEL_WIDTH][VIEW_PIXEL_HEIGHT];//�洢��Ļ�����ص㣨x,y��������ɫ
	unsigned char buffer[VIEW_PIXEL_WIDTH][VIEW_PIXEL_HEIGHT][3];
	void save_jpg();
};

#endif 