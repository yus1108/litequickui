cmake_minimum_required (VERSION 3.20)
project(litequickui LANGUAGES C CXX)

# global options
option(BUILD_SHARED_LIBS "Build LiteQuickUI as a shared library" OFF)

option(LQ_BUILD_FULL "Build full LiteQuickUI (core + text + backend)" OFF)
option(LQ_BUILD_TEST "Build test application" OFF)

option(LQ_TEXT_BACKEND_HB_FT "Enable Harfbuzz & FreeType for text rendering" OFF)
option(LQ_GRAPHICS_BACKEND_GL "Use OpenGL backend" OFF)
option(LQ_GRAPHICS_BACKEND_VULKAN "Use Vulkan backend" OFF)
if(WIN32)
	option(LQ_GRAPHICS_BACKEND_D2D "Use Direct2D backend (Windows)" OFF)
	option(LQ_GRAPHICS_BACKEND_DX12 "Use DirectX 12 backend (Windows)" OFF)
endif()
if(APPLE)
	option(LQ_GRAPHICS_BACKEND_METAL "Use Metal backend (macOS/iOS)" OFF)
endif()

# Convert ON/OFF to numeric values
set(LQ_GRAPHICS_BACKEND_GL_COUNTER     0)
set(LQ_GRAPHICS_BACKEND_VULKAN_COUNTER 0)
set(LQ_GRAPHICS_BACKEND_D2D_COUNTER    0)
set(LQ_GRAPHICS_BACKEND_DX12_COUNTER   0)
set(LQ_GRAPHICS_BACKEND_METAL_COUNTER  0)

if(LQ_GRAPHICS_BACKEND_GL)
    set(LQ_GRAPHICS_BACKEND_GL_COUNTER 1)
endif()

if(LQ_GRAPHICS_BACKEND_VULKAN)
    set(LQ_GRAPHICS_BACKEND_VULKAN_COUNTER 1)
endif()

if(LQ_GRAPHICS_BACKEND_D2D)
    set(LQ_GRAPHICS_BACKEND_D2D_COUNTER 1)
endif()

if(LQ_GRAPHICS_BACKEND_DX12)
    set(LQ_GRAPHICS_BACKEND_DX12_COUNTER 1)
endif()

if(LQ_GRAPHICS_BACKEND_METAL)
    set(LQ_GRAPHICS_BACKEND_METAL_COUNTER 1)
endif()

# ---- Backend conflict check ----

math(EXPR BACKEND_COUNT "
    ${LQ_GRAPHICS_BACKEND_GL_COUNTER}     +
    ${LQ_GRAPHICS_BACKEND_VULKAN_COUNTER} +
    ${LQ_GRAPHICS_BACKEND_D2D_COUNTER}    +
    ${LQ_GRAPHICS_BACKEND_DX12_COUNTER}   +
    ${LQ_GRAPHICS_BACKEND_METAL_COUNTER}
")

if(BACKEND_COUNT GREATER 1)
    message(FATAL_ERROR "Only one graphics backend can be enabled at a time. Please disable extra backends.")
endif()

# ---- Backend-specific checks ----

if(LQ_GRAPHICS_BACKEND_GL)
	if(NOT LQ_TEXT_BACKEND_HB_FT)
		if(LQ_BUILD_FULL)
			message(FATAL_ERROR "OpenGL backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
		else()
			message(WARNING "OpenGL backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
		endif()
	endif()
endif()

if(LQ_GRAPHICS_BACKEND_VULKAN)
	if(NOT LQ_TEXT_BACKEND_HB_FT)
		if(LQ_BUILD_FULL)
			message(FATAL_ERROR "Vulkan backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
		else()
			message(WARNING "Vulkan backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
		endif()
	endif()
endif()

if(WIN32)
	if(LQ_GRAPHICS_BACKEND_D2D)
		if(LQ_TEXT_BACKEND_HB_FT)
			message(FATAL_ERROR "Direct2D backend has its own text rendering and does not use HarfBuzz or FreeType. Please disable them if you want to use Direct2D.")
		endif()
	endif()
	if(LQ_GRAPHICS_BACKEND_DX12)
		if(NOT LQ_TEXT_BACKEND_HB_FT)
			if(LQ_BUILD_FULL)
				message(FATAL_ERROR "DirectX 12 backend requires HarfBuzz and FreeType for text rendering for FULL build. Please enable them or disable the FULL build.")
			else()
				message(WARNING "DirectX 12 backend uses HarfBuzz and FreeType for text rendering as backend. Consider enabling it unless you have your own.")
			endif()
		endif()
	endif()
endif()

if(APPLE)
	if(LQ_GRAPHICS_BACKEND_METAL)
		if(NOT LQ_TEXT_BACKEND_HB_FT)
			if(LQ_BUILD_FULL)
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
if(LQ_TEXT_BACKEND_HB_FT)
	add_subdirectory(external/freetype)
	set(HB_HAVE_FREETYPE ON CACHE BOOL "" FORCE)
	add_subdirectory(external/harfbuzz)
endif()

add_subdirectory(lq_core)
if(LQ_TEXT_BACKEND_HB_FT)
	add_subdirectory(lq_hb_ft)
endif()
if(LQ_BUILD_TEST)
	add_subdirectory(lq_test)
endif()