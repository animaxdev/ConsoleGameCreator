#pragma once
#include "cgcpch.h"
#include "Core/Events/KeyPressedEvent.h"
#include "Core/Events/KeyTypedEvent.h"
#include "Core/Events/MouseClickedEvent.h"
#include "Core/Events/MouseMovedEvent.h"
#include "Core/Events/MouseScrolledEvent.h"

namespace cgc {
  class Input {
  public:
    Input() = delete;

    static void onEvents(std::vector<std::shared_ptr<Event>>& events);
    static bool keyPressed(Keys key);
    static bool keyDown(Keys key);
    static bool keyUp(Keys key);
    static bool keyTyped(char keyChar);
    static bool mouseButtonClicked(MouseButtons button);
    static bool mouseButtonDown(MouseButtons button);
    static bool mouseButtonUp(MouseButtons button);
    static bool mouseButtonDoubleClick(MouseButtons button);
    static std::pair<size_t, size_t> mousePosition();
    static size_t mouseRow();
    static size_t mouseColumn();
    static int verticalScroll();
    static int horizontalScroll();
#ifdef CGC_SUPPORT_UTF8
    static bool keyTyped(const std::string& keyCharUtf8);
#endif

  private:
    static bool onKeyPressed(KeyPressedEvent& e);
    static bool onKeyTyped(KeyTypedEvent& e);
    static bool onMouseClicked(MouseClickedEvent& e);
    static bool onMouseMoved(MouseMovedEvent& e);
    static bool onMouseScrolled(MouseScrolledEvent& e);

  private:
    static std::queue<Keys> s_keysReleased;
    static std::queue<char> s_notTyped;
    static std::queue<MouseButtons> s_mouseReleased;

    static std::unordered_map<Keys, int8_t> s_keysMap;
    static std::unordered_map<char, bool> s_typedMap;
    static std::unordered_map<MouseButtons, int8_t> s_mouseClickedMap;
    static std::unordered_map<MouseButtons, bool> s_mouseDoubleClickedMap;

    static size_t s_row;
    static size_t s_column;
    static int s_vertical;
    static int s_horizontal;

#ifdef CGC_SUPPORT_UTF8
    static std::queue<std::string> s_notTypedUtf8;
    static std::unordered_map<std::string, bool> s_typedMapUtf8;
#endif
  };
}
