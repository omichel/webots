#ifndef WR_FRAME_BUFFER_H
#define WR_FRAME_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

struct WrFrameBuffer;
typedef struct WrFrameBuffer WrFrameBuffer;

struct WrTextureRtt;
typedef struct WrTextureRtt WrTextureRtt;

WrFrameBuffer *wr_frame_buffer_new();
void wr_frame_buffer_delete(WrFrameBuffer *frame_buffer);

void wr_frame_buffer_append_output_texture(WrFrameBuffer *frame_buffer, WrTextureRtt *texture);
void wr_frame_buffer_append_output_texture_disable(WrFrameBuffer *frame_buffer, WrTextureRtt *texture);
void wr_frame_buffer_set_depth_texture(WrFrameBuffer *frame_buffer, WrTextureRtt *texture);
void wr_frame_buffer_enable_depth_buffer(WrFrameBuffer *frame_buffer, bool enable);
/* OpenGL context must be active when calling this function */
void wr_frame_buffer_enable_copying(WrFrameBuffer *frame_buffer, int index, bool enable);
void wr_frame_buffer_set_size(WrFrameBuffer *frame_buffer, int width, int height);
WrTextureRtt *wr_frame_buffer_get_output_texture(WrFrameBuffer *frame_buffer, int index);
WrTextureRtt *wr_frame_buffer_get_depth_texture(WrFrameBuffer *frame_buffer);

void wr_frame_buffer_setup(WrFrameBuffer *frame_buffer);

void wr_frame_buffer_blit_to_screen(WrFrameBuffer *frame_buffer);

/* The data written to 'data' will be in the format of output texture number 'index' */
void wr_frame_buffer_copy_contents(WrFrameBuffer *frame_buffer, int index, void *data);
void wr_frame_buffer_copy_pixel(WrFrameBuffer *frame_buffer, int index, int x, int y, void *data, bool flip_y);
void wr_frame_buffer_copy_depth_pixel(WrFrameBuffer *frame_buffer, int x, int y, void *data, bool flip_y);

#ifdef __cplusplus
}
#endif

#endif  // WR_FRAME_BUFFER_H
