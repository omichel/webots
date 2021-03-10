#include <glad/egl.h>
#include <glad/gl.h>
#include <iostream>

static const EGLint configAttribs[] = {
  EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 8,
  EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,  EGL_NONE};

static const int pbufferWidth = 9;
static const int pbufferHeight = 9;

static const EGLint pbufferAttribs[] = {
  EGL_WIDTH, pbufferWidth, EGL_HEIGHT, pbufferHeight, EGL_NONE,
};

EGLDisplay get_display() {
  EGLDeviceEXT egl_devices[32];
  EGLint num_devices = 0;
  eglQueryDevicesEXT(32, egl_devices, &num_devices);
  for (EGLint i = 0; i < num_devices; ++i) {
    // Set display
    EGLDisplay display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, egl_devices[i], NULL);
    if (eglGetError() == EGL_SUCCESS && display != EGL_NO_DISPLAY) {
      int major, minor;
      EGLBoolean initialized = eglInitialize(display, &major, &minor);
      if (eglGetError() == EGL_SUCCESS && initialized == EGL_TRUE) {
        return display;
      }
    }
  }
  return EGL_NO_DISPLAY;
}

int main(int argc, char **argv) {
  // 1. Initialize EGL
  int egl_version = gladLoaderLoadEGL(NULL);

  EGLint major, minor;
  EGLDisplay eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  EGLBoolean initialized = eglInitialize(eglDpy, &major, &minor);

  if (!initialized) {
    eglDpy = get_display();
  }

  // 2. Select an appropriate configuration
  EGLint numConfigs;
  EGLConfig eglCfg;

  eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs);

  // 3. Create a surface
  EGLSurface eglSurf = eglCreatePbufferSurface(eglDpy, eglCfg, pbufferAttribs);

  // 4. Bind the API
  egl_version = gladLoaderLoadEGL(eglDpy);
  eglBindAPI(EGL_OPENGL_API);

  // 5. Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT, NULL);

  eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);

  if (!gladLoadGL(eglGetProcAddress)) {
    fprintf(stderr, "failed to load GL with glad.\n");
    exit(EXIT_FAILURE);
  }

  const GLubyte *ven = glGetString(GL_VENDOR);
  printf("GL_VENDOR=%s\n", ven);
  const GLubyte *ver = glGetString(GL_VERSION);
  printf("GL_VERSION=%s\n", ver);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  printf("GL_RENDERER=%s\n", renderer);
  const GLubyte *shading = glGetString(GL_SHADING_LANGUAGE_VERSION);
  printf("GL_SHADING_LANGUAGE_VERSION=%s\n", shading);

  return 0;
}
