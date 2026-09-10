#include <rdn_native.h>
#include <raylib.h>

bool rdn_init_window(RDNApi* api) {
    if (api->stack_size(api) < 3) {
        api->raise_error(api , "init-window requires 3 params");
        return false;
    }

    long width;
    long height;
    const char* title;

    if(api->to_integer(api , -3 , &width ) && api->to_integer(api , -2 , &height) && (title = api->to_string(api , -1))) {
        InitWindow((int)width, (int)height, title);
    }else {
        api->raise_error(api , "idk some error");
        return false;
    }
    return true;
}

bool rdn_close_window(RDNApi* api) {
    CloseWindow();
    return true;
}

bool rdn_begin_drawing(RDNApi* api) {
    BeginDrawing();
    return true;
}

bool rdn_end_drawing(RDNApi* api) {
    EndDrawing();
    return true;
}

bool rdn_window_should_close(RDNApi* api) {
    api->push_boolean(api , WindowShouldClose());
    return true;
}

bool rdn_clear_bg(RDNApi* api) {
    if (api->stack_size(api) < 1) {
        api->raise_error(api , "requires hex color");
        return false;
    }
    long hex_color = 0;
    if (!api->to_integer(api , -1 , &hex_color)) {
        api->raise_error(api , "khra");
        return false;
    }
    ClearBackground(GetColor((unsigned int)hex_color));
    return true;
}

bool rdn_draw_rect(RDNApi* api) {
    if (api->stack_size(api) < 5) {
        api->raise_error(api , "requires 5");
        return false;
    }
    long x;
    long y;
    long width;
    long height;
    long hex_color = 0;

    bool result = api->to_integer(api , -1 , &hex_color);
    result &= api->to_integer(api , -2 , &height);
    result &= api->to_integer(api , -3 , &width);
    result &= api->to_integer(api , -4 , &y);
    result &= api->to_integer(api , -5 , &x);
    if (!result) {
        api->raise_error(api , "khra");
        return false;
    }
    DrawRectangle((int)x, (int)y, (int) width, (int) height, GetColor(hex_color));
    return true;
}

bool rdn_get_width(RDNApi* api) {
    api->push_integer(api , (int)GetScreenWidth());
    return true;
}

bool rdn_get_height(RDNApi* api) {
    api->push_integer(api , (int)GetScreenHeight());
    return true;
}

bool rdn_set_target_fps(RDNApi* api) {
    long fps;
    api->to_integer(api,-1,&fps);
    SetTargetFPS((int)fps);
    api->pop(api , 1);
    return true;
}

struct {
    const char* func_name;
    RDNNativeFunction func;
} reg_raylib[] = {

    {
        "rdn-init-window",
        rdn_init_window,
    },

    {
        "rdn-close-window",
        rdn_close_window,
    },

    {
        "rdn-begin-drawing",
        rdn_begin_drawing,
    },

    {
        "rdn-end-drawing",
        rdn_end_drawing,
    },

    {
        "rdn-window-should-close",
        rdn_window_should_close,
    },

    {
        "rdn-clear-background",
        rdn_clear_bg,
    },

    {
        "rdn-draw-rectangle",
        rdn_draw_rect,
    },

    {
        "rdn-get-width",
        rdn_get_width,
    },

    {
        "rdn-get-height",
        rdn_get_height,
    },

    {
        "rdn-set-target-fps",
        rdn_set_target_fps,
    },
};

#define LIB_REG_SIZE sizeof(reg_raylib) / sizeof(reg_raylib[0])

bool rdn_module_init(RDNModule *module) {

    for (size_t i = 0; i < LIB_REG_SIZE; ++i) {
        if(!module->register_function(module , reg_raylib[i].func_name , reg_raylib[i].func)) {
            return false;
        }
    }
    return true;
}
