#version 330

uniform mat4 modelViewProjection;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in float pointSize;

out vec4 fragColor;

void main(void)
{
    fragColor = vertexColor;
    gl_PointSize = pointSize;
    gl_Position = modelViewProjection * vec4(vertexPosition, 1);
}
