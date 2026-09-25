# rdn-raylib

[raylib](https://www.raylib.com/) bindings for **rdn** (Raden), written in C as a native module and
wrapped in rdn for a friendlier API.

Work in progress — see [API coverage](#api-coverage) and [Known issues](#known-issues).

## Requirements

- `gcc` (or another C11 compiler)
- `make`
- The **rdn** interpreter (`rdn`) on `PATH`
- Linux (the vendored raylib binary is `linux_amd64`)
- X11 and OpenGL at runtime — raylib needs an actual display to open a window

`src/compat.c` provides stubs for the C23 glibc symbols (`__isoc23_sscanf`, `__isoc23_strtol`,
`__isoc23_strtoul`) so the vendored raylib binary links against older toolchains, e.g. GCC 12 or 13.

## Layout

```
libs/RdnRaylib.rdn          rdn-side module: colors, flags, type aliases, defun wrappers
src/rdnraylib.c             native FFI layer — 113 rdn_* functions calling raylib
src/helpers.c               struct <-> rdn list conversion helpers
src/compat.c                glibc C23 symbol stubs
src/raylib-6.0_linux_amd64/ vendored raylib 6.0.0 (static lib + headers)
src/rdn/                    vendored rdn runtime (librdn.a, rdn.h, rdn_native.h)
nativelibs/rdnraylib.so     build output (gitignored)
test.rdn                    minimal example: opens a window and clears it to red
```

## Build

```sh
make          # builds nativelibs/rdnraylib.so
make test     # builds, then runs test.rdn
make clean    # removes the .so
```

The build is static: `libraylib.a` and `librdn.a` are linked into the shared object, so
`ldconfig` / `LD_LIBRARY_PATH` tweaks are not needed.

## Usage

```sh
make
rdn your-game.rdn
```

Minimal game loop:

```r
"./libs/RdnRaylib" load

RdnRaylib open

800 600 "rdn programming language" init-window call

window-should-close call ! loop

    begin-drawing call

    RED
    clear-background call

    end-drawing call

    window-should-close call !
end

close-window call
```

Arguments are pushed onto the rdn stack, then the wrapper is called with `call`; return values are
left on the stack for the next expression to consume.

## Naming

The rdn API is raylib's API in kebab-case, with the `rdn_` prefix dropped:

| raylib (C)             | rdn native symbol     | rdn                      |
| ---------------------- | --------------------- | ------------------------ |
| `InitWindow`           | `rdn_init_window`     | `init-window`            |
| `WindowShouldClose`    | `rdn_window_should_close` | `window-should-close` |
| `SetTargetFPS`         | `rdn_set_target_fps`  | `set-target-fps`         |
| `LoadShader`           | `rdn_load_shader`     | `load-shader`            |

`libs/RdnRaylib.rdn` adds the typechecking layer: signatures such as
`init-window (Int Int Str) () defun` and type aliases in `RdnRaylib::RlTypes`, plus runtime
validation before each native call.

## Colors

raylib's `Color` is a struct, so it is represented as either:

- a 4-element integer list — `RED`, `DARKGRAY`, `( 255 0 0 255 )`, or
- a pre-packed 32-bit integer.

The Rdn-side `RdnRaylib::Helpers::check-colors` runs before any color-taking call: it validates the
list length and element types, then packs `( r g b a )` into a single int. A non-list, non-integer
value raises a rdn error.

Constants for 21 named colors are exported directly on the module: `LIGHTGRAY`, `GRAY`, `DARKGRAY`,
`YELLOW`, `GOLD`, `ORANGE`, `PINK`, `RED`, `MAROON`, `GREEN`, `LIME`, `DARKGREEN`, `SKYBLUE`,
`BLUE`, `DARKBLUE`, `PURPLE`, `VIOLET`, `DARKPURPLE`, `BEIGE`, `BROWN`, `DARKBROWN`.

## API coverage

Currently bound:

- **Window** — init/close, resize, position, min/max size, opacity, title, icons, monitor, fullscreen,
  borderless, minimize/maximize/restore, focus, render size, `get-window-handle`
- **Window state** — `is-window-ready`, `is-window-full-screen`, `-hidden`, `-minimized`, `-maximized`,
  `-focused`, `-resized`, `is-window-state`, `set-window-state`, `clear-window-state`
  (flags in `RdnRaylib::ConfigFlags`, e.g. `VsyncHint`, `FullscreenMode`, `WindowResizable`)
- **Monitors** — count, current, position, size, physical size, refresh rate, name
- **Cursor** — show/hide, enable/disable, `is-cursor-hidden`, `is-cursor-on-screen`
- **Clipboard** — `set-clipboard-text`, `get-clipboard-text`, `get-clipboard-image`
- **Drawing** — `begin-drawing`, `end-drawing`, `clear-background`, `draw-rectangle`
- **Cameras** — `begin-mode-2d` / `-3d`, `get-camera-matrix`, `get-camera-matrix-2d`,
  screen-to-world and world-to-screen ray helpers
- **Shaders** — load, load-from-memory, validity, uniform/attrib locations, `set-shader-value`
  (`-v`, `-matrix`, `-texture`), unload
- **Drawing modes** — texture, blend, scissor, VR stereo, and `load-vrstereoconfig`
- **Timing** — `get-frame-time`, `get-time`, `get-fps`, `set-target-fps`, `wait-time`
- **Misc** — `take-screen-shot`, `open-url`, trace log level / callback, `set-config-flag`,
  `get-random-value`, random sequences, `mem-alloc` / `mem-realloc` / `mem-free`

Structs that cross the boundary (`Matrix`, `Vector2`, `Vector3`, `Ray`, `Camera2d`, `Camera3D`,
`Shader`, `Texture`, `VrStereoConfig`) are converted to and from rdn lists in `src/helpers.c`;
`Vector3` is `( x y z )`, `Matrix` is a flat 16-element list, etc.

### Not yet bound

- Input — keyboard, mouse buttons, mouse wheel, gamepads, touch
- Shapes — everything except rectangles (`DrawCircle`, `DrawLine`, …)
- Text and fonts — `DrawText`, `MeasureText`, `LoadFont`
- Textures and models — `LoadTexture`, `DrawTexture`, meshes, materials, `DrawModel`
- Collision detection
- Audio
- VR (`BeginVRStereoMode` is wired up, but the surrounding VR helpers are not)
- Raymath helpers, and the 3D primitives

## Known issues

Bugs found while working through the bindings, tracked but not yet fixed:

- **`draw-rectangle` is broken.** `libs/RdnRaylib.rdn` calls `rdn_draw_rectangle`, but the registered
  native symbol is `rdn_draw_rect` (`src/rdnraylib.c:63`). The call fails until the names match.
- **Blend / scissor / VR-stereo mode wrappers dispatch to the wrong function.** `begin-blend-mode`,
  `end-blend-mode`, `begin-scissor-mode`, `end-scissor-mode`, `begin-vr-stereo-mode`, and
  `end-vr-stereo-mode` in `libs/RdnRaylib.rdn` all call `rdn_begin_shader_mode` /
  `rdn_end_shader_mode`. The correct `rdn_begin_blend_mode` & co. are implemented and registered in
  C — only the rdn-side names are wrong.
- **`get-clipboard-text` has the wrong return type**, declared `(Str) () defun` instead of
  `() (Str) defun`.
- **`set-window-monitor`** is marked TODO: the rdn signature has a placeholder for the monitor
  argument.
- **Error messages are rough** in a few native functions (`"khra"`, `"idk some error"`).

## License

- Vendored raylib 6.0.0 — zlib, see `src/raylib-6.0_linux_amd64/LICENSE`
- Vendored rdn runtime — MIT, (c) 2023-2026 Ray Den, see `src/rdn/include/rdn_native.h`
- The bindings themselves have no separate license file yet
