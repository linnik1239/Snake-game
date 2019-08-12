#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			scn->picking((int)x2,(int)y2);
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		scn->shapeTransformation(scn->zCameraTranslate,yoffset);
		
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		int p1;
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				case GLFW_KEY_SPACE:
					if (scn->Surface_flag == 0) {


						scn->newInit();
					}


					//scn->HideShape(1);
					//if(scn->IsActive())
					//	scn->Deactivate();
					//else
					//	scn->Activate(); GLFW_KEY_S
				break;


				case GLFW_KEY_S:


					//if ((int)scn->getAngelYY() % 360 == 0) {
						scn->changeSnakeViewPoint();
					//}
					
					break;

				case GLFW_KEY_RIGHT:
					//scn->setPickedShape(10);
					scn->UpxMotion(true);
					//scn->shapeTransformation(scn->zGlobalRotate,-20.1f);
					scn->shapeTransformation(scn->zLocalRotate,5.1f);
//						cout<< "right: "<<endl;
					break;
				case GLFW_KEY_LEFT:
					
					//scn->setPickedShape(scn->index);
					scn->UpxMotion(false);
					//scn->shapeTransformation(scn->zGlobalRotate,20.1f);
					scn->shapeTransformation(scn->zLocalRotate,-5.0f);
//					cout<< "left: "<<endl;
					break;
				case GLFW_KEY_UP:
					//scn->setPickedShape(10);
					scn->UpyMotion(true);
					//cout<< "up: "<<endl;
					scn->shapeTransformation(scn->xLocalRotate,5.f);
					break;
				case GLFW_KEY_DOWN:
					//scn->setPickedShape(10);
					scn->UpyMotion(false);
					scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					break;

			default:
				break;
			}
		}
	}
	

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->updatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			int p1 = scn->getPickedShape();
			if (10 < p1 && p1 < 15) {
				scn->setPickedShape(-1);
			}

			scn->mouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			int p1 = scn->getPickedShape();
			if (10 < p1 && p1 < 15) {
				scn->setPickedShape(-1);
			}
			scn->mouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->resize(width,height);
		
	}

	void init(Display &display)
	{
		display.addKeyCallBack(key_callback);
		display.addMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.addResizeCallBack(window_size_callback);

	}
