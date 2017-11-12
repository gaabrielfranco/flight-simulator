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
    float angle = 270.0;
    float position[3] = {0.0, 0.0, 0.0};
    GLfloat facing[3] = {1.0, 0.0, 0.0};
};

Airplane airplane;
Model tower, airfield, road;
int width = 500, height = 500;
GLfloat coords[3] = {0.0, 0.0, 0.0};

void rotate_y(float angle)
{
    angle *= 0.0174533;
    float x = airplane.facing[0];
    float z = airplane.facing[2];
    airplane.facing[0] = x * cos(angle) - z * sin(angle);
    airplane.facing[2] = x * sin(angle) + z * cos(angle);
}

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -1.225, 1.225, 2.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
}

void display(void)
{

    //Movimentação
    if (airplane.moving)
    {
        airplane.position[0] -= 5 * airplane.facing[0];
        airplane.position[2] -= 5 * airplane.facing[2];
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //Mudança entre camera dentro e fora do avião
    if (airplane.inside)
    {
        gluLookAt(
            airplane.position[0] - (airplane.facing[0] * 10),
            airplane.position[1] + 70,
            airplane.position[2] - (airplane.facing[2] * 10),
            airplane.position[0] - (airplane.facing[0] * 300),
            airplane.position[1],
            airplane.position[2] - (airplane.facing[2] * 300),
            0.0,
            1.0,
            0.0);
    }
    else
    {
        gluLookAt(
            airplane.position[0] + (airplane.facing[0] * 300),
            airplane.position[1] + 100,
            airplane.position[2] + (airplane.facing[2] * 300),
            airplane.position[0] - (airplane.facing[0] * 100),
            airplane.position[1] - (airplane.facing[1] * 100),
            airplane.position[2] - (airplane.facing[2] * 100),
            0.0,
            1.0,
            0.0);
    }

    //Desenha "asfalto"
    glPushMatrix();
    glColor4f(0.5, 0.5, 0.5, 0.3);
    glRotatef(30, 0.0, 1.0, 0.0);
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
    glRotatef(90, 0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    for (auto &face : road.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(road.geometric_vertices[vertex.geometric_vertex].x,
                       road.geometric_vertices[vertex.geometric_vertex].y,
                       road.geometric_vertices[vertex.geometric_vertex].z,
                       road.geometric_vertices[vertex.geometric_vertex].w);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha prédio
    glPushMatrix();
    glColor4f(0.0, 1.0, 1.0, 0.8);
    glTranslatef(100, 0, 0);
    glScalef(5.0, 5.0, 5.0);
    glRotatef(45, 0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    for (auto &face : airfield.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(airfield.geometric_vertices[vertex.geometric_vertex].x,
                       airfield.geometric_vertices[vertex.geometric_vertex].y,
                       airfield.geometric_vertices[vertex.geometric_vertex].z,
                       airfield.geometric_vertices[vertex.geometric_vertex].w);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha torre
    glPushMatrix();
    glColor4f(0.0, 1.0, 0.0, 0.8);
    glTranslatef(-1000, 0, -1000);
    glScalef(50.0, 50.0, 50.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : tower.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(tower.geometric_vertices[vertex.geometric_vertex].x,
                       tower.geometric_vertices[vertex.geometric_vertex].y,
                       tower.geometric_vertices[vertex.geometric_vertex].z,
                       tower.geometric_vertices[vertex.geometric_vertex].w);
        }
    }
    glEnd();

    glPopMatrix();

    //Desenha avião
    glPushMatrix();
    glTranslatef(airplane.position[0], airplane.position[1], airplane.position[2]);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
    for (auto &face : airplane.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(airplane.model.geometric_vertices[vertex.geometric_vertex].x,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].y,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].z,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].w);
        }
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }

    if (key == 'x' || key == 'X')
    {
        if (airplane.position[1] < 4000)
            airplane.position[1] += 5.0;
    }

    if (key == 'z' || key == 'Z')
    {
        if (airplane.position[1] > 0)
            airplane.position[1] -= 5.0;
    }

    if (key == 'd' || key == 'D')
    {
        airplane.angle += 1.0;
        rotate_y(-1.0);
    }

    if (key == 'a' || key == 'A')
    {
        airplane.angle -= 1.0;
        rotate_y(1.0);
    }

    if (key == 'w' || key == 'W')
    {
        airplane.moving = true;
    }

    if (key == 's' || key == 'S')
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

    //Load dos modelos
    OBJ obj("Samples/airplane.obj");
    obj.load(model);
    airplane.model = model;

    OBJ obj2("Samples/tower.obj");
    obj2.load(tower);

    OBJ obj3("Samples/building.obj");
    obj3.load(airfield);

    OBJ obj4("Samples/road.obj");
    obj4.load(road);

    //Main do openGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Trabalho de Computação Gráfica");
    init();
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glEnable(GL_CULL_FACE);
    glutMainLoop();

    glutMainLoop();

    return 0;
}
