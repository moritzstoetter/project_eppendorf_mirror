#!/bin/bash

set -e

export CONAN_STORAGE_PATH=$HOME/.conan/data_container
export CI_IMAGE=polaris_edf_idf_ci

mkdir -p ${CONAN_STORAGE_PATH}

podman run -it --rm --userns=keep-id --group-add=keep-groups \
  -e=CONAN_STORAGE_PATH=${CONAN_STORAGE_PATH} \
  --mount=type=bind,source=${CONAN_STORAGE_PATH},target=${CONAN_STORAGE_PATH} \
  -p 2222:22 \
  ${CI_IMAGE}

# TODO: Setup conan in container
# conan user -r skynet -p
# conan user -r skynet-dev -p
# conan config set storage.path="${CONAN_STORAGE_PATH}"
