set(TARGET_NAME wayegl)
add_executable(${TARGET_NAME} main.cpp gl_renderer.cpp wayland_window.cpp)
target_link_libraries(${TARGET_NAME} wayland-client wayland-egl EGL GLESv2)
