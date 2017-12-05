#include <Model/OBJ.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <ctime>
#include <math.h>
#include <iostream>

using namespace std;

/*
    Data definitions
*/

struct Image
{
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

typedef struct Image Image;

struct Flying_machines
{
    Model model;
    bool moving = false;
    bool inside = false;
    float angle = -90.0;
    float position[3] = {0.0, 2.0, 0.0};
    float facing[3] = {1.0, 0.0, 0.0};
    Image *texture;
    GLuint num_texture;

    void setaDados(Model new_model, float new_angle, float new_positionX, float new_positionY, float new_positionZ)
    {
        model = new_model;
        angle = new_angle;
        position[0] = new_positionX;
        position[1] = new_positionY;
        position[2] = new_positionZ;
    }
};

/*
    Variables
*/

Flying_machines airplane, airplane2, arc170;
Model model_airplane, model_plane2, model_arc170, tower, camel, road, farmhouse, mountain, rock, cacto, gundam;
int width = 1440, height = 900, fm_selected = 1;

double time_last, time_now, delta;

Image *textureImage, *texture_road, *texture_rock;
GLuint num_texture_house[0], num_textureAux2[1], num_texture_road[1], num_texture_rock[1], num_texture_cacto[1],
    num_texture_camel[1], num_texture_gundam[1];

int rock_position[100][2], house_position[30][2], cacto_position[30][2], camel_position[10][2];

GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat sun_light[] = {1.0, 0.3, 0.2};

GLfloat sLightPos1[] = {0.0, 50.0, 0.0, 0.0};

GLfloat sLightPos2Airplane[] = {0.0, 100.0, 0.0, 1.0};
GLfloat sLightDir2Airplane[] = {0.0, -1.0, 0.0};
GLfloat sLightPos2Ballon[] = {0.0, 100.0, 0.0, 1.0};
GLfloat sLightDir2Ballon[] = {0.0, -1.0, 0.0};
GLfloat sLightPos2arc170[] = {0.0, 100.0, 0.0, 1.0};
GLfloat sLightDir2arc170[] = {0.0, -1.0, 0.0};

bool active_light = false, light0 = false, light1 = false;

/*
    Shuffle objects positions
*/

void shuffle(char *caso, int qnt)
{
    if (!strcmp(caso, "rock"))
    {
        for (int i = 0; i < qnt; i++)
        {
            !(qnt % 5) ? rock_position[i][0] = (rand() % 5000) * -1 : rock_position[i][0] = rand() % 5000;
            !(qnt % 4) ? rock_position[i][1] = (rand() % 5000) * -1 : rock_position[i][1] = rand() % 5000;
        }
    }
    else if (!strcmp(caso, "house"))
    {
        for (int i = 0; i < qnt; i++)
        {
            !(qnt % 5) ? house_position[i][0] = (rand() % 5000) * -1 : house_position[i][0] = rand() % 5000;
            !(qnt % 4) ? house_position[i][1] = (rand() % 5000) * -1 : house_position[i][1] = rand() % 5000;
        }
    }
    else if (!strcmp(caso, "cacto"))
    {
        for (int i = 0; i < qnt; i++)
        {
            !(qnt % 5) ? cacto_position[i][0] = (rand() % 5000) * -1 : cacto_position[i][0] = rand() % 5000;
            !(qnt % 4) ? cacto_position[i][1] = (rand() % 5000) * -1 : cacto_position[i][1] = rand() % 5000;
        }
    }
    else if (!strcmp(caso, "camel"))
    {
        for (int i = 0; i < qnt; i++)
        {
            !(qnt % 5) ? camel_position[i][0] = (rand() % 5000) * -1 : camel_position[i][0] = rand() % 5000;
            !(qnt % 4) ? camel_position[i][1] = (rand() % 5000) * -1 : camel_position[i][1] = rand() % 5000;
        }
    }
}

/*
    Load .BMP images
*/

int image_load(char *filename, Image *image)
{
    FILE *file;
    int size;
    unsigned long i;
    unsigned short int planes;
    unsigned short int bpp;

    char temp;

    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("File Not Found : %s\n", filename);
        return 0;
    }

    fseek(file, 18, SEEK_CUR);

    if ((i = fread(&image->sizeX, 4, 1, file)) != 1)
    {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }

    if ((i = fread(&image->sizeY, 4, 1, file)) != 1)
    {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }

    size = image->sizeX * image->sizeY * 3;
    if ((fread(&planes, 2, 1, file)) != 1)
    {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }

    if (planes != 1)
    {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }

    if ((i = fread(&bpp, 2, 1, file)) != 1)
    {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }

    if (bpp != 24)
    {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }

    fseek(file, 24, SEEK_CUR);

    image->data = (char *)malloc(size);
    if (image->data == NULL)
    {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1)
    {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i = 0; i < size; i += 3)
    {
        temp = image->data[i];
        image->data[i] = image->data[i + 2];
        image->data[i + 2] = temp;
    }
    return 1;
}

/*
    Load textures
*/

Image *load_texture(char *filename)
{
    Image *image1;
    // allocate space for texture
    image1 = (Image *)malloc(sizeof(Image));
    if (image1 == NULL)
    {
        printf("Error allocating space for image");
        exit(0);
    }

    if (!image_load(filename, image1))
    {
        exit(1);
    }

    return image1;
}

/*
    Set texture
*/

void set_texture(int caso, char *filename)
{
    switch (caso)
    {
    case 1:
        airplane.texture = load_texture(filename);
        glGenTextures(1, &airplane.num_texture);
        glBindTexture(GL_TEXTURE_2D, airplane.num_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, airplane.texture->sizeX, airplane.texture->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, airplane.texture->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;

    case 2:
        airplane2.texture = load_texture(filename);
        glGenTextures(1, &airplane2.num_texture);
        glBindTexture(GL_TEXTURE_2D, airplane2.num_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, airplane2.texture->sizeX, airplane2.texture->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, airplane2.texture->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;

    case 3:
        arc170.texture = load_texture(filename);
        glGenTextures(1, &arc170.num_texture);
        glBindTexture(GL_TEXTURE_2D, arc170.num_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, arc170.texture->sizeX, arc170.texture->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, arc170.texture->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;
    case 4:
        textureImage = load_texture(filename);
        glGenTextures(1, num_textureAux2);
        glBindTexture(GL_TEXTURE_2D, num_textureAux2[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    case 5:
        textureImage = load_texture((char *)"Samples/house1-texture.bmp");
        glGenTextures(1, num_texture_house);
        glBindTexture(GL_TEXTURE_2D, num_texture_house[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    case 6:
        textureImage = load_texture((char *)"Samples/road-texture.bmp");
        glGenTextures(1, num_texture_road);
        glBindTexture(GL_TEXTURE_2D, num_texture_road[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    case 7:
        textureImage = load_texture((char *)"Samples/rock-texture.bmp");
        glGenTextures(1, num_texture_rock);
        glBindTexture(GL_TEXTURE_2D, num_texture_rock[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    case 8:
        textureImage = load_texture((char *)"Samples/cacto-texture.bmp");
        glGenTextures(1, num_texture_cacto);
        glBindTexture(GL_TEXTURE_2D, num_texture_cacto[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    case 9:
        textureImage = load_texture((char *)"Samples/camel-texture.bmp");
        glGenTextures(1, num_texture_camel);
        glBindTexture(GL_TEXTURE_2D, num_texture_camel[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    case 10:
        textureImage = load_texture((char *)"Samples/gundam-texture.bmp");
        glGenTextures(1, num_texture_gundam);
        glBindTexture(GL_TEXTURE_2D, num_texture_gundam[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 0, 3, textureImage->sizeX, textureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
}

/*
    Rotate function:
        Case 1 => Airplane
        Case 2 => Airplane 2
        Case 3 => Arc170
*/

void rotate_y(float angle)
{
    angle *= 0.0174533;
    switch (fm_selected)
    {
        float x, z;
    case 1:
        x = airplane.facing[0];
        z = airplane.facing[2];
        airplane.facing[0] = x * cos(angle) - z * sin(angle);
        airplane.facing[2] = x * sin(angle) + z * cos(angle);
        break;
    case 2:
        x = airplane2.facing[0];
        z = airplane2.facing[2];
        airplane2.facing[0] = x * cos(angle) - z * sin(angle);
        airplane2.facing[2] = x * sin(angle) + z * cos(angle);
        break;
    case 3:
        x = arc170.facing[0];
        z = arc170.facing[2];
        arc170.facing[0] = x * cos(angle) - z * sin(angle);
        arc170.facing[2] = x * sin(angle) + z * cos(angle);
        break;
    }
}

/*
    Display OPENGL function:
        Case 1 => Airplane
        Case 2 => Airplane 2
        Case 3 => Arc170
*/

void display(void)
{
    time_last = time_now;
    time_now = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    delta = time_now - time_last;

    if (airplane.moving || airplane2.moving || arc170.moving)
    {
        switch (fm_selected)
        {
        case 1:
            airplane.position[0] -= 30 * delta * airplane.facing[0];
            airplane.position[2] -= 30 * delta * airplane.facing[2];
            break;

        case 2:
            airplane2.position[0] -= 30 * delta * airplane2.facing[0];
            airplane2.position[2] -= 30 * delta * airplane2.facing[2];
            break;

        case 3:
            arc170.position[0] -= 30 * delta * arc170.facing[0];
            arc170.position[2] -= 30 * delta * arc170.facing[2];
            break;
        }
    }

    sLightPos2Airplane[0] = airplane.position[0] - (airplane.facing[0] * 10);
    sLightPos2Airplane[1] = airplane.position[1] + 100;
    sLightPos2Airplane[2] = airplane.position[2] - (airplane.facing[2] * 10);
    sLightDir2Airplane[0] = (airplane.position[0] - (airplane.facing[0] * 300)) - sLightPos2Airplane[0];
    sLightDir2Airplane[1] = (airplane.position[1]) - sLightPos2Airplane[1];
    sLightDir2Airplane[2] = (airplane.position[2] - (airplane.facing[2] * 300)) - sLightPos2Airplane[2];

    sLightPos2Ballon[0] = airplane2.position[0] - (airplane2.facing[0] * 10);
    sLightPos2Ballon[1] = airplane2.position[1] + 100;
    sLightPos2Ballon[2] = airplane2.position[2] - (airplane2.facing[2] * 10);
    sLightDir2Ballon[0] = (airplane2.position[0] - (airplane2.facing[0] * 300)) - sLightPos2Ballon[0];
    sLightDir2Ballon[1] = (airplane2.position[1]) - sLightPos2Ballon[1];
    sLightDir2Ballon[2] = (airplane2.position[2] - (airplane2.facing[2] * 300)) - sLightPos2Ballon[2];

    sLightPos2arc170[0] = arc170.position[0] - (arc170.facing[0] * 10);
    sLightPos2arc170[1] = arc170.position[1] + 100;
    sLightPos2arc170[2] = arc170.position[2] - (arc170.facing[2] * 10);
    sLightDir2arc170[0] = (arc170.position[0] - (arc170.facing[0] * 300)) - sLightPos2arc170[0];
    sLightDir2arc170[1] = (arc170.position[1]) - sLightPos2arc170[1];
    sLightDir2arc170[2] = (arc170.position[2] - (arc170.facing[2] * 300)) - sLightPos2arc170[2];

    glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
    glLightfv(GL_LIGHT0, GL_POSITION, sLightPos1);
    glDepthFunc(GL_LEQUAL);

    switch (fm_selected)
    {
    case 1:
        glLightfv(GL_LIGHT1, GL_POSITION, sLightPos2Airplane);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sLightDir2Airplane);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
        glLightfv(GL_LIGHT1, GL_SHININESS, white_light);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 120.0);
        break;

    case 2:
        glLightfv(GL_LIGHT1, GL_POSITION, sLightPos2Ballon);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sLightDir2Ballon);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
        glLightfv(GL_LIGHT1, GL_SHININESS, white_light);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);
        break;

    case 3:
        glLightfv(GL_LIGHT1, GL_POSITION, sLightPos2arc170);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sLightDir2arc170);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
        glLightfv(GL_LIGHT1, GL_SHININESS, white_light);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);
        break;
    }

    if (light0)
    {
        glEnable(GL_LIGHT0);
    }
    else
    {
        glDisable(GL_LIGHT0);
    }

    if (light1)
    {
        glEnable(GL_LIGHT1);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    switch (fm_selected)
    {
    case 1:
        if (airplane.inside)
        {
            gluLookAt(
                airplane.position[0] - (airplane.facing[0] * 10),
                airplane.position[1] + 100,
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
        break;

    case 2:
        if (airplane2.inside)
        {
            gluLookAt(
                airplane2.position[0] - (airplane2.facing[0] * 10),
                airplane2.position[1] + 100,
                airplane2.position[2] - (airplane2.facing[2] * 10),
                airplane2.position[0] - (airplane2.facing[0] * 300),
                airplane2.position[1],
                airplane2.position[2] - (airplane2.facing[2] * 300),
                0.0,
                1.0,
                0.0);
        }
        else
        {
            gluLookAt(
                airplane2.position[0] + (airplane2.facing[0] * 300),
                airplane2.position[1] + 100,
                airplane2.position[2] + (airplane2.facing[2] * 300),
                airplane2.position[0] - (airplane2.facing[0] * 100),
                airplane2.position[1] - (airplane2.facing[1] * 100),
                airplane2.position[2] - (airplane2.facing[2] * 100),
                0.0,
                1.0,
                0.0);
        }
        break;

    case 3:
        if (arc170.inside)
        {
            gluLookAt(
                arc170.position[0] - (arc170.facing[0] * 10),
                arc170.position[1] + 100,
                arc170.position[2] - (arc170.facing[2] * 10),
                arc170.position[0] - (arc170.facing[0] * 300),
                arc170.position[1],
                arc170.position[2] - (arc170.facing[2] * 300),
                0.0,
                1.0,
                0.0);
        }
        else
        {
            gluLookAt(
                arc170.position[0] + (arc170.facing[0] * 300),
                arc170.position[1] + 100,
                arc170.position[2] + (arc170.facing[2] * 300),
                arc170.position[0] - (arc170.facing[0] * 100),
                arc170.position[1] - (arc170.facing[1] * 100),
                arc170.position[2] - (arc170.facing[2] * 100),
                0.0,
                1.0,
                0.0);
        }
        break;
    }

    glPushMatrix();
    glColor4f(0.5, 0.5, 0.5, 0.3);
    glRotatef(30, 0.0, 1.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, num_textureAux2[0]);

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-5000.0, 0.0, -5000.0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-5000.0, 0.0, 5000.0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(5000.0, 0.0, 5000.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(5000.0, 0.0, -5000.0);
    glEnd();
    glPopMatrix();

    for (int i = 150; i < 2000; i += 50)
    {
        glPushMatrix();
        glColor4f(1.0, 0.0, 1.0, 0.8);
        glTranslatef(315 - i, 2, 20);
        glScalef(25.0, 25.0, 25.0);
        glRotatef(90, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_road[0]);

        glBegin(GL_QUADS);
        for (auto &face : road.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(road.texture_vertices[vertex.texture_vertex].u,
                             road.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(road.geometric_vertices[vertex.geometric_vertex].x,
                           road.geometric_vertices[vertex.geometric_vertex].y,
                           road.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(road.normal_vertices[vertex.normal_vertex].i,
                           road.normal_vertices[vertex.normal_vertex].j,
                           road.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    for (int i = 0; i < 2000; i += 50)
    {
        glPushMatrix();
        glColor4f(1.0, 0.0, 1.0, 0.8);
        glTranslatef(315, 2, 20 - i);
        glScalef(25.0, 25.0, 25.0);
        glRotatef(180, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_road[0]);

        glBegin(GL_QUADS);
        for (auto &face : road.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(road.texture_vertices[vertex.texture_vertex].u,
                             road.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(road.geometric_vertices[vertex.geometric_vertex].x,
                           road.geometric_vertices[vertex.geometric_vertex].y,
                           road.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(road.normal_vertices[vertex.normal_vertex].i,
                           road.normal_vertices[vertex.normal_vertex].j,
                           road.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    for (int i = 0; i < 2000; i += 50)
    {
        glPushMatrix();
        glColor4f(1.0, 0.0, 1.0, 0.8);
        glTranslatef(315, 2, 20 + i);
        glScalef(25.0, 25.0, 25.0);
        glRotatef(180, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_road[0]);

        glBegin(GL_QUADS);
        for (auto &face : road.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(road.texture_vertices[vertex.texture_vertex].u,
                             road.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(road.geometric_vertices[vertex.geometric_vertex].x,
                           road.geometric_vertices[vertex.geometric_vertex].y,
                           road.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(road.normal_vertices[vertex.normal_vertex].i,
                           road.normal_vertices[vertex.normal_vertex].j,
                           road.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    for (int i = 0; i < 20; i++)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(house_position[i][0], 2, house_position[i][1]);
        glScalef(5.0, 5.0, 5.0);
        glRotatef(45, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_house[0]);

        glBegin(GL_TRIANGLES);
        for (auto &face : farmhouse.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(farmhouse.texture_vertices[vertex.texture_vertex].u,
                             farmhouse.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(farmhouse.geometric_vertices[vertex.geometric_vertex].x,
                           farmhouse.geometric_vertices[vertex.geometric_vertex].y,
                           farmhouse.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(farmhouse.normal_vertices[vertex.normal_vertex].i,
                           farmhouse.normal_vertices[vertex.normal_vertex].j,
                           farmhouse.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    glPushMatrix();
    glColor4f(0.0, 1.0, 0.0, 0.8);
    glTranslatef(-1000, 2, -1000);
    glScalef(50.0, 50.0, 50.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
    for (auto &face : tower.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glVertex3f(tower.geometric_vertices[vertex.geometric_vertex].x,
                       tower.geometric_vertices[vertex.geometric_vertex].y,
                       tower.geometric_vertices[vertex.geometric_vertex].z);

            glNormal3f(tower.normal_vertices[vertex.normal_vertex].i,
                       tower.normal_vertices[vertex.normal_vertex].j,
                       tower.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.36, 0.25, 0.2);
    glTranslatef(-4000, -500, 4000);
    glScalef(20.0, 20.0, 20.0);
    glRotatef(90, 0.0, 1.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, num_texture_rock[0]);

    glBegin(GL_TRIANGLE_STRIP);
    for (auto &face : mountain.faces)
    {
        for (auto &vertex : face.vertices)
        {

            glTexCoord2f(mountain.texture_vertices[vertex.texture_vertex].u,
                         mountain.texture_vertices[vertex.texture_vertex].v);

            glVertex3f(mountain.geometric_vertices[vertex.geometric_vertex].x,
                       mountain.geometric_vertices[vertex.geometric_vertex].y,
                       mountain.geometric_vertices[vertex.geometric_vertex].z);
            glNormal3f(mountain.normal_vertices[vertex.normal_vertex].i,
                       mountain.normal_vertices[vertex.normal_vertex].j,
                       mountain.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.36, 0.25, 0.2);
    glTranslatef(-3000, 0, 0);
    glScalef(4, 4, 4);
    glRotatef(-90, 1.0, 0.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, num_texture_gundam[0]);

    glBegin(GL_TRIANGLES);
    for (auto &face : gundam.faces)
    {
        for (auto &vertex : face.vertices)
        {

            glTexCoord2f(gundam.texture_vertices[vertex.texture_vertex].u,
                         gundam.texture_vertices[vertex.texture_vertex].v);

            glVertex3f(gundam.geometric_vertices[vertex.geometric_vertex].x,
                       gundam.geometric_vertices[vertex.geometric_vertex].y,
                       gundam.geometric_vertices[vertex.geometric_vertex].z);

            glNormal3f(gundam.normal_vertices[vertex.normal_vertex].i,
                       gundam.normal_vertices[vertex.normal_vertex].j,
                       gundam.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(rock_position[i][0], 2, rock_position[i][1]);
        glScalef(20.0, 20.0, 20.0);
        glRotatef(45, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_rock[0]);

        glBegin(GL_TRIANGLE_STRIP);
        for (auto &face : rock.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(rock.texture_vertices[vertex.texture_vertex].u,
                             rock.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(rock.geometric_vertices[vertex.geometric_vertex].x,
                           rock.geometric_vertices[vertex.geometric_vertex].y,
                           rock.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(rock.normal_vertices[vertex.normal_vertex].i,
                           rock.normal_vertices[vertex.normal_vertex].j,
                           rock.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(camel_position[i][0], 2, camel_position[i][1]);
        glScalef(20.0, 20.0, 20.0);
        glRotatef(45, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_camel[0]);

        glBegin(GL_TRIANGLES);
        for (auto &face : camel.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(camel.texture_vertices[vertex.texture_vertex].u,
                             camel.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(camel.geometric_vertices[vertex.geometric_vertex].x,
                           camel.geometric_vertices[vertex.geometric_vertex].y,
                           camel.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(camel.normal_vertices[vertex.normal_vertex].i,
                           camel.normal_vertices[vertex.normal_vertex].j,
                           camel.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    for (int i = 0; i < 30; i++)
    {
        glPushMatrix();
        glColor4f(0.0, 1.0, 1.0, 0.8);
        glTranslatef(cacto_position[i][0], 2, cacto_position[i][1]);
        glScalef(10.0, 10.0, 10.0);
        glRotatef(45, 0.0, 1.0, 0.0);

        glBindTexture(GL_TEXTURE_2D, num_texture_cacto[0]);

        glBegin(GL_TRIANGLE_STRIP);
        for (auto &face : cacto.faces)
        {
            for (auto &vertex : face.vertices)
            {

                glTexCoord2f(cacto.texture_vertices[vertex.texture_vertex].u,
                             cacto.texture_vertices[vertex.texture_vertex].v);

                glVertex3f(cacto.geometric_vertices[vertex.geometric_vertex].x,
                           cacto.geometric_vertices[vertex.geometric_vertex].y,
                           cacto.geometric_vertices[vertex.geometric_vertex].z);

                glNormal3f(cacto.normal_vertices[vertex.normal_vertex].i,
                           cacto.normal_vertices[vertex.normal_vertex].j,
                           cacto.normal_vertices[vertex.normal_vertex].k);
            }
        }
        glEnd();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(airplane2.position[0], airplane2.position[1], airplane2.position[2]);
    glRotatef(airplane2.angle, 0.0, 1.0, 0.0);
    glScalef(0.2, 0.2, 0.2);
    glColor3f(1, 1, 1);

    glBindTexture(GL_TEXTURE_2D, airplane2.num_texture);

    glBegin(GL_TRIANGLES);
    for (auto &face : airplane2.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glTexCoord2f(airplane2.model.texture_vertices[vertex.texture_vertex].u,
                         airplane2.model.texture_vertices[vertex.texture_vertex].v);

            glVertex3f(airplane2.model.geometric_vertices[vertex.geometric_vertex].x,
                       airplane2.model.geometric_vertices[vertex.geometric_vertex].y,
                       airplane2.model.geometric_vertices[vertex.geometric_vertex].z);

            glNormal3f(airplane2.model.normal_vertices[vertex.normal_vertex].i,
                       airplane2.model.normal_vertices[vertex.normal_vertex].j,
                       airplane2.model.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(arc170.position[0], arc170.position[1], arc170.position[2]);
    glScalef(0.2, 0.2, 0.2);
    glRotatef(arc170.angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);

    glBindTexture(GL_TEXTURE_2D, arc170.num_texture);

    glBegin(GL_TRIANGLES);
    for (auto &face : arc170.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glTexCoord2f(arc170.model.texture_vertices[vertex.texture_vertex].u,
                         arc170.model.texture_vertices[vertex.texture_vertex].v);

            glVertex3f(arc170.model.geometric_vertices[vertex.geometric_vertex].x,
                       arc170.model.geometric_vertices[vertex.geometric_vertex].y,
                       arc170.model.geometric_vertices[vertex.geometric_vertex].z);

            glNormal3f(arc170.model.normal_vertices[vertex.normal_vertex].i,
                       arc170.model.normal_vertices[vertex.normal_vertex].j,
                       arc170.model.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(airplane.position[0], airplane.position[1], airplane.position[2]);
    glRotatef(airplane.angle, 0.0, 1.0, 0.0);
    glScalef(3, 3, 3);

    glColor3f(1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, airplane.num_texture);

    glBegin(GL_TRIANGLES);
    for (auto &face : airplane.model.faces)
    {
        for (auto &vertex : face.vertices)
        {
            glTexCoord2f(airplane.model.texture_vertices[vertex.texture_vertex].u,
                         airplane.model.texture_vertices[vertex.texture_vertex].v);

            glVertex3f(airplane.model.geometric_vertices[vertex.geometric_vertex].x,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].y,
                       airplane.model.geometric_vertices[vertex.geometric_vertex].z);

            glNormal3f(airplane.model.normal_vertices[vertex.normal_vertex].i,
                       airplane.model.normal_vertices[vertex.normal_vertex].j,
                       airplane.model.normal_vertices[vertex.normal_vertex].k);
        }
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();
}

/*
    Keyboard OPENGL function:
        Case 1 => Airplane
        Case 2 => Airplane 2
        Case 3 => Arc170
*/

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }

    if (key == 'x' || key == 'X')
    {
        switch (fm_selected)
        {
        case 1:
            if (airplane.position[1] < 4000)
                airplane.position[1] += 5.0;
            break;
        case 2:
            if (airplane2.position[1] < 4000)
                airplane2.position[1] += 5.0;
            break;
        case 3:
            if (arc170.position[1] < 4000)
                arc170.position[1] += 5.0;
            break;
        }
    }

    if (key == 'z' || key == 'Z')
    {

        switch (fm_selected)
        {
        case 1:
            if (airplane.position[1] > 2.0)
                airplane.position[1] -= 5.0;
            break;
        case 2:
            if (airplane2.position[1] > 2.0)
                airplane2.position[1] -= 5.0;
            break;
        case 3:
            if (arc170.position[1] > 2.0)
                arc170.position[1] -= 5.0;
            break;
        }
    }

    if (key == 'a' || key == 'A')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.angle += 1.0;
            rotate_y(-1.0);
            break;
        case 2:
            airplane2.angle += 1.0;
            rotate_y(-1.0);
            break;
        case 3:
            arc170.angle += 1.0;
            rotate_y(-1.0);
            break;
        }
    }

    if (key == 'd' || key == 'D')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.angle -= 1.0;
            rotate_y(1.0);
            break;
        case 2:
            airplane2.angle -= 1.0;
            rotate_y(1.0);
            break;
        case 3:
            arc170.angle -= 1.0;
            rotate_y(1.0);
            break;
        }
    }

    if (key == 'w' || key == 'W')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.moving = true;
            break;
        case 2:
            airplane2.moving = true;
            break;
        case 3:
            arc170.moving = true;
            break;
        }
    }

    if (key == 'e' || key == 'E')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.moving = false;
            break;
        case 2:
            airplane2.moving = false;
            break;
        case 3:
            arc170.moving = false;
            break;
        }
    }

    if (key == 'f' || key == 'F')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.inside = false;
            break;
        case 2:
            airplane2.inside = false;
            break;
        case 3:
            arc170.inside = false;
            break;
        }
    }

    if (key == 'i' || key == 'I')
    {
        switch (fm_selected)
        {
        case 1:
            airplane.inside = true;
            break;
        case 2:
            airplane2.inside = true;
            break;
        case 3:
            arc170.inside = true;
            break;
        }
    }

    if (key == 'l' || key == 'L')
    {
        if (!active_light)
        {
            glEnable(GL_LIGHTING);
            active_light = true;
        }
        else
        {
            glDisable(GL_LIGHTING);
            active_light = false;
        }
    }

    if (key == '4')
    {
        light0 = !light0;
    }

    if (key == '5')
    {
        light1 = !light1;
    }

    if (key == 's' || key == 'S')
    {
        glShadeModel(GL_FLAT);
    }

    if (key == 'g' || key == 'G')
    {
        glShadeModel(GL_SMOOTH);
    }

    if (key == '1')
    {
        fm_selected = 1;
        airplane2.moving = false;
        arc170.moving = false;
    }

    if (key == '2')
    {
        fm_selected = 2;
        airplane.moving = false;
        arc170.moving = false;
    }

    if (key == '3')
    {
        fm_selected = 3;
        airplane.moving = false;
        airplane2.moving = false;
    }
}

/*
    Init OPENGL function
*/

void myinit(void)
{
    glClearColor(0, 0, 0.1, 0.1);
    glEnable(GL_DEPTH_TEST);
    set_texture(1, (char *)"Samples/plane-texture.bmp");
    set_texture(2, (char *)"Samples/plane2-texture.bmp");
    set_texture(3, (char *)"Samples/Arc170_blinn1.bmp");
    set_texture(4, (char *)"Samples/chao.bmp");
    set_texture(5, (char *)"Samples/house1-texture.bmp");
    set_texture(6, (char *)"Samples/road-texture.bmp");
    set_texture(7, (char *)"Samples/rock-texture.bmp");
    set_texture(8, (char *)"Samples/cacto-texture.bmp");
    set_texture(9, (char *)"Samples/camel-texture.bmp");
    set_texture(10, (char *)"Samples/gundam-texture.bmp");
}

void load_models()
{
    OBJ obj("Samples/plane.obj");
    obj.load(model_airplane);
    airplane.model = model_airplane;

    OBJ obj2("Samples/tower.obj");
    obj2.load(tower);

    OBJ obj3("Samples/camel.obj");
    obj3.load(camel);
    shuffle((char *)"camel", 10);

    OBJ obj4("Samples/road.obj");
    obj4.load(road);

    OBJ obj5("Samples/house.obj");
    obj5.load(farmhouse);
    shuffle((char *)"house", 20);

    OBJ obj6("Samples/mountain.obj");
    obj6.load(mountain);

    OBJ obj7("Samples/plane2.obj");
    obj7.load(model_plane2);
    airplane2.setaDados(model_plane2, -90, 0.0, 5.0, 500.0);

    OBJ obj8("Samples/Arc170.obj");
    obj8.load(model_arc170);
    arc170.setaDados(model_arc170, -90, 500.0, 2.0, 500.0);

    OBJ obj9("Samples/rock.obj");
    obj9.load(rock);
    shuffle((char *)"rock", 100);

    OBJ obj10("Samples/cacto.obj");
    obj10.load(cacto);
    shuffle((char *)"cacto", 30);

    OBJ obj11("Samples/gundam.obj");
    obj11.load(gundam);
}

/*
    Main
*/

int main(int argc, char **argv)
{
    srand(time(NULL));

    load_models();
    time_now = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    /*
        OpenGL Main Loop
    */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);

    glutCreateWindow("Flight Simulator v1.0");
    myinit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -1.225, 1.225, 2.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);

    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glutMainLoop();

    glutMainLoop();

    return 0;
}
