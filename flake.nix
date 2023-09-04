{
  description = "STM32 project";

  inputs = {
      nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs:
    let
      system = "x86_64-linux";
      pkgs = import inputs.nixpkgs {
          inherit system;
          config.allowUnfree = true;
      };
      llvm = pkgs.llvmPackages_latest;
      buildInputs = with pkgs; [
        # build tools
        cmake
        gnumake
        makeWrapper
        gdb

        # stm32 tools
        (gcc-arm-embedded.overrideAttrs (final: prev: {
          dontPatchELF = false;
        }))
        stlink

        # dev environment (clangd)
        clang-tools
        llvm.clang
        
        # libraries
        glibc_multi

        # python for gdb script
        python3
      ];
    in
    {
      defaultPackage.${system} = pkgs.stdenv.mkDerivation {
        name = "stm32";
        inherit buildInputs;
        src = ./.;
      };

      devShell.${system} = pkgs.mkShell {
        buildInputs = buildInputs ++ [
          pkgs.pkg-config
        ];
        shellHook = ''
          export GCC_TOOLCHAIN_DIR=${pkgs.gcc-arm-embedded}
          export PYTHON_EXE=${pkgs.python3}
        '';
      };
    };
}
