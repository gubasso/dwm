# dwm Custom Notes

## Branches

- `rice` is the main branch of this fork. It carries the patches and `config.def.h`, and every install path builds it.
- `master` only mirrors the official upstream dwm, `https://git.suckless.org/dwm`. Nothing of this fork lands on it.

## Install

This fork has two install paths. Pick the one that fits the host.

### The operator's hosts: nix-secrets

Use this path on a host that the operator's nix-secrets repository manages. That repository builds this fork through Home Manager on every such host.

- `home/apps/dwm/flake.nix` in nix-secrets pins the `rice` branch of this repository.
- nix-secrets builds the pinned source with the `dwm` package of nixpkgs, and installs the binary into the Home Manager profile.
- A change of `rice` reaches a host in three steps: push `rice`, advance the pin in nix-secrets with `just update-pins`, and activate Home Manager. Then restart dwm.

Do not run `./build_install.sh` on such a host. Its binary goes to `/usr/local/bin`, and the session does not run it, because the Home Manager profile comes first on `PATH`.

### Another host: build_install.sh

Use this path on a host without the nix-secrets setup:

```sh
./build_install.sh
```

The script builds with `/usr/bin/cc`, the C compiler of the host, and installs to `PREFIX` (`/usr/local`) with `sudo make install`. It names `/usr/bin/cc` because a Nix compiler first on `PATH` does not search `/usr/include`. The host needs these:

- A C compiler at `/usr/bin/cc`, and `make`.
- The development headers of X11, Xinerama, Xft, and fontconfig.
- `pkg-config` at `/usr/bin/pkg-config`, which is optional. Without it, `config.mk` uses its fixed include and library paths.

### Which binary runs

The two paths install different binaries: the Home Manager profile holds one, and `/usr/local/bin/dwm` holds the other. The session runs the binary that its startup file names. A startup file that runs `dwm` by its bare name gets the first `dwm` on `PATH`. To see which binary that is, run `command -v dwm` in the session.

## Dev Shell and Hooks

`flake.nix` holds the dev shell: `pre-commit`, `make`, the compiler, `pkg-config`, and the X11 and font libraries. `.envrc` loads it with direnv. Run the hooks in it:

```sh
nix develop --command pre-commit install
nix develop --command pre-commit run --all-files
nix develop --command pre-commit run --all-files --hook-stage pre-push
```

The pre-push stage adds `gitleaks` and a `make clean && make` build in the dev shell. The dev shell is for development. It does not install dwm, see "Install".

## Git Workflow

The upstream mirror is `master` everywhere: local, origin, and upstream. The fork's work is on `rice`, see "Branches".

Remote setup:

```sh
git remote add upstream https://git.suckless.org/dwm
git remote set-url origin gh.gubasso:gubasso/dwm.git
```

Verify remotes:

```sh
git remote -v
# origin    gh.gubasso:gubasso/dwm.git (fetch)
# origin    gh.gubasso:gubasso/dwm.git (push)
# upstream  https://git.suckless.org/dwm (fetch)
# upstream  https://git.suckless.org/dwm (push)
```

Sync and rebase workflow:

```sh
git fetch upstream
git checkout master
git merge upstream/master
git push origin master
git checkout rice
git rebase master
git push origin rice --force-with-lease
```

No branch-name bridge is needed because all repos use master.

## What Rebase Does (LEGO Analogy)

Imagine you're building a LEGO castle.

The official LEGO instructions are the `main` branch. That's what the LEGO
company (suckless) publishes. Sometimes they release new pages with better
pieces or fixes.

Your custom castle is the `rice` branch. You took the official instructions and
added your own cool stuff on top - a purple tower, a secret door, extra flags.

Now the LEGO company releases a new page of instructions. You want those
improvements, but you also want to keep your custom stuff.

Here's what rebase does, step by step:

1. You grab the new official pages (`git fetch upstream` + merge into `main`)
2. You go to your custom castle (`git checkout rice`)
3. Rebase says: "OK, let me temporarily remove all your custom pieces and set
   them aside on the table"
4. Then it rebuilds the castle using the new official instructions from the
   beginning
5. Then it puts your custom pieces back on top, one by one, in the same order
   you originally added them

Visually:

```text
BEFORE (your rice is based on old main):

main:  A --- B --- C
                    \
rice:                D --- E --- F  (your patches)

After main gets updated from upstream:

main:  A --- B --- C --- G --- H  (new official stuff)
                    \
rice:                D --- E --- F  (still based on old C)

After rebase:

main:  A --- B --- C --- G --- H
                                 \
rice:                             D' --- E' --- F'  (your same patches, now on top of H)
```

The tricky part: sometimes when rebase is putting your pieces back, a new
official piece goes in the exact same spot where you put a custom piece. That's
a conflict - and git stops and asks you: "Hey, which piece do you want here,
theirs or yours?" You pick, and it continues.

That's it. Rebase just means "replay my changes on top of the latest base."

## Rebase Step-by-Step Cheatsheet

### Step 1: Fetch New Stuff from Suckless

```sh
git fetch upstream
git checkout master
git merge upstream/master
```

What happened:

```text
master:  A --- B --- C                        (before)
master:  A --- B --- C --- G --- H            (after)

rice:                 D --- E --- F           (untouched, still based on C)
```

### Step 2: Push Updated `master` to Your GitHub Fork

```sh
git push origin master
```

Now your fork on GitHub also has `G` and `H`.
`rice` is still untouched.

### Step 3: Rebase `rice` on Top of New `master`

```sh
git checkout rice
git rebase master
```

What rebase does internally:

1. Removes your patches temporarily:

```text
master:  A --- B --- C --- G --- H
rice patches on the table: [D] [E] [F]
```

2. Points `rice` to the tip of `master`:

```text
rice:  A --- B --- C --- G --- H
```

3. Replays your patches one by one:

```text
rice:  A --- B --- C --- G --- H --- D'
rice:  A --- B --- C --- G --- H --- D' --- E'
rice:  A --- B --- C --- G --- H --- D' --- E' --- F'
```

Done:

```text
master:  A --- B --- C --- G --- H
                                   \
rice:                               D' --- E' --- F'
```

If a conflict happens during replay:

```sh
git status                    # see which files conflict
vim <conflicted file>         # fix it (pick your piece or theirs)
git add <fixed file>          # mark as resolved
git rebase --continue         # continue replaying
```

If you want to abort:

```sh
git rebase --abort            # go back to before rebase, no damage
```

### Step 4: Push `rice` to Your GitHub Fork

```sh
git push origin rice --force-with-lease
```

Why `--force-with-lease`?

Rebase rewrites history (`D` became `D'`, `E` became `E'`, etc.). GitHub sees
different commits and rejects a normal push. `--force-with-lease` forces it but
checks nobody else pushed in the meantime (safer than `--force`).

### Full Cheat Sheet

```sh
git fetch upstream
git checkout master
git merge upstream/master
git push origin master
git checkout rice
git rebase master
# resolve conflicts if any
git push origin rice --force-with-lease
```
