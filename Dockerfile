# docker run --rm -v $PWD/project:/project:Z -w /project eppendorf-esp-firmware /bin/bash -c "cmake --preset debug; cmake --build --preset debug"

FROM espressif/idf:latest

ENV IDF_TARGET="esp32c3"
