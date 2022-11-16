#include "portral.h"

portral::portral(Vector3 pos)
{
    
	Vector3 vertices[] = {
		Vector3(-0.5f,-0.5f,-0.5f),
		Vector3(0.5f, -0.5f, -0.5f),
		Vector3(0.5f,  0.5f, -0.5f),
		Vector3(0.5f,  0.5f, -0.5f),
		Vector3(-0.5f,  0.5f, -0.5f),
		Vector3(-0.5f, -0.5f, -0.5f),

         Vector3(-0.5f, -0.5f,  0.5f),
         Vector3(0.5f, -0.5f,  0.5f),
         Vector3(0.5f,  0.5f,  0.5f),
         Vector3(0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f,  0.5f),
        Vector3(-0.5f, -0.5f,  0.5f),

        Vector3(-0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(-0.5f, -0.5f,  0.5f),
       Vector3(-0.5f,  0.5f,  0.5f),

        Vector3(0.5f,  0.5f,  0.5f),
        Vector3(0.5f,  0.5f, -0.5f),
        Vector3(0.5f, -0.5f, -0.5f),
        Vector3(0.5f, -0.5f, -0.5f),
        Vector3(0.5f, -0.5f,  0.5f),
        Vector3(0.5f,  0.5f,  0.5f),

       Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(0.5f, -0.5f, -0.5f),
        Vector3(0.5f, -0.5f,  0.5f),
        Vector3(0.5f, -0.5f,  0.5f),
        Vector3(-0.5f, -0.5f,  0.5f),
        Vector3(-0.5f, -0.5f, -0.5f),

        Vector3(-0.5f,  0.5f, -0.5f),
        Vector3(0.5f,  0.5f, -0.5f),
        Vector3(0.5f,  0.5f,  0.5f),
        Vector3(0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f,  0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
	};

    this->position = pos;
    this->vertices = vertices;

    BufferData();
}

