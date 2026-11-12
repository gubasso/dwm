{
  description = "dwm dev shell: the build of the make-build hook";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    # The store guard of the canon (exobrain): `exobrain store-guard --daily`
    # in .envrc warns about working-tree copies in the Nix store.
    exobrain-tech = {
      url = "github:gubasso/exobrain-tech/v1.1.0";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      nixpkgs,
      flake-utils,
      exobrain-tech,
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
            pkgs.gnumake
            pkgs.pkg-config
            pkgs.libx11
            pkgs.libxinerama
            pkgs.libxft
            pkgs.libxrender
            pkgs.fontconfig
            pkgs.freetype
          ]
          # exobrain, the store guard that .envrc runs once a day.
          ++ pkgs.lib.optionals (system == "x86_64-linux") [
            exobrain-tech.packages.${system}.default
          ];
        };
      }
    );
}
