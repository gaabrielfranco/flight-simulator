#include "OBJ.h"
#include <cassert>
#include <string>

OBJ::OBJ(const char *filename)
    : lexer(filename)
{
}

void OBJ::next()
{
      token = (to_rewind) ? token : lexer.next();
      to_rewind = false;
}

void OBJ::rewind()
{
      to_rewind = true;
}

void OBJ::reset()
{
      to_rewind = false;
      lexer.reset();
}

void OBJ::load(Model &model)
{
      model.geometric_vertices.clear();
      model.geometric_vertices.push_back(Model::Geometric_Vertex{});
      model.texture_vertices.clear();
      model.texture_vertices.push_back(Model::Texture_Vertex{});
      model.normal_vertices.clear();
      model.normal_vertices.push_back(Model::Normal_Vertex{});

      reset();

      for (bool step = true; step;)
      {
            next();

            switch (token.tag)
            {
            case Tag::V:
            {
                  Model::Geometric_Vertex vertex;

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.x = std::stof(token.lexeme);

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.y = std::stof(token.lexeme);

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.z = std::stof(token.lexeme);

                  next();

                  if (token.tag == Tag::NUMBER)
                  {
                        vertex.w = std::stof(token.lexeme);
                  }
                  else
                  {
                        vertex.w = 1.0;
                        rewind();
                  }

                  next();
                  assert(token.tag == Tag::NEWLINE);

                  model.geometric_vertices.push_back(vertex);
                  break;
            }

            case Tag::VN:
            {
                  Model::Normal_Vertex vertex;

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.i = std::stof(token.lexeme);

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.j = std::stof(token.lexeme);

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.k = std::stof(token.lexeme);

                  next();
                  assert(token.tag == Tag::NEWLINE);

                  model.normal_vertices.push_back(vertex);
                  break;
            }

            case Tag::VT:
            {
                  Model::Texture_Vertex vertex;

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.u = std::stof(token.lexeme);

                  next();
                  assert(token.tag == Tag::NUMBER);

                  vertex.v = std::stof(token.lexeme);

                  next();

                  if (token.tag == Tag::NUMBER)
                  {
                        vertex.w = std::stof(token.lexeme);
                  }
                  else
                  {
                        vertex.w = 1.0;
                        rewind();
                  }

                  next();
                  assert(token.tag == Tag::NEWLINE);

                  model.texture_vertices.push_back(vertex);
                  break;
            }

            case Tag::F:
            {
                  Model::Face face;

                  for (;;)
                  {
                        Model::Face_Vertex vertex;

                        next();
                        if (token.tag == Tag::NEWLINE)
                              break;

                        assert(token.tag == Tag::NUMBER);

                        vertex.geometric_vertex = std::stoull(token.lexeme, nullptr, 10);

                        next();

                        if (token.tag == Tag('/'))
                        {
                              next();

                              if (token.tag == Tag('/'))
                              {
                                    next();
                                    assert(token.tag == Tag::NUMBER);

                                    vertex.normal_vertex =
                                        std::stoull(token.lexeme, nullptr, 10);
                              }
                              else
                              {
                                    assert(token.tag == Tag::NUMBER);

                                    vertex.texture_vertex =
                                        std::stoull(token.lexeme, nullptr, 10);

                                    next();

                                    if (token.tag == Tag('/'))
                                    {
                                          next();
                                          assert(token.tag == Tag::NUMBER);

                                          vertex.normal_vertex =
                                              std::stoull(token.lexeme, nullptr, 10);
                                    }
                                    else
                                    {
                                          rewind();
                                    }
                              }
                        }
                        else
                        {
                              rewind();
                        }

                        face.vertices.push_back(vertex);
                  }

                  model.faces.push_back(face);
                  break;
            }

            case Tag::END:
            {
                  step = false;
            }
            }
      }
}
