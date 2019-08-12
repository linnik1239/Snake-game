#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
	float x_motion = 0.000001;
	float y_motion = 0.00;
	float z_motion = 0.00;
	float angel = 180.0;
	//float angelY = 90.0;
	float angelYY = 0.0;




public:




	int portalShape;
	int getPortalShape() {
		return portalShape;
	}
	void setPortalShape(int portalShape) {
		this->portalShape = portalShape;
	}




	int getTheSign() {
		return theSign;
	}
	void setTheSign(int theSign) {
		this->theSign = theSign;
	}

	int getTheSignY() {
		return theSignY;
	}
	void setTheSignY(int theSignY) {
		this->theSignY = theSignY;
	}





	int getNextShapeRotate() {
		return nextShapeRotate;
	}
	void setNextShapeRotate(int nextShapeRotate) {
		this->nextShapeRotate = nextShapeRotate;
	}
	//

	int getNumRemainedBalls() {
		return numRemainedBalls;
	}
	void setNumRemainedBalls(int numRemainedBalls) {
		this->numRemainedBalls = numRemainedBalls;
	}
	//
	int getGoldFlatIndex() {
		return goldFlatIndex;
	}
	void setGoldFlatIndex(int goldFlatIndex) {
		this->goldFlatIndex = goldFlatIndex;
	}
	//
	int getPrevRotY() {
		return prevRotY;
	}
	void setPrevRotY(int prevRotY) {
		this->prevRotY = prevRotY;
	}
	//
	int getkAstCylindeIndex() {
		return kastCylindeIndex;
	}
	void setkAstCylindeIndex(int kastCylindeIndex) {
		this->kastCylindeIndex = kastCylindeIndex;
	}
	//
	int getNumCilinders() {
		return NumCilinders;
	}
	void setNumCilinders(int NumCilinders) {
		this->NumCilinders = NumCilinders;
	}
	//
	int getLastSphereIndex() {
		return lastSphereIndex;
	}
	void setLastSphereIndex(int lastSphereIndex) {
		this->lastSphereIndex = lastSphereIndex;
	}
	//
	int getShapeOffsetIndex() {
		return shapeOffsetIndex;
	}
	void setShapeOffsetIndex(int shapeOffsetIndex) {
		this->shapeOffsetIndex = shapeOffsetIndex;
	}
	//
	int getSnakeViewAngel() {
		return snakeViewAngel;
	}
	void setSnakeViewAngel(float snakeViewAngel) {
		this->snakeViewAngel = snakeViewAngel;
	}



	//


	int getViewGromSnake() {
		return viewGromSnake;
	}
	void setViewGromSnake(bool viewGromSnake) {
		this->viewGromSnake = viewGromSnake;
	}
	//
	int getViewGromSnakeFlag() {
		return viewGromSnakeFlag;
	}
	void setViewGromSnakeFlag(bool viewGromSnakeFlag) {
		this->viewGromSnakeFlag = viewGromSnakeFlag;
	}
	//
	int getAngelchanges() {
		return angelchanges;
	}
	void setAngelchanges(bool angelchanges) {
		this->angelchanges = angelchanges;
	}
	//
	int getAngelchangesY() {
		return angelchangesY;
	}
	void setAngelchangesY(bool angelchangesY) {
		this->angelchangesY = angelchangesY;
	}
	//
	int getAngelchangesNext() {
		return angelchangesNext;
	}
	void setAngelchangesNext(bool angelchangesNext) {
		this->angelchangesNext = angelchangesNext;
	}
	//
	int getActionDone() {
		return actionDone;
	}
	void setActionDone(bool actionDone) {
		this->actionDone = actionDone;
	}

	int getNextactionDone() {
		return nextactionDone;
	}
	void setNextactionDone(bool nextactionDone) {
		this->nextactionDone = nextactionDone;
	}

	
























	






	int getNumCylinders() {
		return NumCilinders;
	}


	Game(void);
	Game(glm::vec3 position, float angle, float near, float far, Viewport &vp, glm::vec3 lookAt);
	//Game(glm::vec3 position, float angle, float hwRelation, float near, float far, glm::vec3 lookAt);
	void Init();

	float getAngelYY() { return angelYY; }
	glm::vec3 retZVector(glm::vec3 currentCenter);
	glm::vec3 retYVector(glm::vec3 currentCenter);
	glm::vec3 retXVector(glm::vec3 currentCenter);

	void CreatedBox(int xTranslate, int yTranslate, int zTranslate, Node* theNode, int levelLimit);
	void CreatedBaLL(int xTranslate, int yTranslate, int zTranslate, Node* theNode, int levelLimit);
	void changeSnakeViewPoint();
	void playTune(char* str);
	void changeCameraMode();
	bool isCameraMode();
	void changeBendingDirection();
	void newInit();
	void addShape(int type, int parent, unsigned int mode);
	void addShape(int type, int parent, unsigned int mode, int whichShape);
	void makeQuater(int numShape);
	//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update(glm::mat4 MVP, glm::mat4 Normals, Shader* s);

	bool isNowColision(BBD* currentBBD, BBD* otherBBD/*, std::vector<Shape*> shapes*/, int insex1, int index2);

	void makeCubes(BBD* theShape, int level, int ParentIndex, Node* theNode, glm::vec4 PrevFatherData, int levelLimit);
	bool anyWallColision(Shape* theScape);
	bool anyObjectColision(int index1, int index2);
	void UpxMotion(bool up);
	void UpyMotion(bool up);

	void Update2(const glm::mat4 &Projection, const glm::mat4 &Normal1, const glm::mat4 &Normal2, const glm::mat4 &Normal3, const int  shaderIndx);

	void ControlPointUpdate();
	void WhenRotate();
	void buildSegment();
	void buildSegment2();
	void Update(glm::mat4 *matTranses, const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx, int shapeIndex, glm::mat4 Camera);
	void WhenTranslate();
	void Motion();
	int Surface_flag = 0;
	bool cameraMode = false;
	~Game(void);



	private:
		int theSign = 1;
		int theSignY = 1;
		int nextShapeRotate = -1;

		int numRemainedBalls;
		int goldFlatIndex;
		


		bool viewGromSnake = false;
		bool viewGromSnakeFlag = false;
		bool angelchanges = false;
		bool angelchangesY = false;
		bool angelchangesNext = false;
		int prevRotY = 10;

		int kastCylindeIndex;
		bool actionDone = false;
		bool nextactionDone = false;

		int NumCilinders = 36;

		int lastSphereIndex;


		int shapeOffsetIndex;

		float snakeViewAngel = 0.0;
};



