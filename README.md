# Build Instructions

## In Container

Build the container with attached `Dockerfile`, then in it in folder `project` do:

```bash
cmake --preset debug
cmake --build --preset debug
```

## On the Host 

Checkout the ESP-IDF and isntall the tools ([instructions](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html)).

Make sure the following environment variables are set: 

```bash
export IDF_TARGET=esp32c3
export IDF_PATH="<>"
export IDF_TOOLS_PATH="<>"
export IDF_CCACHE_ENABLE=1

```

Then bootstrap the development environment:

```bash
$IDF_PATH/install.sh $IDF_TARGET
source $IDF_PATH/export.sh
```

Finally in `project` do:

```bash
cmake --preset debug
cmake --build --preset debug
```