
#include "wayland_window.h"
#include "gl_renderer.h"

int main(int argc, char **argv)
{
    int width = 320;
    int height = 240;
    WaylandWindow w;
    if(!w.initialize(width, height))
    {
        return 1;
    }

    GLRenderer gl;
    if (!gl.initialize())
    {
        return 2;
    }

    gl.draw(width, height);
    w.flush();
    while (w.dispatch())
    {
    }

    return 0;
}
