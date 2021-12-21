#pragma once
#include <memory>

class WaylandWindow
{
  class WaylandWindowImpl *_impl = nullptr;

public:
  WaylandWindow();
  ~WaylandWindow();
  bool initialize(int width, int height);
  bool dispatch();
  void flush();
};
