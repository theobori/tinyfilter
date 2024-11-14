{
  stdenv,
  lib,
  xdp-tools,
  clang,
  libbpf,
  zlib,
  elfutils,
  pkgsi686Linux,
  llvm_18,
}:
stdenv.mkDerivation {
  pname = "tinyfilter";
  version = "0.0.1";

  src = ./.;

  buildInputs = [
    xdp-tools
    clang
    libbpf
    zlib
    elfutils
    pkgsi686Linux.glibc
    llvm_18
  ];

  hardeningDisable = [
    "stackprotector"
    "zerocallusedregs"
  ];

  installPhase = ''
    mkdir -p $out/bin

    cp tinyfilter $out/bin
  '';

  meta = {
    description = "XDP packet filtering for OSI model layers 2, 3 and 4 ";
    homepage = "https://github.com/theobori/tinyfilter";
    license = lib.licenses.mit;
    mainProgram = "tinyfilter";
  };
}
