#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "VertexArray.hpp"
#include "Mesh.h"
#include "bezier1D.h"
#include "kdtree.h"
#define SCALE 1.0f
#define SCALE2 0.05f
class BBD
{

	BoundingBox* box;
	int shapeIndex;

public:
	BBD* left;
	BBD* right;
	void setShapeIndex(int shapeIndex);
	int getShapeIndex();
	BoundingBox* getBoxData();
	void makeCreations(BBD* it, Node *KD_Node, int level,int levelLimit);
	void makeCenters(int height, BBD* it, Node *KD_Node);
	BBD* getLeft() {
		return this->left;
	}
	BBD* getRight() {
		return this->right;
	}

	glm::vec4 calculateTotalCenetrs(Node *KD_Node);
	glm::vec4 calculateTotalMax(Node *KD_Node, glm::vec3 center);
	int calculateNumNodes(Node *KD_Node);
	BBD();
	BBD(Node *KD_Node, glm::vec4 PrevFatherData,int levelLimit);
	~BBD();
};


class MeshConstructor
{
	VertexArray vao;
	IndexBuffer *ib;

	BBD* data;
	//TO DO: add bounding box data base and build it in the constructor 
	std::vector<VertexBuffer*> vbs;
	bool is2D;
	int unsigned indicesNum;

	void InitLine(IndexedModel &model);
	void InitMesh(IndexedModel &model);
	void CopyMesh(const MeshConstructor &mesh);
	void CopyLine(const MeshConstructor &mesh);
	static const unsigned int VEC3_ATTRIB_NUM = 4;
	static const unsigned int VEC2_ATTRIB_NUM = 1;

public:
	void makeBuildings(Node* theNode, glm::vec4 PrevFatherData, int levelLimit);
	//TO DO: add collision detection function which get other MeshConstructor and Mat4 of related transformasions. The function may return a pointer to the relevant Bounding Box when collide
	enum SimpleShapes
	{
		Axis,
		Plane,
		Cube,
		Octahedron,
		Tethrahedron,
		BezierLine,
		BezierSurface,
	};
	MeshConstructor(const int type);
	MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS, int whichShape);

	MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS);

	MeshConstructor(const MeshConstructor &mesh);
	MeshConstructor(const std::string& fileName);
	BBD* getBBD();
	void Bind() { vao.Bind(); }
	void Unbind() { vao.Unbind(); }
	inline unsigned int GetIndicesNum() { return indicesNum; }
	inline bool Is2D() { return is2D; }
	~MeshConstructor(void);

};

