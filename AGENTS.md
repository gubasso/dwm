# Repository Guidelines

## Project Structure & Module Organization
This repository is a flat C codebase for a customized `dwm` v6.8 build.
- Core sources: `dwm.c` (window manager logic), `drw.c`/`drw.h` (drawing), `util.c`/`util.h` (helpers).
- Build/config: `Makefile`, `config.mk`, `config.def.h`.
- Docs/assets: `README`, `dwm.1`, `dwm.png`, `LICENSE`.
There is no `src/` or `tests/` directory; keep additions at repo root unless a new subsystem justifies structure changes.

## Build, Test, and Development Commands
- `make`: build the local `dwm` binary.
- `make clean`: remove compiled objects and binary.
- `make clean && make`: clean rebuild before review.
- `make install`: install binary and man page (respects `PREFIX`/`DESTDIR`; may require root).
- `make uninstall`: remove installed files.
- `make clean install`: upstream-recommended full build+install flow.
- `nix develop --command pre-commit run --all-files`: run the pre-commit hooks in the dev shell of `flake.nix`. `.envrc` loads the same shell with direnv.
- `nix develop --command pre-commit run --all-files --hook-stage pre-push`: run the pre-push hooks, which add `gitleaks` and the `make-build` hook. `make-build` runs `make clean && make` with the compiler and the libraries of the dev shell.
- `./build_install.sh`: build on the host and install with `sudo make install`. It runs outside the dev shell, because it installs the window manager of the host.

## Coding Style & Naming Conventions
- Follow existing C99 toolchain settings from `config.mk` (`-std=c99 -pedantic -Wall`).
- Match established naming: `lowercase_with_underscores` for functions/locals, `UPPERCASE` for macros.
- Keep edits minimal and local; avoid unrelated reformatting.
- In `Makefile`, recipe lines must use tabs, not spaces.

## Testing Guidelines
There is no automated test framework in this repo. Minimum validation for each change:
1. Run `make clean && make` and confirm successful compilation.
2. Run a manual smoke test in X (`exec dwm` from `.xinitrc`).
3. For keybinding/layout/config changes, verify expected behavior interactively.

## Commit & Pull Request Guidelines
- Use short, imperative commit subjects consistent with history (for example: `dwm: Fix ...`, `drw.c: ...`, `bump version to ...`).
- Keep commits scoped to one logical change.
- PRs should include: purpose, behavior changes, commands run to validate, and screenshots only when UI/bar/layout output changes.

## Configuration & Agent Notes
- `config.def.h` is the single source of truth for configuration (tracked in git). `config.h` is auto-generated from it and gitignored.
- Review `CLAUDE.md` before substantial changes to align with repository-specific architecture and workflow guidance.
