//#include "bezier1D.h"
//
//
//Bezier1D::Bezier1D(void)
//{
//}
//
//
//Bezier1D::~Bezier1D(void)
//{
//}


#include "bezier1D.h"
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include "MeshConstructor.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>

void Bezier1D::makeAppend(IndexedModel &inM, int resT, int segment) {
	for (int i = 0; i < (resT); ++i) {
		LineVertex v = GetVertex(segment, (float)(i % (resT)) / resT);
		inM.colors.push_back(*(v.GetColor()));
		inM.indices.push_back((unsigned int)(i + segment* resT));

		inM.positions.push_back(*(v.GetPos()));

		inM.normals.push_back(glm::vec3(0, 0, 0));
		inM.weights.push_back(glm::vec3(0, 0, 0));
		inM.texCoords.push_back(glm::vec2(0, 0));
		inM.joint_indices.push_back(glm::ivec3(0, 0, 0));


	}

}

IndexedModel Bezier1D::GetLine(int resT) {
	//generates model for rendering using MeshConstructor::initLin
	IndexedModel inM;
	for (int i = 0; i < NUMSEGMENTS; ++i) {
		makeAppend(inM, resT, i);

	}

	LineVertex v = GetVertex((NUMSEGMENTS - 1), (float)1);
	inM.colors.push_back(*(v.GetColor()));
	inM.indices.push_back((unsigned int)(resT + (NUMSEGMENTS - 1)* resT));

	inM.positions.push_back(*(v.GetPos()));



	inM.normals.push_back(glm::vec3(0, 0, 0));
	inM.weights.push_back(glm::vec3(0, 0, 0));
	inM.texCoords.push_back(glm::vec2(0, 0));
	inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

	return inM;

}


LineVertex Bezier1D::GetVertex(int segment, float t) {
	//returns point on curve in the requested segment for value of t
	glm::mat4 theSegment = segments[segment];
	glm::vec4 vecTransform(0.0f);
	glm::vec4 P0, P1, P2, P3;
	glm::vec3 Pt;

	LineVertex V0 = GetControlPoint(segment, 0);
	LineVertex V1 = GetControlPoint(segment, 1);
	LineVertex V2 = GetControlPoint(segment, 2);
	LineVertex V3 = GetControlPoint(segment, 3);



	float m = 0.25;
	Pt = ((float)pow(t, 3))*(*(V3.GetPos())) +
		((float)(pow(t, 2)*(1 - t) * 3))*(*(V2.GetPos())) +
		((float)(pow(1 - t, 2)*(t)* 3))*(*(V1.GetPos())) +
		((float)pow(1 - t, 3))*(*(V0.GetPos()));





	LineVertex v(Pt, glm::vec3(0.0, 0.0, 0.0));
	return v;

}


LineVertex Bezier1D::GetVertex2(int segment, float t, float radius,float lineSize) {
	//returns point on curve in the requested segment for value of t
	glm::mat4 theSegment = segments[segment];
	glm::vec4 vecTransform(0.0f);
	glm::vec4 P0, P1, P2, P3;
	glm::vec3 Pt;

	LineVertex V0 = GetControlPoint(segment, 0);
	LineVertex V1 = GetControlPoint(segment, 1);
	LineVertex V2 = GetControlPoint(segment, 2);
	LineVertex V3 = GetControlPoint(segment, 3);

	float m = lineSize;
	Pt = ((float)pow(t, 3))*(*(V3.GetPos())) +
		((float)(pow(t, 2)*(1 - t) * 3))*(*(V2.GetPos())) +
		((float)(pow(1 - t, 2)*(t)* 3))*(*(V1.GetPos())) +
		((float)pow(1 - t, 3))*(*(V0.GetPos()));


	LineVertex v(glm::vec3(0, radius, t*(DIST_SIZE * m)), glm::vec3(0.0, 0.0, 0.0));
	return v;

}


LineVertex Bezier1D::GetVertex3(int segment, float t) {
	//returns point on curve in the requested segment for value of t
	glm::mat4 theSegment = segments[segment];
	glm::vec4 vecTransform(0.0f);
	glm::vec4 P0, P1, P2, P3;
	glm::vec3 Pt;

	LineVertex V0 = GetControlPoint(segment, 0);
	LineVertex V1 = GetControlPoint(segment, 1);
	LineVertex V2 = GetControlPoint(segment, 2);
	LineVertex V3 = GetControlPoint(segment, 3);

	float m = 0.25;
	Pt = ((float)pow(t, 3))*(*(V3.GetPos())) +
		((float)(pow(t, 2)*(1 - t) * 3))*(*(V2.GetPos())) +
		((float)(pow(1 - t, 2)*(t)* 3))*(*(V1.GetPos())) +
		((float)pow(1 - t, 3))*(*(V0.GetPos()));

	//DIST_SIZE
	LineVertex v(glm::vec3(abs(sqrt(pow(0.8*DIST_SIZE, 2) - pow(t*(0.8*DIST_SIZE) - (0.8*DIST_SIZE), 2))), 0, t*(DIST_SIZE * m)), glm::vec3(0.0, 0.0, 0.0));
	return v;

}


LineVertex Bezier1D::GetVertex4(int segment, float t) {
	//returns point on curve in the requested segment for value of t
	glm::mat4 theSegment = segments[segment];
	glm::vec4 vecTransform(0.0f);
	glm::vec4 P0, P1, P2, P3;
	glm::vec3 Pt;

	LineVertex V0 = GetControlPoint(segment, 0);
	LineVertex V1 = GetControlPoint(segment, 1);
	LineVertex V2 = GetControlPoint(segment, 2);
	LineVertex V3 = GetControlPoint(segment, 3);

	float m = 0.25;
	Pt = ((float)pow(t, 3))*(*(V3.GetPos())) +
		((float)(pow(t, 2)*(1 - t) * 3))*(*(V2.GetPos())) +
		((float)(pow(1 - t, 2)*(t)* 3))*(*(V1.GetPos())) +
		((float)pow(1 - t, 3))*(*(V0.GetPos()));

	//DIST_SIZE
	LineVertex v(glm::vec3(abs(sqrt(pow(0.8*DIST_SIZE, 2) - pow(t*(0.8*DIST_SIZE), 2))), 0, t*(DIST_SIZE * m)), glm::vec3(0.0, 0.0, 0.0));
	return v;

}




LineVertex Bezier1D::GetControlPoint(int segment, int indx) {
	glm::mat4 theSegment = segments[segment];
	glm::vec4 vecTransform(0);
	glm::vec4 ctrlPoint;
	vecTransform[indx] = 1;
	ctrlPoint = theSegment * vecTransform;



	glm::vec3 point;

	point.x = ctrlPoint.x / ctrlPoint.w;
	point.y = ctrlPoint.y / ctrlPoint.w;
	point.z = ctrlPoint.z / ctrlPoint.w;


	LineVertex v(point, glm::vec3(0.0, 0.0, 0.0));
	return v;

}

glm::vec3 Bezier1D::GetVelosity(int segment, float t) {
	LineVertex vertex1 = GetVertex(segment, t);
	LineVertex vertex2 = GetVertex(segment, t + DELTA);
	glm::vec3 point1 = *vertex1.GetPos();
	glm::vec3 point2 = *vertex2.GetPos();

	return glm::vec3((point1.x - point2.x) / DELTA, (point1.y - point2.y) / DELTA, (point1.z - point2.z) / DELTA);


}


void  Bezier1D::MoveControlPoint(int segment, int indx, bool preserveC1, glm::vec4 newPosition) {


	if (preserveC1) {

		if (indx == 1) {
			glm::vec4 pldPos = this->segments[segment][indx];
			glm::vec4 delta = newPosition - pldPos;
			this->segments[segment][indx] = newPosition;
			this->segments[segment - 1][2] -= delta;
		}


		if (indx == 2) {
			glm::vec4 pldPos = this->segments[segment][indx];
			glm::vec4 delta = newPosition - pldPos;
			this->segments[segment][indx] = newPosition;
			this->segments[segment + 1][1] -= delta;
		}


		if (indx == 0) {

			glm::vec4 pldPos = this->segments[segment][indx];
			glm::vec4 delta = newPosition - pldPos;
			this->segments[segment][indx] = newPosition;
			this->segments[segment - 1][2] += delta;
			this->segments[segment][1] += delta;



		}
		else {
			this->segments[segment][indx] = newPosition;
		}

	}
	else {

		this->segments[segment][indx] = newPosition;

	}
}

Bezier1D::Bezier1D(void)
{

	glm::mat4 *mats = (glm::mat4*)malloc(NUMSEGMENTS*sizeof(glm::mat4));
	for (int i = 0; i < NUMSEGMENTS; ++i) {
		float m[16] = {
			0,0,0 + (i*DIST_SIZE * 3),1,
			0,0,DIST_SIZE + (i * DIST_SIZE * 3),1,
			0,0,DIST_SIZE * 2 + (i * DIST_SIZE * 3),1,
			0,0,DIST_SIZE * 3 + (i * DIST_SIZE * 3),1
		};
		memcpy(glm::value_ptr(mats[i]), m, sizeof(m));
		this->segments.push_back(mats[i]);
	}

	free(mats);

}

glm::vec3 Bezier1D::GetAxis() {
	LineVertex VC1 = this->GetControlPoint(0, 0);


	LineVertex VC2 = this->GetControlPoint(NUMSEGMENTS - 1, 3);


	return  (*VC2.GetPos() - *VC1.GetPos());
}


Bezier1D::Bezier1D(glm::mat4 mat)
{
	//this->segments.push_back(mat);
}

Bezier1D::~Bezier1D(void)
{
}

