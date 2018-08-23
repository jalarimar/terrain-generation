//  ========================================================================
//  COSC363: Computer Graphics (2016);  University of Canterbury.
//
//  FILE NAME: TerrainPatches.cpp
//  See Lab-10.pdf for details.
//
//	The program generates and loads the mesh data for a terrain floor (100 verts, 81 elems).
//  Requires files  TerrainPatches.vert, TerrainPatches.frag
//  ========================================================================
#define  GLM_FORCE_RADIANS
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loadTGA.h"
using namespace std;

GLuint program;
GLuint program2;

GLuint vaoID;
GLuint vaoID2;

GLuint mvpMatrixLoc;
GLuint mvpMatrixLoc2;
GLuint mvMatrixLoc;
GLuint norMatrixLoc;
GLuint lightLoc;
GLint texNumLoc;
GLint wireLoc;
GLint snowLoc;
GLint waterLoc;

GLint texLoc;
GLint texLoc2;
GLint texLoc3;
GLint texLoc4;
GLint texLoc5;
GLint texLoc6;
GLint texLoc7;
GLint texLoc8;
GLint texLoc9; //sprite


float CDR = 3.14159265/180.0;     //Conversion from degrees to rad (required in GLM 0.9.6)

float verts[100*3];       //10x10 grid (100 vertices) with x,y,z
float texcoords[100*2];   //texture coords of 100 vertices with s,t
GLushort elems[81*4];       //Element array for 81 quad patches
float colors[100*3];       //100 colours with r,g,b

glm::mat4 projView;
glm::mat4 view;
glm::mat4 normal;
glm::vec4 lightPos;

glm::vec3 cameraPos = glm::vec3(0.0, 20.0, 30.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, 0.0); //glm::vec3(0.0, -20.0, -70.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float yaw = -90;
float pitch = -20;

float snowLevel = 7;
float waterLevel = 1.5;

bool key_w = false;
int key_num = 1;

//Generate vertex and element data for the terrain floor
void generateData()
{
	int indx, start;
	//verts array
	for(int i = 0; i < 10; i++)   //100 vertices on a 10x10 grid
	{
		for(int j = 0; j < 10; j++)
		{
			indx = 10*i + j;
			verts[3*indx] = 10*i - 45;		//x varies from -45 to +45
			verts[3*indx+1] = 0;			//y is set to 0 (ground plane)
			verts[3*indx+2] = -10*j;		//z varies from 0 to -100

			colors[3*indx] = 1.0; // color red
			colors[3*indx+1] = (float)i / 9.0;
			colors[3*indx+2] = (float)j / 9.0;

			texcoords[2*indx] = (float)i/9.0;    //s
			texcoords[2*indx+1] = (float)j/9.0;  //t
		}
	}

	//elems array
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			indx = 9*i +j;
			start = 10*i + j;
			elems[4*indx] = start;
			elems[4*indx+1] = start+10;
			elems[4*indx+2] = start+11;
			elems[4*indx+3] = start+1;			
		}
	}
}

//Loads terrain texture
void loadTextures()
{
	GLuint texID;
    glGenTextures(1, &texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
	loadTGA("Terrain_hm.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID2;
	glGenTextures(1, &texID2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texID2);
	loadTGA("grass4.tga");

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID3;
	glGenTextures(1, &texID3);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texID3);
	loadTGA("stone2.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID4;
	glGenTextures(1, &texID4);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texID4);
	loadTGA("water.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID5;
	glGenTextures(1, &texID5);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texID5);
	loadTGA("snow.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID6;
	glGenTextures(1, &texID6);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texID6);
	loadTGA("River_hm.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID7;
	glGenTextures(1, &texID7);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, texID7);
	loadTGA("beach.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID8;
	glGenTextures(1, &texID8);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, texID8);
	loadTGA("Third_hm.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLuint texID9;
	glGenTextures(1, &texID9);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, texID9);
	loadTGA("Butterfly.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


//Loads a shader file and returns the reference to a shader object
GLuint loadShader(GLenum shaderType, string filename)
{
	ifstream shaderFile(filename.c_str());
	if(!shaderFile.good()) cout << "Error opening shader file." << endl;
	stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();
	string shaderStr = shaderData.str();
	const char* shaderTxt = shaderStr.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderTxt, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char *strShaderType = NULL;
		cerr <<  "Compile failure in shader: " << strInfoLog << endl;
		delete[] strInfoLog;
	}
	return shader;
}

//Initialise the shader program, create and load buffer data
void initialise()
{
//--------Load terrain height map-----------
	loadTextures();

	glPatchParameteri(GL_PATCH_VERTICES, 4);
//--------Load shaders----------------------
	GLuint shaderv = loadShader(GL_VERTEX_SHADER, "TerrainPatches.vert");
	GLuint shaderc = loadShader(GL_TESS_CONTROL_SHADER, "TerrainPatches.cont");
	GLuint shadere = loadShader(GL_TESS_EVALUATION_SHADER, "TerrainPatches.eval");
	GLuint shaderg = loadShader(GL_GEOMETRY_SHADER, "TerrainPatches.geom");
	GLuint shaderf = loadShader(GL_FRAGMENT_SHADER, "TerrainPatches.frag");

	program = glCreateProgram();
	glAttachShader(program, shaderv);
	glAttachShader(program, shaderc);
	glAttachShader(program, shadere);
	glAttachShader(program, shaderg);
	glAttachShader(program, shaderf);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	mvpMatrixLoc = glGetUniformLocation(program, "mvpMatrix");
	mvMatrixLoc = glGetUniformLocation(program, "mvMatrix");
	norMatrixLoc = glGetUniformLocation(program, "norMatrix");
	lightLoc = glGetUniformLocation(program, "lightPos");

	texNumLoc = glGetUniformLocation(program, "texNum");
	wireLoc = glGetUniformLocation(program, "is_wireframe");
	snowLoc = glGetUniformLocation(program, "snowLevel");
	waterLoc = glGetUniformLocation(program, "waterLevel");

	texLoc = glGetUniformLocation(program, "heightMap");
	texLoc2 = glGetUniformLocation(program, "grass");
	texLoc3 = glGetUniformLocation(program, "stone");
	texLoc4 = glGetUniformLocation(program, "water");
	texLoc5 = glGetUniformLocation(program, "snow");
	texLoc6 = glGetUniformLocation(program, "heightMapRiver");
	texLoc7 = glGetUniformLocation(program, "sand");
	texLoc8 = glGetUniformLocation(program, "heightMapThird");


//---------Load buffer data-----------------------
	generateData();

	GLuint vboID[4]; // 4 vbos
	glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(4, vboID); // 4 vbos

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);  // texture coords

	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);  // colors

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

    glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background colour


//------- second program ------------------------------

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);

	GLuint shaderv_p = loadShader(GL_VERTEX_SHADER, "PointSprite.vert");
	GLuint shaderf_p = loadShader(GL_FRAGMENT_SHADER, "PointSprite.frag");

	program2 = glCreateProgram();
	glAttachShader(program2, shaderv_p);
	glAttachShader(program2, shaderf_p);
	glLinkProgram(program2);

	glGetProgramiv(program2, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program2, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	mvpMatrixLoc2 = glGetUniformLocation(program2, "mvpMatrix");
	texLoc9 = glGetUniformLocation(program2, "sprite");

	GLfloat vert[6];
	vert[0] = 10;
	vert[1] = 6; // 6 cloud, 4 butterfly, 4 rainbow
	vert[2] = -40;

	GLuint vboID2[1];

	glGenVertexArrays(1, &vaoID2);

	glBindVertexArray(vaoID2);
	glGenBuffers(1, vboID2);

	glBindBuffer(GL_ARRAY_BUFFER, vboID2[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);      // Vertex position

	glBindVertexArray(0);
}

//Display function to compute uniform values based on transformation parameters and to draw the scene
void display()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = front;

	glm::mat4 proj = glm::perspective(30.0f*CDR, 1.25f, 20.0f, 500.0f); //perspective projection (p) matrix
	view = glm::lookAt(cameraPos, cameraPos + front, cameraUp); //view (mv) matrix
	projView = proj * view;  //Product (mvp) matrix

	normal = glm::inverse(view); // normal matrix

	glm::vec4 light = glm::vec4(0.0, 50.0, -10.0, 1.0); // world coords
	lightPos = view * light; // eye coords

	glUseProgram(program);

	glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, &projView[0][0]);
	glUniformMatrix4fv(mvMatrixLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(norMatrixLoc, 1, GL_TRUE, &normal[0][0]);
	glUniform4fv(lightLoc, 1, &lightPos[0]);

	glUniform1i(texNumLoc, key_num);
	glUniform1i(wireLoc, key_w);
	glUniform1f(snowLoc, snowLevel);
	glUniform1f(waterLoc, waterLevel);

	glUniform1i(texLoc, 0);
	glUniform1i(texLoc2, 1);
	glUniform1i(texLoc3, 2);
	glUniform1i(texLoc4, 3);
	glUniform1i(texLoc5, 4);
	glUniform1i(texLoc6, 5);
	glUniform1i(texLoc7, 6);
	glUniform1i(texLoc8, 7);

	if (key_w) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vaoID);
	glDrawElements(GL_PATCHES, 81*4, GL_UNSIGNED_SHORT, NULL);

	// !!!!!!!!!!!!!!!!!!!!!!
	if (!key_w) {
		glUseProgram(program2);

		glUniformMatrix4fv(mvpMatrixLoc2, 1, GL_FALSE, &projView[0][0]);
		glUniform1i(texLoc9, 8);

		glBindVertexArray(vaoID2);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	// !!!!!!!!!!!!!!!!!!!!!!

	glFlush();
}

void press_key(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case 'w':
		key_w = !key_w;
		break;
	case '1':
		key_num = 1;
		break;
	case '2':
		key_num = 2;
		break;
	case '3':
		key_num = 3;
		break;
	case 'd':
		snowLevel += 0.3f;
		break;
	case 'f':
		snowLevel -= 0.3f;
		break;
	case 'j':
		waterLevel += 0.3f;
		break;
	case 'k':
		waterLevel -= 0.3f;
		break;
	}
	glutPostRedisplay();
}

void press_key_special(int Key, int x, int y)
{
	float sped = 2.0f;
	float rotato_sped = 3.0f;
	switch (Key)
	{
		case GLUT_KEY_UP:
			cameraPos.x += cameraFront.x * sped;
			cameraPos.z += cameraFront.z * sped;
			break;
		case GLUT_KEY_DOWN:
			cameraPos.x -= cameraFront.x * sped;
			cameraPos.z -= cameraFront.z * sped;
			break;
		case GLUT_KEY_LEFT:
			yaw -= rotato_sped;
			break;
		case GLUT_KEY_RIGHT:
			yaw += rotato_sped;
			break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	glutCreateWindow("Terrain");
	glutInitContextVersion (4, 2);
	glutInitContextProfile ( GLUT_CORE_PROFILE );

	if(glewInit() == GLEW_OK)
	{
		cout << "GLEW initialization successful! " << endl;
		cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;
	}
	else
	{
		cerr << "Unable to initialize GLEW  ...exiting." << endl;
		exit(EXIT_FAILURE);
	}

	initialise();
	glutKeyboardFunc(press_key);
	glutSpecialFunc(press_key_special);
	glutDisplayFunc(display);
	glutMainLoop();
}

