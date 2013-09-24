#if !defined (_MESH_CLASS)
#define _MESH_CLASS

#include "supportClass.h"

#define PI				3.1415926

class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
	//Color3	color;
};

class Face
{
public:
	int			nVerts;
	VertexID*	vert;
	//Vector3		facenorm;
	Face()
	{
		nVerts	= 0;
		vert	= NULL;
	}
	~Face()
	{
		if(vert !=NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int			numVerts;
	Point3*		pt;
	//int			numNormals;
	//Vector3*	norm;
	int			numFaces;
	Face*		face;

	float		slideX, slideY, slideZ;
	float		rotateX, rotateY, rotateZ;
public:
	Mesh()
	{
		numVerts	= 0;
		pt			= NULL;
		//numNormals	= 0;
		//norm		= NULL;
		numFaces	= 0;
		face		= NULL;

		slideX = slideY = slideZ = 0;
		rotateX = rotateY = rotateZ = 0;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		//if(norm != NULL)
		//{
		//	delete[] norm;
		//}
		if(face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		//numNormals = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void DrawColor();
	void SetColor(int colorIdx);

	void CreateTetrahedron();
	void CreateCube(float	fSize);
	void CreateSphere(int nSlice, int nStack, float radius);
	void CreateYPlane(int nXSegment, int nZSegment, float fXWidth, float fZWidth);
	void CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ);
	void CreateCylinder(int nSegment, float fHeight, float fRadius);
	//void CreateYPlane(int nXSegment, int nZSegment, float fHalfSizeX, float fHalfSizeZ);
	//void Draw();
	//void Draw1();
	//void CalculateFacesNorm();
};


#endif