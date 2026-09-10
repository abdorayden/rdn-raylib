#include <assert.h>
#include <stdint.h>
#include <rdn_native.h>
// #include <raylib.h>
#include "./raylib-6.0_linux_amd64/include/raylib.h"

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

REG_TYPE reg_raylib[] = {
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
