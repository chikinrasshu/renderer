#include "chk/core/log.h"
#include <chk/core/mem.h>
#include <chk/renderer/renderer.h>

#include <chk/renderer/gl/bindings.h>
#include <chk/renderer/null/bindings.h>
#include <chk/renderer/soft/bindings.h>

/******************************************************************************/
/* RendererConfig impl                                                        */
/******************************************************************************/

bool chk_renderer_config_get_default(RendererConfig* c) {
    if (!c) { return false; }

    c->kind     = RendererKind_OpenGL;
    c->win_impl = NULL;

    return true;
}

/******************************************************************************/
/* Renderer impl                                                              */
/******************************************************************************/

bool chk_renderer_create(Renderer* r, RendererConfig* c) {
    if (!r) { return false; }
    if (!c) { return false; }

    if (!c->win_impl) {
        chk_warn("Renderer", "win_impl was NULL, it should be win->impl");
        return false;
    }

    r->kind     = c->kind;
    r->impl_win = c->win_impl;

    if (!chk_arena_create(&r->cmds, chk_kilobytes(256))) { return false; }

    switch (r->kind) {
        case RendererKind_Null: {
            if (!chk_renderer_null_create(r, c)) { return false; }
        } break;
        case RendererKind_Soft: {
            if (!chk_renderer_soft_create(r, c)) { return false; }
        } break;
        case RendererKind_OpenGL: {
            if (!chk_renderer_gl_create(r, c)) { return false; }
        } break;
    }

    return true;
}

bool chk_renderer_destroy(Renderer* r) {
    if (!r) { return false; }

    switch (r->kind) {
        case RendererKind_Null: {
            if (!chk_renderer_null_destroy(r)) { return false; }
        } break;
        case RendererKind_Soft: {
            if (!chk_renderer_soft_destroy(r)) { return false; }
        } break;
        case RendererKind_OpenGL: {
            if (!chk_renderer_gl_destroy(r)) { return false; }
        } break;
    }

    *r = (Renderer){};

    return true;
}

bool chk_renderer_begin(Renderer* r) {
    if (!r) { return false; }

    if (!chk_arena_reset(&r->cmds)) { return false; }

    return true;
}

bool chk_renderer_end(Renderer* r) {
    if (!r) { return false; }

    void* raw_ptr = (r->cmds.used > 0) ? r->cmds.data : NULL;
    while (raw_ptr != NULL) {
        // Check if we exceeded the arena's used pointer
        if ((raw_ptr - r->cmds.data) > r->cmds.used) { break; }

        RenderCmdBase* raw_cmd = raw_ptr;
        switch (raw_cmd->kind) {
            case RenderCmdKind_Unknown: {
                raw_ptr = NULL;
            } break;
            case RenderCmdKind_Clear: {
                r->fn.clear(r, raw_ptr);
                raw_ptr += sizeof(RenderCmdClear);
            } break;
            case RenderCmdKind_Line: {
                r->fn.line(r, raw_ptr);
                raw_ptr += sizeof(RenderCmdLine);
            } break;
            case RenderCmdKind_Triangle: {
                r->fn.triangle(r, raw_ptr);
                raw_ptr += sizeof(RenderCmdTriangle);
            } break;
        }
    }

    return true;
}

bool chk_renderer_clear(Renderer* r, Rect viewport, RGBA col) {
    if (!r) { return false; }

    RenderCmdClear* cmd = NULL;
    if (!chk_arena_push_struct(&r->cmds, RenderCmdClear, (void**)&cmd)) {
        return false;
    }

    cmd->base.kind  = RenderCmdKind_Clear;
    cmd->base.color = col;
    cmd->viewport   = viewport;

    return true;
}

bool chk_renderer_line(Renderer* r, V2 a, V2 b, RGBA col) {
    if (!r) { return false; }

    RenderCmdLine* cmd = NULL;
    if (!chk_arena_push_struct(&r->cmds, RenderCmdLine, (void**)&cmd)) {
        return false;
    }

    cmd->base.kind  = RenderCmdKind_Line;
    cmd->base.color = col;
    cmd->a          = a;
    cmd->b          = b;

    return true;
}

bool chk_renderer_triangle(Renderer* r, V2 a, V2 b, V2 c, RGBA col) {
    if (!r) { return false; }

    RenderCmdTriangle* cmd = NULL;
    if (!chk_arena_push_struct(&r->cmds, RenderCmdTriangle, (void**)&cmd)) {
        return false;
    }

    cmd->base.kind  = RenderCmdKind_Triangle;
    cmd->base.color = col;
    cmd->a          = a;
    cmd->b          = b;
    cmd->c          = c;

    return true;
}

bool chk_renderer_clear_packed(Renderer* r, Rect viewport, U32 col) {
    if (!r) { return false; }

    RGBA unpacked_col;
    if (!chk_rgba_unpack(&col, &unpacked_col)) { return false; }

    return chk_renderer_clear(r, viewport, unpacked_col);
}

bool chk_renderer_line_packed(Renderer* r, V2 a, V2 b, U32 col) {
    if (!r) { return false; }

    RGBA unpacked_col;
    if (!chk_rgba_unpack(&col, &unpacked_col)) { return false; }

    return chk_renderer_line(r, a, b, unpacked_col);
}

bool chk_renderer_triangle_packed(Renderer* r, V2 a, V2 b, V2 c, U32 col) {
    if (!r) { return false; }

    RGBA unpacked_col;
    if (!chk_rgba_unpack(&col, &unpacked_col)) { return false; }

    return chk_renderer_triangle(r, a, b, c, unpacked_col);
}
