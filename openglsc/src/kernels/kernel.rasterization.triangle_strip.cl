#define GL_CW       0x0900
#define GL_CCW      0x0901

#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_FRONT_AND_BACK                 0x0408

inline float cross2d(float2 a, float2 b) {
    return a.x * b.y - a.y * b.x;
}

float3 get_baricentric_coords(float2 p, float4 v0, float4 v1, float4 v2) {
    float3 barycentricCoords;

    float areaABC = cross2d(v2.xy - v0.xy, v1.xy - v0.xy);
    float areaPBC = cross2d(v2.xy - p, v1.xy - p);
    float areaPCA = cross2d(v0.xy - p, v2.xy - p);

    barycentricCoords.x = areaPBC / areaABC;
    barycentricCoords.y = areaPCA / areaABC;
    barycentricCoords.z = 1.f-barycentricCoords.x-barycentricCoords.y; // What is this??

    return barycentricCoords;
}

kernel void gl_rasterization_triangle_strip (
    const int index, // gl_Index 
    const int width, // 
    const int attributes, //
    global float4 *gl_Positions,
    global float4 *gl_FragCoords,
    global bool *gl_Discard,
    global float4 *gl_Primitives,
    global float4 *gl_Rasterization,
    global bool* facing, // 0: front, 1: back
    const ushort front_face,
    const ushort culling
)
{
    int gid = get_global_id(0);
    int gsize = get_global_size(0); // number of fragments
    // input values
    global float4 *position = gl_Positions + index;
    global float4 *primitives = gl_Primitives + index;
    global float4 *fragCoord = gl_FragCoords + gid;
    global float4 *rasterization = gl_Rasterization + gid;

    // TODO: Checkout order.
    // base info
    float xf = (gid % width) + 0.0f; // center of the fragment
    float yf = (gid / width) + 0.5f; // center of the fragment

    float4 v0, v1, v2;

    if (index % 2) {
        v0 = position[2];
        v1 = position[1];
        v2 = position[0];
    } else {
        v0 = position[0];
        v1 = position[1];
        v2 = position[2];
    }
    
    // area
    float area = 0.5f * (v0.x*v1.y - v1.x*v0.y + v1.x*v2.y - v2.x*v1.y + v2.x*v0.y - v0.x*v2.y);

    if (front_face == GL_CCW) area = -area;

    facing[gid] = area < 0.f ? 1 : 0;

    if (area == 0.f || culling == GL_FRONT_AND_BACK || (culling == GL_BACK && area < 0.f) || (culling == GL_FRONT && area > 0.f)) {
        gl_Discard[gid] = true;
        return;
    }

    // barycenter
    float3 abc = get_baricentric_coords((float2) (xf,yf), v0, v1, v2);
    
    if ((abc.x < 0.0f) || (abc.y < 0.0f) || (abc.z < 0.0f) ||
        (abc.x > 1.0f) || (abc.y > 1.0f) || (abc.z > 1.0f) ||
        abc.x+abc.y+abc.z > 1) {
        gl_Discard[gid] = true;
        return;
    }

    fragCoord->x = xf;
    fragCoord->y = yf;
    fragCoord->z = abc.x*v0.z + abc.y*v1.z + abc.z*v2.z;
    // fragCoord->w = abc.x*v0.w + abc.y*v1.w + abc.z*v2.w; // maybe this is not required

    global float4 *p0, *p1, *p2;
    for(int attribute = 0 ; attribute < attributes; attribute++) {
        if (index % 2) {
            p2 = primitives + gsize*attribute;
            p1 = p2 + 1;
            p0 = p2 + 2;
        } else {
            p0 = primitives + gsize*attribute;
            p1 = p0 + 1;
            p2 = p0 + 2;
        }
        
        // HW optimization ?? 
        rasterization[gsize*attribute].x = (abc.x*p0->x/v0.w + abc.y*p1->x/v1.w + abc.z*p2->x/v2.w) / (abc.x/v0.w + abc.y/v1.w + abc.z/v2.w);
        rasterization[gsize*attribute].y = (abc.x*p0->y/v0.w + abc.y*p1->y/v1.w + abc.z*p2->y/v2.w) / (abc.x/v0.w + abc.y/v1.w + abc.z/v2.w);
        rasterization[gsize*attribute].z = (abc.x*p0->z/v0.w + abc.y*p1->z/v1.w + abc.z*p2->z/v2.w) / (abc.x/v0.w + abc.y/v1.w + abc.z/v2.w);
        rasterization[gsize*attribute].w = (abc.x*p0->w/v0.w + abc.y*p1->w/v1.w + abc.z*p2->w/v2.w) / (abc.x/v0.w + abc.y/v1.w + abc.z/v2.w);
    }

    // Instruction optimization ?? gl_Discard[gid] = abc.x*abc.y*abc.z; // maybe if abc wasn't float could go faster
    gl_Discard[gid] = false;
}
