# Build Instructions

## Container

Check out the container configuration from [https://gitlab.ep-dev.net/polaris/container-images/polaris-esp-idf-ci-image.git](https://gitlab.ep-dev.net/polaris/container-images/polaris-esp-idf-ci-image.git), then build it with:

```bash 
podman build -t polaris_edf_idf_ci --build-arg BASE_IMAGE=docker.io/espressif/idf:release-v5.3 .
```

## Firmware

Start the container by doing `./start_container.sh` from the repos root. Then connect to via ssh with `ssh user@localhost -p2222` (password: _test_), finally in the container do:

```bash  
cmake --preset debug
cmake --build --preset debug 
```

or run the convenience script:

```bash 
./do.sh
```