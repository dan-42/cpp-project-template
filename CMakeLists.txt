cmake_minimum_required(VERSION 3.2)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/dan-42/hunter/archive/v0.12.44-dan.tar.gz"
        SHA1 "b2c2510c34d89df7fb63d6f4e9454ea4f09464a6"        
)



project(template VERSION 0.0.1)
enable_testing()
add_subdirectory(src)



