#ifndef OBJ_H
#define OBJ_H

#include <Model/Lexer.h>
#include <Model/Model.h>

class OBJ
{
   Lexer lexer;

   Token token;
   bool to_rewind;

   void next();
   void rewind();
   void reset();

public:
   OBJ(const char *filename);
   void load(Model &model);
};

#endif   // OBJ_H
