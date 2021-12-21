/*
 * A simple Wayland EGL program to show a triangle
 *
 * cc -o triangle_simple triangle_simple.c -lwayland-client -lwayland-egl -lEGL -lGLESv2
 */
#include <memory>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <assert.h>
#include <string.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include "gl_renderer.h"
#include "wayland_window.h"

struct WaylandGlobals
{
  struct wl_compositor *compositor;
  struct wl_shell *shell;
};

/*
 * Registry callbacks
 */
static void registry_global(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version)
{
  struct WaylandGlobals *globals = (struct WaylandGlobals *)data;
  if (strcmp(interface, "wl_compositor") == 0)
  {
    globals->compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
  }
  else if (strcmp(interface, "wl_shell") == 0)
  {
    globals->shell = (wl_shell *)wl_registry_bind(registry, id, &wl_shell_interface, 1);
  }
}

static const struct wl_registry_listener registry_listener = {registry_global, NULL};

/*
 * Configure EGL and return necessary resources
 * input nativeDisplay
 * input nativeWindow
 * output eglDisplay
 * output eglSurface
 */
static void initEGLDisplay(EGLNativeDisplayType nativeDisplay, EGLNativeWindowType nativeWindow, EGLDisplay *eglDisplay, EGLSurface *eglSurface)
{
  EGLint number_of_config;
  EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE};

  static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE};

  *eglDisplay = eglGetDisplay(nativeDisplay);
  assert(*eglDisplay != EGL_NO_DISPLAY);

  EGLBoolean initialized = eglInitialize(*eglDisplay, NULL, NULL);
  assert(initialized == EGL_TRUE);

  EGLConfig configs[1];

  eglChooseConfig(*eglDisplay, config_attribs, configs, 1, &number_of_config);
  assert(number_of_config);

  EGLContext eglContext = eglCreateContext(*eglDisplay, configs[0], EGL_NO_CONTEXT, context_attribs);

  *eglSurface = eglCreateWindowSurface(*eglDisplay, configs[0], nativeWindow, NULL);
  assert(*eglSurface != EGL_NO_SURFACE);

  EGLBoolean makeCurrent = eglMakeCurrent(*eglDisplay, *eglSurface, *eglSurface, eglContext);
  assert(makeCurrent == EGL_TRUE);
}

class WaylandWindowImpl
{
  wl_display *wlDisplay = nullptr;
  wl_surface *wlSurface = nullptr;
  EGLDisplay eglDisplay;
  EGLSurface eglSurface;

public:
  bool initialize(int width, int height)
  {
    wlDisplay = wl_display_connect(NULL);
    if (!wlDisplay)
    {
      return false;
    }

    WaylandGlobals globals = {0};
    auto registry = wl_display_get_registry(wlDisplay);
    wl_registry_add_listener(registry, &registry_listener, (void *)&globals);

    wl_display_dispatch(wlDisplay);
    wl_display_roundtrip(wlDisplay);
    if (!globals.compositor)
    {
      return false;
    }
    if (!globals.shell)
    {
      return false;
    }

    wlSurface = wl_compositor_create_surface(globals.compositor);
    if (!wlSurface)
    {
      return false;
    }

    auto shellSurface = wl_shell_get_shell_surface(globals.shell, wlSurface);
    wl_shell_surface_set_toplevel(shellSurface);

    auto wlEglWindow = wl_egl_window_create(wlSurface, width, height);
    if (!wlEglWindow)
    {
      return false;
    }

    initEGLDisplay((EGLNativeDisplayType)wlDisplay, (EGLNativeWindowType)wlEglWindow, &eglDisplay, &eglSurface);

    return true;
  }

  ~WaylandWindowImpl()
  {
    if (wlDisplay)
    {
      wl_display_disconnect(wlDisplay);
      wlDisplay = nullptr;
    }
  }

  bool dispatch()
  {
    return wl_display_dispatch(wlDisplay) != -1;
  }

  void flush()
  {
    eglSwapBuffers(eglDisplay, eglSurface);
  }
};

WaylandWindow::WaylandWindow()
    : _impl(new WaylandWindowImpl)
{
}

WaylandWindow::~WaylandWindow()
{
  delete _impl;
}

bool WaylandWindow::initialize(int width, int height)
{
  return _impl->initialize(width, height);
}

bool WaylandWindow::dispatch()
{
  return _impl->dispatch();
}

void WaylandWindow::flush()
{
  _impl->flush();
}
