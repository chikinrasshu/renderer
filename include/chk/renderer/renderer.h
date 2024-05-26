#pragma once

#include <chk/core/arena.h>
#include <chk/core/color.h>
#include <chk/core/rect.h>
#include <chk/core/types.h>
#include <chk/core/vec/v2.h>

/******************************************************************************/
/* RendererConfig related                                                     */
/******************************************************************************/

typedef enum RendererKind {
    RendererKind_Null,
    RendererKind_Soft,
    RendererKind_OpenGL,
} RendererKind;

typedef struct RendererConfig {
    RendererKind kind;
    void*        win_impl;
} RendererConfig;

bool chk_renderer_config_get_default(RendererConfig* c);

/******************************************************************************/
/* RendererCmd related                                                        */
/******************************************************************************/

typedef enum RenderCmdKind {
    RenderCmdKind_Unknown = 0,
    RenderCmdKind_Clear,
    RenderCmdKind_Line,
    RenderCmdKind_Triangle,
} RenderCmdKind;

typedef struct RenderCmdBase {
    RenderCmdKind kind;
    RGBA          color;
} RenderCmdBase;

typedef struct RenderCmdClear {
    RenderCmdBase base;
} RenderCmdClear;

typedef struct RenderCmdLine {
    RenderCmdBase base;

    V2 a, b;
} RenderCmdLine;

typedef struct RenderCmdTriangle {
    RenderCmdBase base;

    V2 a, b, c;
} RenderCmdTriangle;

/******************************************************************************/
/* Renderer related                                                           */
/******************************************************************************/

typedef struct Renderer Renderer;

#define CHK_RENDERER_CLEAR(X)    void X(Renderer* r, RenderCmdClear* cmd)
#define CHK_RENDERER_LINE(X)     void X(Renderer* r, RenderCmdLine* cmd)
#define CHK_RENDERER_TRIANGLE(X) void X(Renderer* r, RenderCmdTriangle* cmd)
typedef CHK_RENDERER_CLEAR(RendererFnClear);
typedef CHK_RENDERER_LINE(RendererFnLine);
typedef CHK_RENDERER_TRIANGLE(RendererFnTriangle);

typedef struct RendererBindings {
    RendererFnClear*    clear;
    RendererFnLine*     line;
    RendererFnTriangle* triangle;
} RendererBindings;

typedef struct RendererCmds {
    Arena arena;
    U32   count;
} RendererCmds;

typedef struct Renderer {
    RendererKind     kind;
    RendererBindings fn;
    RendererCmds     cmds;

    const char* name;

    void* impl;
    void* impl_ex;
    void* impl_win;
} Renderer;

bool chk_renderer_create(Renderer* r, RendererConfig* c);
bool chk_renderer_destroy(Renderer* r);

bool chk_renderer_begin(Renderer* r);
bool chk_renderer_end(Renderer* r);

bool chk_renderer_clear(Renderer* r, RGBA col);
bool chk_renderer_line(Renderer* r, V2 a, V2 b, RGBA col);
bool chk_renderer_triangle(Renderer* r, V2 a, V2 b, V2 c, RGBA col);

bool chk_renderer_clear_packed(Renderer* r, U32 col);
bool chk_renderer_line_packed(Renderer* r, V2 a, V2 b, U32 col);
bool chk_renderer_triangle_packed(Renderer* r, V2 a, V2 b, V2 c, U32 col);
