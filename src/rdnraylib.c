#include <assert.h>
#include <stdint.h>
#include "./raylib-6.0_linux_amd64/include/raylib.h"
#include "rdn/include/rdn.h"
#include "rdn/include/rdn_native.h"

#define REG_FUNC(func) {#func , (func)}
#define REG_TYPE struct { const char *func_name; RDNNativeFunction func; }
#define LIB_REG_SIZE sizeof(reg_raylib) / sizeof(reg_raylib[0])

#define RDN_SIG(name) bool name(RDNApi* api)

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
    assert(false);
    return true;
}

RDN_SIG(rdn_set_window_icons) {
    assert(false);
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

// add some helpres

Camera2D rdn_list_to_camera2d(Value* value, bool* ok) {
    // TODO: implement to_list function on RDNApi interface
    Camera2D cam2d = {0};
    RDNValueList l = value->as.list;

    if (
            l.count != 4

            && l.items[0]->type != VALUE_LIST 
            && l.items[0]->as.list.count != 2 
            && l.items[0]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[0]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[1]->type != VALUE_LIST
            && l.items[1]->as.list.count != 2 
            && l.items[1]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[1]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[2]->type != VALUE_DOUBLE
            && l.items[3]->type != VALUE_DOUBLE
            ) {
        *ok = false;
        return cam2d;
    }
    
    cam2d.offset.x = (float)l.items[0]->as.list.items[0]->as.number;
    cam2d.offset.y = (float)l.items[0]->as.list.items[1]->as.number;

    cam2d.target.x = (float)l.items[1]->as.list.items[0]->as.number;
    cam2d.target.y = (float)l.items[1]->as.list.items[1]->as.number;

    cam2d.rotation = (float)l.items[2]->as.number;
    cam2d.zoom     = (float)l.items[3]->as.number;

    *ok = true;
    return cam2d;
}

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

Camera3D rdn_list_to_camera3d(Value* value, bool* ok) {
    Camera3D cam3d = {0};
    RDNValueList l = value->as.list;
    if (
            l.count != 5

            && l.items[0]->type != VALUE_LIST 
            && l.items[0]->as.list.count != 3 
            && l.items[0]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[0]->as.list.items[1]->type != VALUE_DOUBLE
            && l.items[0]->as.list.items[2]->type != VALUE_DOUBLE

            && l.items[1]->type != VALUE_LIST 
            && l.items[1]->as.list.count != 3 
            && l.items[1]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[1]->as.list.items[1]->type != VALUE_DOUBLE
            && l.items[1]->as.list.items[2]->type != VALUE_DOUBLE

            && l.items[2]->type != VALUE_LIST 
            && l.items[2]->as.list.count != 3 
            && l.items[2]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[2]->as.list.items[1]->type != VALUE_DOUBLE
            && l.items[2]->as.list.items[2]->type != VALUE_DOUBLE

            && l.items[3]->type != VALUE_DOUBLE
            && l.items[4]->type != VALUE_INTEGER
            ){
        *ok = false;
        return cam3d;
    }

    cam3d.position.x    = (float)l.items[0]->as.list.items[0]->as.number;
    cam3d.position.y    = (float)l.items[0]->as.list.items[1]->as.number;
    cam3d.position.z    = (float)l.items[0]->as.list.items[2]->as.number;

    cam3d.target.x      = (float)l.items[1]->as.list.items[0]->as.number;
    cam3d.target.y      = (float)l.items[1]->as.list.items[1]->as.number;
    cam3d.target.z      = (float)l.items[1]->as.list.items[2]->as.number;

    cam3d.up.x          = (float)l.items[2]->as.list.items[0]->as.number;
    cam3d.up.y          = (float)l.items[2]->as.list.items[1]->as.number;
    cam3d.up.z          = (float)l.items[2]->as.list.items[2]->as.number;

    cam3d.fovy          = (float)l.items[3]->as.number;
    cam3d.projection    = (int)l.items[4]->as.integer;

    return cam3d;
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


REG_TYPE reg_raylib[] = {

    REG_FUNC(rdn_begin_mode_2d),
    REG_FUNC(rdn_end_mode_2d),

    REG_FUNC(rdn_begin_mode_3d),
    REG_FUNC(rdn_end_mode_3d),

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
