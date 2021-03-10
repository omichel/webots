# EGL + QT

Since the `test_raw` example is able to initialize the OpenGL context without display here we try to combine it with Qt.
However, the `context->create()` line depicts `QOpenGLFunctions created with non-current context` warning meaning it is not able to create a context (with display it works fine, but not in Docker).
I tried to manually create the EGL context and pass it to `QOpenGLFunctions` (define the `MANUAL_CONTEXT_PASS` macro to test) but with no luck.

## Compile
```
qmake
make
```

## Run
```
export QT_QPA_PLATFORM=offscreen
./test
```