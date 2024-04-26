#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib") 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"
#include "vertex.h"
#include "sphere.h"
#include "Model.h"
#include "Player.h"
#include "Camera.h"

#include "ForceGen.h"

#include "soil/SOIL.h"

using namespace std;
using namespace glm;

static enum object {
	FIELD, SKY, COIN, PLAYER, RIGHTBARRIER,
	LEFTBARRIER, HEALTHPOTION, SPIKEBALL, SPIKEBALL2, SPIKEBALL3, SPIKEBALL4, COIN2 }; // VAO ids.

static enum buffer {
	FIELD_VERTICES, SKY_VERTICES, COIN_VERTICES, COIN_INDICES, PLAYER_VERTICES,
	RIGHTBARRIER_VERTICES, LEFTBARRIER_VERTICES, HEALTHPOTION_VERTICES, HEALTHPOTION_INDICES, SPIKEBALL_VERTICES, 
	SPIKEBALL2_VERTICES, SPIKEBALL3_VERTICES, SPIKEBALL4_VERTICES, COIN2_VERTICES, COIN2_INDICES }; // VBO ids.

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

//	Globals	//

//	Used to calculate delta-Time	//
int newTimeSinceStart;	
int oldTimeSinceStart;

float health = 100.0f;
int score = 0;
//	Object Container	//
vector<GameObject*> objects;	

//	Force Generator	//
ForceGen FG(glm::vec3(0, -9.8, 0));	


//	create normal matrix	//
static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);  

//	Lighting	//
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

//	Front and back material properties	//
static const Material sphereFandB =
{
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

static const Material playerFandB =
{
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};

//	Creating Light	//
static const Light light0 =	//	Sun Light
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(0.66, 0.66, 0.0, 1.0),	//	Diffuse
	vec4(0.66, 0.66, 0.0, 1.0),	//	spec
	vec4(1000.0, 1000.0, 100.0, 1.0)	//	Coords (Where the light is)
};

static const Light light1 =	//	Lava Light
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(0.66, 0.33, 0.0, 1.0),	//	Diffuse
	vec4(0.66, 0.33, 0.0, 1.0),	//	spec
	vec4(0.0, -10.0, 0.0, 1.0)	//	Coords (Where the light is)
};


static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   objectLoc,
   SpikeTexLoc,
   LavaTexLoc,	//	Lava Floor
   skyTexLoc,	//	Sky 
   PlayerTexLoc,//	Player
   buffer[15],    //VBO List
   vao[12],	     //VAO List
   texture[4];   //Texture List


//	Objects	//
static Player player("PlayerModel.obj", vec3 (0.0f, 20.0f, 0.0f), 1.0f);

static Model rightBarrier("Barrier.obj", vec3(50.0f, 10.0f, 0.0f), 1.0f, "Barrier");

static Model leftBarrier("Barrier.obj", vec3(-50.0f, 10.0f, 0.0f), 1.0f, "Barrier");

static Sphere coins(vec3(30.0f, 15.0f, 0.0f), 1.0f, "Coin");

static Sphere healthP(vec3(0.0f, 5.0f, 0.0f), 1.0f, "HealthPotion");

static Model spikeBall("SpikeBall.obj", vec3(20.0f, 20.0f, 0.0f), 5.0f, "SpikeBall", "Sphere");

static Model spikeBall2("SpikeBall.obj", vec3(-20.0f, 20.0f, 0.0f), 5.0f, "SpikeBall", "Sphere");

static Model spikeBall3("SpikeBall.obj", vec3(20.0f, 10.0f, 0.0f), 5.0f, "SpikeBall", "Sphere");

static Model spikeBall4("SpikeBall.obj", vec3(-20.0f, 10.0f, 0.0f), 5.0f, "SpikeBall", "Sphere");

static Sphere coins2(vec3(-30.0f, 15.0f, 0.0f), 1.0f, "Coin");

//	Vertices for the field, Creates simple a 1 x 4 plane
static Vertex fieldVertices[] =  
{
	{vec4(1000.0, 0.0, 1000.0, 1.0), vec2(8.0, 0.0)},
	{vec4(1000.0, 0.0, -1000.0, 1.0), vec2(8.0, 8.0)},
	{vec4(-1000.0, 0.0, 1000.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-1000.0, 0.0, -1000.0, 1.0), vec2(0.0, 8.0)}
}; 

//	Vertices for the sky/background, Creates simple a 1 x 4 plane
static Vertex skyVertices[] =
{
	{vec4(300.0, 0.0, -70.0, 1.0), vec2(1.0, 0.0)},
	{vec4(300.0, 150.0, -70.0, 1.0), vec2(1.0, 1.0)},
	{vec4(-300.0, 0.0, -70.0, 1.0), vec2(0.0, 0.0)},
	{vec4(-300.0, 150.0, -70.0, 1.0), vec2(0.0, 1.0)}
};




//	Making Cameras	//
static int currentCam = 0;
static vector<Camera*> cameras;
//	cameraName  (pos ( x ,   y,    z ), cen ( x ,   y,   z ), up  ( x,   y,   z ));
static Camera c1(vec3(0.0, 10.0, 15.0), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
static Camera c2(vec3(15.0, 5.0, 0.0 ), vec3(0.0, 10.0, 0.0), vec3(0.0, 1.0, 0.0));
static Camera c3(vec3(0.0, 30.0, 1.0 ), vec3(0.0,  0.0, 0.0), vec3(0.0, 1.0, 0.0));


//	Declaring Functions	//
void SetupTextures();
void SetupObjects();
void checkForCollisions(float deltaTime);





// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
   glEnable(GL_DEPTH_TEST);

   // Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 

   //codes for OpenGL lighting
   //	Material For most objects
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

   //	Materials for Player
   glUniform4fv(glGetUniformLocation(programId, "playerFandB.ambRefl"), 1, &playerFandB.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "playerFandB.difRefl"), 1, &playerFandB.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "playerFandB.specRefl"), 1, &playerFandB.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "playerFandB.emitCols"), 1, &playerFandB.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "playerFandB.shininess"), playerFandB.shininess);

   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

   glUniform4fv(glGetUniformLocation(programId, "light1.ambCols"), 1, &light1.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light1.difCols"), 1, &light1.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light1.specCols"), 1, &light1.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light1.coords"), 1, &light1.coords[0]);
   
   SetupObjects();

   SetupTextures();
    

   // Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat");   //uniform mat4 projMat;
   //	 createFrustum(left,right,bottom,top,near,far)
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 500.0); //	
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   
   // Obtain modelview matrix uniform and object uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat");   //uniform mat4 modelViewMat;
   objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;
   
   //	Adding Cameras to Camera Vector	//
   cameras.push_back(&c1);
   cameras.push_back(&c2);
   cameras.push_back(&c3);



   //	Adding Objects to GameObject container	//
   objects.push_back(&coins);
   objects.push_back(&player);
   objects.push_back(&rightBarrier);
   objects.push_back(&leftBarrier);
   objects.push_back(&healthP);
   objects.push_back(&spikeBall);
   objects.push_back(&spikeBall2);
   objects.push_back(&spikeBall3);
   objects.push_back(&spikeBall4);
   objects.push_back(&coins2);

   //	Applying Drag	//
   FG.AddDrag(&player);
   
   FG.AddDrag(&spikeBall);
   FG.AddDrag(&spikeBall2);
   FG.AddDrag(&spikeBall3);
   FG.AddDrag(&spikeBall4);
   
   //	Adding Gravity	//
   FG.AddGravity(&player);
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //	Camera	//
   switch (currentCam)
   {
   case 0:
	   c1.LookAtPlayerSide(player.GetPosition(), modelViewMatLoc);
	   break;
	   
   case 1:
	   c2.LookAtPlayerFront(player.GetPosition(), modelViewMatLoc);
	   break;

   case 2:
	   c3.LookAtPlayerTop(player.GetPosition(), modelViewMatLoc);
	   break;

   default:
	   break;
   }
   

   // Draw field.
   glUniform1ui(objectLoc, FIELD);  //if (object == FIELD)
   glBindVertexArray(vao[FIELD]);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   // Draw sky.
   glUniform1ui(objectLoc, SKY);  //if (object == SKY)
   glBindVertexArray(vao[SKY]);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   //	Iterate through the object container and check if the object is there before drawing
   //	Used this to create a makeshift and very inefficient (but functional) despawn mechanic 
   for (int i = 0; i < objects.size(); i++)	
   {
	   if (objects[i] == &coins)
	   {
		   // Draw Coin
		   coins.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, COIN);  //if (object == COIN)
		   coins.Draw();
	   }
	   else if (objects[i] == &player)
	   {
		   // Draw my Model
		   player.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, PLAYER);  //if (object == PLAYER)
		   player.Draw();
	   }
	   else if (objects[i] == &rightBarrier)
	   {
		   // Draw my right barrier
		   rightBarrier.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, RIGHTBARRIER);  //if (object == RIGHTBARRIER)
		   rightBarrier.Draw();
	   }
	   else if (objects[i] == &leftBarrier)
	   {
		   // Draw my left barrier
		   leftBarrier.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, LEFTBARRIER);  //if (object == LEFTBARRIER)
		   leftBarrier.Draw();
	   }
	   else if (objects[i] == &healthP)
	   {
		   // Draw heathP
		   healthP.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, HEALTHPOTION);  //if (object == HEALTHPOTION)
		   healthP.Draw();
	   }
	   else if (objects[i] == &spikeBall)
	   {
		   // Draw spikeBall
		   spikeBall.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, SPIKEBALL);  //if (object == SPIKEBALL)
		   spikeBall.Draw();
	   }
	   else if (objects[i] == &spikeBall2)
	   {
		   // Draw spikeBall2
		   spikeBall2.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, SPIKEBALL2);  //if (object == SPIKEBALL2)
		   spikeBall2.Draw();
	   }
	   else if (objects[i] == &spikeBall3)
	   {
		   // Draw spikeBal3
		   spikeBall3.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, SPIKEBALL3);  //if (object == SPIKEBALL3)
		   spikeBall3.Draw();
	   }
	   else if (objects[i] == &spikeBall4)
	   {
		   // Draw spikeBall4
		   spikeBall4.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, SPIKEBALL4);  //if (object == SPIKEBALL4)
		   spikeBall4.Draw();
	   }
	   else if (objects[i] == &coins2)
	   {
		   // Draw Coin
		   coins2.updateModelMatrix(modelViewMatLoc, cameras[currentCam]);
		   glUniform1ui(objectLoc, COIN2);  //if (object == COIN2)
		   coins2.Draw();
	   }
   }

   glutSwapBuffers();
}

void Update() 
{
	oldTimeSinceStart = newTimeSinceStart;
	newTimeSinceStart = glutGet(GLUT_ELAPSED_TIME);

	float deltaTime = (newTimeSinceStart - oldTimeSinceStart);
	deltaTime /= 1000.f;
	
	
	checkForCollisions(deltaTime);
	

	for (int i = 0; i < objects.size(); ++i)	//	Reseting total forces
	{
		if (objects[i]->GetPhysicsBody() != nullptr)
		{
			objects[i]->GetPhysicsBody()->ResetTotalForces();
		}

	}

	FG.UpdateForces();	//	Applying Forces

	for (int i = 0; i < objects.size(); ++i)	//	Updating all objects
	{
		objects[i]->Update(deltaTime);
	}

	FG.ApplyDrag(deltaTime);	//	Applying drag

	//d = myModel.GetPosition().x;
	// refresh screen 
	glutPostRedisplay();

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h); 
}

// Keyboard input processing routine.
void KeyInputDown(unsigned char key, int x, int y)
{
	GameObject::keys[key] = true;
	//std::cout << "Key pressed: " << key << " : " << GameObject::keys[key] << std::endl;

	//	Changing Integration Method System //
   switch(key) 
   {
   case '1':
	   cout << "Switched to Explicit Euler" << endl;
	   GameObject::integrationMethod = 1;
	   //cout << GameObject::integrationMethod << endl;	//	Checking if the GameObject static int is being changed	
	   break;

   case '2':
	   cout << "Switched to Semi Implicit Euler" << endl;
	   GameObject::integrationMethod = 2;
	   //cout << GameObject::integrationMethod << endl;
	   break;

   case '3':
	   cout << "Switched to Velocity Verlet" << endl;
	   GameObject::integrationMethod = 3;
	   //cout << GameObject::integrationMethod << endl;
	   break;

   case '4':
	   cout << "Side View" << endl;
	   currentCam = 0;
	   break;

   case '5':
	   cout << "Front View" << endl;
	   currentCam = 1;
	   break;

   case '6':
	   cout << "Top View" << endl;
	   currentCam = 2;
	   break;

   case 27:
         exit(0);
         break;
      default:
         break;
   }
}

void KeyInputUp(unsigned char key, int x, int y)
{
	GameObject::keys[key] = false;
	//std::cout << "Key de-pressed: " << key << " : " << GameObject::keys[key] << std::endl;
}

// Callback routine for non-ASCII key entry.
void SpecialKeyInputDown(int key, int x, int y)
{
	GameObject::specialKeys[key] = true;
	//std::cout << "Special key pressed: " << key << " : " << GameObject::specialKeys[key] << std::endl;

  
}

void SpecialKeyInputUp(int key, int x, int y)
{
	GameObject::specialKeys[key] = false;
	//std::cout << "Special key de-pressed: " << key << " : " << GameObject::specialKeys[key] << std::endl;
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   //cout << "Interaction:" << endl;
   //cout << "Press the left and right arrow keys to move the viewpoint over the field." << endl; 
}

// Main routine.
int main(int argc, char **argv) 
{
	string a;
	cout << "How to play: " << endl;
	cout << endl;
	cout << "A & D to move left and right" << endl;
	cout << "Space to jump" << endl;
	cout << endl;
	cout << "Avoid Spike Balls (Causes 20 damage)" << endl;
	cout << "Avoid Spike Walls (Causes 10 damage)" << endl;
	cout << "You have 100 health, if you reach 0, the game will end!" << endl;
	cout << endl;
	cout << "You can regen 100 health by collecting the green ball" << endl;
	cout << "Collect both yellow balls to win" << endl;
	cout << endl;
	cout << "Press Enter When ready and more importantly, dont touch the Lava!" << endl;
	cout << endl;
	cout << endl;
	cout << "Advanced: To Switch Between Integration Methods" << endl;
	cout << "Press 1: Explicit Euler" << endl;
	cout << "Press 2: Semi Implicit Euler" << endl;
	cout << "Press 3: Velocity Verlet" << endl;
	cout << endl;
	cout << endl;
	cout << "Advanced: To Switch Between Cameras" << endl;
	cout << "Press 4: Side View" << endl;
	cout << "Press 5: Front View" << endl;
	cout << "Press 6: Top View" << endl;
	getline(cin, a);

   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(1920, 1080);
   glutInitWindowPosition(0, 0); 
   glutCreateWindow("Lava Vs Aliens");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutIdleFunc(Update); ///animation function
   glutKeyboardFunc(KeyInputDown);
   glutKeyboardUpFunc(KeyInputUp);
   glutSpecialFunc(SpecialKeyInputDown);
   glutSpecialUpFunc(SpecialKeyInputUp);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}



//	Functions	//


void SetupObjects()
{
	///// Create a field  ///////////

   //Create VAO and VBO
	glGenVertexArrays(1, &vao[FIELD]);
	glGenBuffers(1, &buffer[FIELD_VERTICES]);
	glBindVertexArray(vao[FIELD]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FIELD_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);

	///// Create a Sky /////////////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[SKY]);
	glGenBuffers(1, &buffer[SKY_VERTICES]);
	glBindVertexArray(vao[SKY]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[SKY_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)(sizeof(skyVertices[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(1);

	/////Create a Coin /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[COIN]);
	glGenBuffers(1, &buffer[COIN_VERTICES]);

	//Binding VAO and VBO
	coins.SetIDs(vao[COIN], buffer[COIN_VERTICES], buffer[COIN_INDICES]);
	coins.Setup();

	/////Create a Player /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[PLAYER]);
	glGenBuffers(1, &buffer[PLAYER_VERTICES]);

	//Binding VAO and VBO
	player.SetIDs(vao[PLAYER], buffer[PLAYER_VERTICES], 0);
	player.Setup();

	/////Create a Right Barrier /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[RIGHTBARRIER]);
	glGenBuffers(1, &buffer[RIGHTBARRIER_VERTICES]);

	//Binding VAO and VBO
	rightBarrier.SetIDs(vao[RIGHTBARRIER], buffer[RIGHTBARRIER_VERTICES], 0);
	rightBarrier.Setup();

	/////Create a Left Barrier /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[LEFTBARRIER]);
	glGenBuffers(1, &buffer[LEFTBARRIER_VERTICES]);

	//Binding VAO and VBO
	leftBarrier.SetIDs(vao[LEFTBARRIER], buffer[LEFTBARRIER_VERTICES], 0);
	leftBarrier.Setup();

	/////Create a Health Potion /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[HEALTHPOTION]);
	glGenBuffers(1, &buffer[HEALTHPOTION_VERTICES]);

	//Binding VAO and VBO
	healthP.SetIDs(vao[HEALTHPOTION], buffer[HEALTHPOTION_VERTICES], buffer[HEALTHPOTION_INDICES]);
	healthP.Setup();

	/////Create a Spike Ball /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[SPIKEBALL]);
	glGenBuffers(1, &buffer[SPIKEBALL_VERTICES]);

	//Binding VAO and VBO
	spikeBall.SetIDs(vao[SPIKEBALL], buffer[SPIKEBALL_VERTICES], 0);
	spikeBall.Setup();

	/////Create a Spike Ball2 /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[SPIKEBALL2]);
	glGenBuffers(1, &buffer[SPIKEBALL2_VERTICES]);

	//Binding VAO and VBO
	spikeBall2.SetIDs(vao[SPIKEBALL2], buffer[SPIKEBALL2_VERTICES], 0);
	spikeBall2.Setup();

	/////Create a Spike Ball3 /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[SPIKEBALL3]);
	glGenBuffers(1, &buffer[SPIKEBALL3_VERTICES]);

	//Binding VAO and VBO
	spikeBall3.SetIDs(vao[SPIKEBALL3], buffer[SPIKEBALL3_VERTICES], 0);
	spikeBall3.Setup();

	/////Create a Spike Ball4 /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[SPIKEBALL4]);
	glGenBuffers(1, &buffer[SPIKEBALL4_VERTICES]);

	//Binding VAO and VBO
	spikeBall4.SetIDs(vao[SPIKEBALL4], buffer[SPIKEBALL4_VERTICES], 0);
	spikeBall4.Setup();

	/////Create a Coin2 /////

	//Create VAO and VBO
	glGenVertexArrays(1, &vao[COIN2]);
	glGenBuffers(1, &buffer[COIN2_VERTICES]);

	//Binding VAO and VBO
	coins2.SetIDs(vao[COIN2], buffer[COIN2_VERTICES], buffer[COIN2_INDICES]);
	coins2.Setup();
}

void SetupTextures()
{
	// Load the images.
	std::string TexNames[] = {
		 "Textures/Lava.png",
		 "Textures/sky.bmp",
		 "Textures/Player.png",
		 "Textures/SpikeBall.png",
	};

	// Create texture ids.
	glGenTextures(4, texture);

	int width, height;
	unsigned char* data;

	// Bind Lava image	//
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//load image data using SOIL library
	data = SOIL_load_image(TexNames[0].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	LavaTexLoc = glGetUniformLocation(programId, "LavaTex");
	glUniform1i(LavaTexLoc, 0); //send texture to shader

	// Bind sky image	//
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	data = SOIL_load_image(TexNames[1].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	skyTexLoc = glGetUniformLocation(programId, "skyTex");
	glUniform1i(skyTexLoc, 1); //send texture to shader
	
	// Bind Player image	//
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	data = SOIL_load_image(TexNames[2].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	PlayerTexLoc = glGetUniformLocation(programId, "PlayerTex");
	glUniform1i(PlayerTexLoc, 2); //send texture to shader

	// Bind Spike Ball image	//
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	data = SOIL_load_image(TexNames[3].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	SOIL_free_image_data(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SpikeTexLoc = glGetUniformLocation(programId, "SpikeTex");
	glUniform1i(SpikeTexLoc, 3); //send texture to shader
}

//	Check for and handle collisions	//
void checkForCollisions(float deltaTime)
{
	//	If player touches lava, end game	//
	if (player.GetCollider()->GetPosition().y < fieldVertices->coords.y)
	{
		cout << "\n\n\n\n\n" << endl;
		cout << "Game Over!" << endl;
		cout << "\n\n" << endl;
		cout << "Final score: " << score << endl;
		cout << "\n\n\n\n\n" << endl;
		exit(0);
	}

	//	If player flies too high, push down	//
	if (player.GetCollider()->GetPosition().y > 25.0f)
	{
		vec3 tempV = player.GetPhysicsBody()->GetVelocity();
		tempV.y = -10.0f;
		player.GetPhysicsBody()->SetVelocity(tempV);
	}

	//	If player touches Right Barrier, Lose Health and inverse velocity	//
	if (player.GetCollider()->GetPosition().x > rightBarrier.GetPosition().x - 5.0f)
	{
		vec3 tempV = player.GetPhysicsBody()->GetVelocity();
		tempV.x = 0 - player.GetPhysicsBody()->GetVelocity().x;
		player.GetPhysicsBody()->SetVelocity(vec3(tempV));
		health -= 10.0f;
		cout << "Lost 10 health! (Tip: Dont go into spikey Barriers)" << score << endl;
	}

	//	If player touches left Barrier, Lose Health and inverse velocity	//
	if (player.GetCollider()->GetPosition().x < leftBarrier.GetPosition().x + 5.0f)
	{
		vec3 tempV = player.GetPhysicsBody()->GetVelocity();
		tempV.x = 0 - player.GetPhysicsBody()->GetVelocity().x;
		player.GetPhysicsBody()->SetVelocity(vec3(tempV));
		health -= 10.0f;
		cout << "Lost 10 health! (Tip: Dont go into spikey Barriers)" << score << endl;
	}

	//	If player touches other objects	//
	for (int i = 0; i < objects.size(); i++)	//	Checking for collisions
	{
		if (objects[i]->GetCollider() != nullptr && objects[i] != &player)
		{
			if (player.GetCollider()->CollideCheck(objects[i]->GetCollider()))
			{
				if (objects[i]->GetType() == "Coin")	//	if the player hits a coin, remove the coin and add 100 points	//
				{
					score += 100;
					objects.erase(objects.begin() + i);
					objects.shrink_to_fit();
				}
				else if (objects[i]->GetType() == "HealthPotion")	//	If the player hits a potion, despawn the potion and refill health	//
				{
					health = 100;
					cout << "Health Restored to 100!" << endl;
					objects.erase(objects.begin() + i);
					objects.shrink_to_fit();
				}
				else if (objects[i]->GetType() == "SpikeBall")	//	If Player Hits a spike, resolve the collision and deduct health	//
				{
					//	Finding basis vector
					vec3 x = player.GetPosition() - objects[i]->GetPhysicsBody()->GetPosition();
					normalize(x);

					vec3 v1 = player.GetPhysicsBody()->GetVelocity();
					float x1 = dot(x, v1);

					vec3 v1x = x * x1;
					vec3 v1y = v1 - v1x;

					float m1 = player.GetPhysicsBody()->GetMass();



					x = x * -1.0f;

					vec3 v2 = objects[i]->GetPhysicsBody()->GetVelocity();
					float x2 = dot(x, v2);

					vec3 v2x = x * x2;
					vec3 v2y = v1 - v2x;

					float m2 = objects[i]->GetPhysicsBody()->GetMass();

					//	Velocity of object 1
					vec3 velo1 = v1x * (m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y;
					player.GetPhysicsBody()->SetVelocity(velo1 * 0.1f);

					//	Velocity of object 2
					vec3 velo2 = v1x * (2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y;
					objects[i]->GetPhysicsBody()->SetVelocity(velo2 * 0.1f);

					health -= 20.0f;
					cout << "Lost 20 health! (Tip: Dont go into spikes)" << endl;
				}
			}
		}
	}


	//	Check if Player is alive	//
	if (health < 1.0f)
	{
		cout << "\n\n\n\n\n" << endl;
		cout << "Game Over!" << endl;
		cout << "\n\n" << endl;
		cout << "Final score: " << score << endl;
		cout << "\n\n\n\n\n" << endl;
		exit(0);
	}
}