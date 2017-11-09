#include <Model/OBJ.h>

using namespace std;

int main()
{
   Model model;

   OBJ obj("Samples/teste.obj");
   obj.load(model);

   for (auto &v : model.geometric_vertices)
   {
      printf("v %f %f %f %f\n", v.x, v.y, v.z, v.w);
   }

   for (auto &vt : model.texture_vertices)
   {
      printf("vt %f %f %f\n", vt.u, vt.v, vt.w);
   }

   for (auto &vn : model.normal_vertices)
   {
      printf("vn %f %f %f\n", vn.i, vn.j, vn.k);
   }

   for (auto &face : model.faces)
   {
      printf("f ");
      for (auto &vertex : face.vertices)
      {
         printf("%llu/%llu/%llu ", vertex.geometric_vertex,
                vertex.texture_vertex, vertex.normal_vertex);
      }
      printf("\n");
   }

   return 0;
}
