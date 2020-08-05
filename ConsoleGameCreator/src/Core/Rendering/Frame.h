#pragma once
#include <cgcpch.h>
#include <Core/Rendering/StyledChar.h>
#include <fmt/format.h>

namespace cgc {
  class Frame {
  public:
    Frame(size_t rows, size_t columns, StyledChar defaultChar = ' ');
    Frame(const Frame& rhs);
    Frame(Frame&& rhs) noexcept;
    virtual ~Frame();

    size_t rows() const;
    size_t columns() const;
    size_t length() const;

    StyledChar get(size_t row, size_t column) const;
    void set(size_t row, size_t column, StyledChar val);

    size_t write(size_t row, size_t column, const std::u16string& str, Style style = Style());

    typedef std::vector<std::pair<std::u16string, Style>> StyledStrings;
    size_t write(size_t row, size_t column, const StyledStrings& strings);

    template <class... Args>
    size_t writef(size_t row, size_t column, const std::u16string& str, Style style, Args&&... args) {
      CGC_ASSERT(row < m_rows, "row is out of bounds.");
      CGC_ASSERT(column < m_columns, "column is out of bounds.");

      std::u16string formatted = fmt::format(str, args...);
      return write(row, column, formatted, style);
    }
  private:
    size_t strToBuffer(size_t row, size_t column, const std::u16string& str, Style style);
  public:
    bool wrapping = false;
  private:
    size_t m_rows, m_columns;
    StyledChar** m_buffer;
  };
}
