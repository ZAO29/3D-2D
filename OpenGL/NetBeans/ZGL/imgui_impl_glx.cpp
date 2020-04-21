// dear imgui: Platform Binding for GLX
// This needs to be used along with a Renderer (e.g. OpenGL3, Vulkan..)

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glx.h"
#include <iostream>


int get512Value(int val)
{
    if( val > 0xFF00)
    {
        const int32_t offset_for_function_keys = 256 - 0xFF00;
        return val + offset_for_function_keys;
    }else
        return val;
}

void ImGui_ImplGlx_ScrollCallback(Window* window, double xoffset, double yoffset)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += (float)xoffset;
    io.MouseWheel += (float)yoffset;
}

void ImGui_ImplGlx_CharCallback(Window* window, unsigned int c)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
}

bool ImGui_ImplGlx_Init(Window* window, bool install_callbacks)
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    //io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_glx";

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    const int32_t offset_for_function_keys = 256 - 0xFF00;
    io.KeyMap[ImGuiKey_Tab] = get512Value(XK_Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = get512Value(XK_Left);
    io.KeyMap[ImGuiKey_RightArrow] = get512Value(XK_Right);
    io.KeyMap[ImGuiKey_UpArrow] = get512Value(XK_Up);
    io.KeyMap[ImGuiKey_DownArrow] = get512Value(XK_Down); 
    io.KeyMap[ImGuiKey_PageUp] = get512Value(XK_Page_Up); 
    io.KeyMap[ImGuiKey_PageDown] = get512Value(XK_Page_Down); 
    io.KeyMap[ImGuiKey_Home] = get512Value(XK_Home); 
    io.KeyMap[ImGuiKey_End] = get512Value(XK_End); 
    io.KeyMap[ImGuiKey_Insert] = get512Value(XK_Insert); 
    io.KeyMap[ImGuiKey_Delete] = get512Value(XK_Delete); 
    io.KeyMap[ImGuiKey_Backspace] = get512Value(XK_BackSpace); 
    io.KeyMap[ImGuiKey_Space] = get512Value(XK_space);
    io.KeyMap[ImGuiKey_Enter] = get512Value(XK_Return);
    io.KeyMap[ImGuiKey_Escape] = get512Value(XK_Escape); 
    io.KeyMap[ImGuiKey_KeyPadEnter] = get512Value(XK_KP_Enter); 
    io.KeyMap[ImGuiKey_A] = get512Value(XK_A);
    io.KeyMap[ImGuiKey_C] = get512Value(XK_C);
    io.KeyMap[ImGuiKey_V] = get512Value(XK_V);
    io.KeyMap[ImGuiKey_X] = get512Value(XK_X);
    io.KeyMap[ImGuiKey_Y] = get512Value(XK_Y);
    io.KeyMap[ImGuiKey_Z] = get512Value(XK_Z);

    return true;
}

void ImGui_ImplGlx_OnEvent(XEvent* pEvent)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    int32_t code = XLookupKeysym(&pEvent->xkey, 0);
    code = get512Value(code);
    
    switch (pEvent->type) 
    {
        case MotionNotify:    
            io.MousePos = ImVec2(pEvent->xmotion.x,pEvent->xmotion.y);
            break;

        case ButtonPress:
            io.MouseDown[pEvent->xbutton.button-1] = true;
            if(pEvent->xbutton.button==4)
                io.MouseWheel += 1.;
            if(pEvent->xbutton.button==5)
                io.MouseWheel -= 1.;
            break;
            
        case ButtonRelease:
            io.MouseDown[pEvent->xbutton.button-1] = false;
            break;

        case KeyPress:

            io.KeysDown[code] = true;
            if(code != get512Value(XK_Return))
            {
                io.AddInputCharacter(code);
            }
            

            
            break;

        case KeyRelease:                
            io.KeysDown[code] = false;
            break;
    }
    

    
    io.KeyCtrl = io.KeysDown[get512Value(XK_Control_L)] || io.KeysDown[get512Value(XK_Control_R)];
    io.KeyShift = io.KeysDown[get512Value(XK_Shift_L)] || io.KeysDown[get512Value(XK_Shift_R)];
    io.KeyAlt = io.KeysDown[get512Value(XK_Alt_L)] || io.KeysDown[get512Value(XK_Alt_R)];
    io.KeySuper = io.KeysDown[get512Value(XK_Super_L)] || io.KeysDown[get512Value(XK_Super_R)];
    
}
