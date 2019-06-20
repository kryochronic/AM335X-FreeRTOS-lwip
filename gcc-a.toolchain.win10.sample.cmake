set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)
 
include(CMakeForceCompiler)
 
if ( APPLE )
    set( ENV_LIB_PATHS ENV DYLD_LIBRARY_PATH )
elseif ( UNIX )
    set (TOOLCHAIN_DIR "/mnt/d/toolchains/gcc-arm-8.2-2019.01-i686-mingw32-arm-eabi" CACHE FILEPATH "Toolchain Path")
    set( ENV_LIB_PATHS ENV LD_LIBRARY_PATH )
elseif ( WIN32 )
    #set (TOOLCHAIN_DIR "D:/Projects/BBB/gcc-linaro-7.3.1-2018.05-i686-mingw32_arm-eabi" CACHE FILEPATH "Toolchain Path")
    set (TOOLCHAIN_DIR "D:/toolchains/gcc-arm-8.2-2019.01-i686-mingw32-arm-eabi" CACHE FILEPATH "Toolchain Path") 
    set( ENV_LIB_PATHS TOOLCHAIN_DIR)
else ()
    set( ENV_LIB_PATHS "" )
endif()
 
message (STATUS "toolchain path_variable: ${TOOLCHAIN_DIR}")

find_program(CC NAMES arm-eabi-gcc
  PATHS
  ${TOOLCHAIN_DIR}/bin
  "/usr/bin"
  "~/ti/"
  NO_DEFAULT_PATH)
 
# find_program(CC NAMES arm-eabi-gcc ${TOOLCHAIN_DIR}/bin)
find_program(CXX arm-eabi-gcc ${TOOLCHAIN_DIR}/bin)
find_program(OBJCOPY arm-eabi-objcopy ${TOOLCHAIN_DIR}/bin)
find_program(SIZE_TOOL arm-eabi-size ${TOOLCHAIN_DIR}/bin)

# set(CMAKE_C_COMPILER ${CC})
# set(CMAKE_CXX_COMPILER ${CXX})
 
 
message (STATUS "ARMCC: ${ARMCC}")
message (STATUS "CMAKE_PREFIX: ${CMAKE_PREFIX_PATH}")
 
CMAKE_FORCE_C_COMPILER(${CC} GNU)
CMAKE_FORCE_CXX_COMPILER(${CXX} GNU)
 
#CMAKE_FORCE_C_COMPILER("C:/ti/ccsv6/tools/compiler/gcc-arm-eabi-4_9/bin/arm-eabi-gcc.exe" GNU)
#CMAKE_FORCE_CXX_COMPILER("C:/ti/ccsv6/tools/compiler/gcc-arm-eabi-4_9/bin/arm-eabi-g++.exe" GNU)
 
 
message(STATUS "By Abhinav Tripathi with alot of help from Saket Gupta")
execute_process(
  COMMAND ${CMAKE_C_COMPILER} -print-file-name=libc.a
  OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
get_filename_component(CMAKE_INSTALL_PREFIX
  "${CMAKE_INSTALL_PREFIX}" PATH
)
get_filename_component(CMAKE_INSTALL_PREFIX
  "${CMAKE_INSTALL_PREFIX}/.." REALPATH
)
set(CMAKE_INSTALL_PREFIX  ${CMAKE_INSTALL_PREFIX} CACHE FILEPATH
    "Install path prefix, prepended onto install directories.")
 
message(STATUS "Cross-compiling with the gcc-arm-embedded toolchain")
message(STATUS "Toolchain prefix: ${CMAKE_INSTALL_PREFIX}")
 
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_LD ${CMAKE_INSTALL_PREFIX}/bin/arm-eabi-ld CACHE INTERNAL "ld tool")
set(CMAKE_OBJCOPY ${CMAKE_INSTALL_PREFIX}/bin/objcopy CACHE INTERNAL "objcopy tool")
 
set(CMAKE_FIND_ROOT_PATH ${CMAKE_INSTALL_PREFIX})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_ASM_FLAGS "${CFLAGS} -x assembler-with-cpp")
 
# for libraries and headers in the target directories
#message (STATUS CMAKE_FIND_ROOT_PATH_INCLUDE ${CMAKE_FIND_ROOT_PATH_MODE_INCLUDE})
 
 
#set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
#set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
 
#set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER>  <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
#set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER>  <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
 
 
#set(CMAKE_VERBOSE_MAKEFILE ON)
#SET(CMAKE_CXX_FLAGS_INIT "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS_INIT}")
# avoid just having a space as the initial value for the cache
#IF(CMAKE_CXX_FLAGS_INIT STREQUAL " ")
  #SET(CMAKE_CXX_FLAGS_INIT)
#ENDIF(CMAKE_CXX_FLAGS_INIT STREQUAL " ")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT}" CACHE STRING
     #"-Wall -pedantic -Wextra")