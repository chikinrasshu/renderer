#include <chk/core/log.h>
#include <chk/renderer/soft/bindings.h>

CHK_RENDERER_CLEAR(chk_renderer_soft_clear) {
    chk_info_f("Soft", "Running SoftGfx clear command");
}

CHK_RENDERER_LINE(chk_renderer_soft_line) {
    chk_info_f("Soft", "Running SoftGfx line command");
}

CHK_RENDERER_TRIANGLE(chk_renderer_soft_triangle) {
    chk_info_f("Soft", "Running SoftGfx triangle command");
}

bool chk_renderer_soft_create(Renderer* r, RendererConfig* c) {
    if (!r) { return false; }
    if (!c) { return false; }

    // Initialize the backend

    // Setup bindings
    r->fn.clear    = chk_renderer_soft_clear;
    r->fn.line     = chk_renderer_soft_line;
    r->fn.triangle = chk_renderer_soft_triangle;

    return true;
}

bool chk_renderer_soft_destroy(Renderer* r) {
    if (!r) { return false; }

    // Terminate the backend

    return true;
}
