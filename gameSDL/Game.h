#pragma once
#include "scene.h"
#include "bezier1D.h"

class Game : public Scene
{
	Bezier1D *curve;
	float x_motion = 0.000001;
	float y_motion = 0.00;
	float z_motion = 0.00;
	float angel = 0.0;
public:
	Game(void);
	Game(glm::vec3 position, float angle, float near, float far, Viewport &vp);
	//Game(glm::vec3 position, float angle, float hwRelation, float near, float far);
	void Init();


	void changeCameraMode();
	bool isCameraMode();
	void changeBendingDirection();
	void newInit();
	void addShape(int type, int parent, unsigned int mode);
	void addShape(int type, int parent, unsigned int mode, int whichShape);
	void makeQuater(int numShape);
	//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
	void Update(glm::mat4 MVP, glm::mat4 Normals, Shader* s);

	void UpxMotion(bool up);
	void UpyMotion(bool up);

	void Update2(const glm::mat4 &Projection, const glm::mat4 &Normal1, const glm::mat4 &Normal2, const glm::mat4 &Normal3, const int  shaderIndx);

	void ControlPointUpdate();
	void WhenRotate();
	void buildSegment();
	void buildSegment2();
	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx);
	void WhenTranslate();
	void Motion();
	int Surface_flag = 0;
	bool cameraMode = false;
	~Game(void);
};




//#pragma once
//#include "scene.h"
//#include "bezier1D.h"
//
//class Game : public Scene
//{
//	Bezier1D *curve;
//public:
//	Game(void);
//	Game(glm::vec3 position, float angle, float near, float far, Viewport &vp);
//	void Init();
//	void addShape(int type, int parent, unsigned int mode);
//	//	void Update( glm::mat4 MVP ,glm::mat4 *jointTransforms,const int length,const int  shaderIndx);
//	void Update(const glm::mat4 &MVP, const glm::mat4 &Normal, const int  shaderIndx);
//	void ControlPointUpdate();
//	void WhenRotate();
//	void WhenTranslate();
//	void Motion();
//	~Game(void);
//};
