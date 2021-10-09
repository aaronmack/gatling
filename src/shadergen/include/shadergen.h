#ifndef SHADERGEN_H
#define SHADERGEN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool sgInitialize(const char* shaderPath,
                  const char* mtlxlibPath);

void sgTerminate();

struct SgMaterial* sgCreateMaterialFromMtlx(const char* docStr);

void sgDestroyMaterial(struct SgMaterial* mat);

struct SgMainShaderParams
{
  uint32_t num_threads_x;
  uint32_t num_threads_y;
  uint32_t max_stack_size;
  uint32_t spp;
  uint32_t max_bounces;
  uint32_t rr_bounce_offset;
  float rr_inv_min_term_prob;
};

bool sgGenerateMainShader(const struct SgMainShaderParams* params,
                          uint32_t* spvSize,
                          uint32_t** spv,
                          const char** entryPoint);

#ifdef __cplusplus
}
#endif

#endif
