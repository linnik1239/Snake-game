//#pragma once
//#include "Bezier1D.h"
//
//class Bezier2D
//{
//	int circularSubdivision; //usualy 4, how many subdivision in circular direction
//	Bezier1D b;				 
//
//public:
//	Bezier2D(void);
//	Bezier2D(Bezier1D &b,glm::vec3 axis,int circularSubdivision);  
//
//	IndexedModel GetSurface(int resT,int resS);						//generates model for rendering using MeshConstructor::initMeshs
//	Vertex GetVertex(int segmentT,int segmentS,float t,float s);		//returns point on surface in the requested segments for value of t and s
//	glm::vec3 GetNormal(int segmentT,int segmentS,float t,float s);		//returns point on surface in the requested segments for value of t and s
//
//	~Bezier2D(void);
//};
//

#pragma once
#include "Bezier1D.h"



//std::list<glm::vec4> theListHalfSphere;

class Bezier2D
{
	int circularSubdivision; //usualy 4 how many subdivision in circular direction
	Bezier1D b;
	glm::vec3 axis;

public:
	Bezier2D(void);
	Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision);

	IndexedModel GetSurface(int resT, int resS);
	IndexedModel GetSurface2(int resT, int resS, float radius, float lineSize);
	IndexedModel GetSurface3(int resT, int resS);
	IndexedModel GetSurface4(int resT, int resS);
	IndexedModel GetSurfaceBigCyllinder(int resT, int resS, float radius);
	IndexedModel GetSurfaceFLAT(int resT, int resS);

	glm::vec3 calcWeight(int segmentT, int segmentS, float t, float s);

	//generates model for rendering using MeshConstructor::initMeshs
	Vertex GetVertex(int segmentT, float segmentS, float t, float s);
	Vertex GetVertex2(int segmentT, float segmentS, float t, float s, float radius, float lineSize);
	Vertex GetVertex3(int segmentT, float segmentS, float t, float s);
	Vertex GetVertex4(int segmentT, float segmentS, float t, float s);



	//returns point on surface in the requested segments for value of t and s
	glm::vec3 GetNormal(int segmentT, int segmentS, float t, float s);
	//returns point on surface in the requested segments for value of t and s


	~Bezier2D(void);
};

