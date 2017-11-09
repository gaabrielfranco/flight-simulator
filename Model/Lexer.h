#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <memory>
#include <unordered_map>

enum struct Tag : int
{
   NEWLINE = -1,
   END = -2,
   WORD = 256,
   NUMBER,
   V,
   VT,
   VN,
   F,
   UNKNOW,
};

struct Token
{
   Tag tag;
   std::string lexeme;
};

class Lexer
{
   struct Scope_Guard
   {
      Scope_Guard();
   };

   static std::unordered_map<std::string, Tag> keywords;
   static Scope_Guard guard;

   std::unique_ptr<char[]> buffer;
   size_t at;

public:
   Lexer(const char *filename);
   void reset();
   Token next();
};

#endif   // LEXER_H
