cmake_minimum_required(VERSION 3.20)
project(lq_test)

add_executable(lq_test
    "lq_test.cpp"
 "lq_test.h")
set_property(TARGET lq_test PROPERTY 
    CXX_STANDARD 20
    C_STANDARD 99
)

target_link_libraries(lq_test PRIVATE
    lq_core
)

target_include_directories(lq_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# if(WIN32)
#     set_target_properties(lq_test PROPERTIES
#         WIN32_EXECUTABLE YES
#     )
# endif()
