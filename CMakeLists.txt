cmake_minimum_required(VERSION 3.20)
project(rund)

add_compile_options("/utf-8")
add_subdirectory("fmt")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(is_x64 TRUE)
else()
    set(is_x64 FALSE)
endif()

file(GLOB RUND_SOURCES CONFIGURE_DEPENDS "src/*.cc")
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS "imgui/*.cpp")
set(IMGUI_SOURCES ${IMGUI_SOURCES} "imgui/backends/imgui_impl_dx9.cpp" "imgui/backends/imgui_impl_win32.cpp")
set(SOURCES ${RUND_SOURCES} ${IMGUI_SOURCES})

include_directories("include" "imgui" "imgui/backends")
include_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")
include_directories("fmt/include")

if(is_x64)
    link_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64")
else()
    link_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86")
endif()

add_executable(rund WIN32 ${SOURCES})
target_link_libraries(rund d3d9 d3dx9 fmt)