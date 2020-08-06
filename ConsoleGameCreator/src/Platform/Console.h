#pragma once
#include "Core/Rendering/Frame.h"

namespace cgc {
  class Console {
  public:
    virtual ~Console() = default;

    virtual Frame newFrame() const;
    virtual void draw(const Frame& newFrame);

    virtual std::string getTitle() const = 0;
    virtual std::string setTitle() = 0;
    virtual std::pair<size_t, size_t> getSize() const = 0;
    virtual void setSize(size_t rows, size_t columns) = 0;
    virtual std::pair<size_t, size_t> getCursorPosition() const = 0;
    virtual void setCursorPosition(size_t row, size_t column) = 0;
    virtual void showCursor() = 0;
    virtual void hideCursor() = 0;

    static std::unique_ptr<Console> platformConsole();
  private:
    virtual void drawChar(size_t row, size_t column, StyledChar schar);
  private:
    Frame m_frame;
  };
}