cmake_minimum_required(VERSION 3.27)
project(chk_lib_renderer C)

add_subdirectory("external")

set(src "renderer.c"
    "gl/bindings.c"
    "null/bindings.c"
    "soft/bindings.c"
)
set(hdr "renderer.h" 
    "gl/bindings.h"
    "null/bindings.h"
    "soft/bindings.h"
)

list(TRANSFORM src PREPEND "src/")
list(TRANSFORM hdr PREPEND "include/chk/renderer/")

add_library(chk_renderer ${src} ${hdr})
add_library(chk::renderer ALIAS chk_renderer)

target_compile_features(chk_renderer PUBLIC c_std_11)
target_include_directories(chk_renderer PUBLIC "include")

target_link_libraries(chk_renderer PUBLIC chk::core)
target_link_libraries(chk_renderer PRIVATE optimized glad::gl debug glad::gl_d)
