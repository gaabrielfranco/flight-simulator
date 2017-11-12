#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/gl.h>

struct Model
{
    struct Geometric_Vertex
    {
        GLfloat x = 0.0;
        GLfloat y = 0.0;
        GLfloat z = 0.0;
        GLfloat w = 1.0;
    };

    struct Texture_Vertex
    {
        GLfloat u = 0.0;
        GLfloat v = 0.0;
        GLfloat w = 0.0;
    };

    struct Normal_Vertex
    {
        GLfloat i = 0.0;
        GLfloat j = 0.0;
        GLfloat k = 0.0;
    };

    struct Face_Vertex
    {
        size_t geometric_vertex = 0;
        size_t texture_vertex = 0;
        size_t normal_vertex = 0;
    };

    struct Face
    {
        std::vector<Face_Vertex> vertices;
    };

    std::vector<Geometric_Vertex> geometric_vertices;
    std::vector<Texture_Vertex> texture_vertices;
    std::vector<Normal_Vertex> normal_vertices;
    std::vector<Face> faces;
};

#endif // MODEL_H
