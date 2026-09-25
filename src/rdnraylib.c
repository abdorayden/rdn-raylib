#include <assert.h>
#include <rdn.h>
#include <stddef.h>
#include <stdint.h>
#include "./helpers.c"
#include "raylib-6.0_linux_amd64/include/raylib.h"
#include "rdn/include/rdn.h"

RDN_SIG(rdn_init_window) {
  if (api->stack_size(api) < 3) {
    api->raise_error(api, "init-window requires 3 params");
    return false;
  }

  long width;
  long height;
  const char *title;

  if (api->to_integer(api, -3, &width) && api->to_integer(api, -2, &height) &&
      (title = api->to_string(api, -1))) {
    InitWindow((int)width, (int)height, title);
  } else {
    api->raise_error(api, "idk some error");
    return false;
  }
  return true;
}

RDN_SIG(rdn_close_window) {
  CloseWindow();
  return true;
}

RDN_SIG(rdn_begin_drawing) {
  BeginDrawing();
  return true;
}

RDN_SIG(rdn_end_drawing) {
  EndDrawing();
  return true;
}

RDN_SIG(rdn_window_should_close) {
  api->push_boolean(api, WindowShouldClose());
  return true;
}

RDN_SIG(rdn_clear_background) {
  if (api->stack_size(api) < 1) {
    api->raise_error(api, "requires hex color");
    return false;
  }
  long hex_color = 0;
  if (!api->to_integer(api, -1, &hex_color)) {
    api->raise_error(api, "khra");
    return false;
  }
  ClearBackground(GetColor((unsigned int)hex_color));
  return true;
}

RDN_SIG(rdn_draw_rect) {
  if (api->stack_size(api) < 5) {
    api->raise_error(api, "requires 5");
    return false;
  }
  long x;
  long y;
  long width;
  long height;
  long hex_color = 0;

  bool result = api->to_integer(api, -1, &hex_color);
  result &= api->to_integer(api, -2, &height);
  result &= api->to_integer(api, -3, &width);
  result &= api->to_integer(api, -4, &y);
  result &= api->to_integer(api, -5, &x);
  if (!result) {
    api->raise_error(api, "error in rdn_draw_rect stack params");
    return false;
  }
  DrawRectangle((int)x, (int)y, (int)width, (int)height, GetColor(hex_color));
  return true;
}

RDN_SIG(rdn_get_width) {
  api->push_integer(api, (int)GetScreenWidth());
  return true;
}

RDN_SIG(rdn_get_height) {
  api->push_integer(api, (int)GetScreenHeight());
  return true;
}

RDN_SIG(rdn_set_target_fps) {
  long fps;
  api->to_integer(api, -1, &fps);
  SetTargetFPS((int)fps);
  api->pop(api, 1);
  return true;
}

RDN_SIG(rdn_is_window_ready) {
  api->push_boolean(api, IsWindowReady());
  return true;
}

RDN_SIG(rdn_is_window_full_screen) {
  api->push_boolean(api, IsWindowFullscreen());
  return true;
}

RDN_SIG(rdn_is_window_hidden) {
  api->push_boolean(api, IsWindowHidden());
  return true;
}

RDN_SIG(rdn_is_window_minimized) {
  api->push_boolean(api, IsWindowMinimized());
  return true;
}

RDN_SIG(rdn_is_window_maximized) {
  api->push_boolean(api, IsWindowMaximized());
  return true;
}

RDN_SIG(rdn_is_window_focused) {
  api->push_boolean(api, IsWindowFocused());
  return true;
}

RDN_SIG(rdn_is_window_resized) {
  api->push_boolean(api, IsWindowResized());
  return true;
}

RDN_SIG(rdn_is_window_state) {
  bool ok = true;

  ok = api->stack_size(api) < 1;

  if (!ok) {
    return false;
  }

  long flag;

  ok = api->to_integer(api, -1, &flag);

  if (!ok) {
    return false;
  }

  api->push_boolean(api, IsWindowState((unsigned int)flag));
  return true;
}

RDN_SIG(rdn_set_window_state) {
  bool ok = true;

  ok = api->stack_size(api) < 1;

  if (!ok) {
    return false;
  }

  long flag;

  ok = api->to_integer(api, -1, &flag);

  if (!ok) {
    return false;
  }

  SetWindowState((unsigned int)flag);
  return true;
}

RDN_SIG(rdn_clear_window_state) {
  bool ok = true;

  ok = api->stack_size(api) < 1;

  if (!ok) {
    return false;
  }

  long flag;

  ok = api->to_integer(api, -1, &flag);

  if (!ok) {
    return false;
  }

  ClearWindowState((unsigned int)flag);
  return true;
}

RDN_SIG(rdn_toggle_full_screen) {
  ToggleFullscreen();
  return true;
}

RDN_SIG(rdn_toggle_borderless_windowed) {
  ToggleBorderlessWindowed();
  return true;
}

RDN_SIG(rdn_maximize_window) {
  MaximizeWindow();
  return true;
}

RDN_SIG(rdn_minimize_window) {
  MinimizeWindow();
  return true;
}

RDN_SIG(rdn_restore_window) {
  RestoreWindow();
  return true;
}

RDN_SIG(rdn_set_window_icon) {
    if (api->stack_size(api) < 1) {
        return false;
    }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;
    Image image = rdn_list_to_image(value, &ok);
    if (!ok) {
        return false;
    }
    SetWindowIcon(image);
    return true;
}

RDN_SIG(rdn_set_window_icons) { // NOTE: this will accept a list direcly
    if (api->stack_size(api) < 1) {
        return false;
    }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;
    size_t n = value->as.list.count;
    Image images[n + 1];

    for (size_t i = 0; i < n; ++i) {
        images[i] = rdn_list_to_image(value->as.list.items[i], &ok);
        if (!ok) {
            // list must be all images type
            return false;
        }
    }
    SetWindowIcons(images,n);
    return true;
}

RDN_SIG(rdn_set_window_title) {
    if(api->stack_size(api) < 1) {
        return false;
    }

    const char *title = api->to_string(api, -1);
    if (title == NULL) {
        return false;
    }

    SetWindowTitle(title);
    return true;
}

RDN_SIG(rdn_set_window_position) {
    if(api->stack_size(api) < 2) {
        return false;
    }

    bool ok = true;
    long x;
    long y;

    ok &= api->to_integer(api, -1 , &y);
    ok &= api->to_integer(api, -2 , &x);

    if (!ok) {
        return false;
    }

    SetWindowPosition((int)x, (int)y);
    return true;
}

RDN_SIG(rdn_set_window_monitor) {

    if(api->stack_size(api) < 1) {
        return false;
    }

    bool ok = true;
    long monitor;

    ok &= api->to_integer(api, -1 , &monitor);
    if (!ok) {
        return false;
    }

    SetWindowMonitor((int) monitor);
    return true;
}

RDN_SIG(rdn_set_window_min_size) {

    if(api->stack_size(api) < 2) {
        return false;
    }

    bool ok = true;
    long width;
    long height;

    ok &= api->to_integer(api, -1 , &height);
    ok &= api->to_integer(api, -2 , &width);

    if(!ok) {
        return false;
    }

    SetWindowMinSize((int) width, (int) height);

    return true;
}

RDN_SIG(rdn_set_window_max_size) {

    if(api->stack_size(api) < 2) {
        return false;
    }

    bool ok = true;
    long width;
    long height;

    ok &= api->to_integer(api, -1 , &height);
    ok &= api->to_integer(api, -2 , &width);

    if(!ok) {
        return false;
    }

    SetWindowMaxSize((int) width, (int) height);

    return true;
}

RDN_SIG(rdn_set_window_size) {

    if(api->stack_size(api) < 2) {
        return false;
    }

    bool ok = true;
    long width;
    long height;

    ok &= api->to_integer(api, -1 , &height);
    ok &= api->to_integer(api, -2 , &width);

    if(!ok) {
        return false;
    }

    SetWindowSize((int) width, (int) height);

    return true;
}

RDN_SIG(rdn_set_window_opacity) {

    if(api->stack_size(api) < 1) {
        return false;
    }

    bool ok = true;
    double opacity;
    ok &= api->to_number(api, -1 , &opacity);

    if(!ok) {
        return false;
    }

    SetWindowOpacity(opacity);

    return true;
}

RDN_SIG(rdn_set_window_focused) {
    SetWindowFocused();
    return true;
}

RDN_SIG(rdn_get_window_handle) {
    void* handle = GetWindowHandle();
    bool ok = api->push_integer(api, (uintptr_t)handle);
    if(!ok) {
        return false;
    }
    return true;
}

RDN_SIG(rdn_get_render_width) {
    bool ok = api->push_integer(api, (long)GetRenderWidth());
    if(!ok) {
        return false;
    }
    return true;
}

RDN_SIG(rdn_get_render_height) {
    bool ok = api->push_integer(api, (long)GetRenderHeight());
    if(!ok) {
        return false;
    }
    return true;
}

RDN_SIG(rdn_get_monitor_count) {
    bool ok = api->push_integer(api, (long)GetMonitorCount());
    if(!ok) {
        return false;
    }
    return true;
}

RDN_SIG(rdn_get_current_monitor) {
    bool ok = api->push_integer(api, (long)GetCurrentMonitor());
    if(!ok) {
        return false;
    }
    return true;
}

RDN_SIG(rdn_get_monitor_position) {

    if (api->stack_size(api) < 1) {
        return false;
    }

    bool ok = true;
    long monitor;

    ok &= api->to_integer(api, -1 , &monitor);

    if (!ok) {
        return false;
    }

    Vector2 position = GetMonitorPosition((int) monitor);

    ok &= api->push_list(api);
    ok &= api->push_integer(api, position.x);
    ok &= api->list_append(api, -2,-1);
    ok &= api->pop(api, 1);
    ok &= api->push_integer(api, position.y);
    ok &= api->list_append(api, -2,-1);
    ok &= api->pop(api, 1);

    if (!ok) {
        return false;
    }

    return true;
}

RDN_SIG(rdn_get_monitor_width) {
    if (api->stack_size(api) < 1) {
        return false;
    }

    bool ok = true;
    long monitor;

    ok &= api->to_integer(api, -1 , &monitor);

    if (!ok) {
        return false;
    }

    ok &= api->push_integer(api, (long)GetMonitorWidth((int) monitor));
    return ok;
}

RDN_SIG(rdn_get_monitor_height) {
    if (api->stack_size(api) < 1) {
        return false;
    }

    bool ok = true;
    long monitor;

    ok &= api->to_integer(api, -1 , &monitor);

    if (!ok) {
        return false;
    }

    ok &= api->push_integer(api, (long)GetMonitorHeight((int) monitor));
    return ok;
}

RDN_SIG(rdn_get_monitor_physical_width) {
    if (api->stack_size(api) < 1) {
        return false;
    }
    bool ok = true;
    long monitor;
    ok &= api->to_integer(api, -1 , &monitor);
    if (!ok) {
        return false;
    }
    ok &= api->push_integer(api, (long)GetMonitorPhysicalWidth((int) monitor));
    return ok;
}

RDN_SIG(rdn_get_monitor_physical_height) {
    if (api->stack_size(api) < 1) {
        return false;
    }
    bool ok = true;
    long monitor;
    ok &= api->to_integer(api, -1 , &monitor);
    if (!ok) {
        return false;
    }
    ok &= api->push_integer(api, (long)GetMonitorPhysicalHeight((int) monitor));
    return ok;
}

RDN_SIG(rdn_get_monitor_refresh_rate) {
    if (api->stack_size(api) < 1) {
        return false;
    }
    bool ok = true;
    long monitor;
    ok &= api->to_integer(api, -1 , &monitor);
    if (!ok) {
        return false;
    }
    ok &= api->push_integer(api, (long)GetMonitorRefreshRate((int) monitor));
    return ok;
}

RDN_SIG(rdn_get_window_position) {

    Vector2 position = GetWindowPosition();
    bool ok = true;

    ok &= api->push_list(api);
    ok &= api->push_integer(api,position.x);
    ok &= api->list_append(api, -2, -1);
    ok &= api->pop(api,1);

    ok &= api->push_integer(api,position.y);
    ok &= api->list_append(api, -2, -1);
    ok &= api->pop(api,1);

    return ok;
}

RDN_SIG(rdn_get_window_scale_dpi) {

    Vector2 position = GetWindowScaleDPI();
    bool ok = true;

    ok &= api->push_list(api);
    ok &= api->push_integer(api,position.x);
    ok &= api->list_append(api, -2, -1);
    ok &= api->pop(api,1);

    ok &= api->push_integer(api,position.y);
    ok &= api->list_append(api, -2, -1);
    ok &= api->pop(api,1);

    return ok;
}

RDN_SIG(rdn_get_monitor_name) {

    if (api->stack_size(api) < 1) {
        return false;
    }

    long monitor;
    bool ok = true;

    ok &= api->to_integer(api, -1, &monitor);
    if (!ok) {
        return false;
    }

    ok &= api->push_string(api, GetMonitorName((int)monitor));

    return ok;
}

RDN_SIG(rdn_set_clipboard_text) {

    if (api->stack_size(api) < 1) {
        return false;
    }

    const char* clipbtext = api->to_string(api, -1);
    if (clipbtext == NULL) return false;

    SetClipboardText(clipbtext);
    return true;
}

RDN_SIG(rdn_get_clipboard_text) {
    return api->push_string(api, GetClipboardText());
}

RDN_SIG(rdn_get_clipboard_image) {
    assert(false);
    return true;
}

RDN_SIG(rdn_enable_event_waiting) {
    EnableEventWaiting();
    return true;
}

RDN_SIG(rdn_disable_event_waiting) {
    DisableEventWaiting();
    return true;
}

RDN_SIG(rdn_show_cursor) {
    ShowCursor();
    return true;
}

RDN_SIG(rdn_hide_cursor) {
    HideCursor();
    return true;
}

RDN_SIG(rdn_is_cursor_hidden) {
    return api->push_boolean(api, IsCursorHidden());
}

RDN_SIG(rdn_enable_cursor) {
    EnableCursor();
    return true;
}

RDN_SIG(rdn_disable_cursor) {
    DisableCursor();
    return true;
}

RDN_SIG(rdn_is_cursor_on_screen) {
    return api->push_boolean(api, IsCursorOnScreen());
}

// NOTE:  stopped at line 1048 on raylib header code

RDN_SIG(rdn_begin_mode_2d) {

    if (api->stack_size(api) < 1) {
        return false;
    }

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;
    Camera2D cam2d = rdn_list_to_camera2d(value , &ok);
    if (!ok) {
        return false;
    }
    BeginMode2D(cam2d);
    return true;
}

RDN_SIG(rdn_end_mode_2d) {
    EndMode2D();
    return true;
}

RDN_SIG(rdn_begin_mode_3d) {

    if (api->stack_size(api) < 1) {
        return false;
    }

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;

    Camera3D cam3d = rdn_list_to_camera3d(value , &ok);
    if (!ok) {
        return false;
    }
    BeginMode3D(cam3d);
    return true;
}

RDN_SIG(rdn_end_mode_3d) {
    EndMode3D();
    return true;
}

RDN_SIG(rdn_begin_texture_mode) {
    if(api->stack_size(api) < 1) {
        return false;
    }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;

    RenderTexture2D target = (RenderTexture2D) rdn_list_to_render_texture(value, &ok);
    if (!ok) {
        return false;
    }
    BeginTextureMode(target);
    return true;
}

RDN_SIG(rdn_end_texture_mode) {
    EndTextureMode();
    return true;
}

RDN_SIG(rdn_begin_shader_mode) {
    if(api->stack_size(api) < 1) {
        return false;
    }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;

    Shader shader = rdn_list_to_shader(value, &ok);
    if (!ok) {
        return false;
    }
    BeginShaderMode(shader);
    return true;
}

RDN_SIG(rdn_end_shader_mode) {
    EndShaderMode();
    return true;
}

RDN_SIG(rdn_begin_blend_mode) {

    if (api->stack_size(api) < 1) {
        return false;
    }

    long mode;
    bool ok = api->to_integer(api, -1 , &mode);
    if(!ok) {
        return false;
    }
    BeginBlendMode((int)mode);
    return true;
}

RDN_SIG(rdn_end_blend_mode) {
    EndBlendMode();
    return true;
}

RDN_SIG(rdn_begin_scissor_mode) {

    if (api->stack_size(api) < 4) {
        return false;
    }

    bool ok = true;
    long x;
    long y;
    long width;
    long height;

    ok &= api->to_integer(api, -1  , &x);
    ok &= api->to_integer(api, -2  , &y);
    ok &= api->to_integer(api, -3  , &width);
    ok &= api->to_integer(api, -4  , &height);

    if (!ok) return false;

    BeginScissorMode((int) x, (int) y, (int) width, (int) height);

    return true;
}

RDN_SIG(rdn_end_scissor_mode) {
    EndScissorMode();
    return true;
}

RDN_SIG(rdn_begin_vr_stereo_mode) {
    VrStereoConfig config = {0};

    if (api->stack_size(api) < 1) {
        return false;
    }

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;

    config = rdn_list_to_vrstereoconfig(value, &ok);
    if (!ok) {
        return false;
    }
    BeginVrStereoMode(config);
    return true;
}

RDN_SIG(rdn_end_vr_stereo_mode) {
    EndVrStereoMode();
    return true;
}

RDN_SIG(rdn_load_vrstereoconfig) {
    bool ok = true;
    VrDeviceInfo device = {0};

    if (api->stack_size(api) < 1) {
        return false;
    }

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    device = rdn_list_to_vrdeviceinfo(value,&ok);
    if (!ok) {
        return false;
    }

    VrStereoConfig vrstereoconfig = LoadVrStereoConfig(device);
    ok = rdn_vrstereoconfig_to_list(api,vrstereoconfig);
    return ok;
}

RDN_SIG(rdn_unload_vrstereoconfig) {
    VrStereoConfig config = {0};

    if (api->stack_size(api) < 1) {
        return false;
    }

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;

    config = rdn_list_to_vrstereoconfig(value, &ok);
    UnloadVrStereoConfig(config);
    return true;
}

RDN_SIG(rdn_load_shader) {
    if (api->stack_size(api) < 2) {
        return false;
    }
    const char* fsFileName = api->to_string(api, -1);
    if (fsFileName == NULL) {
        return false;
    }
    const char* vsFileName = api->to_string(api, -2);
    if (vsFileName == NULL) {
        return false;
    }
    Shader shader = LoadShader(vsFileName, fsFileName);
    return rdn_shader_to_list(api, shader);
}

RDN_SIG(rdn_load_shader_from_memory) {
    if (api->stack_size(api) < 2) {
        return false;
    }
    const char* fsFileName = api->to_string(api, -1);
    if (fsFileName == NULL) {
        return false;
    }
    const char* vsFileName = api->to_string(api, -2);
    if (vsFileName == NULL) {
        return false;
    }

    Shader shader = LoadShaderFromMemory(vsFileName, fsFileName);
    
    return api->pop(api,2) && rdn_shader_to_list(api, shader);
}

RDN_SIG(rdn_is_shader_valid) {
    if(api->stack_size(api) < 1) {
        return false;
    }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST) {
        return false;
    }

    bool ok = true;

    Shader shader = rdn_list_to_shader(value, &ok);
    if (!ok) {
        return false;
    }

    return api->push_boolean(api, IsShaderValid(shader));
}

RDN_SIG(rdn_get_shader_location) {
    if (api->stack_size(api) < 2) {
        return false;
    }
    const char *uniformName = api->to_string(api, -1);
    if (uniformName == NULL) {
        return false;
    }
    bool ok = api->pop(api,1);
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST || !ok) {
        return false;
    }
    Shader shader = rdn_list_to_shader(value, &ok);
    if (!ok) {
        return false;
    }
    return api->push_integer(api,GetShaderLocation(shader , uniformName));
}

RDN_SIG(rdn_get_shader_location_attrib) {
    if (api->stack_size(api) < 2) {
        return false;
    }
    const char *attribName = api->to_string(api, -1);
    if (attribName == NULL) {
        return false;
    }
    bool ok = api->pop(api,1);
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* value = rdn_pop_value(state);
    if(value->type != VALUE_LIST || !ok) {
        return false;
    }
    Shader shader = rdn_list_to_shader(value, &ok);
    if (!ok) {
        return false;
    }
    return api->push_integer(api,GetShaderLocationAttrib(shader , attribName));
}


RDN_SIG(rdn_set_shader_value) {
    if (api->stack_size(api) < 4) {
        return false;
    }
    bool ok = true;
    int uniformType;
    const void *value = NULL;
    int locIndex;
    ok &= api->to_integer(api,-1,(long*)&uniformType);
    ok &= api->to_integer(api,-2,(long*)value);
    ok &= api->to_integer(api,-3,(long*)&locIndex);
    ok &= api->pop(api,3);
    if (!ok)    return false;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = rdn_pop_value(state);
    if(val->type != VALUE_LIST || !ok) {
        return false;
    }
    Shader shader = rdn_list_to_shader(val, &ok);
    if (!ok) {
        return false;
    }
    SetShaderValue(shader, locIndex, value, uniformType);
    return true;
}

RDN_SIG(rdn_set_shader_value_v) {
    if (api->stack_size(api) < 5) {
        return false;
    }
    bool ok = true;
    int count;
    int uniformType;
    const void *value = NULL;
    int locIndex;
    ok &= api->to_integer(api,-1,(long*)&count);
    ok &= api->to_integer(api,-2,(long*)&uniformType);
    ok &= api->to_integer(api,-3,(long*)value);
    ok &= api->to_integer(api,-4,(long*)&locIndex);
    ok &= api->pop(api,4);
    if (!ok)    return false;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = rdn_pop_value(state);
    if(val->type != VALUE_LIST || !ok) {
        return false;
    }
    Shader shader = rdn_list_to_shader(val, &ok);
    if (!ok) {
        return false;
    }
    SetShaderValueV(shader, locIndex, value, uniformType, count);
    return true;
}

RDN_SIG(rdn_set_shader_value_matrix) {

    if (api->stack_size(api) < 3) { return false; }

    Value* val = NULL;
    bool ok = true;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    val = rdn_pop_value(state);
    if(val->type != VALUE_LIST) { return false; }

    Matrix matrix = rdn_list_to_matrix(val , &ok);
    if (!ok) { return false; }

    int locIndex;
    ok &= api->to_integer(api,-2,(long*)&locIndex);
    ok &= api->pop(api,1);
    if (!ok) { return false; }

    val = rdn_pop_value(state);
    Shader  shader = rdn_list_to_shader(val, &ok);
    if (!ok) { return false; }

    SetShaderValueMatrix(shader, locIndex, matrix );
    return true;
}

RDN_SIG(rdn_set_shader_value_texture) {
    if (api->stack_size(api) < 3) { return false; }

    Value* val = NULL;
    bool ok = true;
    int locIndex;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    val = rdn_pop_value(state);
    if(val->type != VALUE_LIST) { return false; }

    Texture2D texture2d = rdn_list_to_texture(val, &ok);
    if (!ok) {
        return false;
    }

    ok &= api->to_integer(api,-2,(long*)&locIndex);
    ok &= api->pop(api,1);
    if (!ok) { return false; }

    val = rdn_pop_value(state);
    Shader  shader = rdn_list_to_shader(val, &ok);
    if (!ok) { return false; }

    SetShaderValueTexture(shader, locIndex, texture2d);
    return true;
}

RDN_SIG(rdn_unload_shader) {

    if (api->stack_size(api) < 1) { return false; }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;
    bool ok = true;


    val = rdn_pop_value(state);
    Shader  shader = rdn_list_to_shader(val, &ok);
    if (!ok) { return false; }
    UnloadShader(shader);
    return true;
}

RDN_SIG(rdn_get_screen_to_world_ray) {
  if (api->stack_size(api) < 2) {
    return false;
  }
  RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
  Value* val = NULL;
  bool ok = true;

  val = rdn_pop_value(state);

  Camera camera = rdn_list_to_camera3d(val, &ok);
  if (!ok) {
    return false;
  }

  val = rdn_pop_value(state);

  if(val->type != VALUE_LIST
  && val->as.list.count != 2
  && val->as.list.items[0]->type != VALUE_DOUBLE
  && val->as.list.items[1]->type != VALUE_DOUBLE
  ) {
    return false;
  }

  Vector2 position = {0};
  position.x = (float)val->as.list.items[0]->as.number;
  position.y = (float)val->as.list.items[1]->as.number;

  Ray ray = GetScreenToWorldRay(position, camera);

  ok = rdn_ray_to_list(api , ray);
  return ok;
}

RDN_SIG(rdn_get_screen_to_world_ray_ex) {
    if (api->stack_size(api) < 4) {
        return false;
    }

    long width = 0;
    long height = 0;
    Camera camera = {0};
    Vector2 position = {0};
    bool ok = true;

    ok &= api->to_integer(api , -1 , &height);
    ok &= api->to_integer(api , -2 , &width);

    ok &= api->pop(api,2);

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera3d(val, &ok);
    if (!ok) {
        return false;
    }

    val = rdn_pop_value(state);

    if(val->type != VALUE_LIST
            && val->as.list.count != 2
            && val->as.list.items[0]->type != VALUE_DOUBLE
            && val->as.list.items[1]->type != VALUE_DOUBLE
      ) {
        return false;
    }

    position.x = (float)val->as.list.items[0]->as.number;
    position.y = (float)val->as.list.items[1]->as.number;

    Ray ray = GetScreenToWorldRayEx(position, camera,width,height);

    ok = rdn_ray_to_list(api , ray);
    return ok;

    return true;
}

RDN_SIG(ray_get_world_to_screen) {
    if(api->stack_size(api) < 2) {
        return false;
    }

    bool ok = true;
    Camera camera = {0};
    Vector3 position = {0};

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera3d(val, &ok);
    if (!ok) {
        return false;
    }

    val = rdn_pop_value(state);

    if(
            val->type != VALUE_LIST
            && val->as.list.count != 3
            && val->as.list.items[0]->type != VALUE_DOUBLE
            && val->as.list.items[1]->type != VALUE_DOUBLE
            && val->as.list.items[2]->type != VALUE_DOUBLE
            ) {
        return false;
    }

    position.x = (float)val->as.list.items[0]->as.number;
    position.y = (float)val->as.list.items[1]->as.number;
    position.z = (float)val->as.list.items[2]->as.number;

    Vector2 vec2 = GetWorldToScreen(position,camera);

    ok &= api->push_list(api);
    ok &= api->push_number(api,vec2.x);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);

    ok &= api->push_number(api,vec2.y);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);
    return true;
}

RDN_SIG(ray_get_world_to_screen_ex) {
    if(api->stack_size(api) < 4) {
        return false;
    }

    bool ok = true;
    Camera camera = {0};
    Vector3 position = {0};

    long width = 0;
    long height = 0;

    ok &= api->to_integer(api,-1 , &height);
    ok &= api->to_integer(api,-2 , &width);
    ok &= api->pop(api,2);

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera3d(val, &ok);
    if (!ok) {
        return false;
    }

    val = rdn_pop_value(state);

    if(
            val->type != VALUE_LIST
            && val->as.list.count != 3
            && val->as.list.items[0]->type != VALUE_DOUBLE
            && val->as.list.items[1]->type != VALUE_DOUBLE
            && val->as.list.items[2]->type != VALUE_DOUBLE
            ) {
        return false;
    }

    position.x = (float)val->as.list.items[0]->as.number;
    position.y = (float)val->as.list.items[1]->as.number;
    position.z = (float)val->as.list.items[2]->as.number;

    Vector2 vec2 = GetWorldToScreenEx(position,camera,width,height);

    ok &= api->push_list(api);
    ok &= api->push_number(api,vec2.x);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);

    ok &= api->push_number(api,vec2.y);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);
    return ok;
}

RDN_SIG(ray_get_world_to_screen_2d) {

    if (api->stack_size(api) < 2) {
        return false;
    }

    Camera2D camera = {0};
    Vector2 position = {0};
    bool ok = true;

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera2d(val, &ok);
    if(!ok) {
        return false;
    }

    val = rdn_pop_value(state);

    if(
            val->type != VALUE_LIST
            && val->as.list.count != 2
            && val->as.list.items[0]->type != VALUE_DOUBLE
            && val->as.list.items[1]->type != VALUE_DOUBLE
            ) {
        return false;
    }

    position.x = (float)val->as.list.items[0]->as.number;
    position.y = (float)val->as.list.items[1]->as.number;

    Vector2 res = GetWorldToScreen2D(position, camera);
    ok &= api->push_list(api);
    ok &= api->push_number(api,res.x);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);

    ok &= api->push_number(api,res.y);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);
    return ok;
}

RDN_SIG(ray_get_screen_to_world_2d) {

    if (api->stack_size(api) < 2) {
        return false;
    }

    Camera2D camera = {0};
    Vector2 position = {0};
    bool ok = true;

    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera2d(val, &ok);
    if(!ok) {
        return false;
    }

    val = rdn_pop_value(state);

    if(
            val->type != VALUE_LIST
            && val->as.list.count != 2
            && val->as.list.items[0]->type != VALUE_DOUBLE
            && val->as.list.items[1]->type != VALUE_DOUBLE
            ) {
        return false;
    }

    position.x = (float)val->as.list.items[0]->as.number;
    position.y = (float)val->as.list.items[1]->as.number;

    Vector2 res = GetScreenToWorld2D(position, camera);
    ok &= api->push_list(api);
    ok &= api->push_number(api,res.x);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);

    ok &= api->push_number(api,res.y);
    ok &= api->list_append(api , -2 , -1);
    ok &= api->pop(api ,1);
    return ok;
}

RDN_SIG(rdn_get_camera_matrix) {
    if (api->stack_size(api) < 1) {
        return false;
    }
    Camera camera = {0};
    bool ok = true;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera3d(val, &ok);
    if(!ok) {
        return false;
    }

    return rdn_matrix_to_list(api, GetCameraMatrix(camera));
}

RDN_SIG(rdn_get_camera_matrix_2d) {
    if (api->stack_size(api) < 1) {
        return false;
    }
    Camera2D camera = {0};
    bool ok = true;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    Value* val = NULL;

    val = rdn_pop_value(state);

    camera = rdn_list_to_camera2d(val, &ok);
    if(!ok) {
        return false;
    }

    return rdn_matrix_to_list(api, GetCameraMatrix2D(camera));
}

RDN_SIG(rdn_get_frame_time) {
    return api->push_number(api , GetFrameTime());
}

RDN_SIG(rdn_get_time) {
    return api->push_number(api , GetTime());
}

RDN_SIG(rdn_get_fps) {
    return api->push_integer(api , GetFPS());
}

RDN_SIG(rdn_swap_screen_buffer) {
    SwapScreenBuffer();
    return true;
}

RDN_SIG(rdn_poll_inpus_events) {
    PollInputEvents();
    return true;
}

RDN_SIG(rdn_wait_time){

    if(api->stack_size(api) < 1) {
        return false;
    }

    double seconds = 0;

    if (!api->to_number(api, -1 , &seconds)) {
        return false;
    }

    WaitTime(seconds);
    return true;
}

RDN_SIG(rdn_set_random_seed) {
    if (api->stack_size(api) < 1) {
        return false;
    }

    long seed = 0;

    if (!api->to_integer(api, -1, &seed)) {
        return false;
    }
    SetRandomSeed((unsigned int)seed);
    return true;
}

RDN_SIG(rdn_get_random_value) {

    if (api->stack_size(api) < 2) {
        return false;
    }

    long min = 0;
    long max = 0;
    bool ok = true;

    ok &= api->to_integer(api,-1,&max);
    ok &= api->to_integer(api,-2,&min);

    if (!ok) {
        return false;
    }

    return api->push_integer(api,GetRandomValue((int) min, (int) max));
}

RDN_SIG(rdn_load_random_sequence){
    if(!check_stack(api,3)) {
        return false;
    }

    long min = 0;
    long max = 0;
    long count = 0;
    bool ok = true;

    ok &= api->to_integer(api,-1,&max);
    ok &= api->to_integer(api,-2,&min);
    ok &= api->to_integer(api,-3,&count);
    ok &= api->pop(api,3);

    if (!ok) {
        return false;
    }

    return api->push_integer(api,(uintptr_t)LoadRandomSequence((unsigned int) count, (int) min, (int) max));
}

RDN_SIG(rdn_unload_random_sequence){
    if(!check_stack(api, 1)){
        return false;
    }

    long sequence = 0;

    if(!api->to_integer(api,-1,&sequence)) {
        return false;
    }
    UnloadRandomSequence((int*)sequence);
    return true;
}

RDN_SIG(rdn_take_screen_shot) {
    CHECK(1)

    TakeScreenshot(api->to_string(api,-1));
    return true;
}

RDN_SIG(rdn_set_config_flag) {
    CHECK(1)

    long flags = 0;
    if(!api->to_integer(api,-1,&flags)) {
        return false;
    }
    SetConfigFlags((unsigned int) flags);
    return true;
}

RDN_SIG(rdn_open_url) {
    CHECK(1)
    OpenURL(api->to_string(api,-1));
    api->pop(api,1);
    return true;
}

RDN_SIG(rdn_set_trace_log_level) {
    CHECK(1)

    long logLevel = 0;

    if (!api->to_integer(api,-1,&logLevel)) {
        return false;
    }
    api->pop(api,1);
    SetTraceLogLevel((int)logLevel);
    return true;
}

static Value* stack_with_callback = NULL;

RDN_SIG(rdn_trace_log) {
    CHECK(2)
    if(stack_with_callback == NULL) {
        const char* text = api->to_string(api,-1);
        long logLevel = 0;

        if (!api->to_integer(api,-2,&logLevel)) {
            return false;
        }
        api->pop(api,2);
        // you should use @fmt from rdn runtime
        TraceLog((int)logLevel, "%s", text);
        return true;
    }
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    rdn_push_value(state, stack_with_callback);
    api->call_function(api);
    return true;
}

RDN_SIG(rdn_set_trace_log_callback) {
    // calling this function i will push the rdn function
    // and rdn_trace_log will use it if the flag set it to true
    CHECK(1)
    if(!api->is_function(api,-1))   return false;
    RDNState* state = (RDNState*)((NativeCallState*)api->userdata)->stack;
    stack_with_callback = rdn_pop_value(state);
    return true;
}

RDN_SIG(rdn_mem_alloc) {

    CHECK(1)
    long size = 0;

    if (!api->to_integer(api,-1,&size)) {
        return false;
    }
    void *mem = MemAlloc((unsigned int)size);

    return api->push_integer(api,(uintptr_t)mem);
}

RDN_SIG(rdn_mem_realloc) {
    CHECK(2)
    long size = 0;
    void* mem = NULL;

    if (!api->to_integer(api,-1,&size)) {
        return false;
    }

    if (!api->to_integer(api,-2,mem)) {
        return false;
    }

    return api->push_integer(api,(uintptr_t)MemRealloc(mem, (unsigned int)size));
}

RDN_SIG(rdn_mem_free) {
    CHECK(1)
    void* mem = NULL;

    if (!api->to_integer(api,-1,mem)) {
        return false;
    }
    MemFree(mem);

    return true;
}

REG_TYPE reg_raylib[] = {

    REG_FUNC(rdn_get_monitor_position),
    REG_FUNC(rdn_get_monitor_width),
    REG_FUNC(rdn_get_monitor_height),
    REG_FUNC(rdn_get_monitor_physical_width),
    REG_FUNC(rdn_get_monitor_physical_height),
    REG_FUNC(rdn_get_monitor_refresh_rate),
    REG_FUNC(rdn_get_window_position),
    REG_FUNC(rdn_get_window_scale_dpi),
    REG_FUNC(rdn_get_monitor_name),
    REG_FUNC(rdn_set_clipboard_text),
    REG_FUNC(rdn_get_clipboard_text),
    REG_FUNC(rdn_get_clipboard_image),
    REG_FUNC(rdn_enable_event_waiting),
    REG_FUNC(rdn_disable_event_waiting),
    REG_FUNC(rdn_show_cursor),
    REG_FUNC(rdn_hide_cursor),
    REG_FUNC(rdn_is_cursor_hidden),
    REG_FUNC(rdn_enable_cursor),
    REG_FUNC(rdn_disable_cursor),
    REG_FUNC(rdn_is_cursor_on_screen),

    REG_FUNC(rdn_init_window),
    REG_FUNC(rdn_close_window),
    REG_FUNC(rdn_begin_drawing),
    REG_FUNC(rdn_end_drawing),
    REG_FUNC(rdn_window_should_close),
    REG_FUNC(rdn_clear_background),
    REG_FUNC(rdn_draw_rect),
    REG_FUNC(rdn_get_width),
    REG_FUNC(rdn_get_height),
    REG_FUNC(rdn_set_target_fps),
    REG_FUNC(rdn_is_window_ready),
    REG_FUNC(rdn_is_window_full_screen),
    REG_FUNC(rdn_is_window_hidden),
    REG_FUNC(rdn_is_window_minimized),
    REG_FUNC(rdn_is_window_maximized),
    REG_FUNC(rdn_is_window_focused),
    REG_FUNC(rdn_is_window_resized),
    REG_FUNC(rdn_is_window_state),
    REG_FUNC(rdn_set_window_state),
    REG_FUNC(rdn_clear_window_state),
    REG_FUNC(rdn_toggle_full_screen),
    REG_FUNC(rdn_toggle_borderless_windowed),
    REG_FUNC(rdn_maximize_window),
    REG_FUNC(rdn_minimize_window),
    REG_FUNC(rdn_restore_window),

    REG_FUNC(rdn_set_window_icon),
    REG_FUNC(rdn_set_window_icons),

    REG_FUNC(rdn_set_window_title),
    REG_FUNC(rdn_set_window_position),
    REG_FUNC(rdn_set_window_monitor),
    REG_FUNC(rdn_set_window_min_size),
    REG_FUNC(rdn_set_window_max_size),
    REG_FUNC(rdn_set_window_size),
    REG_FUNC(rdn_set_window_opacity),
    REG_FUNC(rdn_set_window_focused),
    REG_FUNC(rdn_get_window_handle),
    REG_FUNC(rdn_get_render_width),
    REG_FUNC(rdn_get_render_height),
    REG_FUNC(rdn_get_monitor_count),
    REG_FUNC(rdn_get_current_monitor),

    REG_FUNC(rdn_begin_mode_2d),
    REG_FUNC(rdn_end_mode_2d),
    REG_FUNC(rdn_begin_mode_3d),
    REG_FUNC(rdn_end_mode_3d),

    REG_FUNC(rdn_begin_texture_mode),
    REG_FUNC(rdn_end_texture_mode),
    REG_FUNC(rdn_begin_shader_mode),
    REG_FUNC(rdn_end_shader_mode),
    REG_FUNC(rdn_begin_blend_mode),
    REG_FUNC(rdn_end_blend_mode),
    REG_FUNC(rdn_begin_scissor_mode),
    REG_FUNC(rdn_end_scissor_mode),
    REG_FUNC(rdn_begin_vr_stereo_mode),
    REG_FUNC(rdn_end_vr_stereo_mode),
    REG_FUNC(rdn_load_vrstereoconfig),
    REG_FUNC(rdn_unload_vrstereoconfig),
    REG_FUNC(rdn_load_shader),
    REG_FUNC(rdn_load_shader_from_memory),
    REG_FUNC(rdn_is_shader_valid),
    REG_FUNC(rdn_get_shader_location),
    REG_FUNC(rdn_get_shader_location_attrib),
    REG_FUNC(rdn_set_shader_value),
    REG_FUNC(rdn_set_shader_value_v),
    REG_FUNC(rdn_set_shader_value_matrix),
    REG_FUNC(rdn_set_shader_value_texture),
    REG_FUNC(rdn_unload_shader),
    REG_FUNC(rdn_get_screen_to_world_ray),
    REG_FUNC(rdn_get_screen_to_world_ray_ex),
    REG_FUNC(ray_get_world_to_screen),
    REG_FUNC(ray_get_world_to_screen_ex),
    REG_FUNC(ray_get_world_to_screen_2d),
    REG_FUNC(ray_get_screen_to_world_2d),
    REG_FUNC(rdn_get_camera_matrix),
    REG_FUNC(rdn_get_camera_matrix_2d),
    REG_FUNC(rdn_get_frame_time),
    REG_FUNC(rdn_get_time),
    REG_FUNC(rdn_get_fps),
    REG_FUNC(rdn_swap_screen_buffer),
    REG_FUNC(rdn_poll_inpus_events),
    REG_FUNC(rdn_wait_time),

    REG_FUNC(rdn_set_random_seed),
    REG_FUNC(rdn_get_random_value),
    REG_FUNC(rdn_load_random_sequence),
    REG_FUNC(rdn_unload_random_sequence),
    REG_FUNC(rdn_take_screen_shot),
    REG_FUNC(rdn_set_config_flag),
    REG_FUNC(rdn_open_url),
    REG_FUNC(rdn_set_trace_log_level),
    REG_FUNC(rdn_trace_log),
    REG_FUNC(rdn_set_trace_log_callback),
    REG_FUNC(rdn_mem_alloc),
    REG_FUNC(rdn_mem_realloc),

    REG_FUNC(rdn_mem_free),
};

bool rdn_module_init(RDNModule *module) {

  for (size_t i = 0; i < LIB_REG_SIZE; ++i) {
    if (!module->register_function(module, reg_raylib[i].func_name,
                                   reg_raylib[i].func)) {
      return false;
    }
  }
  return true;
}
