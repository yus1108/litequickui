cmake_minimum_required(VERSION 3.20)
project(lq_test)

set(LQ_TEXT_SRC
    "lq_test.cpp"
    "minimal.cpp"
)

# platform-specific source files
if(WIN32)
    list(APPEND LQ_TEXT_SRC 
        "win32_api.cpp"
        "win32_api.h"
        "win32_api.hpp"
        "win32.cpp"
    )
endif()

add_executable(lq_test ${LQ_TEXT_SRC} "lq_test.h")

set_property(TARGET lq_test PROPERTY 
    CXX_STANDARD 20
    C_STANDARD 99
)

# TODO: freetype & harfbuzz should be removed from here and added to lq_hb_ft
target_link_libraries(lq_test PRIVATE
    lq_core
    litequickui
)

if(!BUILD_SHARED_LIBS)
    target_compile_definitions(litequickui PUBLIC LQ_CORE_STATIC LQ_STATIC)
endif()

if(WIN32)
    target_compile_definitions(lq_test PRIVATE UNICODE _UNICODE)
#     set_target_properties(lq_test PROPERTIES
#         WIN32_EXECUTABLE YES
#     )
endif()

add_custom_target(run_lq_test
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE:lq_test>
    DEPENDS lq_test
)