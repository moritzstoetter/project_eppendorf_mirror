#!/bin/bash

set -e

CONAN_STORAGE_PATH=$HOME/.conan/data_container
CI_IMAGE=polaris_edf_idf_ci

if [ -z "${CONAN_LOGIN_USERNAME}" ] || [ -z "${CONAN_PASSWORD}" ]; then
    echo "Error: CONAN_LOGIN_USERNAME and/or CONAN_PASSWORD environment variables are not set"
    exit 1
fi

mkdir -p ${CONAN_STORAGE_PATH}

podman run -it --rm --userns=keep-id --group-add=keep-groups \
  -e=CONAN_STORAGE_PATH=${CONAN_STORAGE_PATH} \
  -e=CONAN_LOGIN_USERNAME=${CONAN_LOGIN_USERNAME} \
  -e=CONAN_PASSWORD=${CONAN_PASSWORD} \
  --mount=type=bind,source=${HOME}/.ssh,target=/home/user/.ssh \
  --mount=type=bind,source=${PWD},target=${PWD} \
  --mount=type=bind,source=${CONAN_STORAGE_PATH},target=${CONAN_STORAGE_PATH} \
  -p 2222:22 \
  ${CI_IMAGE}