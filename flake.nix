{
  description = "dwm dev shell: pre-commit and the build of the make-build hook";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        # The hooks and their build run here. mkShell brings the C compiler
        # of stdenv. config.mk asks pkg-config for the X11 and font flags, so
        # the build takes no header from the host. build_install.sh stays a
        # host build, because it installs the window manager of the host.
        devShells.default = pkgs.mkShell {
          packages = [
            pkgs.pre-commit
            # rustup needs curl when pre-commit sets up the environment of
            # the ripsecrets hook. pre-commit installs rust itself.
            pkgs.curl
            pkgs.gnumake
            pkgs.pkg-config
            pkgs.libx11
            pkgs.libxinerama
            pkgs.libxft
            pkgs.libxrender
            pkgs.fontconfig
            pkgs.freetype
          ];
        };
      }
    );
}
