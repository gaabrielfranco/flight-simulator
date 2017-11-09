#include <Model/OBJ.h>

using namespace std;

int main()
{
   Model model;

   OBJ obj("Samples/teste.obj");
   obj.Load(model);

   return 0;
}
