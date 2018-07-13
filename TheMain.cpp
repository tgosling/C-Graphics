//Tyler Gosling 0680547
//Tyler Gosling 0680547
//Tyler Gosling 0680547
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>		// for the ply model loader
#include <vector>		// for the ply model loader

//static const struct
//{
//	float x, y;
//	float r, g, b;
//} vertices[3] =
//{
//	{ -0.6f, -0.4f,		0.5f, 0.1f, 0.2f },
//	{ 0.6f, -0.4f,		0.f, 1.f, 0.f },
//	{ 0.f,  0.6f,		0.f, 1.f, 0.f }
//};

struct sVert
{
	float x, y, z;		// added "z"
	float r, g, b;
};
//sVert vertices[3] = 
//{
//	{ -0.6f, -0.4f, 0.0f,		0.5f, 0.1f, 0.2f },
//	{ 0.6f, -0.4f, 0.0f, 		0.f, 1.f, 0.f },
//	{ 0.f,  0.6f, 0.0f,			0.f, 1.f, 0.f }
//};
// Here's our heap based vertex array:
sVert* pVertices = 0;						//  = new sVert[10000];
unsigned int g_NumberOfVertsToDraw = 0;		// To draw (on GPU)


unsigned int g_NumberOfVertices = 0;		// From file
unsigned int g_NumberOfTriangles = 0;		// From file


// Function signature for loading the ply model
// Will load model and place into pVertices array
// (overwriting whatever was there)
void LoadTheModel(std::string fileName);


static const char* vertex_shader_text =
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"		// float r, g, b;
"attribute vec3 vPos;\n"		// float x, y, z;
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
int main(void)
{
	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader(( GLADloadproc )glfwGetProcAddress);
	glfwSwapInterval(1);

//	LoadTheModel("bun_zipper_res2_xyz.ply");
	//LoadTheModel("ssj100_xyz.ply");
	LoadTheModel("tyfighter2.ply");
	// NOTE: OpenGL error checks have been omitted for brevity
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	// sVert vertices[3]
	glBufferData( GL_ARRAY_BUFFER, 
				  sizeof(sVert) * ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
				  pVertices,			//vertices, 
				  GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);
	program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");
//struct sVert
//{
//	float x, y, z;		// added "z"
//	float r, g, b;
//};
	glEnableVertexAttribArray(vpos_location);	// vPos
	glVertexAttribPointer(vpos_location, 3,		// vPos
						   GL_FLOAT, GL_FALSE,
						   sizeof(float) * 6, 
						   ( void* )0);

	glEnableVertexAttribArray(vcol_location);	// vCol
	glVertexAttribPointer(vcol_location, 3,		// vCol
						   GL_FLOAT, GL_FALSE,
						   sizeof(float) * 6, 
						   ( void* )( sizeof(float) * 3 ));


	// If you want to draw lines that aren't filled, you 
	//	can change the "polygon mode" to "LINE" 
	// (Note: there IS a GL_LINES, with an "S", which ISN'T
	//  what you want here...)
	//
//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// Print out the GL version
	std::cout << glGetString( GL_VERSION ) << std::endl;

	glm::vec3 cameraEye = glm::vec3( 0.0, 0.0, -550.0f ); 
	glm::vec3 cameraTarget = glm::vec3( 0.0f, 0.0f, 0.0f ); 
	glm::vec3 upVector = glm::vec3( 0.0f, 1.0f, 0.0f );



	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		
		//mat4x4 m, p, mvp;
		glm::mat4 m, p, mvp, view;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / ( float )height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//mat4x4_identity(m);
		m = glm::mat4(1.0f);

		//mat4x4_rotate_Z(m, m, );
		glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f), 
										 0.0f, // (float) glfwGetTime(), 
										 glm::vec3( 0.0f, 0.0, 1.0f ) );

		m = m * rotateZ;

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//		p = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f );

		p = glm::perspective( 0.6f, 
							  ratio, 
							  0.1f,				
							  1000.0f );		

		view = glm::mat4(1.0f);

		int state = glfwGetKey(window, GLFW_KEY_D);
		if (state == GLFW_PRESS)
		{
			cameraEye.x += 1.51f;
		}
		state = glfwGetKey(window, GLFW_KEY_A);
		if (state == GLFW_PRESS)
		{
			cameraEye.x -= 1.51f;
		}
		state = glfwGetKey(window, GLFW_KEY_Q);
		if (state == GLFW_PRESS)
		{
			cameraEye.z += 1.51f;
		}
		state = glfwGetKey(window, GLFW_KEY_E);
		if (state == GLFW_PRESS)
		{
			cameraEye.z -= 1.51f;
		}
		state = glfwGetKey(window, GLFW_KEY_W);
		if (state == GLFW_PRESS)
		{
			cameraEye.y += 1.51f;
		}
		state = glfwGetKey(window, GLFW_KEY_S);
		if (state == GLFW_PRESS)
		{
			cameraEye.y -= 1.51f;
		}


		//glm::vec3 cameraEye = glm::vec3( 0.0, 0.0, -2.0f ); 
		//glm::vec3 cameraTarget = glm::vec3( 0.0f, 0.0f, 0.0f ); 
		//glm::vec3 upVector = glm::vec3( 0.0f, 1.0f, 0.0f );

		view = glm::lookAt( cameraEye,		// position (3d space)
							cameraTarget,	// looking at
							upVector );		// Up vector

		//mat4x4_mul(mvp, p, m);

		mvp = p * view * m; 


		glUseProgram(program);
//		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, ( const GLfloat* )mvp);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));


//		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, ::g_NumberOfVertsToDraw);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void LoadTheModel(std::string fileName)
{
	// Open the file. 
	// Read until we hit the word "vertex"
	// Read until we hit the word "face"
	// Read until we hit the word "end_header"

	std::ifstream thePlyFile( fileName.c_str() );
	if ( ! thePlyFile.is_open() )
	{	// Something is wrong...
		return;
	}

	std::string temp; 
	while ( thePlyFile >> temp )
	{
		if ( temp == "vertex" ) 
		{
			break;
		}
	}; 
	// read the number of vertices...
	thePlyFile >> ::g_NumberOfVertices;

	while ( thePlyFile >> temp )
	{
		if ( temp == "face" ) 
		{
			break;
		}
	}; 
	// read the number of triangles...
	thePlyFile >> ::g_NumberOfTriangles;


	while ( thePlyFile >> temp )
	{
		if ( temp == "end_header" ) 
		{
			break;
		}
	}; 

	// And now, we start reading vertices... Hazzah!
	
	// This is set up to match the ply (3d model) file. 
	// NOT the shader. 
	struct sVertPly
	{
		glm::vec3 pos;
		glm::vec4 colour;
	};

	std::vector<sVertPly> vecTempPlyVerts;

	sVertPly tempVert;
	// Load the vertices...
	for ( unsigned int index = 0; index != ::g_NumberOfVertices; index++ )
	{
		thePlyFile >> tempVert.pos.x >> tempVert.pos.y >> tempVert.pos.z;
		thePlyFile >> tempVert.colour.x >> tempVert.colour.y
			       >> tempVert.colour.z >> tempVert.colour.w; 
		// Add too... what? 
		vecTempPlyVerts.push_back(tempVert);
	}

	struct sTriPly
	{
		unsigned int vindex[3];		// the 3 indices
		sVertPly verts[3];			// The actual vertices
	};

	std::vector<sTriPly> vecTempPlyTriangles;
	sTriPly tempTriangle;
	unsigned int discard = 0;
	for ( unsigned int index = 0; index != ::g_NumberOfTriangles; index++ )
	{
		// 3 5622 5601 5640
		thePlyFile >> discard						// the "3" at the start
			       >> tempTriangle.vindex[0]
			       >> tempTriangle.vindex[1]
			       >> tempTriangle.vindex[2];

		// Look up the vertex that matches the triangle index values.
		tempTriangle.verts[0] = vecTempPlyVerts[ tempTriangle.vindex[0] ];
		tempTriangle.verts[1] = vecTempPlyVerts[ tempTriangle.vindex[1] ];
		tempTriangle.verts[2] = vecTempPlyVerts[ tempTriangle.vindex[2] ];

		vecTempPlyTriangles.push_back( tempTriangle );
	}
	
	// NOW, we need to put them into the vertex array buffer that 
	//	will be passed to OpenGL. Why? 
	// Because we called glDrawArrays(), that's why. 

	::g_NumberOfVertsToDraw = ::g_NumberOfTriangles * 3;	// 3 because "triangles"

	// sVert* pVertices = 0;
	pVertices = new sVert[::g_NumberOfVertsToDraw];
	// Allocate on the HEAP, so infinite size... 
	// delete pVertices			/// error!
	// delete [] pVertices		/// correct!!
	unsigned int vertIndex = 0;
	for ( unsigned int triIndex = 0; 
		  triIndex != ::g_NumberOfTriangles; 
		  triIndex++, vertIndex += 3 )
	{
		sTriPly& curTri = vecTempPlyTriangles[triIndex];

		pVertices[ vertIndex + 0 ].x = curTri.verts[0].pos.x;
		pVertices[ vertIndex + 0 ].y = curTri.verts[0].pos.y;
		pVertices[ vertIndex + 0 ].z = curTri.verts[0].pos.z;
		pVertices[ vertIndex + 0 ].r = curTri.verts[0].colour.x / 255;
		pVertices[ vertIndex + 0 ].g = curTri.verts[0].colour.y / 255;
		pVertices[ vertIndex + 0 ].b = curTri.verts[0].colour.z / 255;

		pVertices[ vertIndex + 1 ].x = curTri.verts[1].pos.x;
		pVertices[ vertIndex + 1 ].y = curTri.verts[1].pos.y;
		pVertices[ vertIndex + 1 ].z = curTri.verts[1].pos.z;
		pVertices[ vertIndex + 1 ].r = curTri.verts[1].colour.x / 255;
		pVertices[ vertIndex + 1 ].g = curTri.verts[1].colour.y / 255;
		pVertices[ vertIndex + 1 ].b = curTri.verts[1].colour.z / 255;

		pVertices[ vertIndex + 2 ].x = curTri.verts[2].pos.x;
		pVertices[ vertIndex + 2 ].y = curTri.verts[2].pos.y;
		pVertices[ vertIndex + 2 ].z = curTri.verts[2].pos.z;
		pVertices[ vertIndex + 2 ].r = curTri.verts[2].colour.x/255;
		pVertices[ vertIndex + 2 ].g = curTri.verts[2].colour.y / 255;
		pVertices[ vertIndex + 2 ].b = curTri.verts[2].colour.z / 255;

	}// for ( unsigned int triIndex = 0...

	return;
}
