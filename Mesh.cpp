#include "Mesh.h"


#define PI				3.1415926
#define	COLORNUM		14


float	ColorArr[COLORNUM][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, 1.0}, 
								{1.0, 1.0,  0.0}, { 1.0, 0.0, 1.0},{ 0.0, 1.0, 1.0}, 
								 {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.5, 1.0, 0.5},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};

void Mesh::SetColor(int colorIdx)
{
	for (int f = 0; f < numFaces; f++)
	{
		for (int v = 0; v < face[f].nVerts; v++)
		{
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}


void Mesh::CreateCylinder(int nSegment, float fHeight, float fRadius)
{
	numVerts = 2*nSegment + 4;
	pt = new Point3[numVerts];
	pt[0].set(0,fHeight/2,0);
	pt[1].set(0,-fHeight/2,0);
	float y=fHeight/2;
	for (int i = 0; i <= nSegment; i++)
	{
		float deg = 360*i/nSegment * PI / 180;
		float x=fRadius*sin(deg);
		float z=fRadius*cos(deg);
		pt[2+i].set(x,y,z);
		pt[2+i+nSegment+1].set(x,-y,z);
	}

	numFaces = 3*nSegment;
	face = new Face[numFaces];

	for (int i = 0; i < nSegment; i++)
	{
		face[i].nVerts = 3;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 0;
		face[i].vert[1].vertIndex = 2+i;
		face[i].vert[2].vertIndex = 2+i+1;

		for(int j = 0; j<face[i].nVerts ; j++)
			face[i].vert[j].colorIndex = i%14;

		face[i + nSegment].nVerts = 3;
		face[i + nSegment].vert = new VertexID[face[i + nSegment].nVerts];
		face[i + nSegment].vert[0].vertIndex = 1;
		face[i + nSegment].vert[2].vertIndex = 2+i+nSegment+1;
		face[i + nSegment].vert[1].vertIndex = 2+i+1+nSegment+1;

		for(int j = 0; j<face[i + nSegment].nVerts ; j++)
			face[i + nSegment].vert[j].colorIndex = i % 14;

		face[2*nSegment+i].nVerts = 4;
		face[2*nSegment+i].vert = new VertexID[face[2*nSegment+i].nVerts];
		face[2*nSegment+i].vert[0].vertIndex = 2+i;
		face[2*nSegment+i].vert[3].vertIndex = 2+i+1;
		face[2*nSegment+i].vert[2].vertIndex = 2+i+1+nSegment+1;
		face[2*nSegment+i].vert[1].vertIndex = 2+i+nSegment+1;
		for(int j = 0; j<face[2*nSegment+i].nVerts ; j++)
			face[2*nSegment+i].vert[j].colorIndex = i % 14;
	}
}

void Mesh::CreateYPlane(int nXSegment, int nZSegment, float fXWidth, float fZWidth)
{
	numVerts = (nXSegment + 1) * (nZSegment + 1);
	pt = new Point3[numVerts];

	int count= 0;
	for (float x = -fXWidth / 2; x <= fXWidth / 2; x += fXWidth / nXSegment)
		for (float z = -fZWidth / 2; z <= fZWidth / 2; z += fZWidth / nZSegment)
			pt[count++].set(x, 0, z);
	
	numFaces = nXSegment * nZSegment * 2;
	face = new Face[numFaces];

	for (int x = 0; x < nXSegment; x++)
		for (int z = 0; z < nZSegment; z++)
		{
			int index = x * nZSegment + z;
			face[index].nVerts = 3;
			face[index].vert = new VertexID[face[index].nVerts];
			face[index].vert[0].vertIndex = index + x;
			face[index].vert[1].vertIndex = index + x + 1;
			face[index].vert[2].vertIndex = index + x + nZSegment + 1;

			face[index + nXSegment * nZSegment].nVerts = 3;
			face[index + nXSegment * nZSegment].vert = new VertexID[face[index + nXSegment * nZSegment].nVerts];
			face[index + nXSegment * nZSegment].vert[0].vertIndex = index + x + 1;
			face[index + nXSegment * nZSegment].vert[1].vertIndex = index + x + nZSegment + 1 + 1;
			face[index + nXSegment * nZSegment].vert[2].vertIndex = index + x + nZSegment + 1;
		}	
}


void Mesh::CreateSphere(int nSlice, int nStack, float radius)
{
	numVerts = (nStack - 1) * (nSlice + 1) + 2;
	pt = new Point3[numVerts];
	pt[0].set(0,-radius,0);
	pt[1].set(0,radius,0);

	int index = 2;
	for (int i = 0; i <= nSlice; i++)
		for (int j = 1; j < nStack; j++)
		{
			float deg_phi = i * 360.0 / nSlice * PI / 180;
			float deg_theta = (-90 + j * 180.0 / nStack) * PI / 180;
			float x = radius*cos(deg_theta)*cos(deg_phi);
			float y = radius*sin(deg_theta);
			float z = radius*cos(deg_theta)*sin(deg_phi);
			pt[index++].set(x,y,z);
		}

	numFaces = nSlice * nStack;
	face = new Face[numFaces];
	index=0;
	for (int s = 0; s < nSlice; s++)
	{
		for (int i = 0; i < nStack-2; i++)
		{
			face[index].nVerts = 4;
			face[index].vert = new VertexID[face[index].nVerts];
			face[index].vert[0].vertIndex = 2+i+(nStack-1)*s;
			face[index].vert[1].vertIndex = 2+i+(nStack-1)*s+1;
			face[index].vert[2].vertIndex = face[index].vert[1].vertIndex+nStack-1;
			face[index].vert[3].vertIndex = face[index].vert[0].vertIndex+nStack-1;
			for(int j = 0; j<face[index].nVerts ; j++)
				face[index].vert[j].colorIndex = s % 14;
			index++;
		}
	}
	int nextS;
	for (int s = 0; s < nSlice; s++)
	{

		nextS=(s!=nSlice-1)?s+1:0;

		face[index].nVerts = 3;
		face[index].vert = new VertexID[face[index].nVerts];
		face[index].vert[0].vertIndex = 0;
		face[index].vert[1].vertIndex = 2+(nStack-1)*s;
		
		face[index].vert[2].vertIndex = 2+(nStack-1)*nextS;
		for(int j = 0; j<face[index].nVerts ; j++)
					face[index].vert[j].colorIndex = s % 14;
		index++;
		face[index].nVerts = 3;
		face[index].vert = new VertexID[face[index].nVerts];
		face[index].vert[0].vertIndex = 1;
		face[index].vert[2].vertIndex = nStack +(nStack-1)*s;
		face[index].vert[1].vertIndex =	nStack+(nStack-1)*nextS;
		for(int j = 0; j<face[index].nVerts ; j++)
			face[index].vert[j].colorIndex = (s) % 14;
		index++;
	}
	
	/*for (int i = 0; i < numVerts; i++)
	{
		std::cout << "Point "<<i<<":("<< pt[i].x  <<"," << pt[i].y <<"," << pt[i].z << ")\n";
	}*/
	
}

void Mesh::CreateCube(float	fSize)
{
	numVerts=8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set( fSize, fSize, fSize);
	pt[2].set( fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set( fSize, -fSize, fSize);
	pt[6].set( fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);


	numFaces= 6;
	face = new Face[numFaces];
	int i = 0;

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for(i = 0; i<face[0].nVerts ; i++)
		face[0].vert[i].colorIndex = 0;
	
	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for(i = 0; i<face[1].nVerts ; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for(i = 0; i<face[2].nVerts ; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for(i = 0; i<face[3].nVerts ; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for(i = 0; i<face[4].nVerts ; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for(i = 0; i<face[5].nVerts ; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateCuboid(float	fSizeX, float fSizeY, float	fSizeZ)
{
	numVerts=8;
	pt = new Point3[numVerts];
	pt[0].set(-fSizeX,  fSizeY,  fSizeZ);
	pt[1].set( fSizeX,  fSizeY,  fSizeZ);
	pt[2].set( fSizeX,  fSizeY, -fSizeZ);
	pt[3].set(-fSizeX,  fSizeY, -fSizeZ);
	pt[4].set(-fSizeX, -fSizeY,  fSizeZ);
	pt[5].set( fSizeX, -fSizeY,  fSizeZ);
	pt[6].set( fSizeX, -fSizeY, -fSizeZ);
	pt[7].set(-fSizeX, -fSizeY, -fSizeZ);


	numFaces= 6;
	face = new Face[numFaces];
	int i = 0;

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for(i = 0; i<face[0].nVerts ; i++)
		face[0].vert[i].colorIndex = 0;
	
	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for(i = 0; i<face[1].nVerts ; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for(i = 0; i<face[2].nVerts ; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for(i = 0; i<face[3].nVerts ; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for(i = 0; i<face[4].nVerts ; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for(i = 0; i<face[5].nVerts ; i++)
		face[5].vert[i].colorIndex = 5;

}

void Mesh::CreateTetrahedron()
{
	numVerts=4;
	pt = new Point3[numVerts];
	pt[0].set(0, 0, 0);
	pt[1].set(1, 0, 0);
	pt[2].set(0, 1, 0);
	pt[3].set(0, 0, 1);

	numFaces= 4;
	face = new Face[numFaces];
	int i = 0;

	face[0].nVerts = 3;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 3;
	for(i = 0; i<face[0].nVerts ; i++)
		face[0].vert[i].colorIndex = 0;
	

	face[1].nVerts = 3;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;	
	face[1].vert[1].vertIndex = 2;
	face[1].vert[2].vertIndex = 1;
	for(i = 0; i<face[1].nVerts ; i++)
		face[1].vert[i].colorIndex = 1;

	
	face[2].nVerts = 3;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 3;
	face[2].vert[2].vertIndex = 2;
	for(i = 0; i<face[2].nVerts ; i++)
		face[2].vert[i].colorIndex = 2;


	face[3].nVerts = 3;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 0;
	for(i = 0; i<face[3].nVerts ; i++)
		face[3].vert[i].colorIndex = 3;
}

void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]); 
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

