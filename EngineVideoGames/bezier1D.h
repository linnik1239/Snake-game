
#pragma once
#include "Mesh.h"
#define NUMSEGMENTS 1
#define DIST_SIZE 1.0  //Distance between cubes, control points.
#define CUBE_SIZE 1 //Size of cube, control point.
#define DELTA 0.000001f

#define FLATSIZE 80

#define SCALE 1.0f
#define SCALE2 0.05f


#define CYLINDER 10
#define BIGCYLINDER 14

#define HALFSPHERE 11
#define HALFSPHERE2 12
#define FLAT 13




class Bezier1D
{
	//each line in the matrix represents x,y,z,w of a control point
	std::vector<glm::mat4> segments;
public:
	Bezier1D(void);
	Bezier1D(glm::mat4 mat);
	IndexedModel GetLine(int resT);						//generates model for rendering using MeshConstructor::initLin
	LineVertex GetVertex(int segment, float t);			//returns point on curve in the requested segment for value of t
	LineVertex GetVertex2(int segment, float t, float radius, float lineSize);			//returns point on curve in the requested segment for value of t
	LineVertex GetVertex3(int segment, float t);			//returns point on curve in the requested segment for value of t
	LineVertex GetVertex4(int segment, float t);			//returns point on curve in the requested segment for value of t



	LineVertex GetControlPoint(int segment, int indx);	//returns a control point in the requested segment. indx will be 0,1,2,3, for p0,p1,p2,p3

	glm::vec3 GetVelosity(int segment, float t);			//returns the derivative of the curve in the requested segment for value of t

	glm::vec3 GetAxis();

	void makeAppend(IndexedModel &inM, int resT, int segment);

	void MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition); //change the positon of one control point. when preserveC1 is true it may affect other  control points 


	~Bezier1D(void);
};

