#include <QApplication>
#include <QLabel>
#include <QtGui/QSurfaceFormat>
#include <QtGui>
#include <QtOpenGL/QGLWidget>
#include <QtWidgets/QMainWindow>

#include <glad/egl.h>
#include <QtPlatformHeaders/QEGLNativeContext>
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
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  QLabel label("Hello, world!");
  label.show();

  QMainWindow mainWindow;
  QGLWidget glWidget(&mainWindow);
  mainWindow.setCentralWidget(&glWidget);
  mainWindow.show();

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

  // eglMakeCurrent(eglDpy, eglSurf, eglSurf, eglCtx);

#ifdef MANUAL_CONTEXT_PASS
  QOpenGLContext *context = new QOpenGLContext;
  context->setNativeHandle(QVariant::fromValue(QEGLNativeContext(eglCtx, eglDpy)));
#else
  QOpenGLContext *context = new QOpenGLContext();
#endif

  context->create();

  QOpenGLFunctions *gl = context->functions();

  const char *vendor = (const char *)gl->glGetString(GL_VENDOR);
  const char *renderer = (const char *)gl->glGetString(GL_RENDERER);

  QTextStream(stdout) << QString("OpenGL vendor: %1").arg(vendor) << endl;

  return app.exec();
}
