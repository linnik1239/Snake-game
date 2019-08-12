#pragma once
#include "MovableGLM.h"
#include "VertexArray.hpp"
#include "shader.h"
#include "MeshConstructor.h"

#include "texture.h"

class Shape : public MovableGLM
{
private:

	MeshConstructor *mesh;
	int texID;
	int shaderID;
	bool isCopy;
	unsigned int mode;
	bool toRender;

public:
	int prevTorAngel = 180.0;  //******************** to make changes.
	int madeVurve = 0;  //******************** to make changes.
	float theAngel = 180.0;
	bool nuwPush = false;  //******************** to make changes.
	bool prevPush = false;  //******************** to make changes.

	glm::vec4 prevCoordinates = glm::vec4(0,0, 0, 0);
	int prevAngel = 180;
	//int prevAngelY = 0;
	int prevAngelYY = 0;
	int curAngel = 180;
	//int curAngelY = 0;
	int curAngelYY = 0;
	MeshConstructor *getMesh();

	Shape(const Shape& shape, unsigned int mode);

	Shape(const std::string& fileName, unsigned int mode);

	Shape(const int SimpleShapeType, unsigned int mode);

	Shape(Bezier1D *curve, unsigned int xResolution, unsigned int yResolution, bool is2D, unsigned int mode);

	Shape(Bezier1D *curve, unsigned int xResolution, unsigned int yResolution, bool is2D, unsigned int mode, int whichShape);


	void Draw(const std::vector<Shader*> shaders, const std::vector<Texture*> textures, bool isPicking);

	inline void Hide() { toRender = false; }

	inline void Unhide() { toRender = true; }

	inline bool Is2Render() { return toRender; }

	inline bool Is2D() { return mesh->Is2D(); }

	inline void SetTexture(int id) { texID = id; }

	inline void SetShader(int id) { shaderID = id; }

	inline int GetShader() { return shaderID; }

	inline int GetTexture() { return texID; }

	virtual ~Shape(void);
};

