#version 430 core

#define FIELD 0
#define SKY 1
#define COIN 2
#define PLAYER 3
#define RIGHTBARRIER 4
#define LEFTBARRIER 5
#define HEALTHPOTION 6
#define SPIKEBALL 7
#define SPIKEBALL2 8
#define SPIKEBALL3 9
#define SPIKEBALL4 10
#define COIN2 11

layout(location=0) in vec4 Coords;
layout(location=1) in vec2 TexCoords;

layout(location=2) in vec4 sphereCoords;
layout(location=3) in vec3 sphereNormals;

layout(location=4) in vec3 objCoords;
layout(location=5) in vec3 objNormals;
layout(location=6) in vec2 objTexCoords;



uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;

out vec2 texCoordsExport;
out vec3 normalExport;

vec4 coords;

void main(void)
{   
   if (object == FIELD)
   {
      coords = Coords;
      texCoordsExport = TexCoords;
   }
   if (object == SKY)
   {
      coords = Coords;
      texCoordsExport = TexCoords;
   }
   if (object == COIN)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }
   if (object == PLAYER)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == RIGHTBARRIER)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == LEFTBARRIER)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == HEALTHPOTION)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }
   if (object == SPIKEBALL)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == SPIKEBALL2)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == SPIKEBALL3)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == SPIKEBALL4)
   {
      coords = vec4(objCoords, 1.0f);
      normalExport = objNormals;
      texCoordsExport = objTexCoords;
   }
   if (object == COIN2)
   {
      coords = sphereCoords;
      normalExport = sphereNormals;
   }

   gl_Position = projMat * modelViewMat * coords;
}
