#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in float color_flag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float colorFlag;

void main(){
    gl_Position = projection * view * model * vec4(position.xyz, 1.0);
    colorFlag = color_flag;
}
