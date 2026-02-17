# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

This is a customized build of dwm (dynamic window manager) v6.8 from suckless.org — a minimal X11 tiling window manager written in C99.

## Build Commands

```sh
make              # Build dwm binary
make clean        # Remove compiled objects and binary
sudo make install # Install to /usr/local/bin and man page to /usr/local/share/man
```

There are no tests or linter. The project compiles with `-std=c99 -pedantic -Wall -Os`.

## Configuration Workflow

dwm uses compile-time configuration — there are no runtime config files.

- `config.def.h` — upstream default configuration template
- `config.h` — your actual configuration (generated from `config.def.h` on first build, then manually edited)

**To change configuration:** edit `config.h` directly, then rebuild with `make clean && make`. Do NOT edit `config.def.h` for personal customizations — that file preserves upstream defaults.

This config uses `Mod4` (Super key) as MODKEY, `rofi` as the launcher, and `kitty` as the terminal.

## Architecture

The entire window manager is ~2,200 lines in a single file (`dwm.c`) plus a small drawing library:

- **dwm.c** — all window management: X11 event loop, client/monitor management, layouts (tile, monocle, float), keybinding dispatch, EWMH/ICCCM compliance
- **drw.c / drw.h** — drawing abstraction: font rendering (Xft/fontconfig), color schemes, text/rectangle drawing to off-screen pixmaps
- **util.c / util.h** — `die()` (fatal error), `ecalloc()` (checked allocation), `MAX`/`MIN`/`BETWEEN`/`LENGTH` macros
- **config.mk** — compiler flags, library paths, Xinerama toggle

Key data structures: `Client` (per-window state in a linked list), `Monitor` (per-display with its own client list and layout), `Drw` (drawing context). Event dispatch uses a handler array indexed by X event type for O(1) lookup.

## Dependencies

Requires: libX11, libXft, libfontconfig. Optional: libXinerama (multi-monitor, enabled by default in `config.mk`).
