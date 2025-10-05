/* horus renderer layer [ vulkan ] */
#include <horus/renderer/vulkan/shader.h>
#include <horus/renderer/vulkan/pipeline.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>

#define RENDERER_PIPELINE_DYNAMIC_STATES_COUNT 2

static const VkDynamicState renderer_pipeline_dynamic_state_scissor = VK_DYNAMIC_STATE_SCISSOR;
static const VkDynamicState renderer_pipeline_dynamic_state_viewport = VK_DYNAMIC_STATE_VIEWPORT;

graphics_pipeline_t *graphics_pipeline_create(renderer_t *renderer, shader_module_t *module) {
  graphics_pipeline_t *pipeline = platform_memory_allocate(sizeof(graphics_pipeline_t));

  pipeline->device = renderer->device;
  pipeline->module = module;

  pipeline->dynamic_states = array_create(RENDERER_PIPELINE_DYNAMIC_STATES_COUNT, sizeof(VkDynamicState));

  array_insert(pipeline->dynamic_states, (void *)&renderer_pipeline_dynamic_state_scissor);
  array_insert(pipeline->dynamic_states, (void *)&renderer_pipeline_dynamic_state_viewport);

  VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info = (VkPipelineDynamicStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .dynamicStateCount = pipeline->dynamic_states->count,
      .pDynamicStates = pipeline->dynamic_states->buffer,
  };

  VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_create_info = (VkPipelineVertexInputStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
  };

  VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info =
      (VkPipelineInputAssemblyStateCreateInfo){
          .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
          .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      };

  VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info = (VkPipelineViewportStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .scissorCount = 1,
      .viewportCount = 1,
  };

  VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info =
      (VkPipelineRasterizationStateCreateInfo){
          .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
          .depthClampEnable = VK_FALSE,
          .rasterizerDiscardEnable = VK_FALSE,
          .polygonMode = VK_POLYGON_MODE_FILL,
          .cullMode = VK_CULL_MODE_BACK_BIT,
          .frontFace = VK_FRONT_FACE_CLOCKWISE,
          .depthBiasEnable = VK_FALSE,
          .depthBiasSlopeFactor = 1.0f,
          .lineWidth = 1.0f,
      };

  VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info = (VkPipelineMultisampleStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .sampleShadingEnable = VK_FALSE,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
  };

  VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_state_create_info =
      (VkPipelineDepthStencilStateCreateInfo){
          .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
          .depthTestEnable = VK_FALSE,
          .stencilTestEnable = VK_FALSE,
      };

  VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_state = (VkPipelineColorBlendAttachmentState){
      .blendEnable = VK_FALSE,
      .colorWriteMask =
          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT,
  };

  VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info = (VkPipelineColorBlendStateCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = &pipeline_color_blend_attachment_state,
  };

  VkPipelineLayoutCreateInfo pipeline_layout_create_info = (VkPipelineLayoutCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = 0,
      .pushConstantRangeCount = 0,
  };

  if (vkCreatePipelineLayout(pipeline->device, &pipeline_layout_create_info, NULL, &pipeline->layout) != VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <device:%p> pipeline layout creation failed", renderer, pipeline->device);

    array_destroy(pipeline->dynamic_states);

    platform_memory_deallocate(pipeline);

    return NULL;
  }

  VkPipelineRenderingCreateInfo pipeline_rendering_create_info = (VkPipelineRenderingCreateInfo){
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &renderer->surface_format.format,
  };

  if (!pipeline->module->has_vertex_stage_create_info) {
    logger_critical_format("<renderer:%p> <module:%p> missing vertex shader stages for pipeline failed", renderer,
                           pipeline->module);

    vkDestroyPipelineLayout(pipeline->device, pipeline->layout, NULL);

    array_destroy(pipeline->dynamic_states);

    platform_memory_deallocate(pipeline);

    return NULL;
  }

  if (!pipeline->module->has_vertex_stage_create_info) {
    logger_critical_format("<renderer:%p> <module:%p> missing vertex shader stage for pipeline creation", renderer,
                           pipeline->module);

    vkDestroyPipelineLayout(pipeline->device, pipeline->layout, NULL);

    array_destroy(pipeline->dynamic_states);

    platform_memory_deallocate(pipeline);

    return NULL;
  }

  if (!pipeline->module->has_fragment_stage_create_info) {
    logger_critical_format("<renderer:%p> <module:%p> missing fragment shader stage for pipeline creation", renderer,
                           pipeline->module);

    vkDestroyPipelineLayout(pipeline->device, pipeline->layout, NULL);

    array_destroy(pipeline->dynamic_states);

    platform_memory_deallocate(pipeline);

    return NULL;
  }

  array_t *shader_stage_create_infos = renderer_vulkan_shader_module_stage_create_infos(pipeline->module);

  VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = (VkGraphicsPipelineCreateInfo){
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .pNext = &pipeline_rendering_create_info,
      .stageCount = shader_stage_create_infos->count,
      .pStages = shader_stage_create_infos->buffer,
      .pVertexInputState = &pipeline_vertex_input_state_create_info,
      .pInputAssemblyState = &pipeline_input_assembly_state_create_info,
      .pViewportState = &pipeline_viewport_state_create_info,
      .pRasterizationState = &pipeline_rasterization_state_create_info,
      .pMultisampleState = &pipeline_multisample_state_create_info,
      .pDepthStencilState = &pipeline_depth_stencil_state_create_info,
      .pColorBlendState = &pipeline_color_blend_state_create_info,
      .pDynamicState = &pipeline_dynamic_state_create_info,
      .layout = pipeline->layout,
      .renderPass = NULL,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1,
  };

  if (vkCreateGraphicsPipelines(pipeline->device, NULL, 1, &graphics_pipeline_create_info, NULL, &pipeline->pipeline) !=
      VK_SUCCESS) {
    logger_critical_format("<renderer:%p> <device:%p> pipeline creation failed", renderer, pipeline->device);

    vkDestroyPipelineLayout(pipeline->device, pipeline->layout, NULL);

    array_destroy(shader_stage_create_infos);

    array_destroy(pipeline->dynamic_states);

    platform_memory_deallocate(pipeline);

    return NULL;
  }

  array_destroy(shader_stage_create_infos);

  return pipeline;
}

b8 graphics_pipeline_destroy(graphics_pipeline_t *pipeline) {
  vkDeviceWaitIdle(pipeline->device);

  vkDestroyPipeline(pipeline->device, pipeline->pipeline, NULL);

  vkDestroyPipelineLayout(pipeline->device, pipeline->layout, NULL);

  array_destroy(pipeline->dynamic_states);

  platform_memory_deallocate(pipeline);

  return true;
}

b8 graphics_pipeline_bind(graphics_pipeline_t *pipeline, renderer_t *renderer) {
  VkCommandBuffer graphics_command_buffer;

  array_retrieve(renderer->graphics_command_buffers, renderer->current_frame_in_flight_index, &graphics_command_buffer);

  vkCmdBindPipeline(graphics_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);

  return true;
}
