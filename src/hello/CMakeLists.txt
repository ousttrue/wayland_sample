set(TARGET_NAME hello)
add_executable(${TARGET_NAME} main.c shm.c xdg-shell-protocol.c)
target_link_libraries(${TARGET_NAME} wayland-client rt)

add_custom_target(generate DEPENDS xdg-shell-protocol.c
                                   xdg-shell-client-protocol.h cat.h)
add_custom_command(
  OUTPUT xdg-shell-protocol.c
  COMMAND
    wayland-scanner private-code
    /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
    ${CMAKE_CURRENT_LIST_DIR}/xdg-shell-protocol.c)

add_custom_command(
  OUTPUT xdg-shell-client-protocol.h
  COMMAND
    wayland-scanner client-header
    /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
    ${CMAKE_CURRENT_LIST_DIR}/xdg-shell-client-protocol.h)

add_custom_command(
  OUTPUT cat.h COMMAND convert ${CMAKE_CURRENT_LIST_DIR}/cat.png -define
                       h:format=bgra -depth 8 ${CMAKE_CURRENT_LIST_DIR}/cat.h)

add_dependencies(hello generate)
