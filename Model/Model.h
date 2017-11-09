#ifndef MODEL_H
#define MODEL_H

#include <vector>

struct Model
{
   struct Geometric_Vertex
   {
      float x = 0;
      float y = 0;
      float z = 0;
      float w = 1;
   };

   struct Texture_Vertex
   {
      float u = 0;
      float v = 0;
      float w = 0;
   };

   struct Normal_Vertex
   {
      float i = 0;
      float j = 0;
      float k = 0;
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

#endif   // MODEL_H
