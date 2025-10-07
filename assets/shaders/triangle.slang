/* triangle vertices positions */
static float3 positions[3] = float3[](float3(0.0, -0.5, 0.0), /* middle top   */
                                      float3(0.5, 0.5, 0.0),  /* bottom right */
                                      float3(-0.5, 0.5, 0.0)  /* bottom left  */
);

/* triangle vertices colors */
static float4 colors[3] = float4[](float4(1.0, 0.0, 0.0, 1.0), /* red   [ middle top   ] */
                                   float4(0.0, 1.0, 0.0, 1.0), /* green [ bottom right ] */
                                   float4(0.0, 0.0, 1.0, 1.0)  /* blue  [ bottom left  ] */
);

/* vertex shader output structure */
struct vertex_output_t {
  float4 position : SV_Position; /* vertex position */

  float4 color;
};

/* vertex shader entrypoint */
[shader("vertex")]
vertex_output_t vertex_entrypoint(uint vertex_id: SV_VertexID) {
  /* output structure */
  vertex_output_t output;

  /* vertex position */
  output.position = float4(positions[vertex_id], 1.0);

  /* vertex color */
  output.color = colors[vertex_id];

  return output;
}

/* vertex shader entrypoint */
[shader("fragment")]
float4 fragment_entrypoint(vertex_output_t vertex) : SV_Target {
  /* fragment color */
  float4 color = vertex.color;

  return color;
}
