#include <chk/core/log.h>
#include <chk/renderer/gl/bindings.h>

#include <glad/gl.h>

CHK_RENDERER_CLEAR(chk_renderer_gl_clear) {
    // chk_info_f("GL", "Running OpenGL clear command");

    RGBA* c = &cmd->base.color;

    glClearColor(c->r, c->g, c->b, c->a);
    glClear(GL_COLOR_BUFFER_BIT);
}

CHK_RENDERER_LINE(chk_renderer_gl_line) {
    // chk_info_f("GL", "Running OpenGL line command");
}

CHK_RENDERER_TRIANGLE(chk_renderer_gl_triangle) {
    // chk_info_f("GL", "Running OpenGL triangle command");
}

bool chk_renderer_gl_create(Renderer* r, RendererConfig* c) {
    if (!r) { return false; }
    if (!c) { return false; }

    // Initialize the backend
    glEnable(GL_BLEND);

    // Setup bindings
    r->fn.clear    = chk_renderer_gl_clear;
    r->fn.line     = chk_renderer_gl_line;
    r->fn.triangle = chk_renderer_gl_triangle;

    return true;
}

bool chk_renderer_gl_destroy(Renderer* r) {
    if (!r) { return false; }

    // Terminate the backend

    return true;
}
