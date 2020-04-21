// dear imgui: Platform Binding for GLX
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

// About GLSL version:
// The 'glsl_version' initialization parameter defaults to "#version 150" if NULL.
// Only override if your GL version doesn't handle this GLSL version. Keep NULL if unsure!

#pragma once

#include <GL/glx.h>
//struct Window;

IMGUI_IMPL_API bool     ImGui_ImplGlx_Init(Window* window, bool install_callbacks);
IMGUI_IMPL_API void     ImGui_ImplGlx_OnEvent(XEvent* pEvent);