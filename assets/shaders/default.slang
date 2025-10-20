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
  float4x4 model;
  float4x4 projection;
};

/* global uniform buffer object */
ConstantBuffer<uniform_buffer_t> uniform_buffer_object;

/* vertex shader entrypoint */
[shader("vertex")]
vertex_output_t vertex_entrypoint(vertex_input_t input) {
  /* output structure */
  vertex_output_t output;

  /* vertex position */
  output.position = float4(input.position, 1.0);

  /* rotated vertex position */
  /*
  float angle = uniform_buffer_object.time * 1.0;

  float c = cos(angle);
  float s = sin(angle);

  output.position.x = input.position.x * c - input.position.y * s;
  output.position.y = input.position.x * s + input.position.y * c;
  */

  /* matrices multiplication */
  float4 vertex_position = float4(input.position, 1.0);

  float4 world_position = mul(uniform_buffer_object.model, vertex_position);
  float4 view_position = mul(uniform_buffer_object.view, world_position);

  output.position = mul(uniform_buffer_object.projection, view_position);

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
