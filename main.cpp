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
    float position[3] = {0.0, -95.0, 0.0};
    GLfloat facing[3] = {1.0, 0.0, 0.0};
};

Airplane airplane;
Airplane air_balloon;
Model tower, building, road, farmhouse, mountain;
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
    glVertex3f(-10000.0, 0.0, -10000.0);
    glVertex3f(-10000.0, 0.0, 10000.0);
    glVertex3f(10000.0, 0.0, 10000.0);
    glVertex3f(10000.0, 0.0, -10000.0);
    glEnd();
    glPopMatrix();

    
    //Desenha estrada
    for(int i = 0; i < 500; i+=50)
    {
        glPushMatrix();
        glColor4f(1.0, 0.0, 1.0, 0.8);
        glTranslatef(315-i, 0, 20);
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
    }

    //Desenha prédio
    for (int i = 150; i < 2000; i+=150)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(100, 0, 0+i);
        glScalef(5.0, 5.0, 5.0);
        glRotatef(45, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        for (auto &face : building.faces)
        {
            for (auto &vertex : face.vertices)
            {
                glVertex4f(building.geometric_vertices[vertex.geometric_vertex].x,
                        building.geometric_vertices[vertex.geometric_vertex].y,
                        building.geometric_vertices[vertex.geometric_vertex].z,
                        building.geometric_vertices[vertex.geometric_vertex].w);
            }
        }
        glEnd();
        glPopMatrix();
    }

    //Desenha casa
    for (int i = 500; i < 5000; i+=500)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(100+i, 0, 100+i);
        glScalef(5.0, 5.0, 5.0);
        glRotatef(45, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        for (auto &face : farmhouse.faces)
        {
            for (auto &vertex : face.vertices)
            {
                glVertex4f(farmhouse.geometric_vertices[vertex.geometric_vertex].x,
                            farmhouse.geometric_vertices[vertex.geometric_vertex].y,
                            farmhouse.geometric_vertices[vertex.geometric_vertex].z,
                            farmhouse.geometric_vertices[vertex.geometric_vertex].w);
            }
        }
        glEnd();
        glPopMatrix();
    }

    
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

    //Desenha montanha
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 0.8);
    glTranslatef(-1500, -50, -1500);
    glScalef(5.0, 5.0, 5.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : mountain.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(mountain.geometric_vertices[vertex.geometric_vertex].x,
                       mountain.geometric_vertices[vertex.geometric_vertex].y,
                       mountain.geometric_vertices[vertex.geometric_vertex].z,
                       mountain.geometric_vertices[vertex.geometric_vertex].w);
        }
    }
    glEnd();
    glPopMatrix();

    //Desenha balão
    glPushMatrix();
    glTranslatef(air_balloon.position[0], air_balloon.position[1], air_balloon.position[2]);
    glScalef(1.0, 1.0, 1.0);
    glRotatef(air_balloon.angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
    for (auto &face : air_balloon.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex4f(air_balloon.model.geometric_vertices[vertex.geometric_vertex].x,
                       air_balloon.model.geometric_vertices[vertex.geometric_vertex].y,
                       air_balloon.model.geometric_vertices[vertex.geometric_vertex].z,
                       air_balloon.model.geometric_vertices[vertex.geometric_vertex].w);
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
        if (airplane.position[1] > -95.0)
            airplane.position[1] -= 5.0;
    }

    if (key == 'a' || key == 'A')
    {
        airplane.angle += 1.0;
        rotate_y(-1.0);
    }

    if (key == 'd' || key == 'D')
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

    if (key == '1')
    {
        printf("apertei 1\n");
    }

    if (key == '2')
    {
        printf("apertei 2\n");
    }

    if (key == '3')
    {
        printf("apertei 3\n");
    }
}

int main(int argc, char **argv)
{
    Model model, model2;

    //Load dos modelos
    OBJ obj("Samples/airplane.obj");
    obj.load(model);
    airplane.model = model;

    OBJ obj2("Samples/tower.obj");
    obj2.load(tower);

    OBJ obj3("Samples/building.obj");
    obj3.load(building);

    OBJ obj4("Samples/road.obj");
    obj4.load(road);
    
    OBJ obj5("Samples/farmhouse.obj");
    obj5.load(farmhouse);

    OBJ obj6("Samples/mountain.obj");
    obj6.load(mountain);

    OBJ obj7("Samples/air_balloon.obj");
    obj7.load(model2);
    air_balloon.model = model2;
    air_balloon.angle = 90.0;
    air_balloon.position[0] = 250.0;
    air_balloon.position[1] = 5.0;
    air_balloon.position[2] = 50.0;

    //Main do openGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Trabalho de CG");
    init();
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glEnable(GL_CULL_FACE);
    glutMainLoop();

    glutMainLoop();

    return 0;
}
