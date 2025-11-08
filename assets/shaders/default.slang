/* vertex shader input structure */
struct vertex_input_t {
  float3 position; /* vertex position */

  float4 color; /* vertex color */

  float3 normal; /* vertex normal */

  float2 coordinates; /* vertex texture coordinates */
};

/* vertex shader output structure */
struct vertex_output_t {
  float4 position : SV_Position; /* vertex position */

  float4 color : COLOR0; /* vertex color */

  float3 normal : NORMAL0; /* vertex normal */

  float2 coordinates : TEXCOORD0; /* vertex texture coordinates */

  float3 world_position : TEXCOORD1; /* world position */
};

/* uniform buffer structure */
struct uniform_buffer_t {
  float time;

  float4x4 view;
  float4x4 projection;

  float3 camera_position;
};

/* instance buffer structure */
struct instance_buffer_t {
  float4x4 model;
};

/* global uniform buffer object */
ConstantBuffer<uniform_buffer_t> uniform_buffer_object : register(b0);

/* instance buffer object */
StructuredBuffer<instance_buffer_t> instance_buffer_object : register(t1);

/* diffuse texture sampler */
Sampler2D diffuse_texture_sampler : register(t2) : register(s2);

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

  /* vertex normal */
  output.normal = normalize(mul((float3x3)instance_buffer_object[instance_id].model, input.normal));

  /* vertex texture coordinates */
  output.coordinates = input.coordinates;

  /* world position */
  output.world_position = world_position.xyz;

  return output;
}

/* vertex shader entrypoint */
[shader("fragment")]
float4 fragment_entrypoint(vertex_output_t vertex) : SV_Target {
  /* interpolated normal */
  float3 normal = normalize(vertex.normal);

  /* view direction */
  float3 view_direction = normalize(uniform_buffer_object.camera_position - vertex.world_position);

  /* diffuse light factor */
  float diffuse_factor = max(0.0, dot(normal, view_direction));

  /* amdient light strength */
  float ambient_strength = 0.25;

  /* shading factor */
  float shading_factor = clamp(ambient_strength + diffuse_factor, 0.0, 1.0);

  /* diffuse sampled color */
  float4 diffuse_sampled = diffuse_texture_sampler.Sample(vertex.coordinates);

  /* fragment color */
  float4 color = vertex.color * diffuse_sampled * shading_factor;

  return color;
}
