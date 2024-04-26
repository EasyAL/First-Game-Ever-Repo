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

in vec2 texCoordsExport;
in vec3 normalExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};

uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;
uniform Light light1;
uniform Material playerFandB;

uniform sampler2D LavaTex;
uniform sampler2D skyTex;
uniform sampler2D PlayerTex;
uniform sampler2D SpikeTex;
uniform uint object;

out vec4 colorsOut;

vec4 LavaTexColor, skyTexColor, PlayerTexColor, SpikeTexColor;
vec3 normal, lightDirection;
vec4 fAndBDif;

void main(void)
{  
   LavaTexColor = texture(LavaTex, texCoordsExport);
   skyTexColor = texture(skyTex, texCoordsExport);
   PlayerTexColor = texture(PlayerTex, texCoordsExport);
   SpikeTexColor = texture(SpikeTex, texCoordsExport);

   if (object == FIELD)
   {
		colorsOut = LavaTexColor;
   }
   

   if (object == SKY) 
   {
		colorsOut = skyTexColor;
   }
   

   if (object == COIN) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }


   if (object == PLAYER) //	Affected by both lights; from sun and lava
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light1.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light1.difCols * playerFandB.difRefl) + PlayerTexColor;
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif += max(dot(normal, lightDirection), 0.0f) * (light0.difCols * playerFandB.difRefl) + PlayerTexColor;
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }

   if (object == RIGHTBARRIER) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + LavaTexColor;
   }
   if (object == LEFTBARRIER) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + LavaTexColor;
   }
   if (object == HEALTHPOTION) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + vec4(0.0f, 0.66f, 0.0f, 0.0f);	//	Adding green
   }
   if (object == SPIKEBALL) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + SpikeTexColor + vec4(0.2f, 0.0f, 0.0f, 0.0f);
   }
   if (object == SPIKEBALL2) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + SpikeTexColor + vec4(0.2f, 0.0f, 0.0f, 0.0f);
   }
   if (object == SPIKEBALL3) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + SpikeTexColor + vec4(0.2f, 0.0f, 0.0f, 0.0f);
   }
   if (object == SPIKEBALL4) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0) + SpikeTexColor + vec4(0.2f, 0.0f, 0.0f, 0.0f);
   }
   if (object == COIN2) 
   {
		normal = normalize(normalExport);
		lightDirection = normalize(vec3(light0.coords));
		fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); 
		colorsOut =  vec4(vec3(min(fAndBDif, vec4(1.0))), 1.0);
   }
}