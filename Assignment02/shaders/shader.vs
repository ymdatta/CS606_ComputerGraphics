#version 330 core

layout (location = 0) in vec3 Position;

// Vertex Normal is color itself.
layout (location = 1) in vec3 vertexColor;

// uniform mat4 gWorld;
out vec4 vCol;
out vec3 vPos;
out vec3 lightPos;
flat out int lflag;

uniform mat4 MVP;
uniform vec3 lPos;
uniform int fId;

void main()
{
    if (fId == 0) {
    gl_Position = MVP * vec4(Position, 1.0);
    }
    else {
//    gl_PointSize = 1000.0;
    gl_Position = vec4(lPos, 1.0f);
    }
    vCol = vec4(vertexColor, 1.0f);
    vPos = vec3(gl_Position/gl_Position.w);
    lightPos = lPos;
    lflag = fId;
}
