cmake_minimum_required(VERSION 3.20)
project(litequickui-core)

set(LITEQUICKUI_SOURCE 
    "src/defines.cpp"
    "src/document.cpp"
    "src/utf8.cpp"
)
set(LITEQUICKUI_HEADER 
    "include/lq_core.h"
    "include/lq_core/defines.h"
    "include/lq_core/document.h"
    "include/lq_core/types.h"
    "include/lq_core/utf8.h"
    "include/lq_core/wrapping_types.h"
)

add_library(lq_core ${LITEQUICKUI_SOURCE} ${LITEQUICKUI_HEADER} )

set_property(TARGET lq_core PROPERTY 
    CXX_STANDARD 20
    C_STANDARD 99
)

target_link_libraries(lq_core PRIVATE
    litehtml
    qjs
)

target_include_directories(lq_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)