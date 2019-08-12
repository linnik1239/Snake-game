#include <GL\glew.h>
#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <map>
#include <math.h>
#include "glm/ext.hpp"
#include "bezier2D.h"
#include "camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <limits>
#include <time.h>       /* time */
#include <windows.h>
#include "obj_loader.h"
#include <cstddef>
#include <glm/vec4.hpp>  
 
#include <SDL.h>


#define DEGREEMOVMENT 10
#define DEGREEMOVMENTY 10



#define NUMBALLS 2
#define NUMBLOCKS 30

#define MUS_PATH "Sounds/Jump.wav"
#define MUS_PATH_CRUSH "Sounds/Jump.wav"




static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

using namespace glm;

glm::vec4 oldpos[NUMSEGMENTS * 3 + 1];


std::vector<int> shapesIndexesBoxes;
std::vector<int> shapesIndexesBalls;



static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return (float)atof(token.substr(start, end - start).c_str());
}
//
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
	unsigned int result = start;
	while (result < length)
	{
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}


glm::vec3 ParseOBJVec3(const std::string& line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x, y, z);

	//glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

std::list<glm::vec4> giveList(const std::string& fileName,float ScaleMult)
{
	std::list<glm::vec4> theList;


	std::ifstream file;
	file.open(fileName.c_str());

	std::string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			unsigned int lineLength = line.length();

			if (lineLength < 2)
				continue;

			const char* lineCStr = line.c_str();

			switch (lineCStr[0])
			{
			case 'v':
				if (lineCStr[1] == 't') {
				}
				else if (lineCStr[1] == 'n')
				{

				}
				else if (lineCStr[1] == ' ' || lineCStr[1] == '\t') {
					//std::cout << "coord: " << glm::to_string(ParseOBJVec3(line)) << std::endl;
					glm::vec4 curList = glm::vec4(ParseOBJVec3(line)*ScaleMult, 1);
					curList.z -= 1;
					theList.push_back(curList);
				}

				break;
			default: break;
			};
		}
	}
	else
	{
		std::cerr << "Unable to load mesh: " << fileName << std::endl;
	}
	return theList;
}
void Game::changeSnakeViewPoint() {
	viewGromSnake = !viewGromSnake;
	float radAngel = ((float)angel / 360) * 2 * M_PI;
	float radAngelY = ((float)angelYY / 360) * 2 * M_PI;

	if (viewGromSnake) {


		glm::vec4 trans = this->getTraslate();
		myTranslate(glm::vec3(-trans)/*+ glm::vec3(0,-3,0)*/, 0);

		//printf("Angel = %f\n", angel);
		myRotate(angel, glm::vec3(0, 0, 0), 4);
		// myRotate(angelYY, glm::vec3(0, 0, 0), 5);
		 

		//myTranslate(glm::vec3(-shapes[6]->getTraslate()), 0);
		glm::vec3 trans2 = glm::vec3(shapes[kastCylindeIndex]->getTraslate());


		






		globalSystemRot(angelYY/*-(float)angelYY*(cos(radAngel))*/, glm::vec3(cos(radAngel), 0, sin(radAngel)), 6);



		glm::mat3 rotY = glm::mat3(0.0);
		rotY[1][1] = 1.0;
		rotY[0][0] = cos(radAngel);
		rotY[0][2] = -sin(radAngel);
		rotY[2][0] = sin(radAngel);
		rotY[2][2] = cos(radAngel);

		
		

		myTranslate(glm::vec3(0, -3, -3), 0);

		

		glm::vec3 rotatedVecY = -rotY*trans2;

	








		glm::mat3 rotX = glm::mat3(0.0);
		rotX[0][0] = -cos(radAngel);


		rotX[0][1] = sin(-radAngelY)*(sin(radAngel)) ;

		//rotX[1][0] = -sin(-radAngelY)*(sin(radAngel));  // To delete


		rotX[2][0] = -sin(radAngel)*(/*sin(-radAngelY)+ cos(-radAngelY)*/ 1);

		rotX[1][1] = -cos(-radAngelY); //!!!
		rotX[1][2] = sin(-radAngelY);
		rotX[2][1] = -sin(-radAngelY)*(cos(radAngel)/*+ sin(radAngel)*/);
		rotX[2][2] = -cos(-radAngelY)*(cos(radAngel)/*+ sin(radAngel)*/);

		rotX[0][2] = sin(radAngel)*(cos(-radAngelY));


		glm::mat3 matI = glm::mat3(0.0);
		matI[0][0] = 1.0;


		matI[1][1] = rotY[1][1];
		matI[2][2] = rotY[2][2];


		glm::vec3 rotatedVecX = rotX*(trans2);


		myTranslate(rotatedVecX, 0);




	}
	else {
		glm::vec4 trans = this->getTraslate();
		myTranslate(glm::vec3(-trans)/*+ glm::vec3(0,-3,0)*/, 0);
		//angelYY
		globalSystemRot(-angel, glm::vec3(0, 0, 0), 4);

		//globalSystemRot(-(float)angelYY*(cos(radAngel)), glm::vec3(0, 0, 0), 1);

		globalSystemRot(angelYY/*-(float)angelYY*(cos(radAngel))*/, -glm::vec3(cos(radAngel), 0, sin(radAngel)), 6);
		//globalSystemRot(-(float)angelYY*(sin(radAngel)), glm::vec3(0, 0, 1), 6);
			
		myTranslate(glm::vec3(0, -30, -58), 0);
	}

	

}
void Game::makeCubes(BBD* theShape, int level, int ParentIndex, Node* theNode, glm::vec4 PrevFatherData,int levelLimit) {


	if (theShape != NULL) {
		if (theShape->getBoxData()->getSize().x == 0 || theShape->getBoxData()->getSize().y == 0 || theShape->getBoxData()->getSize().z == 0) {
			return;
		}


		addShape(Cube, -1, LINE_LOOP);
		pickedShape = shapes.size() - 1;
		shapes[pickedShape]->getMesh()->makeBuildings(theNode, PrevFatherData, levelLimit);
		theShape->setShapeIndex(pickedShape);


		shapeTransformation(xScale, theShape->getBoxData()->getSize().x);
		shapeTransformation(yScale, theShape->getBoxData()->getSize().y);
		shapeTransformation(zScale, theShape->getBoxData()->getSize().z);


		shapeTransformation(xGlobalTranslate, theShape->getBoxData()->getCenter().x / theShape->getBoxData()->getSize().x);
		shapeTransformation(yGlobalTranslate, theShape->getBoxData()->getCenter().y / theShape->getBoxData()->getSize().y);
		shapeTransformation(zGlobalTranslate, theShape->getBoxData()->getCenter().z / theShape->getBoxData()->getSize().z);


		chainParents.pop_back();
		chainParents.push_back(ParentIndex);


		if (level >= 0 /*&& ParentIndex<=shapesIndexes[shapesIndexes.size()-1]+1*/) {
			shapes[pickedShape]->Hide();
		}

		makeCubes(theShape->left, level + 1, ParentIndex, theNode->left, theNode->data, levelLimit);
		makeCubes(theShape->right, level + 1, ParentIndex, theNode->right, theNode->data, levelLimit);


	}

}
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

Game::Game(glm::vec3 position, float angle, float near1, float far1, Viewport &vp,glm::vec3 lookAt) : Scene(position, angle, near1, far1, vp, lookAt)
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

void my_audio_callback(void *userdata, Uint8 *stream, int len) {

	if (audio_len == 0)
		return;

	len = (len > audio_len ? audio_len : len);
	//SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

	audio_pos += len;
	audio_len -= len;
}

void Game::Init()
{


	buildSegment2();

	plane2D = new Shape(Plane, TRIANGLES);
	plane2D->SetShader(2);


	addShape(Axis, -1, LINES);



	if (viewGromSnake) {
		myTranslate(glm::vec3(0, -3, 6), 0);
	}
	else {
		myTranslate(glm::vec3(0, -30, -58), 0);
		
	}


	pickedShape = -1;
	newInit();
}




void Game::CreatedBox(int xTranslate, int yTranslate, int zTranslate, Node* theNode,int levelLimit) {
	addShape(Cube, -1, TRIANGLES);
	shapesIndexesBoxes.push_back(shapes.size() - 1);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(rand() % 3 + 4);





	int boxPickedShape = shapes.size() - 1;




	addShape(Cube, -1, LINE_LOOP);
	shapes[shapes.size() - 1]->getMesh()->makeBuildings(theNode, glm::vec4(0, 0, 0, 1), levelLimit);
	shapes[shapes.size() - 1]->Hide();

	pickedShape = boxPickedShape + 1;
	shapeTransformation(zGlobalTranslate, 1);
	shapes[shapes.size() - 1]->Hide();
	makeCubes(shapes[shapes.size() - 1]->getMesh()->getBBD(), 0, boxPickedShape + 1, theNode, glm::vec4(0, 0, 0, 1), levelLimit);
	chainParents[boxPickedShape + 1] = boxPickedShape;

	pickedShape = boxPickedShape;



	shapeTransformation(zGlobalTranslate, zTranslate);
	shapeTransformation(xGlobalTranslate, xTranslate);
	shapeTransformation(yGlobalTranslate, yTranslate);
}





void Game::CreatedBaLL(int xTranslate, int yTranslate, int zTranslate, Node* theNode, int levelLimit){
	addShapeFromFile("../res/objs/ball.obj", -1, TRIANGLES);
	shapesIndexesBalls.push_back(shapes.size() - 1);

	int ballPickedShape = shapes.size() - 1;
	pickedShape = shapes.size() - 1;

	shapeTransformation(yScale, SCALE2* SCALE);
	shapeTransformation(xScale, SCALE2* SCALE);
	shapeTransformation(zScale, SCALE2*SCALE);


	addShape(Cube, -1, LINE_LOOP);
	shapes[shapes.size() - 1]->getMesh()->makeBuildings(theNode, glm::vec4(0, 0, 0, 1), levelLimit);
	shapes[shapes.size() - 1]->Hide();

	pickedShape = ballPickedShape + 1;
	shapeTransformation(zGlobalTranslate, 1);
	shapes[shapes.size() - 1]->Hide();
	makeCubes(shapes[shapes.size() - 1]->getMesh()->getBBD(), 0, ballPickedShape + 1, theNode, glm::vec4(0, 0, 0, 1), levelLimit);
	chainParents[ballPickedShape + 1] = ballPickedShape;

	pickedShape = ballPickedShape;

	shapeTransformation(xGlobalTranslate, xTranslate / SCALE2);
	shapeTransformation(yGlobalTranslate, yTranslate / SCALE2);
	shapeTransformation(zGlobalTranslate, zTranslate / SCALE2);
}



int ballPickedShape3;


void Game::newInit()
{






	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE);
	shapes[shapes.size() - 1]->SetTexture(8);
	shapes[shapes.size() - 1]->curAngel = 180;

	shapes[shapes.size() - 1]->curAngelYY = 0;
	shapes[shapes.size() - 1]->prevAngelYY = 0;
	



	setPickedShape(6);

	shapeTransformation(zGlobalTranslate, -9 -30);
	shapeTransformation(yGlobalTranslate, 1);





	
	for (int i = 0; i < NumCilinders; ++i) {


		addShape(6/*BezierSurface*/, -1, 7/*Quads*/, CYLINDER);
		shapes[shapes.size() - 1]->curAngel = 180;

		shapes[shapes.size() - 1]->curAngelYY = 0;
		shapes[shapes.size() - 1]->prevAngelYY = 0;
		shapes[shapes.size() - 1]->SetTexture(8);

		setPickedShape(i+7);
		shapeTransformation(/*zScale*/zGlobalTranslate, -30 -0.25*(NumCilinders-1 -i));
		shapeTransformation(/*zScale*/yGlobalTranslate, 1);

	}

	

	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE2);
	shapes[shapes.size() - 1]->curAngel = 180;
	kastCylindeIndex = shapes.size() - 1;

	shapes[shapes.size() - 1]->curAngelYY = 0;
	shapes[shapes.size() - 1]->prevAngelYY = 0;
	lastSphereIndex = shapes.size() - 1;
	shapes[lastSphereIndex]->SetTexture(8);

	setPickedShape(shapes.size()-1);

	shapeTransformation(/*zScale*/zGlobalTranslate,  1-30);
	shapeTransformation(/*zScale*/yGlobalTranslate, 1);




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(15);


	shapeTransformation(xLocalRotate, 90);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2 );
	shapeTransformation(yGlobalTranslate, -FLATSIZE / 4);



	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(15);
	//shapes[shapes.size() - 1]->Hide();


	shapeTransformation(xLocalRotate, 90);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(zGlobalTranslate, FLATSIZE );
	shapeTransformation(yGlobalTranslate, -FLATSIZE / 4+0.001);








	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);

	shapes[shapes.size() - 1]->SetTexture(14);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zLocalRotate, 180);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2 +0.002 );
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2 );
	shapeTransformation(yGlobalTranslate, FLATSIZE );
	shapeTransformation(yLocalRotate, 180);
	shapes[shapes.size() - 1]->Hide();

	portalShape = shapes.size() - 1;



	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);

	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zLocalRotate, 180);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(yGlobalTranslate, FLATSIZE);
	shapeTransformation(yLocalRotate, 180);





	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);

	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zLocalRotate, 90);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(yGlobalTranslate, -FLATSIZE );
	shapeTransformation(yLocalRotate, 180);





	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT );
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2 );
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2 );
	shapeTransformation(yLocalRotate, -90);






	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2+20);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2+0.001);
	shapeTransformation(yLocalRotate, -90);



	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(yGlobalTranslate, -FLATSIZE);
	shapeTransformation(yLocalRotate, -90);




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2 );





	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2+20);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2 + 0.001);
	shapeTransformation(yGlobalTranslate, -FLATSIZE);
	shapeTransformation(yLocalRotate, -90);




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2+20);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2+0.001);






	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(yGlobalTranslate, -FLATSIZE);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2);





	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(12);

	shapeTransformation(xLocalRotate, -90);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2);
	shapeTransformation(zGlobalTranslate, FLATSIZE/2 );
	shapeTransformation(yGlobalTranslate, FLATSIZE +0.001 );




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(yGlobalTranslate, -FLATSIZE);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2+20);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2 + 0.001);



	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	setPickedShape(shapes.size() - 1);
	shapes[shapes.size() - 1]->SetTexture(12);


	shapeTransformation(xLocalRotate, -90);
	shapeTransformation(yLocalRotate, 90);
	shapeTransformation(xGlobalTranslate, FLATSIZE / 2+0.001);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2+20);
	shapeTransformation(yGlobalTranslate, FLATSIZE);






	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2 +20);
	shapeTransformation(xGlobalTranslate, -FLATSIZE / 2);
	shapeTransformation(yGlobalTranslate, FLATSIZE);
	shapeTransformation(yLocalRotate, -90);
	shapeTransformation(zLocalRotate, -90);
	shapeTransformation(xLocalRotate, -90);





	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(4);
	setPickedShape(shapes.size() - 1);
	shapeTransformation(zGlobalTranslate, FLATSIZE / 2+20);
	shapeTransformation(xGlobalTranslate, -FLATSIZE /2 );
	shapeTransformation(yLocalRotate, -90);
	shapeTransformation(zLocalRotate, -90);
	shapeTransformation(xLocalRotate, -90);




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, FLAT);
	shapes[shapes.size() - 1]->SetTexture(5); 
	setPickedShape(shapes.size() - 1);

	goldFlatIndex = shapes.size() - 1;
	shapes[shapes.size() - 1]->Hide();

	shapeTransformation(zGlobalTranslate, -FLATSIZE / 2 +0.01);
	shapeTransformation(xGlobalTranslate, 4.45);

	shapeTransformation(yGlobalTranslate, FLATSIZE / 2+4.7);

	shapeTransformation(yScale, 0.095);
	shapeTransformation(xScale, 0.17);

	shapeTransformation(yLocalRotate, -90);
	shapeTransformation(zLocalRotate, -90);
	shapeTransformation(xLocalRotate, 90);



	/* initialize random seed: */
	srand(time(NULL));

	/* generate secret number between 1 and 10: */
	Kdtree *kdtree3;
	kdtree3 = new Kdtree();

	std::list<glm::vec4> theListBoxes = giveList("../res/objs/boxIndexes.obj", 1);
	kdtree3->makeTree(theListBoxes);

	Node* theNodeBoxes = kdtree3->getRoot();



	for (int i = 0;i < NUMBLOCKS;++i) {
		CreatedBox(rand() % 80 - 40, rand() % 120 - 60, rand() % 80 - 40, theNodeBoxes,-1);
	}




	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, BIGCYLINDER);
	shapes[shapes.size() - 1]->SetTexture(2);
	int cylinderIndex = getShapeSize() - 1;
	pickedShape = cylinderIndex;
	shapeTransformation(/*zScale*/zGlobalTranslate, -FLATSIZE / 2 - 1);
	shapeTransformation(/*zScale*/xGlobalTranslate, -8);
	//int cylinderIndex = getShapeSize() - 1;



	Activate();
	Surface_flag = 1;




	shapes[shapes.size() - 1]->SetTexture(7);

	std::list<glm::vec4> theList = giveList("../res/objs/ball.obj", SCALE2);
	//shapesIndexes.push_back(shapes.size() - 1);
	Kdtree *kdtree1;
	kdtree1 = new Kdtree();
	kdtree1->makeTree(theList);
	Kdtree *kdtree2;
	kdtree2 = new Kdtree();

	



	Node* theNode = kdtree1->getRoot();
	
	
	std::list<glm::vec4> theList2 = giveList("../res/objs/halfSphereIndexes.obj",1);
	kdtree2->makeTree(theList2);

	Node* theNode2 = kdtree2->getRoot();



	


	for (int i = 0;i < NUMBALLS;++i) {
		CreatedBaLL(rand() % 50 - 25, rand() % 60 - 10, rand() % 50 - 25, theNode,5);
	}




	numRemainedBalls = shapesIndexesBalls.size();


	addShape(Cube, -1, LINE_LOOP);
	ballPickedShape3 = shapes.size() - 1;

	shapes[ballPickedShape3]->getMesh()->makeBuildings(theNode2, glm::vec4(0, 0, 0, 1),5);

	makeCubes(shapes[shapes.size() - 1]->getMesh()->getBBD(), 6, ballPickedShape3, theNode2, glm::vec4(0, 0, 0, 1),5);
	chainParents[ballPickedShape3] = kastCylindeIndex;






	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, CYLINDER);
	shapeOffsetIndex = shapes.size() - 1;

	setPickedShape(shapeOffsetIndex);
	shapeTransformation(/*zScale*/zGlobalTranslate, -0.8 );
	shapeTransformation(/*zScale*/yGlobalTranslate, 1);


	addShape(6/*BezierSurface*/, -1, 7/*Quads*/, HALFSPHERE);







}

void Game::changeBendingDirection() {


}


void Game::Update(glm::mat4 *matTranses, const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx, int shapeIndex, glm::mat4 Camera)
{

	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;


		s->Bind();

		s->SetUniformMat4fv("jointTransforms", matTranses, getNumCylinders() + 2);
		s->SetUniformMat4f("MVP", MVP);
		s->SetUniformMat4f("Normal", Normal);
		s->SetUniformMat4f("Camera", Camera);
		s->SetUniform1i("linksNum", getNumCylinders() + 2);
		s->SetUniform1i("shapeIndex", shapeIndex);
		s->SetUniform1i("flag2", 1);




		s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
		if (shaderIndx == 0)
			s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		else
			s->SetUniform4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
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


	


	if (pickedShape==6) {
		s->SetUniform1i("flag2", 1);
		s->SetUniform1i("sign", 1);

		mat4 FatherTransforms = mat4(1);

		for (int i = pickedShape+1; i <= kastCylindeIndex; ++i) {
			FatherTransforms = FatherTransforms* shapes[i]->makeTrans();
		}
		glm::mat3x4 pre = glm::mat3x4(glm::transpose(shapes[pickedShape]->makeTrans()));
		glm::mat3x4 cur = glm::mat3x4(glm::transpose(Normal2/*shapes[pickedShape]->makeTrans()*/));
	
		glm::mat3x4 next = glm::mat3x4(glm::transpose(shapes[pickedShape +1]->makeTrans()));

		glm::dualquat q_prev = glm::dualquat_cast(pre);
		glm::vec4 rot_prev = glm::vec4(q_prev.real.x, q_prev.real.y, q_prev.real.z, q_prev.real.w);
		glm::vec4 trans_prev = glm::vec4(q_prev.dual.x, q_prev.dual.y, q_prev.dual.z, q_prev.dual.w);

		glm::dualquat q_cur = glm::dualquat_cast(cur);
		glm::vec4 rot_cur = glm::vec4(q_cur.real.x, q_cur.real.y, q_cur.real.z, q_cur.real.w);
		glm::vec4 trans_cur = glm::vec4(q_cur.dual.x, q_cur.dual.y, q_cur.dual.z, q_cur.dual.w);



		glm::dualquat q_next = glm::dualquat_cast(next);
		glm::vec4 rot_next = glm::vec4(q_next.real.x, q_next.real.y, q_next.real.z, q_next.real.w);
		glm::vec4 trans_next = glm::vec4(q_next.dual.x, q_next.dual.y, q_next.dual.z, q_next.dual.w);


		s->SetUniform4f("rot_prev", rot_prev.x, rot_prev.y, rot_prev.z, rot_prev.w);
		s->SetUniform4f("rot_prev", trans_prev.x, trans_prev.y, trans_prev.z, trans_prev.w);

		s->SetUniform4f("rot_cur", rot_cur.x, rot_cur.y, rot_cur.z, rot_cur.w);
		s->SetUniform4f("trans_cur", trans_cur.x, trans_cur.y, trans_cur.z, trans_cur.w);

		s->SetUniform4f("rot_next", rot_next.x, rot_next.y, rot_next.z, rot_next.w);
		s->SetUniform4f("trans_next", trans_next.x, trans_next.y, trans_next.z, trans_next.w);

	}



	s->Unbind();
}


void Game::WhenRotate()
{
	
}




void Game::WhenTranslate()
{
	if (pickedShape >= 0)
	{
		std::cout << "pickedShape: " << pickedShape << std::endl;


		glm::vec4 pos = GetShapeTransformation()*glm::vec4(0, 0, 0, 1);

		glm::vec4 delta = pos - oldpos[pickedShape];

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
	this->cameraMode = !this->cameraMode;
}
bool Game::isCameraMode() {
	return this->cameraMode;
}

void Game::UpxMotion(bool up) {

	if (viewGromSnake) {
		changeSnakeViewPoint();
		viewGromSnakeFlag = true;
	}

	if (up) {


		if (viewGromSnake) {
			snakeViewAngel += DEGREEMOVMENT;
		}


		angel += DEGREEMOVMENT;

		angel = (float)((int)angel % 360);



		theSign = 1;
		//shapeTransformation(yLocalRotate, -DEGREEMOVMENT);
		actionDone = true;
		angelchanges = true;

		//shapeTransformation(xGlobalTranslate, x_motion);
		x_motion += 0.01;
	}
	else {

		if (viewGromSnake) {
			snakeViewAngel -= DEGREEMOVMENT;
		}

		//shapeTransformation(yLocalRotate, DEGREEMOVMENT);
		actionDone = true;
		angelchanges = true;


		//printf("Angel = %f, shape angel = %f \n", angel, shapes[6]->theAngel);
		angel -= DEGREEMOVMENT;
		angel = (float)((int)angel % 360);
		theSign = -1;
		x_motion -= 0.01;
	}

}
void Game::UpyMotion(bool up) {


	if (viewGromSnake) {
		changeSnakeViewPoint();
		viewGromSnakeFlag = true;
	}

	if (up) {





		angelYY -= DEGREEMOVMENTY;
		angelYY = (float)((int)angelYY % 360);


		theSignY = 1;
		actionDone = true;
		angelchangesY = true;

		z_motion += 0.01;
	}
	else {


		actionDone = true;
		angelchangesY = true;


		angelYY += DEGREEMOVMENTY;
		angelYY = (float)((int)angelYY % 360);
		theSignY = -1;
		z_motion -= 0.01;
	}






}


float CurrentDegreeY = 0.0;
float CurrentDegreeX = 0.0;

glm::vec3 Game::retZVector(glm::vec3 currentCenter) {

	float theAngel = (-angel / 360) * 2 * M_PI;

	return glm::vec3(currentCenter.x*cos(theAngel) - currentCenter.y*sin(theAngel),
		currentCenter.x*sin(theAngel) + currentCenter.y*cos(theAngel),
		currentCenter.z);
}


glm::vec3 Game::retYVector(glm::vec3 currentCenter) {
	return glm::vec3(currentCenter.x*cos(CurrentDegreeY) - currentCenter.z*sin(CurrentDegreeY),
		currentCenter.y,
		currentCenter.x*sin(CurrentDegreeY) + currentCenter.z*cos(CurrentDegreeY));
}

glm::vec3 Game::retXVector(glm::vec3 currentCenter) {
	return glm::vec3(currentCenter.x,
		currentCenter.y*cos(CurrentDegreeX) - currentCenter.z*sin(CurrentDegreeX),
		+currentCenter.y*sin(CurrentDegreeX) + currentCenter.z*cos(CurrentDegreeX));
}


bool Game::isNowColision(BBD* currentBBD, BBD* otherBBD,int index1,int index2) {
	bool leftBool;
	bool rightBool;

	BoundingBox *otherBoxData = otherBBD->getBoxData();  


	glm::vec3 currentCenter = currentBBD->getBoxData()->getCenter();
	glm::vec3 rotatedCenter = retYVector(retZVector(currentCenter));



	glm::vec3 XI = glm::vec3(1,0,0);
	glm::vec3 YI = glm::vec3(0, 1, 0);
	glm::vec3 ZI = glm::vec3(0, 0, 1);
	glm::vec3 theNewCenter;

	glm::mat4 NewRotated = shapes[currentBBD->getShapeIndex()]->GetRot();
	glm::mat4 NewTrans = glm::translate(glm::mat4(1), glm::vec3(shapes[currentBBD->getShapeIndex()]->getTraslate()));


	theNewCenter = glm::vec3(NewTrans*glm::vec4(currentCenter,1));

	glm::vec3 XInew = glm::vec3(NewRotated*glm::vec4(XI, 1));
	glm::vec3 YInew = glm::vec3(NewRotated*glm::vec4(XI, 1));
	glm::vec3 ZInew = glm::vec3(NewRotated*glm::vec4(XI, 1));



	
	glm::vec3 Pa = rotatedCenter + glm::vec3(shapes[index1]->getTraslate());// glm::vec3(shapes[shapesIndexes[7]]->getTraslate().x, shapes[shapesIndexes[7]]->getTraslate().y, shapes[shapesIndexes[7]]->getTraslate().z);//globalCenter;

	glm::vec3 SizeA = currentBBD->getBoxData()->getSize();

	float theAngel = (-angel/360)*2*M_PI;

	glm::vec3 Ax1 = glm::normalize(glm::vec3(cos(theAngel)*XI.x - sin(theAngel)*XI.y,
		+sin(theAngel)*XI.x+cos(theAngel)*XI.y, XI.z));


	glm::vec3 Ay1 = glm::normalize(glm::vec3(cos(theAngel)*YI.x - sin(theAngel)*YI.y,
		+sin(theAngel)*YI.x + cos(theAngel)*YI.y, YI.z));

	glm::vec3 Az1 = glm::normalize(glm::vec3(cos(theAngel)*ZI.x - sin(theAngel)*ZI.y,
		+sin(theAngel)*ZI.x + cos(theAngel)*ZI.y, ZI.z));




	glm::vec3 Ax2 = glm::normalize(glm::vec3(cos(CurrentDegreeY)*Ax1.x-sin(CurrentDegreeY)*Ax1.z, Ax1.y, 
		sin(CurrentDegreeY)*Ax1.x + cos(CurrentDegreeY)*Ax1.z));
	glm::vec3 Ay2 = glm::normalize(glm::vec3(cos(CurrentDegreeY)*Ay1.x - sin(CurrentDegreeY)*Ay1.z, Ay1.y,
		sin(CurrentDegreeY)*Ay1.x + cos(CurrentDegreeY)*Ay1.z));
	glm::vec3 Az2 = glm::normalize(glm::vec3(cos(CurrentDegreeY)*Az1.x - sin(CurrentDegreeY)*Az1.z, Az1.y,
		sin(CurrentDegreeY)*Az1.x + cos(CurrentDegreeY)*Az1.z));




	glm::vec3 Ax = glm::normalize( glm::vec3(Ax2.x, Ax2.y*cos(CurrentDegreeX)- Ax2.z*sin(CurrentDegreeX),
		+Ax2.y*sin(CurrentDegreeX)+ Ax2.z*cos(CurrentDegreeX)));


	glm::vec3 Ay = glm::normalize(glm::vec3(Ay2.x, Ay2.y*cos(CurrentDegreeX) - Ay2.z*sin(CurrentDegreeX),
		+Ay2.y*sin(CurrentDegreeX) + Ay2.z*cos(CurrentDegreeX)));


	glm::vec3 Az = glm::normalize(glm::vec3(Az2.x, Az2.y*cos(CurrentDegreeX) - Az2.z*sin(CurrentDegreeX),
		+Az2.y*sin(CurrentDegreeX) + Az2.z*cos(CurrentDegreeX)));








	glm::vec3 Pb = glm::vec3(shapes[index2]->getTraslate());// otherBoxData->getCenter();
	glm::vec3 SizeB = otherBoxData->getSize();
	glm::vec3 Bx = otherBoxData->getXint();
	glm::vec3 By = otherBoxData->getYint();
	glm::vec3 Bz = otherBoxData->getZint();




	glm::vec3 T = Pa - Pb;
	glm::vec3 L[15];

	L[0] = Ax;
	L[1] = Ay;
	L[2] = Az;
	L[3] = Bx;
	L[4] = By;
	L[5] = Bz;
	L[6] = glm::cross(Ax, Bx);
	L[7] = glm::cross(Ax, By);
	L[8] = glm::cross(Ax, Bz);
	L[9] = glm::cross(Ay, Bx);
	L[10] = glm::cross(Ay, By);
	L[11] = glm::cross(Ay, Bz);
	L[12] = glm::cross(Az, Bx);
	L[13] = glm::cross(Az, By);
	L[14] = glm::cross(Az, Bz);



	for (int i = 0; i < 15; ++i) {
		float left = glm::abs(glm::dot(T, L[i]));
		float right =
			glm::abs(glm::dot(SizeA.x*Ax, L[i]))
			+ glm::abs(glm::dot(SizeA.y*Ay, L[i]))
			+ glm::abs(glm::dot(SizeA.z*Az, L[i]))
			+ glm::abs(glm::dot(SizeB.x*Bx, L[i]))
			+ glm::abs(glm::dot(SizeB.y*By, L[i]))
			+ glm::abs(glm::dot(SizeB.z*Bz, L[i]))
			;

		if (left > right) {
			return false;
		}
	}

	

	if (otherBBD->left != NULL && currentBBD->right != NULL) {
		leftBool = isNowColision(currentBBD->right, otherBBD->left, index1,index2);
		if (leftBool) {

			return true;
		}

	}


	if (otherBBD->left != NULL && currentBBD->left != NULL) {
		leftBool = isNowColision(currentBBD->left, otherBBD->left, index1, index2);
		if (leftBool) {

			return true;
		}

	}

	if (otherBBD->right != NULL && currentBBD->right != NULL) {
		//printf("Now is colission left: %d \n", otherBBD->right->getShapeIndex());
		leftBool = isNowColision(currentBBD->right, otherBBD->right, index1, index2);
		if (leftBool) {

			return true;
		}

	}




	if (otherBBD->right != NULL && currentBBD->left != NULL) {


		rightBool = isNowColision(currentBBD->left, otherBBD->right, index1, index2);
		if (rightBool) {


			return true;
		}
		else {
			shapes[currentBBD->getShapeIndex()]->Hide();
			shapes[otherBBD->getShapeIndex()]->Hide();
			return false;
		}
	
	}


	return true;
}


bool Game::anyWallColision(Shape* theScape) {


	glm::vec4 shapeCprd = theScape->getTraslate();
	glm::vec4 fronWallCord = shapes[12]->getTraslate();
	glm::vec4 leftWallCord = shapes[13]->getTraslate();
	glm::vec4 rightWallCord = shapes[14]->getTraslate();
	glm::vec4 buttom = shapes[11]->getTraslate();
	if (shapeCprd.z < fronWallCord.z) { return true; }
	if (shapeCprd.x < leftWallCord.x) { return true; }
	if (shapeCprd.x > rightWallCord.x) { return true; }

	return false;
}





bool Game::anyObjectColision(int index1,int index2) {
	


	int firstIndex = index1 +1;
	int secondIndex = ballPickedShape3;//shapesIndexes[8] + 1;

	return isNowColision(shapes[firstIndex]->getMesh()->getBBD(), shapes[secondIndex]->getMesh()->getBBD(), index1, index2);;
}


void Game::playTune(char* str) {

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return;

	//local variables
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music

	

	if (SDL_LoadWAV(str, &wav_spec, &wav_buffer, &wav_length) == NULL) {
		return;
	}
	// set the callback function
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length

							/* Open the audio device */
	if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	/* Start playing */
	SDL_PauseAudio(0);

	// wait until we're don't playing
	while (audio_len > 0) {
		SDL_Delay(100);
	}

	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);

}
int counterTurning = 0;

float alpha = 0.0f;

int prevRatio = 0;

bool prevLeft = true;
bool madeYRotate = false;

bool madeYChanges = false;

int prevAngeliOfTail = 0;
void Game::Motion()
{
	if (isActive)
	{
		counterTurning += 1;
		alpha += 0.01;
		if (alpha > 2 * M_PI) {
			alpha -= 2 * M_PI;
		}
		 
		int p = pickedShape;

		float total = abs(x_motion + y_motion + z_motion) + 1;

		float vel = 0.25;// 3.0;
		float vel2 = 0.05;









		if (numRemainedBalls == 0) {
			shapes[portalShape]->Unhide();
			shapes[goldFlatIndex]->Unhide();
			shapes[portalShape + 1]->Hide();
		}












		for (int i = 6+ NumCilinders+1; i >= 6; --i) {
			shapes[i]->prevCoordinates = shapes[i]->getTraslate();
			pickedShape = i;
			if (i <kastCylindeIndex) {
				float theVel = vel;
				


				


					shapeTransformation(xGlobalTranslate, -shapes[i]->getTraslate().x);
					shapeTransformation(yGlobalTranslate, -shapes[i]->getTraslate().y);
					shapeTransformation(zGlobalTranslate, -shapes[i]->getTraslate().z);

					shapeTransformation(yLocalRotate, -shapes[i]->curAngel);
					shapeTransformation(xLocalRotate, -(shapes[i]->curAngelYY));



					

					shapeTransformation(yLocalRotate, shapes[i + 1]->prevAngel);
					shapeTransformation(xLocalRotate, (shapes[i + 1]->prevAngelYY));


					shapeTransformation(xGlobalTranslate, shapes[i + 1]->prevCoordinates.x);
					shapeTransformation(yGlobalTranslate, shapes[i + 1]->prevCoordinates.y);
					shapeTransformation(zGlobalTranslate, shapes[i + 1]->prevCoordinates.z);;


					shapes[i]->prevAngel = shapes[i]->curAngel;

					shapes[i]->curAngel = shapes[i + 1]->prevAngel;




					shapes[i]->prevAngelYY = shapes[i]->curAngelYY;

					shapes[i]->curAngelYY = shapes[i + 1]->prevAngelYY;
				}


				
				


			

			if (i == kastCylindeIndex) {
				if (angelchanges) {


					shapeTransformation(yLocalRotate, -shapes[i]->prevAngel);
					shapeTransformation(xLocalRotate, -shapes[i]->curAngelYY);

					if (viewGromSnake) {











						










						
					}
					shapeTransformation(xLocalRotate, shapes[i]->curAngelYY);


					shapeTransformation(yLocalRotate, -angel);


					int s = shapes[i]->curAngel;


					shapes[i]->prevAngel = (-shapes[i]->curAngel % 360);
					shapes[i]->curAngel = (float)((int)angel % 360);
					if (viewGromSnakeFlag) {
						changeSnakeViewPoint();
						viewGromSnakeFlag = false;
					}

					break;

				}
				else if (angelchangesY) {

					madeYRotate = true;


					float DEGREEMOVMENTrad = ((float)DEGREEMOVMENTY / 360) * 2 * M_PI;

					shapeTransformation(yLocalRotate, -shapes[i]->curAngel);
					shapeTransformation(xLocalRotate, -shapes[i]->curAngelYY);
					if (viewGromSnake) {



					}


					shapeTransformation(xLocalRotate, angelYY);
					shapeTransformation(yLocalRotate, shapes[i]->curAngel);



					int s = shapes[i]->curAngel;



					shapes[i]->prevAngelYY = (shapes[i]->curAngelYY % 360);
					shapes[i]->curAngelYY = (float)((int)angelYY % 360);



					if (viewGromSnakeFlag) {
						changeSnakeViewPoint();
						viewGromSnakeFlag = false;
					}
					break;



				}
				else {



					shapeTransformation(xGlobalTranslate, vel*sin((angel / 360) * 2 * M_PI)*sin(((-angelYY + 90) / 360) * 2 * M_PI));
					shapeTransformation(yGlobalTranslate, -vel*cos(((-angelYY+90) / 360) * 2 * M_PI));
					shapeTransformation(zGlobalTranslate, -vel*cos((angel / 360) * 2 * M_PI)*sin(((-angelYY + 90) / 360) * 2 * M_PI));

					if (viewGromSnake) {


						glm::vec4 trans = this->getTraslate();


						myTranslate(glm::vec3(0, 0,  vel), 0);
					
					}

					int s = shapes[i]->curAngel;
					shapes[i]->prevAngelYY = (shapes[i]->curAngelYY % 360);
					shapes[i]->curAngelYY = (float)((int)angelYY % 360);


					shapes[i]->prevAngel = (-shapes[i]->curAngel % 360);
					shapes[i]->curAngel = (float)((int)angel % 360);
				}
				
			}
			


		}

		vec3 curtailSnakePosition = vec3(shapes[6]->getTraslate());
		if (curtailSnakePosition.z < -FLATSIZE/2) {
			printf("You won\n");
			isActive = false;
		}


		vec3 curHeadSnakePosition = vec3(shapes[kastCylindeIndex]->getTraslate());
		vec3 curgoldenFlatPopsition = vec3(shapes[goldFlatIndex]->getTraslate());

		if (numRemainedBalls <= 0 && curHeadSnakePosition.z < -FLATSIZE / 2 &&
			curHeadSnakePosition.x < curgoldenFlatPopsition.x + 0.17*(FLATSIZE / 2) &&
			curHeadSnakePosition.x > curgoldenFlatPopsition.x - 0.17*(FLATSIZE / 2) &&
			curHeadSnakePosition.y < curgoldenFlatPopsition.y + 0.295*(FLATSIZE / 2) &&
			curHeadSnakePosition.y > curgoldenFlatPopsition.y - 0.295*(FLATSIZE / 2)) {

		}
		else {


			if (curHeadSnakePosition.x < -FLATSIZE / 2 || curHeadSnakePosition.x > FLATSIZE / 2 || curHeadSnakePosition.y < -FLATSIZE / 4 || curHeadSnakePosition.y > FLATSIZE
				|| curHeadSnakePosition.z > 20 + FLATSIZE / 2 || curHeadSnakePosition.z < -FLATSIZE / 2) {
				printf("Crush on wall, you lost\n");
				playTune(MUS_PATH_CRUSH);
				isActive = false;
			}
		}


		for (int i = 0; i < shapesIndexesBoxes.size();++i) {
			if (anyObjectColision(shapesIndexesBoxes[i], lastSphereIndex)) {

				printf("Crush, you lost\n");
				playTune(MUS_PATH_CRUSH);
				isActive = false;

			}
		}



		for (int i = 0; i < shapesIndexesBalls.size();++i) {
			 if (anyObjectColision(shapesIndexesBalls[i], lastSphereIndex)) {

					shapes[shapesIndexesBalls[i]]->Hide();
					pickedShape = shapesIndexesBalls[i];
					shapeTransformation(xGlobalTranslate, 100000);
					numRemainedBalls--;

					printf("Collided to ball, number remained balls: %d\n", numRemainedBalls);

					playTune(MUS_PATH);

				}
		}





		if (angelchanges) {
			angelchangesNext = true;
		}
		angelchanges = false;
		angelchangesY = false;




		for (int i = 0; i < shapesIndexesBalls.size();++i) {
			pickedShape = shapesIndexesBalls[i];
			shapeTransformation(zGlobalTranslate, 1 * cos(alpha));
			shapeTransformation(xGlobalTranslate, 1 * sin(alpha));
			shapeTransformation(yGlobalTranslate, 1 * sin(alpha));



		}





		if (actionDone) {
			nextactionDone = true;
		}
		actionDone = false;



		pickedShape = p;



	}

}




Game::~Game(void)
{
	delete curve;
}
