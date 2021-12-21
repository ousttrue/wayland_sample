# project
cmake_minimum_required(VERSION 3.16.0)
project(waygl VERSION 0.1.0)

# target
add_executable(${PROJECT_NAME} main.cpp gl_renderer.cpp)
target_link_libraries(${PROJECT_NAME} wayland-client wayland-egl EGL GLESv2)
