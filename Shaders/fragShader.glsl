#version 330 core
/**
Here a fragment corresponds to a point
**/

out vec4 outColor; 
in float colorFlag;
uniform bool Isgrid;

void main(){
    if(Isgrid){
        outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else{
        if(colorFlag == 1.0f){
            outColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
        }
        else{
            outColor = vec4(0.4f, 0.0f, 0.0f, 0.2f);
        }
    }
}