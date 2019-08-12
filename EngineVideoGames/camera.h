#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Viewport.h"
#include <iostream>

#include "glm/ext.hpp"
//#include "../Game/game.h"

struct Camera : public MovableGLM
{
public:
	Camera(const glm::vec3& pos,const glm::vec3& forward, float fov, float zNear, float zFar, const Viewport &view,glm::vec3 lookAt)
	{
		this->pos = pos;
		this->forward = forward; 
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov,GetWHRelation() , zNear, zFar);
		this->projection = this->projection * glm::lookAt(pos+ lookAt, pos + forward, up);
		this->fov = fov;
		this->near = zNear;
		this->far = zFar;
		this->vp = view;
	}

	void setProjection(float zNear, float zFar, Viewport &view)
	{
		this->vp = view;
		this->projection = glm::perspective(fov,view.GetWHRelation(), zNear, zFar)* glm::lookAt(pos, pos + forward, up);
		this->near = zNear;
		this->far = zFar;		
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection ;
	}



	glm::vec3 retXVector(glm::vec3 currentCenter,  int angelX) {

		float radAng = ((float)angelX / 360) * 2 * M_PI;

		return glm::vec3(currentCenter.x,
			currentCenter.y*cos(radAng) - currentCenter.z*sin(radAng),
			+currentCenter.y*sin(radAng) + currentCenter.z*cos(radAng));
	}


	glm::vec3 retZVector(glm::vec3 currentCenter,  int angelZ) {





		float radAng = ((float)angelZ / 360) * 2 * M_PI;

		return glm::vec3(currentCenter.x*cos(radAng) - currentCenter.y*sin(radAng),
			currentCenter.x*sin(radAng) + currentCenter.y*cos(radAng),
			currentCenter.z);
	}



	glm::vec3 retYVector(glm::vec3 currentCenter,int angelY) {

		float radAng = ((float)angelY / 360) * 2 * M_PI;

		return glm::vec3(currentCenter.x*cos(radAng) - currentCenter.z*sin(radAng),
			currentCenter.y,
			currentCenter.x*sin(radAng) + currentCenter.z*cos(radAng));
	}

	void cameraUpdate(Shape *snakeoffset,Shape *snakeHead) {

		//this->projection = glm::perspective(fov, GetWHRelation(), near, far)*glm::lookAt(pos, pos+forward, up);


		//this->projection = glm::perspective(fov, GetWHRelation(), near, far)*glm::lookAt(pos, pos+forward, up);


		//glm::vec3 tpos(snakeHead->makeTransScale()[3]/*-glm::vec4(0,0,3,0)*/);
		//glm::vec3 mypos(snakeHead->makeTransScale()*shapeOffset->makeTrans()[3]/*- snakeHead->getTraslate()*/);
		//this->projection = glm::perspective(fov, GetWHRelation(), near, far)*glm::lookAt(mypos, tpos, up);

		glm::vec4 vec0ff = glm::vec4(0, 3, 3, 0);

		glm::vec4 vec0ff2 = snakeoffset->makeTrans()[3];

		glm::vec3 tpos(snakeHead->makeTransScale()[3]);

		glm::vec4 mypos1 = snakeHead->makeTransScale()*vec0ff2;


		//		//vec0ff2 = glm::vec4(vec0ff2.x, vec0ff2.y, vec0ff2.z, vec0ff2.w);


		glm::vec3 mypos(/*abs*/(mypos1.x), /*abs*/(mypos1.y), /*abs*/(mypos1.z)) ;
		//mypos = retZVector(mypos,90);
		//mypos = retXVector(mypos, 90);
		//mypos = retYVector(mypos, 90);

		std::cout <<"my tpos--"<< glm::to_string(tpos) << std::endl;


		//this->projection = glm::perspective(fov, GetWHRelation(), near, far)*glm::lookAt(tpos + glm::vec3(0, 1.0, 0), mypos, up);

		this->projection = glm::perspective(fov, GetWHRelation(),near,far)*glm::lookAt(mypos , tpos+glm::vec3(0,1,0), up);
		//this->projection[1][1] = abs(this->projection[1][1]);

		std::cout << "my proj--" << glm::to_string(this->projection) << std::endl;









	}


	int GetWidth()
	{
		return vp.GetWidth();
	}

	int GetHeight()
	{
		return vp.GetHeight();
	}

	int GetLeft()
	{
		return vp.GetLeft();
	}

	int GetBottom()
	{
		return vp.GetBottom();
	}

	void MoveForward(float amt)
	{
		pos += forward * amt;
	}

	void MoveRight(float amt)
	{
		pos += glm::cross(up, forward) * amt;
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));
	}

	void RotateY(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	}
	inline float GetAngle()
	{
		return fov;
	}

	inline float GetNear()
	{
		return near;
	}
	inline float GetFar()
	{
		return far;
	}
	inline float GetWHRelation()
	{
		return vp.GetWHRelation();
	}
protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	float fov;
	float far,near;
	Viewport vp;
};

#endif
