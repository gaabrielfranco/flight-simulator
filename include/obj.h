#ifndef OBJ_H
#define OBJ_H

#include <vector>
#include <string>
#include <gm.h>

using namespace std;
using namespace gm;

namespace OBJ {

    struct index {
        int v = 0;
        int vt = 0;
        int vn = 0;
    };

    struct face {
        vector<index> data;
    };

    typedef struct {

        vector<vec3> v;
        vector<vec3> vt;
        vector<vec3> vn;
        vector<face> f;

    } OBJ;

    OBJ * load(const char *filename);

}

#endif
