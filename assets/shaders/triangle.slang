static float2 positions[3] = float2[](float2(0.0, -0.5), float2(0.5, 0.5), float2(-0.5, 0.5));

static float3 colors[3] = float3[](float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0));

struct vertex_output_t {
  float4 position : SV_Position;

  float3 color;
};

[shader("vertex")]
vertex_output_t vertex_entrypoint(uint vertex_id: SV_VertexID) {
  vertex_output_t output;

  output.position = float4(positions[vertex_id], 0.0, 1.0);

  output.color = colors[vertex_id];

  return output;
}

[shader("fragment")]
float4 fragment_entrypoint(vertex_output_t vertex) : SV_Target {
  float3 color = vertex.color;

  return float4(color, 1.0);
}
