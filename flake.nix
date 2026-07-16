{
  description = "QMK Firmware build environment for Keychron K2 HE";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };

      pythonEnv = pkgs.python3.withPackages (ps: with ps; [
        appdirs
        argcomplete
        colorama
        dotty-dict
        hid
        hjson
        jsonschema
        milc
        pygments
        pyserial
        pyusb
        pillow
      ]);
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        name = "qmk-firmware";

        buildInputs = with pkgs; [
          clang-tools
          dfu-programmer
          dfu-util
          diffutils
          git
          gnumake
          pythonEnv
          qmk
          gcc-arm-embedded
        ];

        shellHook = ''
          unset NIX_CFLAGS_COMPILE_FOR_TARGET
        '';
      };
    };
}
