/* vertex shader input structure */
struct vertex_input_t {
  float3 position; /* vertex position */

  float4 color; /* vertex color */
};

/* vertex shader output structure */
struct vertex_output_t {
  float4 position : SV_Position; /* vertex position */

  float4 color : COLOR0; /* vertex color */
};

/* uniform buffer structure */
struct uniform_buffer_t {
  float time;

  float4x4 view;
  float4x4 projection;
};

/* instance buffer structure */
struct instance_buffer_t {
  float4x4 model;
};

/* global uniform buffer object */
ConstantBuffer<uniform_buffer_t> uniform_buffer_object : register(b0);

/* instance buffer object */
StructuredBuffer<instance_buffer_t> instance_buffer_object : register(t1);

/* vertex shader entrypoint */
[shader("vertex")]
vertex_output_t vertex_entrypoint(vertex_input_t input, uint vertex_id: SV_VertexID, uint instance_id: SV_InstanceID) {
  /* output structure */
  vertex_output_t output;

  /* vertex position */
  float4 vertex_position = float4(input.position, 1.0);

  /* matrices multiplication */
  float4 world_position = mul(instance_buffer_object[instance_id].model, vertex_position);

  float4 camera_position = mul(uniform_buffer_object.view, world_position);

  output.position = mul(uniform_buffer_object.projection, camera_position);

  /* vertex color */
  output.color = input.color;

  return output;
}

/* vertex shader entrypoint */
[shader("fragment")]
float4 fragment_entrypoint(vertex_output_t vertex) : SV_Target {
  /* fragment color */
  float4 color = vertex.color;

  return color;
}
