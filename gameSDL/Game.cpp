#include <GL\glew.h>
#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <limits>
#include <time.h>       /* time */
#include <windows.h>

#include <cstddef>
#include <glm/vec4.hpp>  
#define DEGREEMOVMENT 90

using namespace glm;
int theSign = 1;
int nextShapeRotate = -1;

bool angelchanges = false;
bool angelchangesNext = false;

bool actionDone = false;
bool nextactionDone = false;
glm::vec4 oldpos[NUMSEGMENTS * 3 + 1];





static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}


void Game::makeQuater(int numShape) {
	glm::mat4 prevMat = shapes[numShape - 1]->makeTrans();
	glm::mat4 curMat = shapes[numShape]->makeTrans();
	glm::mat4 nextMat = shapes[numShape + 1]->makeTrans();

	glm::vec4 prev_q_vec = vec4(prevMat[3][0] / 2, prevMat[3][1] / 2, prevMat[3][2] / 2, 0);
	glm::vec4 cur_q_vec = vec4(curMat[3][0] / 2, curMat[3][1] / 2, curMat[3][2] / 2, 0);
	glm::vec4 next_q_vec = vec4(nextMat[3][0] / 2, nextMat[3][1] / 2, nextMat[3][2] / 2, 0);

	mat3 prevMat3 = mat3(vec3(prevMat[0]), vec3(prevMat[1]), vec3(prevMat[2]));
	mat3 curMat3 = mat3(vec3(curMat[0]), vec3(curMat[1]), vec3(curMat[2]));
	mat3 nextMat3 = mat3(vec3(nextMat[0]), vec3(nextMat[1]), vec3(nextMat[2]));


}



Game::Game() :Scene() { curve = 0; }

Game::Game(glm::vec3 position, float angle, float near1, float far1, Viewport &vp) : Scene(position, angle, near1, far1, vp,glm::vec3(0,0,0))
{
	curve = new Bezier1D();
}

void Game::addShape(int type, int parent, unsigned int mode)
{
	chainParents.push_back(parent);
	if (type != BezierLine && type != BezierSurface)
		shapes.push_back(new Shape(type, mode));
	else
	{
		if (type == BezierLine)
			shapes.push_back(new Shape(curve, 30, 30, false, mode));
		else
			shapes.push_back(new Shape(curve, 30, 30, true, mode));
	}
}

void Game::addShape(int type, int parent, unsigned int mode, int whichShape)
{
	chainParents.push_back(parent);
	if (type != BezierLine && type != BezierSurface)
		shapes.push_back(new Shape(type, mode));
	else
	{
		if (type == BezierLine)
			shapes.push_back(new Shape(curve, 30, 30, false, mode));
		else
			shapes.push_back(new Shape(curve, 30, 30, true, mode, whichShape));
	}
}

void  Game::buildSegment() {
	int numCtrlPoints = NUMSEGMENTS * 3 + 1;


	for (int k = 0; k < 1; ++k) {
		for (int i = 0; i < numCtrlPoints; ++i) {

			addShape(Cube, -1, TRIANGLES);
			shapeTransformation(xScale, CUBE_SIZE);
			shapeTransformation(yScale, CUBE_SIZE);
			shapeTransformation(zScale, CUBE_SIZE);
		}





		addShape(BezierLine, -1, LINE_STRIP);



		for (int i = (numCtrlPoints*k); i <((numCtrlPoints*k) + numCtrlPoints - 1); ++i) {
			pickedShape = i;

			LineVertex v = this->curve->GetControlPoint(i / 3, i % 3);
			shapeTransformation(xGlobalTranslate, (*v.GetPos()).x / CUBE_SIZE);
			shapeTransformation(yGlobalTranslate, (*v.GetPos()).y / CUBE_SIZE);
			shapeTransformation(zGlobalTranslate, (*v.GetPos()).z / CUBE_SIZE);
			oldpos[i] = glm::vec4((*v.GetPos()).x, (*v.GetPos()).y, (*v.GetPos()).z, 1);
		}
		pickedShape = (numCtrlPoints - 1);
		LineVertex v = this->curve->GetControlPoint((pickedShape - 1) / 3, 3);
		shapeTransformation(xGlobalTranslate, (*v.GetPos()).x / CUBE_SIZE);
		shapeTransformation(yGlobalTranslate, (*v.GetPos()).y / CUBE_SIZE);
		shapeTransformation(zGlobalTranslate, (*v.GetPos()).z / CUBE_SIZE);
		oldpos[pickedShape] = glm::vec4((*v.GetPos()).x, (*v.GetPos()).y, (*v.GetPos()).z, 1);
	}


}

void  Game::buildSegment2() {
	int numCtrlPoints = NUMSEGMENTS * 3 + 1;


	for (int k = 0; k < 1; ++k) {
		for (int i = 0; i < numCtrlPoints; ++i) {

			addShape(Cube, -1, TRIANGLES);
			shapeTransformation(xScale, CUBE_SIZE);
			shapeTransformation(yScale, CUBE_SIZE);
			shapeTransformation(zScale, CUBE_SIZE);
			shapes[i]->Hide();
		}

		addShape(BezierLine, -1, LINE_STRIP);

		for (int i = (numCtrlPoints*k); i <((numCtrlPoints*k) + numCtrlPoints - 1); ++i) {
			pickedShape = i;


			LineVertex v = this->curve->GetControlPoint(i / 3, i % 3);
			shapeTransformation(xGlobalTranslate, (*v.GetPos()).x / CUBE_SIZE);
			shapeTransformation(yGlobalTranslate, (*v.GetPos()).y / CUBE_SIZE);
			shapeTransformation(zGlobalTranslate, (*v.GetPos()).z / CUBE_SIZE);
			oldpos[i] = glm::vec4((*v.GetPos()).x, (*v.GetPos()).y, (*v.GetPos()).z, 1);
		}
		pickedShape = (numCtrlPoints - 1);
		LineVertex v = this->curve->GetControlPoint((pickedShape - 1) / 3, 3);
		shapeTransformation(xGlobalTranslate, (*v.GetPos()).x / CUBE_SIZE);
		shapeTransformation(yGlobalTranslate, (*v.GetPos()).y / CUBE_SIZE);
		shapeTransformation(zGlobalTranslate, (*v.GetPos()).z / CUBE_SIZE);
		oldpos[pickedShape] = glm::vec4((*v.GetPos()).x, (*v.GetPos()).y, (*v.GetPos()).z, 1);
	}


}

void Game::Init()
{
	buildSegment2();

	plane2D = new Shape(Plane, TRIANGLES);
	plane2D->SetShader(2);




	addShape(Axis, -1, LINES);

	//shapes[0]->Hide();
	//addShapeFromFile("../res/objs/monkey3.obj",-1,TRIANGLES);

	//addShape(Cube,-1,TRIANGLES);


	//translate all scene away from camera
	myTranslate(glm::vec3(0, 0, -5), 0);

	//pickedShape = 1;
	//shapeTransformation(xGlobalTranslate,2);
	//ScaleAllDirections(2);
	//SetShapeTex(1,0);
	//SetShapeTex(2,1);

	//ReadPixel();
	pickedShape = -1;
	//Activate();
}



void Game::newInit()
{
	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE);
	//int halfSphereIndex = getShapeSize() - 1;
	//pickedShape = halfSphereIndex;

	//int currentSize = shapes.size();


	//shapeTransformation(xLocalRotate, 180);
	//shapeTransformation(/*zScale*/yGlobalTranslate, 0);

	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, CYLINDER);
	//int cylinderIndex = getShapeSize() - 1;



	//setPickedShape(getShapeSize() - 1);
	//shapeTransformation(/*zScale*/yGlobalTranslate, -3);

	//chainParents.pop_back();
	//chainParents.push_back(halfSphereIndex);

	//addShapeCopy(cylinderIndex, -1, QUADS);
	//setPickedShape(getShapeSize() - 1);
	//shapeTransformation(/*zScale*/yGlobalTranslate, -3);

	//chainParents.pop_back();
	//chainParents.push_back(shapes.size() - 2);
	//addShapeCopy(cylinderIndex, -1, QUADS);
	//setPickedShape(getShapeSize() - 1);
	//shapeTransformation(/*zScale*/yGlobalTranslate, -3);

	//chainParents.pop_back();
	//chainParents.push_back(shapes.size() - 2);



	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	//setPickedShape(getShapeSize() - 1);
	//shapeTransformation(/*zScale*/yGlobalTranslate, -3);
	//chainParents.pop_back();
	//chainParents.push_back(shapes.size() - 2);


	//pickedShape = halfSphereIndex;


	//shapeTransformation(xLocalRotate, 180);


	//Surface_flag = 1;




	// *************begin





	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	//shapes[shapes.size() - 1]->SetTexture(1);

	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, CYLINDER);
	//shapes[shapes.size() - 1]->SetTexture(1);
	int cylinderIndex = shapes.size() - 1;

	addShapeCopy(cylinderIndex, -1, QUADS);
	shapes[shapes.size() - 1]->SetTexture(1);

	addShapeCopy(cylinderIndex, -1, QUADS);
	shapes[shapes.size() - 1]->SetTexture(1);




	addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE);
	shapes[shapes.size() - 1]->SetTexture(1);









	changeBendingDirection();








	int halfSphereIndex = getShapeSize() - 1;
	pickedShape = halfSphereIndex;

	int currentSize = shapes.size();


	//shapeTransformation(xLocalRotate, 180);
	//shapeTransformation(/*zScale*/yGlobalTranslate, 0);

	setPickedShape(6);
	shapeTransformation(/*zScale*/xGlobalRotate, 180);
	//shapeTransformation(xGlobalRotate, 90);
	//chainParents.at(6) = -1;
	shapeTransformation(/*zScale*/zGlobalTranslate, -9);
	setPickedShape(7);
	shapeTransformation(/*zScale*/xGlobalRotate, 180);
	shapeTransformation(/*zScale*/zGlobalTranslate, -6);

	//chainParents.at(7) = 6;
	//chainParents.pop_back();
	//chainParents.push_back(halfSphereIndex);

	//addShapeCopy(cylinderIndex, -1, QUADS);
	setPickedShape(8);
	shapeTransformation(/*zScale*/xGlobalRotate, 180);
	shapeTransformation(/*zScale*/zGlobalTranslate, -3);
	//chainParents.at(8) = 7;
	//chainParents.pop_back();
	//chainParents.push_back(shapes.size() - 2);


	//addShapeCopy(cylinderIndex, -1, QUADS);
	setPickedShape(9);
	shapeTransformation(/*zScale*/xGlobalRotate, 180);
	shapeTransformation(/*zScale*/zGlobalTranslate, 0);
	//chainParents.at(9) = 8;
	//chainParents.pop_back();
	//chainParents.push_back(shapes.size() - 2);



	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	setPickedShape(10);
	//shapeTransformation(xGlobalRotate, 90);
	shapeTransformation(/*zScale*/xGlobalRotate, 180);
	shapeTransformation(/*zScale*/zGlobalTranslate, 3);
	//chainParents.at(10) = 9;

	//chainParents.pop_back();
	//chainParents.push_back(shapes.size() - 2);



	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(2);



	shapeTransformation(xLocalRotate, 90);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2);




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(0);
	setPickedShape(shapes.size() - 1);
	//shapeTransformation(xLocalRotate, 90);
	////shapeTransformation(yLocalRotate, 90);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);


	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(0);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(yLocalRotate, 90);
	////shapeTransformation(yLocalRotate, 90);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);


	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(0);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(yLocalRotate, 90);
	////shapeTransformation(yLocalRotate, 90);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2);




	/* initialize random seed: */
	srand(time(NULL));

	/* generate secret number between 1 and 10: */


	addShape(Cube, -1, TRIANGLES);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(rand() % 3);


	shapeTransformation(zGlobalTranslate, -30);
	shapeTransformation(xGlobalTranslate, -10);
	shapeTransformation(yGlobalTranslate, 1);




	addShape(Cube, -1, TRIANGLES);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(rand() % 3);


	shapeTransformation(zGlobalTranslate, -30);
	shapeTransformation(xGlobalTranslate, -20);
	shapeTransformation(yGlobalTranslate, 1);

	addShape(Cube, -1, TRIANGLES);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(rand() % 3);



	shapeTransformation(zGlobalTranslate, -30);
	shapeTransformation(xGlobalTranslate, 10);
	shapeTransformation(yGlobalTranslate, 1);


	addShape(Cube, -1, TRIANGLES);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(rand() % 3);


	addShape(Cube, -1, TRIANGLES);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(rand() % 3);


	shapeTransformation(zGlobalTranslate, -30);
	shapeTransformation(xGlobalTranslate, 20);
	shapeTransformation(yGlobalTranslate, 1);
	//shapeTransformation(xLocalRotate, 180);

















	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, BIGCYLINDER);
	shapes[shapes.size() - 1]->SetTexture(2);
	cylinderIndex = getShapeSize() - 1;
	pickedShape = cylinderIndex;
	shapeTransformation(/*zScale*/zGlobalTranslate, -FLATSIZE / 2 - 1);
	shapeTransformation(/*zScale*/xGlobalTranslate, -8);
	//int cylinderIndex = getShapeSize() - 1;



	Activate();
	Surface_flag = 1;




	// *************End




	//  ***************************** Something previous  ********************
	////addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE);
	////int halfSphereIndex = getShapeSize() - 1;
	////pickedShape = halfSphereIndex;

	////int currentSize = shapes.size();


	////shapeTransformation(xLocalRotate, 180);
	////shapeTransformation(/*zScale*/yGlobalTranslate, 0);

	////addShape(5/*BezierSurface*/, -1, 7/*Quads*/, CYLINDER);
	////int cylinderIndex = getShapeSize() - 1;



	////setPickedShape(getShapeSize() - 1);
	////shapeTransformation(/*zScale*/yGlobalTranslate, -3);

	////chainParents.pop_back();
	////chainParents.push_back(halfSphereIndex);

	////addShapeCopy(cylinderIndex, -1, QUADS);
	////setPickedShape(getShapeSize() - 1);
	////shapeTransformation(/*zScale*/yGlobalTranslate, -3);

	////chainParents.pop_back();
	////chainParents.push_back(shapes.size() - 2);
	////addShapeCopy(cylinderIndex, -1, QUADS);
	////setPickedShape(getShapeSize() - 1);
	////shapeTransformation(/*zScale*/yGlobalTranslate, -3);

	////chainParents.pop_back();
	////chainParents.push_back(shapes.size() - 2);



	////addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	////setPickedShape(getShapeSize() - 1);
	////shapeTransformation(/*zScale*/yGlobalTranslate, -3);
	////chainParents.pop_back();
	////chainParents.push_back(shapes.size() - 2);


	////pickedShape = halfSphereIndex;


	////shapeTransformation(xLocalRotate, 180);


	////Surface_flag = 1;










	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE);
	//shapes[shapes.size() - 1]->SetTexture(1);

	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, CYLINDER);
	//shapes[shapes.size() - 1]->SetTexture(1);
	//int cylinderIndex = getShapeSize() - 1;

	//addShapeCopy(cylinderIndex, -1, QUADS);
	//shapes[shapes.size() - 1]->SetTexture(1);

	//addShapeCopy(cylinderIndex, -1, QUADS);
	//shapes[shapes.size() - 1]->SetTexture(1);




	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	//shapes[shapes.size() - 1]->SetTexture(1);









	//changeBendingDirection();








	//int halfSphereIndex = getShapeSize() - 1;
	//pickedShape = halfSphereIndex;

	//int currentSize = shapes.size();


	////shapeTransformation(xLocalRotate, 180);
	////shapeTransformation(/*zScale*/yGlobalTranslate, 0);

	//setPickedShape(6);
	//shapeTransformation(/*zScale*/xGlobalRotate, 180);
	////shapeTransformation(xGlobalRotate, 90);
	//chainParents.at(6) = -1;

	//setPickedShape(7);
	//shapeTransformation(/*zScale*/zGlobalTranslate, 3);

	//chainParents.at(7) = 6;
	////chainParents.pop_back();
	////chainParents.push_back(halfSphereIndex);

	////addShapeCopy(cylinderIndex, -1, QUADS);
	//setPickedShape(8);
	//shapeTransformation(/*zScale*/zGlobalTranslate, 3);
	//chainParents.at(8) = 7;
	////chainParents.pop_back();
	////chainParents.push_back(shapes.size() - 2);


	////addShapeCopy(cylinderIndex, -1, QUADS);
	//setPickedShape(9);
	//shapeTransformation(/*zScale*/zGlobalTranslate, 3);
	//chainParents.at(9) = 8;
	////chainParents.pop_back();
	////chainParents.push_back(shapes.size() - 2);



	////addShape(5/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	//setPickedShape(10);
	////shapeTransformation(xGlobalRotate, 90);
	//shapeTransformation(/*zScale*/zGlobalTranslate, 3);
	//chainParents.at(10) = 9;

	////chainParents.pop_back();
	////chainParents.push_back(shapes.size() - 2);



	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	//setPickedShape(shapes.size() - 1);
	//shapes[shapes.size() - 1]->SetTexture(2);



	//shapeTransformation(xLocalRotate, 90);
	//shapeTransformation(yLocalRotate, 90);
	//shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	//shapeTransformation(zGlobalTranslate, FLATSIZE / 2);




	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	//shapes[shapes.size() - 1]->SetTexture(0);
	//setPickedShape(shapes.size() - 1);
	////shapeTransformation(xLocalRotate, 90);
	//////shapeTransformation(yLocalRotate, 90);
	//shapeTransformation(zGlobalTranslate, -FLATSIZE / 2);
	//shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);


	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	//shapes[shapes.size() - 1]->SetTexture(0);
	//setPickedShape(shapes.size() - 1);
	//shapeTransformation(yLocalRotate, 90);
	//////shapeTransformation(yLocalRotate, 90);
	//shapeTransformation(zGlobalTranslate, FLATSIZE / 2);
	//shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);


	//addShape(5/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	//shapes[shapes.size() - 1]->SetTexture(0);
	//setPickedShape(shapes.size() - 1);
	//shapeTransformation(yLocalRotate, 90);
	//////shapeTransformation(yLocalRotate, 90);
	//shapeTransformation(zGlobalTranslate, FLATSIZE / 2);
	//shapeTransformation(xGlobalTranslate, FLATSIZE / 2);




	///* initialize random seed: */
	//srand(time(NULL));

	///* generate secret number between 1 and 10: */


	//addShape(Cube, -1, TRIANGLES);
	//setPickedShape(shapes.size() - 1);
	//shapes[shapes.size() - 1]->SetTexture(rand() % 3 + 3);


	//shapeTransformation(zGlobalTranslate, -30);
	//shapeTransformation(xGlobalTranslate, -10);
	//shapeTransformation(yGlobalTranslate, 1);


	//addShape(Cube, -1, TRIANGLES);
	//setPickedShape(shapes.size() - 1);
	//shapes[shapes.size() - 1]->SetTexture(rand() % 3 + 3);


	//shapeTransformation(zGlobalTranslate, -30);
	//shapeTransformation(xGlobalTranslate, -20);
	//shapeTransformation(yGlobalTranslate, 1);

	//addShape(Cube, -1, TRIANGLES);
	//setPickedShape(shapes.size() - 1);
	//shapes[shapes.size() - 1]->SetTexture(rand() % 3 + 3);



	//shapeTransformation(zGlobalTranslate, -30);
	//shapeTransformation(xGlobalTranslate, 10);
	//shapeTransformation(yGlobalTranslate, 1);


	//addShape(Cube, -1, TRIANGLES);
	//setPickedShape(shapes.size() - 1);
	//shapes[shapes.size() - 1]->SetTexture(rand() % 3 + 3);


	//addShape(Cube, -1, TRIANGLES);
	//setPickedShape(shapes.size() - 1);
	//shapes[shapes.size() - 1]->SetTexture(rand() % 3 + 3);


	//shapeTransformation(zGlobalTranslate, -30);
	//shapeTransformation(xGlobalTranslate, 20);
	//shapeTransformation(yGlobalTranslate, 1);
	////shapeTransformation(xLocalRotate, 180);

	//Activate();
	//Surface_flag = 1;

	//******************************************************
}

void Game::changeBendingDirection() {

	//chainParents.at(10) = -1;
	//chainParents.at(9) = 10;
	//chainParents.at(8) = 9;
	//chainParents.at(7) = 8;
	//chainParents.at(6) = 7;



	//chainParents.at(6) = -1;
	//chainParents.at(7) = 6;
	//chainParents.at(8) = 7;
	//chainParents.at(9) = 8;
	//chainParents.at(10) = 9;





	//chainParents.pop_back();
	//chainParents.pop_back();
	//chainParents.pop_back();
	//chainParents.pop_back();

	//chainParents.push_back(-1);
	//chainParents.push_back(-1);
	//chainParents.push_back(-1);
	//chainParents.push_back(-1);
}


void Game::Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Normal);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.7f, 0.8f, 0.1f, 1.0f);
	s->Unbind();
}
void Game::Update2(const glm::mat4 &Projection, const glm::mat4 &Normal1, const glm::mat4 &Normal2, const glm::mat4 &Normal3, const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("Projection", Projection);
	s->SetUniformMat4f("Normal1", Normal1);
	s->SetUniformMat4f("Normal2", Normal2);
	s->SetUniformMat4f("Normal3", Normal3);



	s->SetUniform4f("rot_prev", 1.0f, 1.0f, 1.0f, 1.0f);
	s->SetUniform4f("trans_prev", 1.0f, 1.0f, 1.0f, 1.0f);
	s->SetUniform4f("rot_cur", 1.0f, 1.0f, 1.0f, 1.0f);
	s->SetUniform4f("trans_cur", 1.0f, 1.0f, 1.0f, 1.0f);
	s->SetUniform4f("rot_next", 1.0f, 1.0f, 1.0f, 1.0f);
	s->SetUniform4f("trans_next", 1.0f, 1.0f, 1.0f, 1.0f);

	s->SetUniform1i("flag2", 0);
	s->SetUniform1i("sign", 1);

	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.1f, 0.8f, 0.7f, 1.0f);


	//if (pickedShape>6 && pickedShape<10) {
	//	s->SetUniform1i("flag2", 1);
	//	
	//	
	//	////glm::mat4 prevMat = shapes[pickedShape - 1]->makeTrans();
	//	mat4 FatherTransforms = mat4(1);
	//	mat4 SonTransforms = mat4(1);

	//	for (int i = pickedShape+1; i <= 10; ++i) {
	//		FatherTransforms = FatherTransforms* shapes[i]->makeTrans();
	//	}
	//	

	//	for (int i = pickedShape-1; i >=  6; --i) {
	//		SonTransforms = SonTransforms* shapes[i]->makeTrans();
	//	}



	//	//glm::mat3x4 pre = glm::mat3x4(shapes[pickedShape - 1]->makeTrans());
	//	glm::mat3x4 pre = glm::mat3x4(glm::transpose(shapes[pickedShape - 1]->makeTrans()));
	//	//glm::mat3x4 pre = glm::mat3x4(SonTransforms);


	//	glm::mat3x4 cur = glm::mat3x4(glm::transpose(shapes[pickedShape ]->makeTrans()));
	//	//glm::mat3x4 cur = glm::mat3x4(glm::mat4(1));



	//	glm::mat3x4 next = glm::mat3x4(glm::transpose(shapes[pickedShape +1]->makeTrans()));
	//	//glm::mat3x4 next = glm::mat3x4(FatherTransforms);

	//		//= glm::mat2x4(prevMat);
	//	glm::dualquat q_prev = glm::dualquat_cast(pre);
	//	glm::vec4 rot_prev = glm::vec4(q_prev.real.x, q_prev.real.y, q_prev.real.z, q_prev.real.w);
	//	glm::vec4 trans_prev = glm::vec4(q_prev.dual.x, q_prev.dual.y, q_prev.dual.z, q_prev.dual.w);

	//	glm::dualquat q_cur = glm::dualquat_cast(cur);
	//	glm::vec4 rot_cur = glm::vec4(q_cur.real.x, q_cur.real.y, q_cur.real.z, q_cur.real.w);
	//	glm::vec4 trans_cur = glm::vec4(q_cur.dual.x, q_cur.dual.y, q_cur.dual.z, q_cur.dual.w);

	//	glm::dualquat q_next = glm::dualquat_cast(next);
	//	glm::vec4 rot_next = glm::vec4(q_next.real.x, q_next.real.y, q_next.real.z, q_next.real.w);
	//	glm::vec4 trans_next = glm::vec4(q_next.dual.x, q_next.dual.y, q_next.dual.z, q_next.dual.w);

	//	

	//	s->SetUniform4f("rot_prev", rot_prev.x, rot_prev.y , rot_prev.z , rot_prev.w );
	//	s->SetUniform4f("rot_prev", trans_prev.x, trans_prev.y, trans_prev.z, trans_prev.w);

	//	s->SetUniform4f("rot_cur", rot_cur.x, rot_cur.y, rot_cur.z, rot_cur.w);
	//	s->SetUniform4f("trans_cur", trans_cur.x, trans_cur.y, trans_cur.z, trans_cur.w);

	//	s->SetUniform4f("rot_next", rot_next.x, rot_next.y, rot_next.z, rot_next.w);
	//	s->SetUniform4f("trans_next", trans_next.x, trans_next.y, trans_next.z, trans_next.w);
	//	//s->SetUniformMat4f("trans", trans);
	//	////makeQuater(pickedShape);
	//}


	//if (pickedShape==6) {
	//	s->SetUniform1i("flag2", 1);
	//	s->SetUniform1i("sign", 1);

	//	mat4 FatherTransforms = mat4(1);

	//	for (int i = pickedShape+1; i <= 10; ++i) {
	//		FatherTransforms = FatherTransforms* shapes[i]->makeTrans();
	//	}
	//	////glm::mat4 prevMat = shapes[pickedShape - 1]->makeTrans();
	//	glm::mat3x4 pre = glm::mat3x4(glm::transpose(shapes[pickedShape]->makeTrans()));
	//	glm::mat3x4 cur = glm::mat3x4(glm::transpose(Normal2/*shapes[pickedShape]->makeTrans()*/));
	//	//glm::mat3x4 cur = glm::mat3x4(mat4(1));
	//	//cur = glm::mat3x4(shapes[pickedShape]->makeTrans()*FatherTransforms);
	//	//glm::mat3x4 cur = glm::mat3x4(FatherTransforms);
	//	glm::mat3x4 next = glm::mat3x4(glm::transpose(shapes[pickedShape +1]->makeTrans()));
	//	//glm::mat3x4 next = glm::mat3x4(mat4(1));  // **  Switch from above.
	//	//glm::mat3x4 next = glm::mat3x4(FatherTransforms);

	//	//= glm::mat2x4(prevMat); 
	//	glm::dualquat q_prev = glm::dualquat_cast(pre);
	//	glm::vec4 rot_prev = glm::vec4(q_prev.real.x, q_prev.real.y, q_prev.real.z, q_prev.real.w);
	//	glm::vec4 trans_prev = glm::vec4(q_prev.dual.x, q_prev.dual.y, q_prev.dual.z, q_prev.dual.w);

	//	glm::dualquat q_cur = glm::dualquat_cast(cur);
	//	glm::vec4 rot_cur = glm::vec4(q_cur.real.x, q_cur.real.y, q_cur.real.z, q_cur.real.w);
	//	glm::vec4 trans_cur = glm::vec4(q_cur.dual.x, q_cur.dual.y, q_cur.dual.z, q_cur.dual.w);
	//	//rot_cur =  glm::vec4(3000,3000,3000,3000) + rot_cur;



	//	glm::dualquat q_next = glm::dualquat_cast(next);
	//	glm::vec4 rot_next = glm::vec4(q_next.real.x, q_next.real.y, q_next.real.z, q_next.real.w);
	//	glm::vec4 trans_next = glm::vec4(q_next.dual.x, q_next.dual.y, q_next.dual.z, q_next.dual.w);
	//	//trans_next = glm::vec4(3000, 3000, 3000, 3000) + trans_next;


	//	s->SetUniform4f("rot_prev", rot_prev.x, rot_prev.y, rot_prev.z, rot_prev.w);
	//	s->SetUniform4f("rot_prev", trans_prev.x, trans_prev.y, trans_prev.z, trans_prev.w);

	//	s->SetUniform4f("rot_cur", rot_cur.x, rot_cur.y, rot_cur.z, rot_cur.w);
	//	s->SetUniform4f("trans_cur", trans_cur.x, trans_cur.y, trans_cur.z, trans_cur.w);

	//	s->SetUniform4f("rot_next", rot_next.x, rot_next.y, rot_next.z, rot_next.w);
	//	s->SetUniform4f("trans_next", trans_next.x, trans_next.y, trans_next.z, trans_next.w);
	//	//s->SetUniformMat4f("trans", trans);
	//	////makeQuater(pickedShape);
	//}

	//if (pickedShape == 10) {
	//	s->SetUniform1i("flag2", 1);
	//	//s->SetUniform1i("sign", 1);


	//	mat4 SonTransforms = mat4(1);



	//	for (int i = pickedShape-1; i >= 6; --i) {
	//		SonTransforms = SonTransforms* glm::transpose(shapes[i]->makeTrans());
	//	}

	//	////glm::mat4 prevMat = shapes[pickedShape - 1]->makeTrans();
	//	glm::mat3x4 pre = glm::mat3x4(glm::transpose(shapes[pickedShape - 1]->makeTrans()));
	//	//pre = -pre;
	//	//glm::mat3x4 pre = glm::mat3x4(SonTransforms);


	//	glm::mat3x4 cur = glm::mat3x4(glm::transpose(shapes[pickedShape]->makeTrans()));
	//	//glm::mat3x4 cur = glm::mat3x4(mat4(1));


	//	glm::mat3x4 next = glm::mat3x4(mat4(1));
	//	//= glm::mat2x4(prevMat);
	//	glm::dualquat q_prev = glm::dualquat_cast(pre);
	//	glm::vec4 rot_prev = glm::vec4(q_prev.real.x, q_prev.real.y, q_prev.real.z, q_prev.real.w);
	//	glm::vec4 trans_prev = glm::vec4(q_prev.dual.x, q_prev.dual.y, q_prev.dual.z, q_prev.dual.w);

	//	glm::dualquat q_cur = glm::dualquat_cast(cur);
	//	glm::vec4 rot_cur = glm::vec4(q_cur.real.x, q_cur.real.y, q_cur.real.z, q_cur.real.w);
	//	glm::vec4 trans_cur = glm::vec4(q_cur.dual.x, q_cur.dual.y, q_cur.dual.z, q_cur.dual.w);

	//	glm::dualquat q_next = glm::dualquat_cast(next);
	//	glm::vec4 rot_next = glm::vec4(q_next.real.x, q_next.real.y, q_next.real.z, q_next.real.w);
	//	glm::vec4 trans_next = glm::vec4(q_next.dual.x, q_next.dual.y, q_next.dual.z, q_next.dual.w);



	//	s->SetUniform4f("rot_prev", rot_prev.x, rot_prev.y, rot_prev.z, rot_prev.w);
	//	s->SetUniform4f("rot_prev", trans_prev.x, trans_prev.y, trans_prev.z, trans_prev.w);

	//	s->SetUniform4f("rot_cur", rot_cur.x, rot_cur.y, rot_cur.z, rot_cur.w);
	//	s->SetUniform4f("trans_cur", trans_cur.x, trans_cur.y, trans_cur.z, trans_cur.w);

	//	s->SetUniform4f("rot_next", rot_next.x, rot_next.y, rot_next.z, rot_next.w);
	//	s->SetUniform4f("trans_next", trans_next.x, trans_next.y, trans_next.z, trans_next.w);
	//	//s->SetUniformMat4f("trans", trans);
	//	////makeQuater(pickedShape);
	//}



	s->Unbind();
}


void Game::WhenRotate()
{
	//if(pickedShape>=0)
	//	printMat(GetShapeTransformation());
}

//void Game::WhenTranslate()
//{
//	if(pickedShape>=0)
//	{
//		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0,0,0,1);
//	//	std::cout<<"( "<<pos.x<<", "<<pos.y<<", "<<pos.z<<")"<<std::endl;
//	}
//}



void Game::WhenTranslate()
{
	if (pickedShape >= 0)
	{
		std::cout << "pickedShape: " << pickedShape << std::endl;


		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);

		glm::vec4 delta = pos - oldpos[pickedShape];
		//delta /= CUBE_SIZE;
		//delta /= CUBE_SIZE;
		std::cout << "( " << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;



		if (pickedShape >= 0 && pickedShape <= 5) {
			Shape *s1 = shapes[shapes.size() - 1];
			shapes.pop_back();
			delete s1;



			Shape *s3 = shapes[shapes.size() - 1];
			shapes.pop_back();
			delete s3;


			if (Surface_flag == 1) {
				Shape *s2 = shapes[shapes.size() - 1];
				shapes.pop_back();


				delete s2;
			}


			this->curve->MoveControlPoint(0, pickedShape, false, pos);

			shapes.push_back(new Shape(Axis, LINES));

			int p = pickedShape;
			pickedShape = shapes.size() - 1;


			shapeTransformation(yScale, 10);
			shapeTransformation(xScale, 10);
			shapeTransformation(zScale, 10);

			pickedShape = p;

			shapes.push_back(new Shape(curve, 30, 30, false, LINE_STRIP));

			if (Surface_flag == 1) {
				shapes.push_back(new Shape(curve, 30, 30, true, QUADS));
			}
		}

		oldpos[pickedShape] = pos;
	}
}


void Game::changeCameraMode() {
	this->cameraMode = -this->cameraMode;
}
bool Game::isCameraMode() {
	return this->cameraMode;
}

void Game::UpxMotion(bool up) {
	if (up) {
		angel += DEGREEMOVMENT;
		shapes[6]->theAngel += DEGREEMOVMENT;


		theSign = 1;
		//shapeTransformation(yLocalRotate, -DEGREEMOVMENT);
		actionDone = true;
		angelchanges = true;

		//shapeTransformation(xGlobalTranslate, x_motion);
		x_motion += 0.01;
	}
	else {
		//shapeTransformation(yLocalRotate, DEGREEMOVMENT);
		actionDone = true;
		angelchanges = true;


		printf("Angel = %f, shape angel = %f \n", angel, shapes[6]->theAngel);
		angel -= DEGREEMOVMENT;
		shapes[6]->theAngel -= DEGREEMOVMENT;
		theSign = -1;
		x_motion -= 0.01;
	}

}
void Game::UpyMotion(bool up) {
	if (up) {
		z_motion -= 0.01;
	}
	else {
		z_motion += 0.01;
	}

}


void Game::Motion()
{
	if (isActive)
	{
		int p = pickedShape;
		//pickedShape = 10;

		float total = abs(x_motion + y_motion + z_motion) + 1;

		float vel = 3.0;


		//int p = pickedShape;
		//pickedShape = 2;
		//shapeTransformation(zLocalRotate, 0.75);


		for (int i = 6; i <= 10; ++i) {
			pickedShape = i;
			if (i >6) {

				if (shapes[i]->nuwPush) {


					shapeTransformation(xGlobalTranslate, vel*sin((shapes[i]->theAngel / 360) * 2 * M_PI));
					shapeTransformation(yGlobalTranslate, 0);
					shapeTransformation(zGlobalTranslate, -vel*cos((shapes[i]->theAngel / 360) * 2 * M_PI));
					shapes[i]->theAngel = shapes[i - 1]->theAngel;
					shapeTransformation(yLocalRotate, -theSign*DEGREEMOVMENT);
					shapes[i]->prevTorAngel = -theSign*DEGREEMOVMENT;
					angelchangesNext = false;
					shapes[i]->nuwPush = false;
					shapes[i]->prevPush = true;
				}
				else {
					//if (shapes[i-1]->prevTorAngel!=0) 
					//{   
					//	//shapes[i]->nuwPush = true;
					//	break;
					//}
					//else {
					shapeTransformation(xGlobalTranslate, vel*sin(((shapes[i]->theAngel) / 360) * 2 * M_PI));
					shapeTransformation(yGlobalTranslate, 0);
					shapeTransformation(zGlobalTranslate, -vel*cos(((shapes[i]->theAngel) / 360) * 2 * M_PI));
					shapes[i]->prevTorAngel = 0;
					if (shapes[i - 1]->prevPush) {
						shapes[i - 1]->prevPush = false;
						shapes[i]->nuwPush = true;
					}

					//}


				}
			}

			if (i == 6) {
				if (angelchanges) {
					shapes[i]->prevTorAngel = -theSign*DEGREEMOVMENT;

					shapeTransformation(yLocalRotate, -theSign*DEGREEMOVMENT);
					shapes[i + 1]->nuwPush = true;
					break;
				}
				else {
					shapeTransformation(xGlobalTranslate, vel*sin((shapes[6]->theAngel / 360) * 2 * M_PI));
					shapeTransformation(yGlobalTranslate, 0);
					shapeTransformation(zGlobalTranslate, -vel*cos((shapes[6]->theAngel / 360) * 2 * M_PI));
					shapes[i]->prevTorAngel = 0;
				}
			}


		}


		if (angelchanges) {
			angelchangesNext = true;
		}
		angelchanges = false;


		//switch (nextShapeRotate) {

		//case 10: pickedShape = 10;
		//	shapeTransformation(yLocalRotate, -theSign * DEGREEMOVMENT);

		//	nextShapeRotate = -1;
		//	break;
		//case 9: pickedShape = 9;
		//	shapeTransformation(yLocalRotate, -theSign*DEGREEMOVMENT);

		//	nextShapeRotate = 10;
		//	break;
		//case 8: pickedShape = 8;
		//	shapeTransformation(yLocalRotate, -theSign * DEGREEMOVMENT);

		//	nextShapeRotate = 9;
		//	break;
		//case 7: pickedShape = 7;
		//	shapeTransformation(xGlobalTranslate, vel*(sin(((angel-10) / 360) * 2 * M_PI)));
		//	shapeTransformation(yGlobalTranslate, 0);
		//	shapeTransformation(zGlobalTranslate,( -vel*cos(((angel - 10) / 360) * 2 * M_PI)));
		//	shapeTransformation(yLocalRotate, -theSign * DEGREEMOVMENT);

		//	nextShapeRotate = -1;
		//	break;

		//default: break;
		//}


		if (actionDone) {
			nextactionDone = true;
			//nextShapeRotate = 7;
		}
		actionDone = false;
		pickedShape = p;
	}
	Sleep(800);
}


//void Game::Motion()
//{
//	if(isActive)
//	{
//		
//	}
//}

Game::~Game(void)
{
	delete curve;
}
