
/** HEADER */
/** THIS MUST BE INCLUDED TO SUPPORT TEXTURING **/

/** TEXTURE DEFINITIONS **/
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_MIRRORED_REPEAT                0x8370

#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703

#define GL_R8                             0x8229
#define GL_RG8                            0x822B
#define GL_RGB8                           0x8051
#define GL_RGBA8                          0x8058
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGB565                         0x8D62

typedef global unsigned char* image_t;

typedef struct {
    ushort s, t, min_filter, mag_filter;
} texture_wraps_t;

typedef struct __attribute__((packed)) {
  uint width, height;
  uint internalformat;
  texture_wraps_t wraps;
} sampler2D_t;

kernel void main_vs (
  global const float4 *position,
  global const float4 *in_texCoord,
  global float4 *out_texCoord,
  global float4 *gl_Position
) {
  int gid = get_global_id(0);

  gl_Position[gid] = position[gid];
  out_texCoord[gid] = in_texCoord[gid];
}

kernel void main_fs (
  global const float4 *out_texCoord,
  global float4 *gl_FragColor,
  sampler2D_t ourTexture,
  image_t image
)
{
  int gid = get_global_id(0);

    float2 coord = out_texCoord[gid].xy;

    coord.x = (coord.x - floor(coord.x));
    coord.y = (coord.y - floor(coord.y));

    uint width   = ourTexture.width * coord.x;
    uint height  = ourTexture.height * coord.y;

    global uchar* color = image + (height*ourTexture.width + width)*4;
    gl_FragColor[gid] = (float4) ((float)*color / 255, (float)*(color+1) / 255, (float)*(color+2) / 255, (float)*(color+3) / 255);
}
