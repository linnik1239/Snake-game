#include "InputManager.h"
#include "glm\glm.hpp"
#include <windows.h>

	int main(int argc,char *argv[])
{

	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 600;
	Viewport vp1(0,0,DISPLAY_WIDTH-0,DISPLAY_HEIGHT-0);
	Viewport vp2(0,0,0,0);
	Viewport vp3(0, 0, 0, 0);

	const float zFar = 100.0f;
	const float zNear = 1.0f; 
	const float CAM_ANGLE = 60.0f;

	Game *scn = new Game(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, zNear,zFar,vp1,glm::vec3(0,0,0));
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->Init();

	scn->addShader("../res/shaders/basicShader");

	scn->addShader("../res/shaders/basicShader4");


	scn->AddTexture("../res/textures/box0.bmp",false);
	scn->AddTexture("../res/textures/plane.png",true);
	scn->AddTexture(vp1.GetWidth(),vp1.GetHeight(),COLOR);
	scn->AddTexture(vp1.GetWidth(),vp1.GetHeight(),DEPTH);
	scn->AddBuffer(2,0,COLOR);
	scn->AddTexture("../res/textures/bricks.jpg", false);
	scn->AddTexture("../res/textures/coin1.png", false);
	scn->AddTexture("../res/textures/coin2.png", false);
	scn->AddTexture("../res/textures/coin3.png", false);
	scn->AddTexture("../res/textures/snake1.png", false);
	scn->AddTexture("../res/textures/sea.png", false);
	scn->AddTexture("../res/textures/sea2.png", false);
	scn->AddTexture("../res/textures/sea3.png", false);
	scn->AddTexture("../res/textures/sea4.png", false);
	//scn->AddTexture("../res/textures/sea_2.png", false);
	scn->AddTexture("../res/textures/sea4.png", false);
	scn->AddTexture("../res/textures/bricks2.jpg", false);
	scn->AddTexture("../res/textures/sea5.png", false); //15


	scn->AddCamera(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, zNear, zFar, vp3);
	scn->AddCamera(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, zNear, zFar, vp2);

	//scn->getCameras()[1]->myTranslate(glm::vec3(-40, -40, 35), -1);
	scn->getCameras()[1]->myRotate(90, glm::vec3(0, 1, 0), -1);

	display.setScene(scn);

	while(!display.closeWindow())
	{



		scn->Draw(1,0,BACK,true,false);
		scn->Draw(1, 1, BACK, false, false);
		scn->Draw(1, 2, BACK, false, false);


		if ((scn->getAngelchanges() && scn->getViewGromSnakeFlag()) || (scn->getAngelchangesY() && scn->getViewGromSnakeFlag())) {
			scn->Motion();
		}
		else {
			scn->Motion();

			display.SwapBuffers();
			display.PollEvents();
		}






		
	}
	delete scn;
	return 0;
}
