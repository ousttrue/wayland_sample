set(TARGET_NAME wayegl)
add_executable(${TARGET_NAME} main.cpp)
target_link_libraries(${TARGET_NAME} wayland-client wayland-egl EGL GLESv2)
