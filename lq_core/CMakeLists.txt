cmake_minimum_required(VERSION 3.20)
project(lq_core)

set(LQ_CORE_SOURCE 
 "src/algorithms.cpp"
 "src/array.cpp"
 "src/defines.cpp"
 "src/document.cpp"
 "src/string.cpp"
 "src/wrapper_types.cpp"
)

set(LQ_CORE_HEADER 
 "include/lq_core.h"
 "include/lq_core/algorithms.h"
 "include/lq_core/array.h"
 "include/lq_core/defines.h"
 "include/lq_core/document.h"
 "include/lq_core/font_query.h"
 "include/lq_core/interfaces.h"
 "include/lq_core/types.h"
 "include/lq_core/string.h"
 "include/lq_core/wrapper_types.h"
)

add_library(lq_core ${LQ_CORE_SOURCE} ${LQ_CORE_HEADER} )

if(BUILD_SHARED_LIBS)
    target_compile_definitions(lq_core PRIVATE LQ_CORE_EXPORTS)
else()
    target_compile_definitions(lq_core PUBLIC LQ_CORE_STATIC)
endif()

target_include_directories(lq_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(lq_core PRIVATE
    litehtml
    qjs
)

set_property(TARGET lq_core PROPERTY 
    CXX_STANDARD 20
    C_STANDARD 99
)