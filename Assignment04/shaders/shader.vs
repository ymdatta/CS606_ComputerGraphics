#version 330 core

layout (location = 0) in vec3 Position;

// Vertex Normal is color itself.
layout (location = 1) in vec3 vertexColor;

// Spherical coords
layout (location = 2) in vec2 spherTexCord;
layout (location = 3) in vec2 CycTexCord;
layout (location = 4) in vec2 ArbTexCord;

// uniform mat4 gWorld;
out vec4 vCol;
out vec3 vPos;
out vec3 lightPos;
out vec2 texCord;
flat out int lflag;

uniform mat4 MVP;
uniform mat4 vMat;
uniform vec3 lPos;
uniform int fId;
uniform int cId;

void main()
{
    if (fId == 0) {
        gl_Position = vMat * MVP * vec4(Position, 1.0);
    }
    else {
//    gl_PointSize = 1000.0;
      gl_Position = vec4(lPos, 1.0f);
    }

    
    vCol = vec4(vertexColor, 1.0f);
    vPos = vec3(gl_Position/gl_Position.w);
    lightPos = lPos;
    lflag = fId;

//    texCord = spherTexCord;
//      texCord = CycTexCord;
	texCord = ArbTexCord;

    if (cId == 0) {
       texCord = spherTexCord;
     }
     else if (cId == 1) {
       texCord = CycTexCord;
       }
       else {
       texCord = ArbTexCord;
       }
      
}
