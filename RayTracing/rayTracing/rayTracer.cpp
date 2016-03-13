#include "rayTracer.h"
#include "common.h"
#include <iostream>
#include <omp.h>
#include "picLibrary/pic.h"

Tracer::Tracer()
{	
	m_scn = new Scene();
}

Tracer::~Tracer()
{
	delete m_scn;
 }

/*跟踪函数，遍历场景中的所有物体，返回交点处的颜色*/
Vector3 Tracer::trace(Ray ray,int depth)
{
	float distance = INFINITY;/*初始化无限大距离*/
	int dep = depth;/*递归深度*/
	Object* aim;/*交点物体*/
	Vector3 point;/*交点*/
	Vector3 n;/*交点处的法向量*/
	Vector3 l;/*交点到光源的方向*/
	Vector3 v;/*交点到眼睛的方向*/
	Vector3 r;/*光线沿l的入射光线在交点处的出射方向*/
	Color hit_color = m_scn->get_amblight();/*交点处的颜色*/
	int res_hit;/*相交情况结果*/

	/*遍历场景中的每一个物体,获取其中的一个离眼睛最近的点所在物体*/
	for(int k=0;k<m_scn->get_o_num();k++)
	{
		Object* obj = m_scn->get_objects(k);/*当前物体*/
		int res;
		if(res = obj->is_intersected(ray,distance))/*判断是否有交点，有交点改变distance*/
		{
			aim = obj;/*获取目标物体*/
			res_hit = res;
		}
	}
	
	/*有相交物体*/
	if(distance != INFINITY)
	{
		point = ray.get_point(distance);/*获取交点*/
		n = aim->get_normal(point);/*交点处的法向量*/
		n.normalize();/*单位化*/
		/*在物体内与物体相交*/
		if(res_hit == INTERSECTED_IN)
		{
			n = -n;
		}

		v = m_eye - point;/*交点到眼睛的方向*/
		v.normalize();

		/*遍历场景中的每一个光源*/
		for(int k = 0; k < m_scn->get_l_num(); k++)
		{
			PointLight pl = m_scn->get_light(k);/*光源*/
			l = pl.get_pst() - point;/*交点到光源的方向*/
			l.normalize();
			//if (aim->style == 2) printf("交点到光源的方向:(%f,%f,%f)\n", l.x, l.y, l.z);

			Ray l_ray = Ray(point + l*SMALL, l);/*构造交点到光源的射线*/

			float shade = 1.0f;/*阴影,1.0表示交点不在阴影中*/
			float distance = INFINITY;/*初始化无限大距离*/

			/*测试是否有物体遮挡住光源*/
			for(int k=0;k<m_scn->get_o_num();k++)
			{
				Object* obj = m_scn->get_objects(k);/*当前物体*/
				if(obj->is_intersected(l_ray,distance))
				{
					shade = 0.0f;
					break;
				}
			}
			
			/*漫反射*/
			if(aim->get_diffuse() > 0)
			{
				float cos = DOT(l,n);/*光线与交点处法线的夹角的余弦*/
				if(cos > 0)
				{
					float diffuse = cos*aim->get_diffuse()*shade;
					/*将漫反射光加到光线颜色中*/
					hit_color = hit_color + diffuse*pl.get_color()*aim->get_color(point);
				}
			}

			/*镜面反射*/
			if(aim->get_spec() > 0)
			{
				/*Phong高光*/
				Vector3 h = 2*DOT(n,l)*n - l;/*反射光线*/
				float cos = DOT(h,v);/*反射光线与视线方向的夹角余弦*/
				if(cos > 0)
				{
					float specular = powf(cos,20)*aim->get_spec()*shade;
					hit_color = hit_color + specular*pl.get_color();
				}
			}
		}
		
		/*反射光线*/
		if(aim->get_refl() > 0 && dep < TOTALDEPTH)
		{
			Vector3 refl = 2*DOT(n,l)*n - l;//反射光线
			hit_color = hit_color + trace(Ray(point + refl*0.0001f, refl), ++dep);
		}
	}
	else{
		//return Color(1.0,1.0,1.0);
	}
	return hit_color;/*没有相交时，是初始黑色；有相交时，是交点处的颜色*/
}

/*绘制，获取颜色信息*/
void Tracer::render()
{
	m_eye.set(0.0f,0.0f,4.0f);/*眼睛位置*/
	m_scn->init_scene();/*读入场景*/

	/*视窗坐标增量*/
	static float dx = (float)(VIEW_WIDTH / VIEW_PIXEL_WIDTH);
	static float dy = (float)(VIEW_HEIGHT/VIEW_PIXEL_HEIGHT);

	/*视窗初始坐标*/
	float view_x = - VIEW_WIDTH/2.0;
	float view_y = - VIEW_HEIGHT/2.0;
	int percent;
	int a = 1;
	int i, j;
	#pragma omp parallel for private(i,j,view_x,view_y) 
	/*遍历所有像素点*/
	for(i=0; i<VIEW_PIXEL_HEIGHT; i++)
	{
		view_x = - VIEW_WIDTH/2.0;
		view_y = dy*i - VIEW_HEIGHT / 2.0;
		for(j=0; j<VIEW_PIXEL_WIDTH; j++)
		{
		   Vector3 dir = Vector3(view_x,view_y,0) - m_eye;/*射线方向*/
		   dir.normalize();
		   Ray ray(m_eye, dir);/*射线*/
		   screen_color[i][j] = trace(ray,1);/*遍历场景中的所有物体*/
		   view_x = dx*(j + 1) - VIEW_WIDTH / 2.0;
		}
		percent = i % 10;
		if (percent == 0){
			char t = 8;
			for (int k = 0; k<11; k++){
				std::cout << t;
			}
			std::cout << "进度：" << a++ << "%";
		}
	}
}

void Tracer::save_jpg()
{
	//int width_trans = VIEW_PIXEL_WIDTH/JPG_PIXEL_WIDTH;
	//int height_trans = VIEW_PIXEL_HEIGHT / JPG_PIXEL_HEIGHT;
	for (int i = 0; i<VIEW_PIXEL_WIDTH; i++)
	{
		for (int j = 0; j<VIEW_PIXEL_HEIGHT; j++)
		{
			/*buffer[JPG_PIXEL_WIDTH - i - 1][j][0] = 256 * screen_color[width_trans*i][height_trans*j].x;
			buffer[JPG_PIXEL_WIDTH - i - 1][j][1] = 256 * screen_color[width_trans*i][height_trans*j].y;
			buffer[JPG_PIXEL_WIDTH - i - 1][j][2] = 256 * screen_color[width_trans*i][height_trans*j].z;*/
			buffer[VIEW_PIXEL_WIDTH - i - 1][j][0] = 256 * screen_color[i][j].x;
			buffer[VIEW_PIXEL_WIDTH - i - 1][j][1] = 256 * screen_color[i][j].y;
			buffer[VIEW_PIXEL_WIDTH - i - 1][j][2] = 256 * screen_color[i][j].z;
		}
	}
	Pic *in = NULL;
	char *filename = "result.jpg";
	in = pic_alloc(VIEW_PIXEL_WIDTH, VIEW_PIXEL_HEIGHT, 3, NULL);
	printf("Saving JPEG file: %s\n", filename);

	memcpy(in->pix, buffer, 3 * VIEW_PIXEL_WIDTH * VIEW_PIXEL_HEIGHT);
	if (jpeg_write(filename, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}