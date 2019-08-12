#define GLEW_STATIC
#include <GL\glew.h>
#include "MeshConstructor.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "bezier2D.h"
#include "obj_loader.h"


BoundingBox* BBD::getBoxData() {
	return this->box;
}


int BBD::calculateNumNodes(Node *KD_Node) {
	if (KD_Node == NULL) {
		return 0;
	}
	return 1 + calculateNumNodes(KD_Node->left) + calculateNumNodes(KD_Node->right);
}

glm::vec4 BBD::calculateTotalCenetrs(Node *KD_Node) {
	if (KD_Node == NULL) {
		return glm::vec4(0.0, 0.0, 0.0, 0.0);
	}
	return KD_Node->data + calculateTotalCenetrs(KD_Node->left) + calculateTotalCenetrs(KD_Node->right);

}



glm::vec4 BBD::calculateTotalMax(Node *KD_Node, glm::vec3 center) {


	if (KD_Node == NULL) {
		return glm::vec4(0, 0, 0, 1);
	}
	glm::vec4 currentMax = glm::abs(glm::vec4(KD_Node->data.x - center.x, KD_Node->data.y - center.y, KD_Node->data.z - center.z, 1));
	if (KD_Node->left == NULL && KD_Node->right == NULL) {
		return (currentMax);
	}

	glm::vec4 maxLeft = calculateTotalMax(KD_Node->left, glm::vec3(center));
	glm::vec4 maxRight = calculateTotalMax(KD_Node->right, glm::vec3(center));



	return glm::vec4(std::max({ (maxLeft.x), (maxRight.x),  (currentMax.x) }),
		std::max({ (maxLeft.y), (maxRight.y),(currentMax.y) }),
		std::max({ (maxLeft.z), (maxRight.z),  (currentMax.z) }), 1);
}


int counter = 0;

void BBD::makeCreations(BBD* it, Node *KD_Node, int level,int levelLimit) {
	if (level >levelLimit) {
		return;
	}
	if (KD_Node != NULL) {


		if (KD_Node->left != NULL && it->calculateNumNodes(KD_Node->left) >= 2) {


			glm::vec4 center = (it->calculateTotalCenetrs(KD_Node->left) + KD_Node->data) / (1 + (float)it->calculateNumNodes(KD_Node->left));
			glm::vec4 size = it->calculateTotalMax(KD_Node->left, glm::vec3(center));


			it->left = new BBD();
			it->left->shapeIndex = -1;
			it->left->box = new BoundingBox();
			it->left->box->setCenter(glm::vec3(center));
			it->left->box->setSize(glm::vec3(size));


			makeCreations(it->left, KD_Node->left, level + 1, levelLimit);
		}
		if (KD_Node->right != NULL && it->calculateNumNodes(KD_Node->right) >= 2) {

			glm::vec4 center = (it->calculateTotalCenetrs(KD_Node->right) + KD_Node->data) / (1 + (float)it->calculateNumNodes(KD_Node->right));
			glm::vec4 size = it->calculateTotalMax(KD_Node->right, glm::vec3(center));


			it->right = new BBD();
			it->right->shapeIndex = -1;
			it->right->box = new BoundingBox();
			it->right->box->setCenter(glm::vec3(center));
			it->right->box->setSize(glm::vec3(size));
			makeCreations(it->right, KD_Node->right, level + 1, levelLimit);
		}

	}
}



void BBD::makeCenters(int height, BBD* it, Node *KD_Node) {

	if (it == NULL) {
		return;
	}


	glm::vec4 newCenter = it->calculateTotalCenetrs(KD_Node);
	int numNodes = it->calculateNumNodes(KD_Node);
	newCenter /= (float)numNodes;


	it->getBoxData()->setSize(glm::vec3(it->calculateTotalMax(KD_Node, glm::vec3(newCenter))));

	it->getBoxData()->setCenter(glm::vec3(newCenter));


	makeCenters(height + 1, it->left, KD_Node->left);
	makeCenters(height + 1, it->right, KD_Node->right);


}


void BBD::setShapeIndex(int sjapeIndex) {
	this->shapeIndex = sjapeIndex;
}
int BBD::getShapeIndex() {
	return this->shapeIndex;
}


BBD::BBD(Node *KD_Node, glm::vec4 PrevFatherData,int levelLimit) {
	this->box = new BoundingBox();
	this->shapeIndex = -1;
	this->left = NULL;
	this->right = NULL;


	glm::vec4 center = (this->calculateTotalCenetrs(KD_Node) + PrevFatherData) / (1 + (float)this->calculateNumNodes(KD_Node));


	glm::vec4 size = this->calculateTotalMax(KD_Node, glm::vec3(center));


	this->box->setCenter(glm::vec3(center));
	this->box->setSize(glm::vec3(size));


	makeCreations(this, KD_Node, 0, levelLimit);

}




BBD::BBD() {
	this->left = NULL;
	this->right = NULL;

}

BBD::~BBD() {

}
int created = 0;



MeshConstructor::MeshConstructor(const int type)
{
	switch (type)
	{
	case Axis:
		InitLine(AxisGenerator());
		break;
	case Plane:
		InitMesh(PlaneTriangles());
		break;
	case Cube:
		InitMesh(CubeTriangles());
		break;
	case Octahedron:
		InitMesh(OctahedronGenerator());
		break;
	case Tethrahedron:
		InitMesh(TethrahedronGenerator());
		break;
	default:
		break;
	}

}

BBD* MeshConstructor::getBBD() {
	return this->data;

}


MeshConstructor::MeshConstructor(const std::string& fileName)
{
	InitMesh(OBJModel(fileName).ToIndexedModel());
}

MeshConstructor::MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS)
{
	//if(isSurface)
	//{
	//	Bezier2D surface(*curve,glm::vec3(0,0,1),4);
	//	InitMesh(surface.GetSurface(resT,resS));		
	//}
	//else
	//{
	//	InitLine( curve->GetLine(resT));
	//}
}

MeshConstructor::MeshConstructor(const MeshConstructor &mesh)
{
	indicesNum = mesh.indicesNum;
	if (mesh.is2D)
		CopyMesh(mesh);
	else
		CopyLine(mesh);
}

MeshConstructor::~MeshConstructor(void)
{
	if (ib)
		delete ib;
	for (unsigned int i = 0; i < vbs.size(); i++)
	{
		if (vbs[i])
			delete vbs[i];
	}
}

void MeshConstructor::InitLine(IndexedModel &model) {

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();

	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum*sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	ib = new IndexBuffer((unsigned int*)model.GetData(5), indicesNum);

	vao.Unbind();
	is2D = false;

}

void MeshConstructor::InitMesh(IndexedModel &model) {

	int verticesNum = model.positions.size();
	indicesNum = model.indices.size();

	vao.Bind();
	int i = 0;
	for (; i < VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum*sizeof(model.positions[0])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}
	for (; i < VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(model.GetData(i), verticesNum*sizeof(model.texCoords[0])));
		vao.AddBuffer(*vbs.back(), i, 2, GL_FLOAT);
	}

	ib = new IndexBuffer((unsigned int*)model.GetData(5), indicesNum);

	vao.Unbind();
	is2D = true;

}

MeshConstructor::MeshConstructor(Bezier1D *curve, bool isSurface, unsigned int resT, unsigned int resS, int whichShape)
{
	if (isSurface)
	{
		Bezier2D surface(*curve, glm::vec3(0, 0, 1), 4);

		switch (whichShape) {
		case CYLINDER: InitMesh(surface.GetSurface2(resT, resS, 0.8, 0.25)); break;
		case HALFSPHERE2: InitMesh(surface.GetSurface4(resT, resS)); break;
		case FLAT:  InitMesh(surface.GetSurfaceFLAT(resT, resS)); break;
		case BIGCYLINDER:  InitMesh(surface.GetSurfaceBigCyllinder(resT, resS, 5)); break;
		default: InitMesh(surface.GetSurface3(resT, resS)); break;
		}


	}
	else
	{
		InitLine(curve->GetLine(resT));
	}
}


void MeshConstructor::CopyLine(const MeshConstructor &mesh) {

	vao.Bind();

	for (int i = 0; i < 2; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}

	ib = new IndexBuffer(*mesh.ib);

	vao.Unbind();

	is2D = false;

}

void MeshConstructor::CopyMesh(const MeshConstructor &mesh) {

	vao.Bind();
	int i = 0;
	for (; i < VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 3, GL_FLOAT);
	}
	for (; i < VEC2_ATTRIB_NUM + VEC3_ATTRIB_NUM; i++)
	{
		vbs.push_back(new VertexBuffer(*(mesh.vbs[i])));
		vao.AddBuffer(*vbs.back(), i, 2, GL_FLOAT);
	}

	ib = new IndexBuffer(*mesh.ib);
	//ib = mesh.ib;
	vao.Unbind();

	is2D = true;

}
void MeshConstructor::makeBuildings(Node* theNode, glm::vec4 PrevFatherData,int levelLimit) {





	this->data = new BBD(theNode, PrevFatherData,levelLimit);




}