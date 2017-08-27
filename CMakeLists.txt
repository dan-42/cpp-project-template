cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.82.tar.gz"
    SHA1 "3858cdfe2c05edf2372c807dd5eb2ae3b1ccaa20"
)



project(template VERSION 1.0.0)
enable_testing()
add_subdirectory(src)



