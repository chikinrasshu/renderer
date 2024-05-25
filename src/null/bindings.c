#include <chk/core/log.h>
#include <chk/renderer/null/bindings.h>

CHK_RENDERER_CLEAR(chk_renderer_null_clear) {
    chk_info_f("Null", "Running NullGfx clear command");
}

CHK_RENDERER_LINE(chk_renderer_null_line) {
    chk_info_f("Null", "Running NullGfx line command");
}

CHK_RENDERER_TRIANGLE(chk_renderer_null_triangle) {
    chk_info_f("Null", "Running NullGfx triangle command");
}

bool chk_renderer_null_create(Renderer* r, RendererConfig* c) {
    if (!r) { return false; }
    if (!c) { return false; }

    // Initialize the backend

    // Setup bindings
    r->fn.clear    = chk_renderer_null_clear;
    r->fn.line     = chk_renderer_null_line;
    r->fn.triangle = chk_renderer_null_triangle;

    return true;
}

bool chk_renderer_null_destroy(Renderer* r) {
    if (!r) { return false; }

    // Terminate the backend

    return true;
}
