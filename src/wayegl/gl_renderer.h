#pragma once

class GLRenderer
{
  class GLRendererImpl *_impl = nullptr;

public:
  GLRenderer();
  ~GLRenderer();
  bool initialize();
  void draw(int width, int height);
};
