/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vid_model.cpp,v 1.9 2005/08/15 06:15:38 ecco Exp $   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Implementation Private Includes
/////////////////////////////////////////////////////////////////////////////
#include "defs.h"

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Definitions, Enumeration and Typedefs
/////////////////////////////////////////////////////////////////////////////
#define MODEL_COUNT 16

struct point3d
{
	float x, y, z;
};

struct int4
{
	int vertex[4];
	int normals[4];
};

struct model_model_t
{
	unsigned int glid;
	bool used;
};

model_model_t models[MODEL_COUNT];

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Interface Variables
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Implementation Private Functions
/////////////////////////////////////////////////////////////////////////////
static unsigned int getFreeModel()
{
	unsigned int i;
	for (i = 1; i < MODEL_COUNT; i++)
		if (models[i].used == false)
			return i;

	console_Print("VID_MODEL: No free models");
	return 0;
}

static void glLightingInit()
{
	GLfloat LightModelAmbient[] = { 0.5, 0.5, 0.5, 1.0};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);


	GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
   	GLfloat LightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LightPosition[]= { 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glEnable(GL_LIGHT1);      

   	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   	//GLfloat mat_ambient[] = { 0.1, 0.1, 0.5, 1.0 };
   	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   	GLfloat mat_shininess[] = { 255.0 };

	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glEnable(GL_COLOR_MATERIAL);
}

static void makeDisplayList(int meshcount, point3d *meshdata, int normalcount, point3d *normaldata, int facecount, int4 *faces)
{
	int i;

	glPushMatrix();
	float scale = 15.0f;
	glScalef(3.0f * scale, 3.0f * scale, 3.0f * scale);
	//glScalef(5.0f, 5.0f, 5.0f);
	
	glBegin(GL_QUADS);
		for (i = 0; i < facecount; i++)
		{
			/*if (i < facecount / 2)
				glColor3f(0.0f, 0.0f, 0.3f);
			else
				glColor3f(0.0f, 0.0f, 0.5f);*/

			//glColor3f(0.0f, 0.0f, 0.5f);

			glNormal3f(normaldata[faces[i].normals[0]].x, normaldata[faces[i].normals[0]].y, normaldata[faces[i].normals[0]].z);
			glVertex3f(meshdata[faces[i].vertex[0]].x, meshdata[faces[i].vertex[0]].y, meshdata[faces[i].vertex[0]].z);

			glNormal3f(normaldata[faces[i].normals[1]].x, normaldata[faces[i].normals[1]].y, normaldata[faces[i].normals[1]].z);
			glVertex3f(meshdata[faces[i].vertex[1]].x, meshdata[faces[i].vertex[1]].y, meshdata[faces[i].vertex[1]].z);

			glNormal3f(normaldata[faces[i].normals[2]].x, normaldata[faces[i].normals[2]].y, normaldata[faces[i].normals[2]].z);
			glVertex3f(meshdata[faces[i].vertex[2]].x, meshdata[faces[i].vertex[2]].y, meshdata[faces[i].vertex[2]].z);

			glNormal3f(normaldata[faces[i].normals[3]].x, normaldata[faces[i].normals[3]].y, normaldata[faces[i].normals[3]].z);
			glVertex3f(meshdata[faces[i].vertex[3]].x, meshdata[faces[i].vertex[3]].y, meshdata[faces[i].vertex[3]].z);
		}
	glEnd();


	glPopMatrix();
}

static int meshcount;
static int facecount;
static int normalcount;
static int normalindexcount;
static point3d *meshdata;
static point3d *normaldata;
static int4 *faces;

static bool readXFile(FILE* fp)
{
	int i = 0;
	char buffer[1024];
	int logicalfilepos = 0;

	while (!feof(fp))
	{
		fgets(buffer, 1024, fp);

		// Find the mesh definition
		if (logicalfilepos == 0)
		{
			if (strncmp(buffer, "Mesh ", 5) == 0)
				logicalfilepos = 1;
		}
		// Read the mesh point list count
		else if (logicalfilepos == 1)
		{
			sscanf(buffer, "%d;", &meshcount);
			console_Print("%d Mesh Items", meshcount);
			meshdata = new point3d[meshcount];

			logicalfilepos = 2;
		}
		// Read the mesh point list
		else if (logicalfilepos == 2)
		{
			float x, y, z;
			i++;

			sscanf(buffer, "%f; %f; %f;,", &x, &y, &z);
			meshdata[i-1].x = x;
			meshdata[i-1].y = y;
			meshdata[i-1].z = z;

			if (i == meshcount)
			{
				logicalfilepos = 3;
				i = 0;
			}
		}
		// Read the face index count
		else if (logicalfilepos == 3)
		{
			sscanf(buffer, "%d;", &facecount);
			//console_Print("%s", buffer);
			console_Print("%d Face Items", facecount);
			logicalfilepos = 4;
			faces = new int4[facecount];
		}
		// Read the face indexes
		else if (logicalfilepos == 4)
		{
			int v1, v2, v3, v4;
			i++;

			sscanf(buffer, "%*d; %d; %d; %d; %d;,", &v1, &v2, &v3, &v4);
			//console_Print("Face %d: %d, %d, %d", i, v1, v2, v3, v4);

			faces[i-1].vertex[0] = v1;
			faces[i-1].vertex[1] = v2;
			faces[i-1].vertex[2] = v3;
			faces[i-1].vertex[3] = v4;

			if (i == facecount)
			{
				i = 0;
				logicalfilepos = 5;
			}
		}
		else if (logicalfilepos == 5)
		{
			if (strncmp(buffer, " MeshNormals", 12) == 0)
				logicalfilepos = 6;
		}
		else if (logicalfilepos == 6)
		{
			sscanf(buffer, "%d;", &normalcount);
			console_Print("%d Normal Vertex Items", normalcount);
			normaldata = new point3d[normalcount];
			logicalfilepos = 7;
		}
		else if (logicalfilepos == 7)
		{
			float x, y, z;
			i++;

			sscanf(buffer, "%f; %f; %f;,", &x, &y, &z);
			normaldata[i-1].x = x;
			normaldata[i-1].y = y;
			normaldata[i-1].z = z;

			if (i == normalcount)
			{
				logicalfilepos = 8;
				i = 0;
			}
		}
		else if (logicalfilepos == 8)
		{
			sscanf(buffer, "%d;", &normalindexcount);
			console_Print("%d Normal Vertex Index Items", normalindexcount);
			logicalfilepos = 9;
		}
		else if (logicalfilepos == 9)
		{
			int v1, v2, v3, v4;
			i++;

			sscanf(buffer, "%*d; %d; %d; %d; %d;,", &v1, &v2, &v3, &v4);
			//console_Print("Face %d: %d, %d, %d", i, v1, v2, v3, v4);

			faces[i-1].normals[0] = v1;
			faces[i-1].normals[1] = v2;
			faces[i-1].normals[2] = v3;
			faces[i-1].normals[3] = v4;

			if (i == normalindexcount)
			{
				i = 0;
				logicalfilepos = 10;
				return true;
			}
		}
	}

	return false;
}

static unsigned int loadModel(char *filename)
{
	// Setup the filename
	char tmpfilename[1024];
	unsigned int tmpid;

	tmpid = getFreeModel();

	sprintf(tmpfilename, "%sdata/%s", sysAppPath, filename);

	//console_Print("%s", tmpfilename);

	models[tmpid].glid = glGenLists(1);
	glNewList(models[tmpid].glid, GL_COMPILE);
	models[tmpid].used = true;

	// Open the x file
	FILE *fp;

	fp = fopen(tmpfilename, "rt");

	while (!feof(fp))
	{
		if (readXFile(fp))
			makeDisplayList(meshcount, meshdata, normalcount, normaldata, facecount, faces);
		else
			break;
	} 

	fclose(fp);

	makeDisplayList(meshcount, meshdata, normalcount, normaldata, facecount, faces);

	glEndList();
	
	delete meshdata;
	delete faces;
	delete normaldata;
	meshcount=0;
	facecount=0;

	return tmpid;
}
 
/////////////////////////////////////////////////////////////////////////////
// Interface Public Functions
/////////////////////////////////////////////////////////////////////////////
unsigned int vid_LoadModel(char *filename)
{
	glLightingInit();
	return loadModel(filename);
}

void vid_DrawModel(int x, int y, float rotation, int team, unsigned int modelid)
{
	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glTranslatef(x, y, 0.0f);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (team == 1)
		glColor4f(0.5f, 0.0f, 0.0f, 1.0f);
	else if (team == 2)
		glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
	else if (team == 3)
		glColor4f(0.0f, 0.0f, 0.5f, 1.0f);

	glScalef(0.015f * 2, 0.015f * 2, 0.015f * 2);
	glRotatef(rotation + 180, 0.0f, 0.0f, 1.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);

	glCallList(models[modelid].glid);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glPopMatrix();	
}
