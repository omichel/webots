#ifndef WR_GL_STATE_H
#define WR_GL_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

bool wr_gl_state_is_initialized();

void wr_gl_state_set_context_active(bool active);

const char *wr_gl_state_get_vendor();
const char *wr_gl_state_get_renderer();
const char *wr_gl_state_get_version();
const char *wr_gl_state_get_glsl_version();

int wr_gl_state_get_gpu_memory();

bool wr_gl_state_is_anisotropic_texture_filtering_supported();
float wr_gl_state_max_texture_anisotropy();

void wr_gl_state_disable_check_error();

#ifdef __cplusplus
}
#endif

#endif  // WR_GL_STATE_H
