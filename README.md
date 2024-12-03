# Build Instructions

## Container

Check out the container configuration from [https://gitlab.ep-dev.net/polaris/container-images/polaris-esp-idf-ci-image.git](https://gitlab.ep-dev.net/polaris/container-images/polaris-esp-idf-ci-image.git), then build it with:

```bash 
podman build -t polaris_edf_idf_ci --build-arg BASE_IMAGE=docker.io/espressif/idf:release-v5.3 .
```

## Firmware

Run the container by doing `./run_in_docker.sh` from the repos root. Then in the container:

### Setup conan in container

```bash  
conan user -r skynet -p
```
```bash
conan user -r skynet-dev -p
```
```bash
conan config set storage.path="${CONAN_STORAGE_PATH}"
```

### Build 

Run `./do/sh`