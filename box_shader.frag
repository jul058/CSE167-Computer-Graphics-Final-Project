#version 330 core

//in vec3 FragPos;
uniform int ID;
out vec4 color;

void main()
{
    if(ID == 0)
        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    else if(ID == 1)
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else
        color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    //color = vec4(TexCoords,1.0);
}