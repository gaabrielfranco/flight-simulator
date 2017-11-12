#include <Model/OBJ.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <gm.h>

#define num_objects 10

using namespace std;

struct Airplane
{
    Model model;
    bool moving = false;
    bool inside = false;
    float angle = 0.0;
    float position[3] = {0.0, 0.0, 0.0};
};

struct Freeze
{
    //OBJ::OBJ *model;
    float scale = 1.0;
    float y[num_objects];
    float x[num_objects];
    float z[num_objects];
    float angle[num_objects];
};

Airplane airplane;
Freeze elements[5];
Model tower, airfield, road;
int width = 800, height = 600;

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0); //cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); //proje��o paralela

    //glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 20.0); //proje��o perspectiva

    //gluPerspective(70.0, 1.0, 2.0, 20.0); // proje��o perspectiva

    glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
}

void display(void)
{

    if (airplane.moving)
    {
        airplane.position[0] += 5;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    glLoadIdentity();
    gluLookAt(
        airplane.position[0] - 70,
        airplane.position[1] + 35,
        airplane.position[2] - 2.5,
        airplane.position[0],
        airplane.position[1],
        airplane.position[2],
        0.0,
        1.0,
        0.0);

    //Desenha "asfasto"
    glPushMatrix();
    glColor4f(0.5, 0.5, 0.5, 0.3);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-1000.0, 0.0, -1000.0);
    glVertex3f(-1000.0, 0.0, 1000.0);
    glVertex3f(1000.0, 0.0, 1000.0);
    glVertex3f(1000.0, 0.0, -1000.0);
    glEnd();
    glPopMatrix();

    //Desenha estrada
    glPushMatrix();
    glColor4f(1.0, 0.0, 1.0, 0.8);
    glTranslatef(0, 0, 0);
    glScalef(15.0, 15.0, 15.0);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : road.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex3f(road.geometric_vertices[vertex.geometric_vertex].x,
                       road.geometric_vertices[vertex.geometric_vertex].y,
                       road.geometric_vertices[vertex.geometric_vertex].z);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha prédio
    glPushMatrix();
    glColor4f(0.0, 1.0, 1.0, 0.8);
    glTranslatef(100, 0, 0);
    glScalef(5.0, 5.0, 5.0);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : airfield.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex3f(airfield.geometric_vertices[vertex.geometric_vertex].x,
                       airfield.geometric_vertices[vertex.geometric_vertex].y,
                       airfield.geometric_vertices[vertex.geometric_vertex].z);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha torre
    glPushMatrix();
    glColor4f(0.0, 1.0, 0.0, 0.8);
    glTranslatef(-1000, 0, -1000);
    glScalef(50.0, 50.0, 50.0);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : tower.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex3f(tower.geometric_vertices[vertex.geometric_vertex].x,
                       tower.geometric_vertices[vertex.geometric_vertex].y,
                       tower.geometric_vertices[vertex.geometric_vertex].z);
        }
    }
    glEnd();

    glPopMatrix();

    //Desenha avião
    glPushMatrix();
    glTranslatef(airplane.position[0], airplane.position[1], airplane.position[2]);
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
    for (auto &face : airplane.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex3f(airplane.model.geometric_vertices[vertex.geometric_vertex].x,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].y,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].z);

            /*glNormal3f(airplane.model.normal_vertices[vertex.normal_vertex].i,
                       airplane.model.normal_vertices[vertex.normal_vertex].j,
                       airplane.model.normal_vertices[vertex.normal_vertex].k);*/
        }
    }
    glEnd();
    glPopMatrix();

    glTranslatef(airplane.position[0], airplane.position[1], airplane.position[2]);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glTranslatef(-airplane.position[0], -airplane.position[1], -airplane.position[2]);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{

    if (key == 27)
    {
        exit(0);
    }

    if (key == 'c' || key == 'C')
    {
        if (airplane.position[1] < 4000)
            airplane.position[1] += 5.0;
    }

    if (key == 'b' || key == 'B')
    {
        if (airplane.position[1] > 0)
            airplane.position[1] -= 5.0;
    }

    if (key == 'e' || key == 'E')
    {
        airplane.angle -= 1;
        //airplane.facing.rotate_y(-5.0);
    }

    if (key == 'd' || key == 'D')
    {
        airplane.angle += 1;
        //airplane.facing.rotate_y(5.0);
    }

    if (key == 'a' || key == 'A')
    {
        airplane.moving = true;
    }

    if (key == 'p' || key == 'P')
    {
        airplane.moving = false;
    }

    if (key == 'f' || key == 'F')
    {
        airplane.inside = false;
    }

    if (key == 'i' || key == 'I')
    {
        airplane.inside = true;
    }
}

int main(int argc, char **argv)
{
    Model model;

    OBJ obj("Samples/airplane.obj");
    obj.load(model);
    airplane.model = model;

    OBJ obj2("Samples/tower.obj");
    obj2.load(tower);

    OBJ obj3("Samples/building.obj");
    obj3.load(airfield);

    OBJ obj4("Samples/road.obj");
    obj4.load(road);

    glutInit(&argc, argv);                                    //inicializa a glut
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores/profundidade
    glutInitWindowSize(width, height);                        //dimens�es da janela
    //glutInitWindowPosition(200, 200);                         //posicao da janela
    glutCreateWindow("Visualizacao 3D - Exemplo 1"); //cria a janela
    init();
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display); //determina fun��o callback de desenho
    glEnable(GL_CULL_FACE);   //habilita remo��o de superf�cies ocultas
    glutMainLoop();

    glutMainLoop();

    /*for (auto &v : model.geometric_vertices)
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
    }*/

    /*for (auto &face : model.faces)
    {
        printf("f ");
        for (auto &vertex : face.vertices)
        {
            //printf("%llu/%llu/%llu ", vertex.geometric_vertex,
            //       vertex.texture_vertex, vertex.normal_vertex);
            printf("%f/%f/%f/%f\n", model.geometric_vertices[vertex.geometric_vertex].x,
                   model.geometric_vertices[vertex.geometric_vertex].y,
                   model.geometric_vertices[vertex.geometric_vertex].z,
                   model.geometric_vertices[vertex.geometric_vertex].w);
        }
        printf("\n");
    }*/

    return 0;
}
