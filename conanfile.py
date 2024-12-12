from conans import ConanFile, CMake
import os


class Polaris(ConanFile):
    name = "esp-firmware"
    url = "https://scm-ih-02.eppendorf.de/polaris/esp-firmware"
    description = "The esp-firmware project."
    version = "0.0.1"
    license = "Internal"
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "EdfCMakeToolchainGenerator/0.0.2@edf/stable",
        "freeRTOS/10.5.1-beta.1@polaris/esp",
        "edf/4.9.0-beta.2@polaris/esp",
        "polaris_shared/0.0.1-beta.1@polaris/esp",
        "emio/0.8.0",
    )
    generators = "cmake", "EdfCMakeToolchainGenerator"
    exports_sources = None
    options = {
    }
    default_options = {}

    def configure(self):
        # TODO: still a little hard coded :/
        source_path = os.path.dirname(os.path.abspath(__file__))
        self.options["freeRTOS"].sdkconfig = os.path.join(source_path, "_build", "debug", "config")

        self.options["polaris_shared"].target_board = None
