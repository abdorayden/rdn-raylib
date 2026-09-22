#include <assert.h>
#include <rdn.h>
#include <rdn_native.h>
#include <stddef.h>
#include <stdint.h>
#include "./raylib-6.0_linux_amd64/include/raylib.h"
#include "rdn/include/rdn.h"
#include "rdn/include/rdn_native.h"

#define REG_FUNC(func) {#func , (func)}
#define REG_TYPE struct { const char *func_name; RDNNativeFunction func; }
#define LIB_REG_SIZE sizeof(reg_raylib) / sizeof(reg_raylib[0])

#define RDN_SIG(name) bool name(RDNApi* api)

// helpers

#define CONCAT_MAT(n) matrix.m##n = (float)l.items[(n)]->as.number

Matrix rdn_list_to_matrix(Value* value , bool* ok) {
    Matrix matrix = {0};
    RDNValueList l = value->as.list;

    if(l.count != 16) {
        *ok = false;
        return matrix;
    }

    for (size_t i = 0; i < l.count; ++i) {
        if(l.items[i]->type != VALUE_DOUBLE) {
            *ok = false;
            return matrix;
        }
    }

    CONCAT_MAT(0); CONCAT_MAT(1); CONCAT_MAT(2);
    CONCAT_MAT(3); CONCAT_MAT(4); CONCAT_MAT(5);
    CONCAT_MAT(6); CONCAT_MAT(7); CONCAT_MAT(8);
    CONCAT_MAT(9); CONCAT_MAT(10); CONCAT_MAT(11);
    CONCAT_MAT(12); CONCAT_MAT(13); CONCAT_MAT(14);
    CONCAT_MAT(15);

    return matrix;
}

#define MAT_ADD(n) \
    ok &= api->push_number(api, (n));\
    ok &= api->list_append(api,-2,-1);\
    ok &= api->pop(api,1);


bool rdn_matrix_to_list(RDNApi* api, Matrix matrix) {
    bool ok = true;
    api->push_list(api);
    MAT_ADD(matrix.m0); MAT_ADD(matrix.m1); MAT_ADD(matrix.m2); MAT_ADD(matrix.m3);
    MAT_ADD(matrix.m4); MAT_ADD(matrix.m5); MAT_ADD(matrix.m6); MAT_ADD(matrix.m7);
    MAT_ADD(matrix.m8); MAT_ADD(matrix.m9); MAT_ADD(matrix.m10); MAT_ADD(matrix.m11);
    MAT_ADD(matrix.m12); MAT_ADD(matrix.m13); MAT_ADD(matrix.m14); MAT_ADD(matrix.m15);
    return ok;
}

bool rdn_vrstereoconfig_to_list(RDNApi* api, VrStereoConfig vrstereoconfig) {
    bool ok = true;
    api->push_list(api);

    // matrix[] = (
    // () -> rdn_matrix_to_list(api, vrstereoconfig.projection[0]);
    // () -> rdn_matrix_to_list(api, vrstereoconfig.projection[1]);
    // )
    api->push_list(api);

    ok = rdn_matrix_to_list(api, vrstereoconfig.projection[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok = rdn_matrix_to_list(api, vrstereoconfig.projection[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // matrix[] = (
    // () -> rdn_matrix_to_list(api, vrstereoconfig.viewOffset[0]);
    // () -> rdn_matrix_to_list(api, vrstereoconfig.viewOffset[1]);
    // )
    api->push_list(api);

    ok = rdn_matrix_to_list(api, vrstereoconfig.viewOffset[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok = rdn_matrix_to_list(api, vrstereoconfig.viewOffset[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // float[] = () -> leftLensCenter
    api->push_list(api);
    api->push_number(api, vrstereoconfig.leftLensCenter[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    api->push_number(api, vrstereoconfig.leftLensCenter[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // float[] = () -> rightLensCenter
    api->push_list(api);
    api->push_number(api, vrstereoconfig.rightLensCenter[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    api->push_number(api, vrstereoconfig.rightLensCenter[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // float[] = () -> leftScreenCenter
    api->push_list(api);
    api->push_number(api, vrstereoconfig.leftScreenCenter[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    api->push_number(api, vrstereoconfig.leftScreenCenter[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // float[] = () -> rightScreenCenter
    api->push_list(api);
    api->push_number(api, vrstereoconfig.rightScreenCenter[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    api->push_number(api, vrstereoconfig.rightScreenCenter[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // float[] = () -> scale
    api->push_list(api);
    api->push_number(api, vrstereoconfig.scale[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    api->push_number(api, vrstereoconfig.scale[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    // float[] = () -> scaleIn
    api->push_list(api);
    api->push_number(api, vrstereoconfig.scaleIn[0]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    api->push_number(api, vrstereoconfig.scaleIn[1]);
    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);

    ok &= api->list_append(api,-2,-1);
    ok &= api->pop(api,1);


    return ok;
}

VrDeviceInfo rdn_list_to_vrdeviceinfo(Value* value , bool* ok) {
    VrDeviceInfo vrdeviceinfo = {0};
    RDNValueList l = value->as.list;

    if (
            l.count != 9

            && l.items[0]->type != VALUE_INTEGER
            && l.items[1]->type != VALUE_INTEGER

            && l.items[2]->type != VALUE_DOUBLE
            && l.items[3]->type != VALUE_DOUBLE
            && l.items[4]->type != VALUE_DOUBLE
            && l.items[5]->type != VALUE_DOUBLE
            && l.items[6]->type != VALUE_DOUBLE


            && l.items[7]->type != VALUE_LIST
            && l.items[7]->as.list.count != 4
            && l.items[7]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[7]->as.list.items[1]->type != VALUE_DOUBLE
            && l.items[7]->as.list.items[2]->type != VALUE_DOUBLE
            && l.items[7]->as.list.items[3]->type != VALUE_DOUBLE

            && l.items[8]->type != VALUE_LIST
            && l.items[8]->as.list.count != 4
            && l.items[8]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[8]->as.list.items[1]->type != VALUE_DOUBLE
            && l.items[8]->as.list.items[2]->type != VALUE_DOUBLE
            && l.items[8]->as.list.items[3]->type != VALUE_DOUBLE

       ) 
    {
        *ok = false;
        return vrdeviceinfo;
    }

    vrdeviceinfo.hResolution = l.items[0]->as.integer;
    vrdeviceinfo.vResolution = l.items[1]->as.integer;

    vrdeviceinfo.hScreenSize = l.items[2]->as.number;
    vrdeviceinfo.vScreenSize = l.items[3]->as.integer;

    vrdeviceinfo.eyeToScreenDistance = l.items[4]->as.number;
    vrdeviceinfo.lensSeparationDistance = l.items[5]->as.integer;
    vrdeviceinfo.interpupillaryDistance = l.items[6]->as.integer;

    vrdeviceinfo.lensDistortionValues[0] = l.items[7]->as.list.items[0]->as.number;
    vrdeviceinfo.lensDistortionValues[1] = l.items[7]->as.list.items[1]->as.number;
    vrdeviceinfo.lensDistortionValues[2] = l.items[7]->as.list.items[2]->as.number;
    vrdeviceinfo.lensDistortionValues[3] = l.items[7]->as.list.items[3]->as.number;

    vrdeviceinfo.chromaAbCorrection[0] = l.items[8]->as.list.items[0]->as.number;
    vrdeviceinfo.chromaAbCorrection[1] = l.items[8]->as.list.items[1]->as.number;
    vrdeviceinfo.chromaAbCorrection[2] = l.items[8]->as.list.items[2]->as.number;
    vrdeviceinfo.chromaAbCorrection[3] = l.items[8]->as.list.items[3]->as.number;

    return vrdeviceinfo;
}

VrStereoConfig rdn_list_to_vrstereoconfig(Value* value , bool* ok) {
    VrStereoConfig vrstereoconfig = {0};
    RDNValueList l = value->as.list;

    if(
            l.count != 8
            && l.items[0]->type != VALUE_LIST
            && l.items[0]->as.list.count != 2
            && l.items[1]->type != VALUE_LIST
            && l.items[1]->as.list.count != 2

            && l.items[2]->type != VALUE_LIST
            && l.items[2]->as.list.count != 2
            && l.items[2]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[2]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[3]->type != VALUE_LIST
            && l.items[3]->as.list.count != 2
            && l.items[3]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[3]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[4]->type != VALUE_LIST
            && l.items[4]->as.list.count != 2
            && l.items[4]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[4]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[5]->type != VALUE_LIST
            && l.items[5]->as.list.count != 2
            && l.items[5]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[5]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[6]->type != VALUE_LIST
            && l.items[6]->as.list.count != 2
            && l.items[6]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[6]->as.list.items[1]->type != VALUE_DOUBLE

            && l.items[7]->type != VALUE_LIST
            && l.items[7]->as.list.count != 2
            && l.items[7]->as.list.items[0]->type != VALUE_DOUBLE
            && l.items[7]->as.list.items[1]->type != VALUE_DOUBLE
      ){
        *ok = false;
        return vrstereoconfig;
    }

    // i think it's safe
    vrstereoconfig.projection[0] = rdn_list_to_matrix(l.items[0]->as.list.items[0], ok);
    if (!*ok) {
        return vrstereoconfig;
    }
    vrstereoconfig.projection[1] = rdn_list_to_matrix(l.items[0]->as.list.items[1], ok);
    if (!*ok) {
        return vrstereoconfig;
    }
    vrstereoconfig.viewOffset[0] = rdn_list_to_matrix(l.items[1]->as.list.items[0], ok);
    if (!*ok) {
        return vrstereoconfig;
    }
    vrstereoconfig.viewOffset[1] = rdn_list_to_matrix(l.items[1]->as.list.items[1], ok);
    if (!*ok) {
        return vrstereoconfig;
    }

    vrstereoconfig.leftLensCenter[0] = l.items[2]->as.list.items[0]->as.number;
    vrstereoconfig.leftLensCenter[1] = l.items[2]->as.list.items[1]->as.number;

    vrstereoconfig.rightLensCenter[0] = l.items[3]->as.list.items[0]->as.number;
    vrstereoconfig.rightLensCenter[1] = l.items[3]->as.list.items[1]->as.number;

    vrstereoconfig.leftScreenCenter[0] = l.items[4]->as.list.items[0]->as.number;
    vrstereoconfig.leftScreenCenter[1] = l.items[4]->as.list.items[1]->as.number;

    vrstereoconfig.rightScreenCenter[0] = l.items[5]->as.list.items[0]->as.number;
    vrstereoconfig.rightScreenCenter[1] = l.items[5]->as.list.items[1]->as.number;

    vrstereoconfig.scale[0] = l.items[6]->as.list.items[0]->as.number;
    vrstereoconfig.scale[1] = l.items[6]->as.list.items[1]->as.number;

    vrstereoconfig.scaleIn[0] = l.items[7]->as.list.items[0]->as.number;
    vrstereoconfig.scaleIn[1] = l.items[7]->as.list.items[1]->as.number;

    return vrstereoconfig;
}

Image rdn_list_to_image(Value* value , bool* ok) {
    Image image = {0};
    RDNValueList l = value->as.list;
    if(
            l.count != 5
            && l.items[0]->type != VALUE_INTEGER
            && l.items[1]->type != VALUE_INTEGER
            && l.items[2]->type != VALUE_INTEGER
            && l.items[3]->type != VALUE_INTEGER
            && l.items[4]->type != VALUE_INTEGER
      ){
        *ok = false;
        return image;
    }

    image.data = (void*)(uintptr_t)l.items[0]->as.integer;
    image.width = (int)l.items[1]->as.integer;
    image.height = (int)l.items[2]->as.integer;
    image.mipmaps = (int)l.items[3]->as.integer;
    image.format = (int)l.items[4]->as.integer;
    return image;
}

bool rdn_shader_to_list(RDNApi* api, Shader shader) {
    bool ok = true;

    ok = api->push_list(api);

    ok = api->push_integer(api,shader.id);
    ok = api->list_append(api,-2,-1);
    ok = api->pop(api,1);

    ok = api->push_integer(api, (long)(uintptr_t)shader.locs);
    ok = api->list_append(api,-2,-1);
    ok = api->pop(api,1);

    return ok;
}

Shader rdn_list_to_shader(Value* value, bool* ok) {
    Shader shader = {0};
    RDNValueList l = value->as.list;
    if(
            l.count != 2
            && l.items[0]->type != VALUE_INTEGER
            && l.items[1]->type != VALUE_INTEGER
      ) {
        *ok = false;
        return shader;
    }

    shader.id = l.items[0]->as.integer;
    // WARNING: this propebly will filled by other raylib function
    // i don't know, is it safe ??
    shader.locs = (int*)(uintptr_t)l.items[0]->as.integer;
    return shader;
}

Texture rdn_list_to_texture(Value* value , bool* ok) {
    Texture texture = {0};
    RDNValueList l = value->as.list;
    if (
            l.count != 5
            && l.items[0]->type != VALUE_INTEGER 
            && l.items[1]->type != VALUE_INTEGER 
            && l.items[2]->type != VALUE_INTEGER 
            && l.items[3]->type != VALUE_INTEGER 
            && l.items[4]->type != VALUE_INTEGER 
            ) {
        *ok = false;
        return texture;
    }
    texture.id = l.items[0]->as.integer;
    texture.width = l.items[1]->as.integer;
    texture.height = l.items[2]->as.integer;
    texture.mipmaps = l.items[3]->as.integer;
    texture.format = l.items[4]->as.integer;
    return texture;
}

RenderTexture rdn_list_to_render_texture(Value* value, bool* ok) {
    RenderTexture render_texture = {0};
    RDNValueList l = value->as.list;

    if (
            l.count != 3
            && l.items[0]->type != VALUE_INTEGER
            && l.items[1]->type != VALUE_LIST
            && l.items[2]->type != VALUE_LIST
       ) {
        *ok = false;
        return render_texture;
    }

    render_texture.id = l.items[0]->as.integer;
    render_texture.texture = rdn_list_to_texture(l.items[1], ok);
    if (!*ok) {
        return render_texture;
    }
    render_texture.depth = rdn_list_to_texture(l.items[2], ok);
    if (!*ok) {
        return render_texture;
    }
    return render_texture;
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
