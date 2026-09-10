#include <rdn_native.h>
// #include <raylib.h>
#include "./raylib-6.0_linux_amd64/include/raylib.h"

#define REG_FUNC(func) {#func , (func)}

bool rdn_init_window(RDNApi *api) {
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

bool rdn_close_window(RDNApi *api) {
  CloseWindow();
  return true;
}

bool rdn_begin_drawing(RDNApi *api) {
  BeginDrawing();
  return true;
}

bool rdn_end_drawing(RDNApi *api) {
  EndDrawing();
  return true;
}

bool rdn_window_should_close(RDNApi *api) {
  api->push_boolean(api, WindowShouldClose());
  return true;
}

bool rdn_clear_background(RDNApi *api) {
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

bool rdn_draw_rect(RDNApi *api) {
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

bool rdn_get_width(RDNApi *api) {
  api->push_integer(api, (int)GetScreenWidth());
  return true;
}

bool rdn_get_height(RDNApi *api) {
  api->push_integer(api, (int)GetScreenHeight());
  return true;
}

bool rdn_set_target_fps(RDNApi *api) {
  long fps;
  api->to_integer(api, -1, &fps);
  SetTargetFPS((int)fps);
  api->pop(api, 1);
  return true;
}

bool rdn_is_window_ready(RDNApi *api) {
  api->push_boolean(api, IsWindowReady());
  return true;
}

bool rdn_is_window_full_screen(RDNApi *api) {
  api->push_boolean(api, IsWindowFullscreen());
  return true;
}

bool rdn_is_window_hidden(RDNApi *api) {
  api->push_boolean(api, IsWindowHidden());
  return true;
}

bool rdn_is_window_minimized(RDNApi *api) {
  api->push_boolean(api, IsWindowMinimized());
  return true;
}

bool rdn_is_window_maximized(RDNApi *api) {
  api->push_boolean(api, IsWindowMaximized());
  return true;
}

bool rdn_is_window_focused(RDNApi *api) {
  api->push_boolean(api, IsWindowFocused());
  return true;
}

bool rdn_is_window_resized(RDNApi *api) {
  api->push_boolean(api, IsWindowResized());
  return true;
}

bool rdn_is_window_state(RDNApi *api) {
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

bool rdn_set_window_state(RDNApi *api) {
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

bool rdn_clear_window_state(RDNApi *api) {
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

bool rdn_toggle_full_screen(RDNApi *api) {
  ToggleFullscreen();
  return true;
}

bool rdn_toggle_borderless_windowed(RDNApi *api) {
  ToggleBorderlessWindowed();
  return true;
}

bool rdn_maximize_window(RDNApi *api) {
  MaximizeWindow();
  return true;
}

bool rdn_minimize_window(RDNApi *api) {
  MinimizeWindow();
  return true;
}

bool rdn_restore_window(RDNApi *api) {
  RestoreWindow();
  return true;
}

struct {
  const char *func_name;
  RDNNativeFunction func;
} reg_raylib[] = {
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

};

#define LIB_REG_SIZE sizeof(reg_raylib) / sizeof(reg_raylib[0])

bool rdn_module_init(RDNModule *module) {

  for (size_t i = 0; i < LIB_REG_SIZE; ++i) {
    if (!module->register_function(module, reg_raylib[i].func_name,
                                   reg_raylib[i].func)) {
      return false;
    }
  }
  return true;
}
