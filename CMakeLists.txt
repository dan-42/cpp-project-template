cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.128.tar.gz"
    SHA1 "727c8f491b266668ed02e4457ef8a64c0a7389fd"
)


project(template VERSION 1.0.0)
enable_testing()
add_subdirectory(src)



