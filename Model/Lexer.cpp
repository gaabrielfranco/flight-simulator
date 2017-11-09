#include <Model/Lexer.h>

std::unordered_map<std::string, Tag> Lexer::keywords;
Lexer::Scope_Guard Lexer::guard;

Lexer::Scope_Guard::Scope_Guard()
{
   keywords["v"] = Tag::V;
   keywords["vt"] = Tag::VT;
   keywords["vn"] = Tag::VN;
   keywords["f"] = Tag::F;
}

enum States
{
   IGNORE,
   S,
   NEWLINE,
   WORD,
   COMMENT,
   INT,
   FLOAT0,
   FLOAT1,
   FLOAT2,
   FLOAT3,
   FLOAT4
};

Lexer::Lexer(const char *filename)
{
   auto stream = fopen(filename, "rb");

   if (!stream) exit(1);

   fseek(stream, 0, SEEK_END);
   auto stream_size = ftell(stream);
   fseek(stream, 0, SEEK_SET);

   buffer = std::make_unique<char[]>(stream_size + 1);
   buffer[stream_size] = 0;

   fread(buffer.get(), stream_size, 1, stream);
   fclose(stream);

   at = 0;
}

void Lexer::reset()
{
   at = 0;
}

Token Lexer::next()
{
   char c;
   size_t begin;
   Token token;

   int state = IGNORE;

   for (int step = true; step; at++)
   {
      switch (state)
      {
         case IGNORE:
         {
            switch (c = buffer[at])
            {
               case ' ':
               case '\t':
               case '\v':
               case '\f':
                  break;

               case '#':
                  state = COMMENT;
                  break;

               default:
                  begin = at;
                  at -= 1;
                  state = S;
            }

            break;
         }

         case COMMENT:
         {
            switch (c = buffer[at])
            {
               case '\n':
                  at -= 1;
                  state = IGNORE;
                  break;

               case '\r':
                  at -= 1;
                  state = IGNORE;
                  break;
            }

            break;
         }

         case S:
         {
            switch (c = buffer[at])
            {
               case 0:
                  at -= 1;
                  token = Token{ Tag::END };
                  step = false;
                  break;

               case '\n':
                  token = Token{ Tag::NEWLINE };
                  step = false;
                  break;

               case '\r':
                  state = NEWLINE;
                  break;

               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
               case '-':
                  state = INT;
                  break;

               case '_':
               case 'a':
               case 'b':
               case 'c':
               case 'd':
               case 'e':
               case 'f':
               case 'g':
               case 'h':
               case 'i':
               case 'j':
               case 'k':
               case 'l':
               case 'm':
               case 'n':
               case 'o':
               case 'p':
               case 'q':
               case 'r':
               case 's':
               case 't':
               case 'u':
               case 'v':
               case 'w':
               case 'x':
               case 'y':
               case 'z':
               case 'A':
               case 'B':
               case 'C':
               case 'D':
               case 'E':
               case 'F':
               case 'G':
               case 'H':
               case 'I':
               case 'J':
               case 'K':
               case 'L':
               case 'M':
               case 'N':
               case 'O':
               case 'P':
               case 'Q':
               case 'R':
               case 'S':
               case 'T':
               case 'U':
               case 'V':
               case 'W':
               case 'X':
               case 'Y':
               case 'Z':
                  state = WORD;
                  break;

               case '.':
                  state = FLOAT0;
                  break;

               default:
                  token = Token{ Tag(c) };
                  step = false;
            }

            break;
         }

         case NEWLINE:
         {
            switch (c = buffer[at])
            {
               case '\n':
                  token = Token{ Tag::NEWLINE };
                  step = false;
                  break;

               default:
                  at -= 1;
                  token = Token{ Tag::NEWLINE };
                  step = false;
            }

            break;
         }

         case WORD:
         {
            switch (c = buffer[at])
            {
               case '_':
               case 'a':
               case 'b':
               case 'c':
               case 'd':
               case 'e':
               case 'f':
               case 'g':
               case 'h':
               case 'i':
               case 'j':
               case 'k':
               case 'l':
               case 'm':
               case 'n':
               case 'o':
               case 'p':
               case 'q':
               case 'r':
               case 's':
               case 't':
               case 'u':
               case 'v':
               case 'w':
               case 'x':
               case 'y':
               case 'z':
               case 'A':
               case 'B':
               case 'C':
               case 'D':
               case 'E':
               case 'F':
               case 'G':
               case 'H':
               case 'I':
               case 'J':
               case 'K':
               case 'L':
               case 'M':
               case 'N':
               case 'O':
               case 'P':
               case 'Q':
               case 'R':
               case 'S':
               case 'T':
               case 'U':
               case 'V':
               case 'W':
               case 'X':
               case 'Y':
               case 'Z':
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  break;

               default:

                  auto lexeme = std::string(buffer.get() + begin, at - begin);

                  if (keywords.find(lexeme) != keywords.end())
                  {
                     token = Token{ keywords[lexeme] };
                  }
                  else
                  {
                     token = Token{ Tag::WORD, lexeme };
                  }

                  at -= 1;
                  step = false;
            }

            break;
         }

         case INT:
         {
            switch (c = buffer[at])
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  break;

               case '.':
                  state = FLOAT1;
                  break;

               case 'e':
               case 'E':
                  state = FLOAT2;
                  break;

               default:
                  token = Token{ Tag::NUMBER, std::string(buffer.get() + begin,
                                                          at - begin) };
                  at -= 1;
                  step = false;
            }

            break;
         }

         case FLOAT0:
         {
            switch (c = buffer.get()[at])
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  state = FLOAT1;
                  break;

               default:
                  at -= 1;
                  token = Token{ Tag('.') };
                  step = false;
            }
            break;
         }

         case FLOAT1:
         {
            switch (c = buffer.get()[at])
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  break;

               case 'e':
               case 'E':
                  state = FLOAT2;
                  break;

               default:
                  token = Token{ Tag::NUMBER, std::string(buffer.get() + begin,
                                                          at - begin) };
                  at -= 1;
                  step = false;
            }

            break;
         }

         case FLOAT2:
         {
            switch (c = buffer[at])
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  state = FLOAT4;
                  break;

               case '+':
               case '-':
                  state = FLOAT3;
                  break;

               default:
                  token = Token{ Tag::UNKNOW, std::string(buffer.get() + begin,
                                                          at - begin) };
                  at -= 1;
                  step = false;
            }

            break;
         }

         case FLOAT3:
         {
            switch (c = buffer[at])
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  state = FLOAT4;
                  break;

               default:
                  token = Token{ Tag::UNKNOW, std::string(buffer.get() + begin,
                                                          at - begin) };
                  at -= 1;
                  step = false;
            }

            break;
         }

         case FLOAT4:
         {
            switch (c = buffer[at])
            {
               case '0':
               case '1':
               case '2':
               case '3':
               case '4':
               case '5':
               case '6':
               case '7':
               case '8':
               case '9':
                  break;

               default:
                  token = Token{ Tag::NUMBER, std::string(buffer.get() + begin,
                                                          at - begin) };
                  at -= 1;
                  step = false;
            }

            break;
         }
      }
   }

   return token;
}
