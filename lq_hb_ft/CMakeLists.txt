cmake_minimum_required(VERSION 3.20)
project(lq_hb_ft)

set(LQ_HB_FT_SOURCE 
 "src/lq_hb_ft.cpp"

 "src/font_register.cpp"
)

set(LQ_HB_FT_HEADER 
 "include/lq_hb_ft.h"
 
 "include/lq_hb_ft/font_register.h")

add_library(lq_hb_ft ${LQ_HB_FT_SOURCE} ${LQ_HB_FT_HEADER} )

if(BUILD_SHARED_LIBS)
    target_compile_definitions(lq_hb_ft PRIVATE LQ_CORE_EXPORTS LQ_HB_FT_EXPORTS)
else()
    target_compile_definitions(lq_hb_ft PUBLIC LQ_CORE_STATIC LQ_HB_FT_STATIC)
endif()

target_include_directories(lq_hb_ft
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(lq_hb_ft PRIVATE
    lq_core
    freetype
    harfbuzz
)

set_property(TARGET lq_hb_ft PROPERTY 
    CXX_STANDARD 20
    C_STANDARD 99
)