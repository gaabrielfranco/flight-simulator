#ifndef GM_H
#define GM_H

#include <GL/gl.h>
#include <math.h>

namespace gm {

    struct vec3 {

        GLfloat coords[3];

        vec3(float v[3]) {
            coords[0] = v[0];
            coords[1] = v[1];
            coords[2] = v[2];
        }

        vec3() {
            coords[0] = 0.0;
            coords[1] = 0.0;
            coords[2] = 0.0;
        }

        void rotate_x(float angle) {
            angle *= 0.0174533;
            float y = coords[1], z = coords[2];
            coords[1] = y * cos(angle) - z * sin(angle);
            coords[2] = y * sin(angle) + z * sin(angle);
        }

        void rotate_y(float angle) {
            angle *= 0.0174533;
            float x = coords[0], z = coords[2];
            coords[0] = x * cos(angle) - z * sin(angle);
            coords[2] = x * sin(angle) + z * cos(angle);
        }

        void rotate_z(float angle) {
            angle *= 0.0174533;
            float x = coords[0], y = coords[1];
            coords[0] = x * cos(angle) + y * sin(angle);
            coords[1] = - x * sin(angle) + y * sin(angle);
        }

    };

}

#endif
