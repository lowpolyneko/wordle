{ pkgs }: {
	deps = [
		pkgs.less
  pkgs.run
  pkgs.file
  pkgs.vim
  pkgs.valgrind
  pkgs.neovim
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}