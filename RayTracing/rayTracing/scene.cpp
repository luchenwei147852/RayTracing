#include "scene.h"
#include "common.h"
#include <iostream>

/*射线类*/

/*构造函数*/
Ray::Ray()
{
	m_origin = Vector3(0,0,0);
	m_direction = Vector3(0,0,1);
}

Ray::Ray(Vector3 &org, Vector3 &drct)
{
	m_origin = org;
	m_direction = drct;
}

/*读取射线起点*/
const Vector3& Ray::get_origin()
{
	return m_origin;
}

/*读取射线方向*/
const Vector3& Ray::get_direction()
{
	return m_direction;
}

/*设置起点*/
void Ray::set_origin(Vector3& org)
{
	m_origin = org;
}

/*设置射线方向*/
void Ray::set_direction(Vector3& drct)
{
	m_direction = drct;
}

/*获取射线上离起点距离为d的某点*/
Vector3 Ray::get_point(float d)
{
	return m_origin+d*m_direction;
}

/*一般物体类*/

/*构造函数*/
Object::Object()
{
	m_spec = 1.0f;
	m_refl = 1.0f;
}

float Object::get_spec()
{
	return m_spec;
}

float Object::get_refl()
{
	return m_refl;
}

float Object::get_diffuse()
{
	return 1.0f - m_spec;
}

/*重新设置成员变量值*/
void Object::reset(float spec, float refl)
{
	m_spec = spec;
	m_refl = refl;
}

/*球体类*/
Sphere::Sphere()
{
	m_color = Vector3(0, 0, 0);
	m_center = Vector3(0,0,0);
	m_radius = 1.0f;
	style = 0;
}

Sphere::Sphere(Vector3 cent, float radi)
{
	m_color = Vector3(0, 0, 0);
	m_center = cent;
	m_radius = radi;
	style = 0;
}

Sphere::Sphere(Vector3 cent, float radi, Vector3 color, float spec, float refl)
{
	m_center = cent;
	m_radius = radi;
	m_color = color;
	m_refl = refl;
	m_spec = spec;
	style = 0;
}

/*重新设置球心和半径*/
void Sphere::reset_c_r(Vector3 cent, float radi, Vector3 color)
{
	m_center = cent;
	m_radius = radi;
	m_color = color;
}

Color Sphere::get_color(Vector3 position)
{
	return m_color;
}

Vector3 Sphere::get_center()
{
	return m_center;
}

float Sphere::get_radius()
{
	return m_radius;
}

/*获取球体表面一点的法线*/
Vector3 Sphere::get_normal(Vector3 point)
{
	return (point - m_center);
}

/*判断光线是否与球体相交，若有交点则取最近交点，计算其到眼睛的距离*/
INTERSECTION_TYPE Sphere::is_intersected(Ray ray, float& dst) 
{
	float a = 1.0f;
	Vector3 v = ray.get_origin() - m_center;
	float b = 2*DOT(ray.get_direction(), v);
	float c = DOT(v, v) - m_radius * m_radius;
	float det = b*b-4*a*c;
	INTERSECTION_TYPE retval = MISS;
	if(det>0)
	{
		det = sqrt(det);
		float t1 = (-b-det)/2*a;
		float t2 = (-b+det)/2*a;
		if(t2>0)
		{ 
			if(t1<0)
			{
				if(t2<dst)
				{
					dst = t2;
					retval = INTERSECTED_IN;
				}		
			}
			else
			{
				if(t1<dst)
				{
					dst = t1;
					retval = INTERSECTED;
				}
			}
		}
	}
	return retval;
}

/*光源类*/
PointLight::PointLight()
{
	m_position = Vector3(0,0,0);
	m_color = Vector3(1,1,1);
}

Vector3 PointLight::get_pst()
{
	return m_position;
}

Color PointLight::get_color()
{
	return m_color;
}

/*重新设置光源位置和颜色*/
void PointLight::reset(Vector3 position, Vector3 color)
{
	m_position = position;
	m_color = color;
}

/*三角形类*/
Triangle::Triangle() :m_normal(0, 0, 0)
{
	style = 2;
}

Triangle::Triangle(Vector3 nor, Vector3 po1, Vector3 po2, Vector3 po3)
{
	m_normal = nor;
	position[0] = po1;
	position[1] = po2;
	position[2] = po3;
	style = 2;
}

Triangle::Triangle(Vector3 nor, Vector3 po1, Vector3 po2, Vector3 po3, Vector3 color1, Vector3 color2, Vector3 color3, float spec, float refl)
{
	position[0] = po1;
	position[1] = po2;
	position[2] = po3;
	m_color[0] = color1;
	m_color[1] = color2;
	m_color[2] = color3;
	if (color1 == color2&&color1 == color3){
		is_color_equal = true;
	}
	else{
		is_color_equal = false;
	}
	m_normal = nor;
	m_refl = refl;
	m_spec = spec;
	style = 2;
}

Vector3 Triangle::get_color(Vector3 pos)
{
	//printf("获得颜色！\n");
	if (is_color_equal) return m_color[0];
	else{
		//printf("clolor:<%f,%f,%f>\n", m_color[0], m_color[1], m_color[2]);
		float dis1 = pos.distance(position[0]);
		float dis2 = pos.distance(position[1]);
		float dis3 = pos.distance(position[2]);
		float dis = dis1 + dis2 + dis3;
		dis1 /= dis;
		dis2 /= dis;
		dis3 /= dis;
		return dis1*m_color[0] + dis2*m_color[1] + dis3*m_color[2];
	}
}

Vector3 Triangle::get_normal(Vector3 position)
{
	Vector3 p = position;
	return m_normal;
}

INTERSECTION_TYPE Triangle::is_intersected(Ray ray, float& dist)
{
	float cos = DOT(m_normal, ray.get_direction());
	if (cos != 0)
	{
		float t = 0.0, u = 0.0, v = 0.0;
		if (InTriangle(ray.get_origin(), ray.get_direction(), &t, &u, &v))
		{
			if (t>0 && t < dist)
			{
				dist = t;
				return INTERSECTED;
			}
		}
	}
	return MISS;
}

bool Triangle::InTriangle(const Vector3& orig, const Vector3& dir, float* t, float* u, float* v)
{
	// E1
	Vector3 E1 = position[1] - position[0];

	// E2
	Vector3 E2 = position[2] - position[0];

	// P
	Vector3 P(dir.y*E2.z - dir.z*E2.y, dir.z*E2.x - dir.x*E2.z, dir.x*E2.y - dir.y*E2.x);

	// determinant
	float det = E1.dot(P);

	// keep det > 0, modify T accordingly
	Vector3 T;
	if (det >0)
	{
		T = orig - position[0];
	}
	else
	{
		T = position[0] - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0001f)
		return false;

	*u = T.dot(P);
	if (*u < 0.0f || *u > det)
		return false;

	// Q
	Vector3 Q = T.cross(E1);

	// Calculate v and make sure u + v <= 1
	*v = dir.x*Q.x + dir.y*Q.y + dir.z*Q.z;;
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = E2.dot(Q);

	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

    return true;
}
/*场景类*/

Scene::Scene():m_obj_num(0),m_lig_num(0)
{
}

Scene::~Scene()
{
	delete m_obj;
	delete m_light;
}

/*初始化场景*/
void Scene::init_scene()
{
	/*
	m_light = new PointLight[100];
	int i = 0;//计算光源个数
	m_light[i].reset(Vector3(0.0f, 10.0f, -35.0f), Vector3(1.0, 1.0, 1.0));//光源1
	//m_light[i].reset(Vector3(0.0f, 5.0f, 5.0f), Vector3(1.5, 1.5, 1.5));//光源1
	//m_light[++i].reset(Vector3(0,5.0f,-5.0f), Vector3(1,1,1));//光源2
	m_lig_num = i + 1;

	//场景中物体列表
	m_obj = new Object*[100];
	int j = 0;//计算场景中物体个数

	//球
	//m_obj[j] = new Sphere(Vector3(0.0, 0.0, -3.0), 1.0f);//球1，球心位置，半径
	//m_obj[j]->reset(Vector3(0.9f, 0.9f, 0.1f), 0.5f, 0.3f);//颜色，镜面反射系数，环境反射系数
	//m_obj[++j] = new Sphere(Vector3(-3,0,-5),2.0f);//球2
	//m_obj[j]->reset(Vector3(0.7f,0.7f,1.0f), 0.9f, 0.2f);
	//三角形
	m_obj[j] = new Triangle(Vector3(0, 1, 0), Vector3(-125.0, -5.0, -70.0), Vector3(125.0, -5.0, -70.0), Vector3(-125.0, -5.0, 0));//法向量朝上的三角形
	m_obj[j]->reset(Vector3(0.5f, 0.5f, 0.5f), 0.5f, 1.0);
	m_obj[++j] = new Triangle(Vector3(0, 1, 0), Vector3(125.0, -5.0, -70.0), Vector3(-125.0, -5.0, 0), Vector3(125.0, -5.0, 0));//法向量朝上的三角形
	m_obj[j]->reset(Vector3(0.5f, 0.5f, 0.5f), 0.5f, 1.0);
	//平板
	//m_obj[++j] = new Plane(Vector3(0, 1, 0), 100.0f);//法向量朝上的平板
	//m_obj[j]->reset(Vector3(0.5f, 0.8f, 0.5f), 0.4f, 0.2f);
	//m_obj[++j] = new Plane(Vector3(1,0,0),100.0f);
	//m_obj[j]->reset(Vector3(0.5f, 0.5f, 0.8f), 0.4f, 0.2f);

	m_obj_num = j + 1; */
	loadScene("screenfile.txt");
}

void Scene::set_amblight(Color amb)
{
	amb_light = amb;
}

Color Scene::get_amblight()
{
	return amb_light;
}

int Scene::get_o_num()
{
	return m_obj_num;
}

int Scene::get_l_num()
{
	return m_lig_num;
}

/*取出编号为idx的物体*/
Object* Scene::get_objects(int idx)
{
	return m_obj[idx];
}

/*取出编号为idx的光源*/
PointLight Scene::get_light(int idx)
{
	return m_light[idx];
}

int Scene::loadScene(char *filename)
{
	m_light = new PointLight[100];
	m_obj = new Object*[1500];
	FILE *file = fopen(filename, "r");
	int number_of_objects;
	char type[50];
	int i;
	fscanf(file, "%i", &number_of_objects);

	printf("number of objects: %i\n", number_of_objects);
	char str[200];

	double ambient_light[3];
	parse_doubles(file, "amb:", ambient_light);
	amb_light = Color(ambient_light[0], ambient_light[1], ambient_light[2]);

	for (i = 0; i < number_of_objects; i++)
	{
		fscanf(file, "%s\n", type);
		printf("%s\n", type);
		if (stricmp(type, "triangle") == 0)
		{
			printf("found triangle\n");
			int j;
			double position[3][3], normal[3], diffuse[3][3], specular[3], shininess;
			for (j = 0; j < 3; j++)
			{
				parse_doubles(file, "pos:", position[j]);
				parse_doubles(file, "nor:", normal);
				parse_doubles(file, "dif:", diffuse[j]);
				parse_doubles(file, "spe:", specular);
				parse_shi(file, &shininess);
			}
			m_obj[m_obj_num++] = new Triangle(Vector3(normal[0], normal[1], normal[2]), Vector3(position[0][0], position[0][1], position[0][2]), Vector3(position[1][0], position[1][1], position[1][2]), Vector3(position[2][0], position[2][1], position[2][2]), Vector3(diffuse[0][0], diffuse[0][1], diffuse[0][2]), Vector3(diffuse[1][0], diffuse[1][1], diffuse[1][2]), Vector3(diffuse[2][0], diffuse[2][1], diffuse[2][2]), (float)specular[0], 1.0);
			if (m_obj_num == 10000)
			{
				printf("too many triangles, you should increase MAX_TRIANGLES!\n");
				exit(0);
			}
		}
		else if (stricmp(type, "sphere") == 0)
		{
			printf("found sphere\n");
			double position[3], radius, diffuse[3], specular[3], shininess;
			parse_doubles(file, "pos:", position);
			parse_rad(file, &radius);
			parse_doubles(file, "dif:", diffuse);
			parse_doubles(file, "spe:", specular);
			m_obj[m_obj_num++] = new Sphere(Vector3(position[0], position[1], position[2]), radius, Vector3(diffuse[0], diffuse[1], diffuse[2]), specular[0], 1);
			parse_shi(file, &shininess);

			if (m_obj_num == 1000)
			{
				printf("too many spheres, you should increase MAX_SPHERES!\n");
				exit(0);
			}
		}
		else if (stricmp(type, "light") == 0)
		{
			printf("found light\n");
			double pos[3], col[3];
			parse_doubles(file, "pos:", pos);
			parse_doubles(file, "col:", col);

			if (m_lig_num == 10)
			{
				printf("too many lights, you should increase MAX_LIGHTS!\n");
				exit(0);
			}
			m_light[m_lig_num++].reset(Vector3((float)pos[0], (float)pos[1], (float)pos[2]), Vector3((float)col[0], (float)col[1], (float)col[2]));
		}
		else
		{
			printf("unknown type in scene description:\n%s\n", type);
			exit(0);
		}
	}
	printf("对象数目:%d\n光源数目:%d\n", m_obj_num, m_lig_num);
	return 0;
}

void Scene::parse_check(char *expected, char *found)
{
	if (stricmp(expected, found))
	{
		char error[100];
		printf("Expected '%s ' found '%s '\n", expected, found);
		printf("Parse error, abnormal abortion\n");
		exit(0);
	}

}

void Scene::parse_doubles(FILE*file, char *check, double p[3])
{
	char str[100];
	fscanf(file, "%s", str);
	parse_check(check, str);
	fscanf(file, "%lf %lf %lf", &p[0], &p[1], &p[2]);
	printf("%s %lf %lf %lf\n", check, p[0], p[1], p[2]);
}

void Scene::parse_rad(FILE*file, double *r)
{
	char str[100];
	fscanf(file, "%s", str);
	parse_check("rad:", str);
	fscanf(file, "%lf", r);
	printf("rad: %f\n", *r);
}

void Scene::parse_shi(FILE*file, double *shi)
{
	char s[100];
	fscanf(file, "%s", s);
	parse_check("shi:", s);
	fscanf(file, "%lf", shi);
	printf("shi: %f\n", *shi);
}
