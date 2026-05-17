cmake_minimum_required (VERSION 3.20)
project(litequickui LANGUAGES C CXX)

option(LITEQUICKUI_BUILD_FULL "Build full LiteQuickUI (core + text + backend)" OFF)
option(LITEQUICKUI_BUILD_TEST "Build test application" OFF)

option(LITEQUICKUI_TEXT_BACKEND_HB_FT "Enable Harfbuzz & FreeType for text rendering" OFF)
option(LITEQUICKUI_GRAPHICS_BACKEND_GL "Use OpenGL backend" OFF)
option(LITEQUICKUI_GRAPHICS_BACKEND_VULKAN "Use Vulkan backend" OFF)
if(WIN32)
	option(LITEQUICKUI_GRAPHICS_BACKEND_D2D "Use Direct2D backend (Windows)" OFF)
	option(LITEQUICKUI_GRAPHICS_BACKEND_DX12 "Use DirectX 12 backend (Windows)" OFF)
endif()
if(APPLE)
	option(LITEQUICKUI_GRAPHICS_BACKEND_METAL "Use Metal backend (macOS/iOS)" OFF)
endif()

# Convert ON/OFF to numeric values
set(GRAPHICS_BACKEND_GL     0)
set(GRAPHICS_BACKEND_VULKAN 0)
set(GRAPHICS_BACKEND_D2D    0)
set(GRAPHICS_BACKEND_DX12   0)
set(GRAPHICS_BACKEND_METAL  0)

if(LITEQUICKUI_GRAPHICS_BACKEND_GL)
    set(GRAPHICS_BACKEND_GL 1)
endif()

if(LITEQUICKUI_GRAPHICS_BACKEND_VULKAN)
    set(GRAPHICS_BACKEND_VULKAN 1)
endif()

if(LITEQUICKUI_GRAPHICS_BACKEND_D2D)
    set(GRAPHICS_BACKEND_D2D 1)
endif()

if(LITEQUICKUI_GRAPHICS_BACKEND_DX12)
    set(GRAPHICS_BACKEND_DX12 1)
endif()

if(LITEQUICKUI_GRAPHICS_BACKEND_METAL)
    set(GRAPHICS_BACKEND_METAL 1)
endif()

# ---- Backend conflict check ----

math(EXPR BACKEND_COUNT "
    ${GRAPHICS_BACKEND_GL} +
    ${GRAPHICS_BACKEND_VULKAN} +
    ${GRAPHICS_BACKEND_D2D} +
    ${GRAPHICS_BACKEND_DX12} +
    ${GRAPHICS_BACKEND_METAL}
")

if(BACKEND_COUNT GREATER 1)
    message(FATAL_ERROR "Only one graphics backend can be enabled at a time. Please disable extra backends.")
endif()

# ---- Backend-specific checks ----

if(LITEQUICKUI_GRAPHICS_BACKEND_GL)
	if(NOT LITEQUICKUI_TEXT_BACKEND_HB_FT)
		if(LITEQUICKUI_BUILD_FULL)
			message(FATAL_ERROR "OpenGL backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
		else()
			message(WARNING "OpenGL backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
		endif()
	endif()
endif()

if(LITEQUICKUI_GRAPHICS_BACKEND_VULKAN)
	if(NOT LITEQUICKUI_TEXT_BACKEND_HB_FT)
		if(LITEQUICKUI_BUILD_FULL)
			message(FATAL_ERROR "Vulkan backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
		else()
			message(WARNING "Vulkan backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
		endif()
	endif()
endif()

if(WIN32)
	if(LITEQUICKUI_GRAPHICS_BACKEND_D2D)
		if(LITEQUICKUI_TEXT_BACKEND_HB_FT)
			message(FATAL_ERROR "Direct2D backend has its own text rendering and does not use HarfBuzz or FreeType. Please disable them if you want to use Direct2D.")
		endif()
	endif()
	if(LITEQUICKUI_GRAPHICS_BACKEND_DX12)
		if(NOT LITEQUICKUI_TEXT_BACKEND_HB_FT)
			if(LITEQUICKUI_BUILD_FULL)
				message(FATAL_ERROR "DirectX 12 backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
			else()
				message(WARNING "DirectX 12 backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
			endif()
		endif()
	endif()
endif()

if(APPLE)
	if(LITEQUICKUI_GRAPHICS_BACKEND_METAL)
		if(NOT LITEQUICKUI_TEXT_BACKEND_HB_FT)
			if(LITEQUICKUI_BUILD_FULL)
				message(FATAL_ERROR "Metal backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
			else()
				message(WARNING "Metal backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
			endif()
		endif()
	endif()
endif()


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (MSVC)
	# warning level 4
	add_compile_options(/W4)
	add_compile_options(/permissive- /utf-8)
else()
	# additional warnings
	add_compile_options(-Wall -Wextra -Wpedantic -Wfloat-conversion)
endif()

add_subdirectory(external/litehtml)
add_subdirectory(external/quickjs)

add_subdirectory(lq_core)
if(LITEQUICKUI_BUILD_TEST)
	add_subdirectory(lq_test)
endif()

#if(LITEQUICKUI_ENABLE_FREETYPE)
#    add_subdirectory(external/freetype)
#    target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
#    target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_USE_FREETYPE)
#endif()
# 
# if(LITEQUICKUI_ENABLE_HARFBUZZ)
#     add_subdirectory(external/harfbuzz)
#     target_link_libraries(${PROJECT_NAME} PRIVATE harfbuzz)
#     target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_USE_HARFBUZZ)
# endif()
# 
# if(WIN32)
#     if(LITEQUICKUI_BACKEND_D2D)
#         target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_BACKEND_D2D)
#         target_link_libraries(${PROJECT_NAME} PRIVATE d2d1 dwrite)
#     elseif(LITEQUICKUI_BACKEND_DX12)
#         target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_BACKEND_DX12)
#         target_link_libraries(${PROJECT_NAME} PRIVATE dx12)
#     endif()
# endif()
# 
# if(APPLE)
#     if(LITEQUICKUI_BACKEND_METAL)
#         target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_BACKEND_METAL)
#         find_library(METAL Metal)
#         find_library(QUARTZ QuartzCore)
#         target_link_libraries(${PROJECT_NAME} PRIVATE ${METAL} ${QUARTZ})
#     endif()
# endif()
# 
# if(UNIX AND NOT APPLE)
#     if(LITEQUICKUI_BACKEND_GL)
#         find_package(Cairo REQUIRED)
#         target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
#         target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_BACKEND_GL)
#     endif()
# 
#     if(LITEQUICKUI_BACKEND_VULKAN)
#         find_package(OpenGL REQUIRED)
#         target_compile_definitions(${PROJECT_NAME} PRIVATE LITEQUICKUI_BACKEND_VULKAN)
#     endif()
# endif()
# 
# if(LQU_BACKEND_D2D)
#     target_sources(${PROJECT_NAME} PRIVATE src/backend/d2d/d2d_renderer.cpp)
# endif()


#target_include_directories(${PROJECT_NAME}
#    PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/include
#		${CMAKE_CURRENT_SOURCE_DIR}/external/litehtml/include
#		${CMAKE_CURRENT_SOURCE_DIR}/external/quickjs
#)
#
#target_link_libraries(${PROJECT_NAME}
#    PRIVATE
#        litehtml
#		qjs
#)