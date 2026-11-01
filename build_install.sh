#!/usr/bin/env sh
# Build dwm with the compiler of the host, and install it to PREFIX
# (/usr/local) with sudo. This is the install path of a host without the Nix
# setup. See "Install" in README.md.
#
# The build names /usr/bin/cc and /usr/bin/pkg-config instead of the first cc
# and pkg-config on PATH. A Nix compiler on PATH, for example the gcc wrapper
# of a Home Manager profile, does not search /usr/include, so it does not find
# the X11 headers of the host. Without /usr/bin/pkg-config, config.mk falls
# back to its fixed paths.
set -eu

host_cc=/usr/bin/cc
host_pkg_config=/usr/bin/pkg-config

if [ ! -x "$host_cc" ]; then
	echo "build_install.sh: $host_cc does not exist. Install the C compiler of the host." >&2
	exit 1
fi

make clean && make CC="$host_cc" PKG_CONFIG="$host_pkg_config"
sudo make install CC="$host_cc" PKG_CONFIG="$host_pkg_config"
