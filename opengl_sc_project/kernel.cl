float4 mul(float16 mat, float4 vec) {
  float4 result = 0;

  for(int i=0; i<16; ++i) {
    result[i%4] += mat[i]*vec[i/4]; 
  }

  return result;
}

kernel void main_vs (
  constant float4 *color,
  constant float16 *modelMatrix,
  global const float4 *vertex,
  global float4 *out_color,
  global float4 *gl_Position
) {
  int gid = get_global_id(0);

  gl_Position[gid] = mul(*modelMatrix, vertex[gid]);
  out_color[gid] = *color;
}

kernel void main_fs (
  global const float4 *out_color,
  global float4 *gl_FragColor
)
{
  int gid = get_global_id(0);
  
  gl_FragColor[gid] = out_color[gid];
}
