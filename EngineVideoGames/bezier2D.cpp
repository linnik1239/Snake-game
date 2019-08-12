//#include "bezier2D.h"
//
//
//Bezier2D::Bezier2D(void)
//{
//}
//
//
//Bezier2D::~Bezier2D(void)
//{
//}

#include <list>

#include "bezier2D.h"
#include "bezier1D.h"
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <fstream>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include "bezier1D.h"


using namespace std;






std::list<glm::vec4> theListHalfSphere;




# define M_PI  3.14159265358979323846

IndexedModel Bezier2D::GetSurface(int resT, int resS)
//generates model for rendering using MeshConstructor::initMeshs
{


	IndexedModel inM;



	//LineVertex ver = this->b.GetControlPoint(1, 3);
	//float dist = (*ver.GetPos()).x;
	//float step = dist / resT;

	int k = 0;
	for (int m = 0; m < NUMSEGMENTS; ++m) {
		for (int i = 0; i < resT; ++i) {


			for (int j = 0; j < resS; ++j) {

				Vertex V1 = GetVertex(m, 0, (float)i / resT, (float)j / resS);
				Vertex V2 = GetVertex(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS);
				Vertex V3 = GetVertex(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS);
				Vertex V4 = GetVertex(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS);


				glm::vec3 theColor = glm::vec3(0.0f, 0.0f, 1.0f);


				inM.colors.push_back(theColor);
				inM.positions.push_back(*V1.GetPos());
				inM.normals.push_back(*V1.GetNormal());
				inM.indices.push_back(k);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V2.GetPos());
				inM.normals.push_back(*V2.GetNormal());
				inM.indices.push_back(k + 1);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V3.GetPos());
				inM.normals.push_back(*V3.GetNormal());
				inM.indices.push_back(k + 2);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V4.GetPos());
				inM.normals.push_back(*V4.GetNormal());
				inM.indices.push_back(k + 3);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				k += 4;
			}
		}

	}




	return inM;


}




IndexedModel Bezier2D::GetSurfaceFLAT(int resT, int resS) {
	IndexedModel inM;



	//LineVertex ver = this->b.GetControlPoint(1, 3);
	//float dist = (*ver.GetPos()).x;
	//float step = dist / resT;

	int k = 0;
	for (int m = 0; m < NUMSEGMENTS; ++m) {
		for (int i = 0; i < resT; ++i) {


			for (int j = 0; j < resS; ++j) {


				Vertex V1 = Vertex(glm::vec3(FLATSIZE*((float)i / resT), FLATSIZE * ((float)j / resS), 0), glm::vec2((float)i / resT, (float)j / resS), GetNormal(m, 0, (float)i / resT, (float)j / resS), glm::vec3(0, 0, 1));
				Vertex V2 = Vertex(glm::vec3(FLATSIZE * ((float)i / resT), FLATSIZE * ((float)j / resS + (float)1 / resS), 0), glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS), GetNormal(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS), glm::vec3(0, 0, 1));
				Vertex V3 = Vertex(glm::vec3(FLATSIZE * ((float)i / resT + (float)1 / resT), FLATSIZE * ((float)j / resS + (float)1 / resS), 0), glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS), GetNormal(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS), glm::vec3(0, 0, 1));
				Vertex V4 = Vertex(glm::vec3(FLATSIZE * ((float)i / resT + (float)1 / resT), FLATSIZE * ((float)j / resS), 0), glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS), GetNormal(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS), glm::vec3(0, 0, 1));

				//Vertex V1 = GetVertex2(m, 0, (float)i / resT, (float)j / resS);
				//Vertex V2 = GetVertex2(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS);
				//Vertex V3 = GetVertex2(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS);
				//Vertex V4 = GetVertex2(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS);


				glm::vec3 theColor = glm::vec3(1, 0, 0);


				inM.colors.push_back(theColor);
				inM.positions.push_back(*V1.GetPos());
				inM.normals.push_back(*V1.GetNormal());
				inM.indices.push_back(k);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V2.GetPos());
				inM.normals.push_back(*V2.GetNormal());
				inM.indices.push_back(k + 1);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V3.GetPos());
				inM.normals.push_back(*V3.GetNormal());
				inM.indices.push_back(k + 2);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V4.GetPos());
				inM.normals.push_back(*V4.GetNormal());
				inM.indices.push_back(k + 3);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				k += 4;
			}
		}

	}




	return inM;


}


IndexedModel Bezier2D::GetSurface2(int resT, int resS, float radius,float lineSize) {
	IndexedModel inM;



	//LineVertex ver = this->b.GetControlPoint(1, 3);
	//float dist = (*ver.GetPos()).x;
	//float step = dist / resT;

	int k = 0;
	for (int m = 0; m < NUMSEGMENTS; ++m) {
		for (int i = 0; i < resT; ++i) {


			for (int j = 0; j < resS; ++j) {

				Vertex V1 = GetVertex2(m, 0, (float)i / resT, (float)j / resS, radius, lineSize);
				Vertex V2 = GetVertex2(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS, radius, lineSize);
				Vertex V3 = GetVertex2(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS, radius, lineSize);
				Vertex V4 = GetVertex2(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS, radius, lineSize);


				glm::vec3 theColor = glm::vec3(1, 0, 0);


				inM.colors.push_back(theColor);
				inM.positions.push_back(*V1.GetPos());
				inM.normals.push_back(*V1.GetNormal());
				inM.indices.push_back(k);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V2.GetPos());
				inM.normals.push_back(*V2.GetNormal());
				inM.indices.push_back(k + 1);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V3.GetPos());
				inM.normals.push_back(*V3.GetNormal());
				inM.indices.push_back(k + 2);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V4.GetPos());
				inM.normals.push_back(*V4.GetNormal());
				inM.indices.push_back(k + 3);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				k += 4;
			}
		}

	}




	return inM;


}





IndexedModel Bezier2D::GetSurfaceBigCyllinder(int resT, int resS, float radius) {
	IndexedModel inM;



	//LineVertex ver = this->b.GetControlPoint(1, 3);
	//float dist = (*ver.GetPos()).x;
	//float step = dist / resT;

	int k = 0;
	for (int m = 0; m < NUMSEGMENTS; ++m) {
		for (int i = 0; i < resT; ++i) {


			for (int j = 0; j < resS; ++j) {

				Vertex V1 = GetVertex2(m, 0, (float)i / resT, (float)j / resS, radius,3.0);
				Vertex V2 = GetVertex2(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS, radius, 3.0);
				Vertex V3 = GetVertex2(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS, radius, 3.0);
				Vertex V4 = GetVertex2(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS, radius, 3.0);


				glm::vec3 theColor = glm::vec3(1, 0, 0);


				inM.colors.push_back(theColor);
				inM.positions.push_back(*V1.GetPos());
				inM.normals.push_back(*V1.GetNormal());
				inM.indices.push_back(k);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V2.GetPos());
				inM.normals.push_back(*V2.GetNormal());
				inM.indices.push_back(k + 1);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V3.GetPos());
				inM.normals.push_back(*V3.GetNormal());
				inM.indices.push_back(k + 2);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V4.GetPos());
				inM.normals.push_back(*V4.GetNormal());
				inM.indices.push_back(k + 3);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				k += 4;
			}
		}

	}




	return inM;


}



IndexedModel Bezier2D::GetSurface4(int resT, int resS) {
	IndexedModel inM;



	//LineVertex ver = this->b.GetControlPoint(1, 3);
	//float dist = (*ver.GetPos()).x;
	//float step = dist / resT;


	//ofstream myfile;
	//myfile.open("halfSphereIndexes.obj");
	//

	int k = 0;
	for (int m = 0; m < NUMSEGMENTS; ++m) {
		for (int i = 0; i < resT; ++i) {


			for (int j = 0; j < resS; ++j) {



				Vertex V1 = GetVertex4(m, 0, (float)i / resT, (float)j / resS);
				Vertex V2 = GetVertex4(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS);
				Vertex V3 = GetVertex4(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS);
				Vertex V4 = GetVertex4(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS);


				//theListHalfSphere.push_back(glm::vec4(*V1.GetPos(),1));

				//myfile << "v "<< (*V1.GetPos()).x <<" " << (*V1.GetPos()).y << " " << (*V1.GetPos()).z <<"\n";

				glm::vec3 theColor = glm::vec3(1, 0, 0);


				inM.colors.push_back(theColor);
				inM.positions.push_back(*V1.GetPos());
				inM.normals.push_back(*V1.GetNormal());
				inM.indices.push_back(k);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V2.GetPos());
				inM.normals.push_back(*V2.GetNormal());
				inM.indices.push_back(k + 1);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V3.GetPos());
				inM.normals.push_back(*V3.GetNormal());
				inM.indices.push_back(k + 2);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V4.GetPos());
				inM.normals.push_back(*V4.GetNormal());
				inM.indices.push_back(k + 3);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				k += 4;
			}
		}

	}




	return inM;


}


Vertex Bezier2D::GetVertex4(int segmentT, float segmentS, float t, float s)
//returns point on surface in the requested segments for value of t and s
{





	LineVertex currentVertex = this->b.GetVertex4(segmentT, (float)t);  //Somehing here to change with Rad.
	glm::mat4 RU = glm::rotate(360 * s, b.GetAxis());


	glm::vec4 newPos = glm::vec4(*currentVertex.GetPos() - *(b.GetVertex(0, 0).GetPos()), 1)*RU;

	glm::vec3 theNewPos = glm::vec3(newPos);

	theNewPos += *(b.GetVertex(0, 0).GetPos());

	glm::vec3 axisPos = this->b.GetAxis();



	glm::vec3 norm = GetNormal(segmentT, segmentS, t, s);


	return Vertex(theNewPos, glm::vec2(t, s), norm, glm::vec3(0, 0, 1));
}




IndexedModel Bezier2D::GetSurface3(int resT, int resS) {
	IndexedModel inM;



	//LineVertex ver = this->b.GetControlPoint(1, 3);
	//float dist = (*ver.GetPos()).x;
	//float step = dist / resT;

	int k = 0;
	for (int m = 0; m < NUMSEGMENTS; ++m) {
		for (int i = 0; i < resT; ++i) {


			for (int j = 0; j < resS; ++j) {

				Vertex V1 = GetVertex3(m, 0, (float)i / resT, (float)j / resS);
				Vertex V2 = GetVertex3(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS);
				Vertex V3 = GetVertex3(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS);
				Vertex V4 = GetVertex3(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS);


				glm::vec3 theColor = glm::vec3(1, 0, 0);


				inM.colors.push_back(theColor);
				inM.positions.push_back(*V1.GetPos());
				inM.normals.push_back(*V1.GetNormal());
				inM.indices.push_back(k);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V2.GetPos());
				inM.normals.push_back(*V2.GetNormal());
				inM.indices.push_back(k + 1);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V3.GetPos());
				inM.normals.push_back(*V3.GetNormal());
				inM.indices.push_back(k + 2);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS + (float)1 / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				inM.colors.push_back(theColor);
				inM.positions.push_back(*V4.GetPos());
				inM.normals.push_back(*V4.GetNormal());
				inM.indices.push_back(k + 3);
				inM.weights.push_back(calcWeight(m, 0, (float)i / resT + (float)1 / resT, (float)j / resS));
				inM.texCoords.push_back(glm::vec2((float)i / resT + (float)1 / resT, (float)j / resS));
				inM.joint_indices.push_back(glm::ivec3(0, 0, 0));

				k += 4;
			}
		}

	}




	return inM;


}


Vertex Bezier2D::GetVertex3(int segmentT, float segmentS, float t, float s)
//returns point on surface in the requested segments for value of t and s
{



	LineVertex currentVertex = this->b.GetVertex3(segmentT, (float)t);  //Somehing here to change with Rad.
	glm::mat4 RU = glm::rotate(360 * s, b.GetAxis());


	glm::vec4 newPos = glm::vec4(*currentVertex.GetPos() - *(b.GetVertex(0, 0).GetPos()), 1)*RU;

	glm::vec3 theNewPos = glm::vec3(newPos);

	theNewPos += *(b.GetVertex(0, 0).GetPos());

	glm::vec3 axisPos = this->b.GetAxis();



	glm::vec3 norm = GetNormal(segmentT, segmentS, t, s);


	return Vertex(theNewPos, glm::vec2(t, s), norm, glm::vec3(0, 0, 1));
}



Vertex Bezier2D::GetVertex2(int segmentT, float segmentS, float t, float s, float radius,float lineSize)
//returns point on surface in the requested segments for value of t and s
{



	LineVertex currentVertex = this->b.GetVertex2(segmentT, (float)t, radius, lineSize);  //Somehing here to change with Rad.
	glm::mat4 RU = glm::rotate(360 * s, b.GetAxis());


	glm::vec4 newPos = glm::vec4(*currentVertex.GetPos() - *(b.GetVertex(0, 0).GetPos()), 1)*RU;

	glm::vec3 theNewPos = glm::vec3(newPos);

	theNewPos += *(b.GetVertex(0, 0).GetPos());

	glm::vec3 axisPos = this->b.GetAxis();



	glm::vec3 norm = GetNormal(segmentT, segmentS, t, s);


	return Vertex(theNewPos, glm::vec2(t, s), norm, glm::vec3(0, 0, 1));
}


Vertex Bezier2D::GetVertex(int segmentT, float segmentS, float t, float s)
//returns point on surface in the requested segments for value of t and s
{




	LineVertex currentVertex = this->b.GetVertex(segmentT, (float)t);
	glm::mat4 RU = glm::rotate(360 * s, b.GetAxis());


	glm::vec4 newPos = glm::vec4(*currentVertex.GetPos() - *(b.GetVertex(0, 0).GetPos()), 1)*RU;

	glm::vec3 theNewPos = glm::vec3(newPos);

	theNewPos += *(b.GetVertex(0, 0).GetPos());

	glm::vec3 axisPos = this->b.GetAxis();



	glm::vec3 norm = GetNormal(segmentT, segmentS, t, s);


	return Vertex(theNewPos, glm::vec2(t, s), norm, glm::vec3(0, 0, 1));
}
glm::vec3 Bezier2D::GetNormal(int segmentT, int segmentS, float t, float s)
//returns point on surface in the requested segments for value of t and s
{


	//glm::vec3 velT = b.GetVelosity(segmentT, t);
	//glm::vec3 posT = *(b.GetVertex(segmentT, t)).GetPos();

	//axis = glm::vec3(1, 0, 0); //added

	//glm::vec3 c0 = *(b.GetControlPoint(0, 0)).GetPos();
	//glm::vec3 normalized_axis = glm::normalize(axis);
	//glm::mat4 rotateMat = glm::rotate(360.0f * s, axis);
	//glm::vec3 posS = glm::vec3(rotateMat * glm::vec4(posT, 1));

	//glm::vec3 radius_vec = posS - c0;
	//glm::vec3 center = c0 + normalized_axis * (glm::dot(radius_vec, normalized_axis));
	//glm::vec3 velS = glm::cross(radius_vec, axis);

	//if (glm::length(velS) < DELTA)
	//	return axis;

	//glm::vec3 normal = -glm::normalize(glm::cross(velT, velS));
	//return normal;




	glm::vec3 VecT = *(this->b.GetVertex(segmentT, (float)t)).GetPos();
	glm::vec3 dev_VecT = this->b.GetVelosity(segmentT, t);



	glm::vec3 firstCtrlPoint = *(this->b.GetControlPoint(0, 0)).GetPos();
	glm::vec3 normalizedAxis = glm::normalize(b.GetAxis());
	glm::mat4 rotateMat = glm::rotate(360.0f* s, b.GetAxis());
	glm::vec3 VecS = glm::vec3(rotateMat* glm::vec4(VecT, 1));

	glm::vec3 radVec = VecS - firstCtrlPoint;
	glm::vec3 center = firstCtrlPoint + normalizedAxis*(glm::dot(radVec, normalizedAxis));
	glm::vec3 dev_VecS = glm::cross(radVec, b.GetAxis());

	if (glm::length(dev_VecS) < DELTA) {
		return b.GetAxis();
	}

	glm::vec3 n = -glm::normalize(glm::cross(dev_VecT, dev_VecS));



	return glm::normalize(glm::cross(dev_VecT, dev_VecS));




}









glm::vec3 Bezier2D::calcWeight(int segmentT, int segmentS, float t, float s)
{
	float f1 = 0, f3 = 0;
	if (t>0.5)
		f3 = (1 - 4.0f*(1 - t)*t)*(1 - t) / 2.0f + (1 - 4.0f*(1 - t)*t)*t / 2.0f;
	else
		f1 = (1 - 4.0f*(1 - t)*t)*(1 - t) / 2.0f + (1 - 4.0f*(1 - t)*t)*t / 2.0f;
	float f2 = (2.0f*(1 - t)*(t + 0.0) + 0.5f);
	return glm::vec3(f1, f2, f3);
}






Bezier2D::Bezier2D(Bezier1D &b, glm::vec3 axis, int circularSubdivision) {
	this->b = b;
	this->circularSubdivision = circularSubdivision;
	this->axis = axis;
}

Bezier2D::Bezier2D(void)
{
}


Bezier2D::~Bezier2D(void)
{
}
