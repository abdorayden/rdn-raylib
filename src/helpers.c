#ifndef HELPERS
#define HELPERS

#include "./raylib-6.0_linux_amd64/include/raylib.h"
#include "rdn/include/rdn.h"
#include "rdn/include/rdn_native.h"
#include <stdint.h>

#define REG_FUNC(func) {#func , (func)}
#define REG_TYPE struct { const char *func_name; RDNNativeFunction func; }
#define LIB_REG_SIZE sizeof(reg_raylib) / sizeof(reg_raylib[0])
#define RDN_SIG(name) bool name(RDNApi* api)

#define CONCAT_MAT(n) matrix.m##n = (float)l.items[(n)]->as.number
#define MAT_ADD(n) \
    ok &= api->push_number(api, (n));\
    ok &= api->list_append(api,-2,-1);\
    ok &= api->pop(api,1);

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

#endif // !HELPERS
