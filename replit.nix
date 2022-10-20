{ pkgs }: {
	deps = [
		pkgs.neovim
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}