cmake_minimum_required(VERSION 3.2)



IF(DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the types of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release."  )
ELSE()
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the types of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release" )
ENDIF()




list (INSERT _COMPILE_FLAGS 0
                    -std=c++17
                    -Wall 
                    -Werror
                    -fPIC
                  
                   # -Wno-unused-function 
                   # -Wno-unused-local-typedefs 
                   -Wno-unused-variable 
                   # -Wno-unused-value                   
                   # -Wno-deprecated-declarations
                   # -Wno-placement-new # needed for gcc6 and boost 1.61 boost.function
                    )


IF(CMAKE_BUILD_TYPE STREQUAL Debug)

    list (APPEND _COMPILE_FLAGS  
                    -g
                    -gdwarf-2)
                    
     if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set (CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections ")       

        list(APPEND _COMPILE_FLAGS                     
                -ffunction-sections
                -fdata-sections                             
        )
    endif()
    
ELSE()
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        #set (CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections -flto")
        set (CMAKE_EXE_LINKER_FLAGS " -Wl,--gc-sections ")

        list(APPEND _COMPILE_FLAGS
                -fvisibility=hidden                
                -ffunction-sections
                -fdata-sections
                -flto 
                -fuse-linker-plugin                        
        )        
        
    endif()
    list(APPEND _COMPILE_FLAGS        
                -Os         
                -s                       
        )    
ENDIF()


hunter_add_package(Boost COMPONENTS system filesystem)
set(BOOST_ALL_DYN_LINK         ON)
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_LIBS      OFF)
set(Boost_USE_STATIC_RUNTIME   OFF)

find_package(Boost CONFIG REQUIRED system filesystem)
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

# pre
#hunter_add_package(lib-cpp-pre)
#find_package(lib-cpp-pre REQUIRED)
#include_directories(AFTER ${LIB_CPP_PRE_INCLUDE_DIRS})

# download pugixml
hunter_add_package(pugixml)
find_package(pugixml CONFIG REQUIRED)
get_target_property(PUGI_INCLUDE_DIRS pugixml INTERFACE_INCLUDE_DIRECTORIES)


hunter_add_package(nlohmann_json)
find_package(nlohmann_json CONFIG REQUIRED)

# Threads
find_package(Threads)


####
# Create targets
file(GLOB_RECURSE _SOURCES_CPP "*.cpp")


add_definitions(${_COMPILE_FLAGS})
include_directories(${CMAKE_CURRENT_LIST_DIR} ${PUGI_INCLUDE_DIRS})
add_executable(main ${_SOURCES_CPP})
target_link_libraries(main ${Boost_LIBRARIES} ${PSAPI_LIBRARIES} pthread pugixml nlohmann_json)

