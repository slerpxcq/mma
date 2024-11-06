#version 460 core

#define GRID_SIZE 50
#define GRID_SPACING 50
#define GRID_ALPHA 0.75
#define AXIS_LEN 50

uniform mat4 u_viewProjection;

out vec4 vert_color;

void swap(inout float x, inout float y)
{
    float z = y;
    y = x;
    x = z;
}

void main()
{
    int id;
    vec3 pos;
    int swap = 0;

    if (gl_VertexID < 2 * (GRID_SIZE + 1)) {
        // 0-21 -> 0-21
        id = gl_VertexID;
        pos.z = id/2-GRID_SIZE/2;
        if (pos.z==0)
            pos.x = ((id&1)==1 ? 0 : -1) * (GRID_SIZE/2);
        else 
            pos.x = ((id&1)==1 ? 1 : -1) * (GRID_SIZE/2);
        pos.y = 0;
        vert_color = vec4(vec3(1), GRID_ALPHA);
    } else if (gl_VertexID < 4 * (GRID_SIZE + 1)) {
        // 22-43 -> 0-21
        id = gl_VertexID - (2 * (GRID_SIZE + 1));
        pos.x = id/2-GRID_SIZE/2;
        if (pos.x==0)
            pos.z = ((id&1)==1 ? 0 : -1) * (GRID_SIZE/2);
        else 
            pos.z = ((id&1)==1 ? 1 : -1) * (GRID_SIZE/2);
        pos.y = 0;
        vert_color = vec4(vec3(1), GRID_ALPHA);
    } else {
        // 44-49 -> 0-6
        const vec4 colorTable[3] = vec4[3](
            vec4(1,0,0,1),
            vec4(0,1,0,1),
            vec4(0,0,1,1));
        const vec3 posTable[6] = vec3[6](
            vec3(0,0,0),
            vec3(AXIS_LEN,0,0),
            vec3(0,0,0),
            vec3(0,AXIS_LEN,0),
            vec3(0,0,0),
            vec3(0,0,AXIS_LEN));

        id = gl_VertexID - 4 * (GRID_SIZE + 1);
        vert_color = colorTable[id/2];
        pos = posTable[id];
    }

    gl_Position = u_viewProjection * vec4(pos, 1);
}