{
  description = "AGS dev flake";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    # firefox.url = "github:nix-community/flake-firefox-nightly";
    flake-utils.url = "github:numtide/flake-utils";
    browser-previews.url = "github:nix-community/browser-previews";

  };
  outputs = { self, nixpkgs, flake-utils, browser-previews }:
  flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
        config = {
          allowUnfree = true;
        };
      };
    in         
      {
        devShell = with pkgs; mkShell rec {
          buildInputs =  [
            browser-previews.packages.${pkgs.system}.google-chrome-dev # google-chrome-unstable --enable-features=Vulkan
            # firefox.packages.${pkgs.system}.firefox-nightly-bin

            llvmPackages_17.openmp
            xorg.libX11
            llvmPackages_17.libcxx
            boost

            clang-tools
            bear
            binutils
            cmake
            # gnumake

          ];
        };
    });
}


