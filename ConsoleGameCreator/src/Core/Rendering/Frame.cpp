#include <cgcpch.h>
#include <Core/Rendering/Frame.h>
#include <Core/Debug.h>

namespace cgc {
  Frame::Frame(size_t _rows, size_t _columns, StyledChar defaultChar)
    : m_rows(_rows), m_columns(_columns) {
    m_buffer = new StyledChar* [m_rows];
    for (size_t r = 0; r < rows(); r++) {
      m_buffer[r] = new StyledChar[m_columns];
      for (size_t c = 0; c < columns(); c++) {
        m_buffer[r][c] = defaultChar;
      }
    }
  }

  Frame::Frame(const Frame& rhs)
    : m_rows(rhs.m_rows), m_columns(rhs.m_columns) {
    m_buffer = new StyledChar* [m_rows];
    for (size_t r = 0; r < rows(); r++) {
      m_buffer[r] = new StyledChar[m_columns];
      for (size_t c = 0; c < columns(); c++) {
        m_buffer[r][c] = rhs.m_buffer[r][c];
      }
    }
  }

  Frame::Frame(Frame&& rhs) noexcept
    : m_rows(rhs.m_rows), m_columns(rhs.m_columns) {
    m_buffer = rhs.m_buffer;
    rhs.m_buffer = nullptr;
  }

  Frame::~Frame() {
    if (!m_buffer) return;

    for (size_t r = 0; r < rows(); r++) {
      delete[] m_buffer[r];
    }
    delete[] m_buffer;
  }

  size_t Frame::rows() const {
    return m_rows;
  }

  size_t Frame::columns() const {
    return m_columns;
  }

  size_t Frame::length() const {
    return m_rows * m_columns;
  }

  StyledChar Frame::get(size_t row, size_t column) const {
    CGC_ASSERT(row < m_rows, "row is out of bounds");
    CGC_ASSERT(column < m_columns, "column is out of bounds");

    return m_buffer[row][column];
  }

  void Frame::set(size_t row, size_t column, StyledChar val) {
    CGC_ASSERT(row < m_rows, "row is out of bounds");
    CGC_ASSERT(column < m_columns, "column is out of bounds");

    m_buffer[row][column] = val;
  }

  size_t Frame::write(size_t row, size_t column, const std::u16string& str, Style style) {
    CGC_ASSERT(row < m_rows, "row is out of bounds.");
    CGC_ASSERT(column < m_columns, "column is out of bounds.");

    size_t maxColLen = m_columns - column;
    std::u16string towrite = str.substr(0, maxColLen);
    size_t written = strToBuffer(row, column, towrite, style);

    if (wrapping) {
      for (size_t i = maxColLen, r = row + 1;
        i < str.length() && r < m_rows;
        i += m_columns, r++) {
        towrite = str.substr(i, m_columns);
        written += strToBuffer(r, 0, towrite, style);
      }
    }

    return written;
  }
  size_t Frame::write(size_t row, size_t column, const StyledStrings& strings) {
    CGC_ASSERT(row < m_rows, "row is out of bounds.");
    CGC_ASSERT(column < m_columns, "column is out of bounds.");

    // Write every string with its style.
    size_t written = 0, colwritten = 0;
    for (const auto[str, style] : strings) {
      size_t wr = write(row, column + colwritten, str, style);
      written += wr; colwritten += wr;

      // When got to the end of the row,
      // if wrapping and not at the last row,
      // go to next row.
      // else, end writting.
      if (column + colwritten >= m_columns) {
        if (wrapping && row + 1 < m_rows) {
          colwritten -= m_columns;
          column = 0;
          row += 1;
        }
        else break;
      }
    }
    return written;
  }

  size_t Frame::strToBuffer(size_t row, size_t column, const std::u16string& str, Style style) {
    CGC_ASSERT(row < m_rows, "row is out of bounds.");
    CGC_ASSERT(column < m_columns, "column is out of bounds.");
    CGC_ASSERT(str.length() <= m_columns - column, "str is out of bounds.");

    size_t length = std::min(m_columns - column, str.length());
    for (size_t i = 0; i < length; i++) {
      m_buffer[row][column + i] = StyledChar(str[i], style);
    }

    return length;
  }
}
