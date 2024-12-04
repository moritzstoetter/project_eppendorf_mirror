set -e

cmake --preset debug
conan install . -pr=esp32c3 -if=_build/debug --build=missing
cmake --preset debug
cmake --build --preset debug
