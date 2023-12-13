#pragma once

#include "vertex.h"
#include "citro3d.h"
#include "defines.h"
#include "componentmanager.h"
#include "material.h"

class GameObject;

class mesh {
    public:
    // Configure attributes for use with the vertex shader
	C3D_AttrInfo attrInfo;
    C3D_BufInfo buf;
    int numVerts;
    char vertsize;
    void *vertices;
    float radius;
    material* mat;

    mesh(GameObject& parent, void* data);

    mesh(void* vertices, unsigned int numVerts, unsigned char vertsize, float radius, unsigned char attrnum, unsigned char attrtypes[], unsigned char attrlen[]);
    
    ~mesh();
};