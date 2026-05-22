cmake_minimum_required(VERSION 3.20)
project(litequickui)

set(LITEQUICKUI_SOURCE 
 "src/litequickui.cpp"
)

set(LITEQUICKUI_HEADER 
 "include/litequickui.h"
 "include/litequickui/defines.h")

add_library(litequickui ${LITEQUICKUI_SOURCE} ${LITEQUICKUI_HEADER} )

if(BUILD_SHARED_LIBS)
    target_compile_definitions(litequickui PRIVATE LQ_CORE_EXPORTS LQ_EXPORTS)
    if(LQ_TEXT_BACKEND_HB_FT)
        target_compile_definitions(litequickui PRIVATE LQ_HB_FT_EXPORTS)
    endif()
else()
    target_compile_definitions(litequickui PUBLIC LQ_CORE_STATIC LQ_STATIC)
    if(LQ_TEXT_BACKEND_HB_FT)
        target_compile_definitions(litequickui PRIVATE LQ_HB_FT_STATIC)
    endif()
endif()

if(LQ_TEXT_BACKEND_HB_FT)
    target_compile_definitions(litequickui PRIVATE LQ_TEXT_BACKEND_HB_FT)
endif()

target_include_directories(litequickui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(litequickui PRIVATE lq_core)
if(LQ_TEXT_BACKEND_HB_FT)
    target_link_libraries(litequickui PRIVATE lq_hb_ft)
endif()

set_property(TARGET litequickui PROPERTY 
    CXX_STANDARD 20
    C_STANDARD 99
)