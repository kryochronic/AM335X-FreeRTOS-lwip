cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0013 OLD)
cmake_policy(SET CMP0014 OLD)

enable_language(C ASM)
project(freeRTOSBBB C)
set(BUILD_EXT "elf")

# # GLOB_RECURSE(${SRC} src/app/*.c src/bsp/*.c)
# set(SRC "SOURCE/imageLoad.S")
# set(SRC "SOURCE/main.c")
#GLOB_RECURSE(${HEADER} )

add_executable(${PROJECT_NAME}.elf ${SRC})
set (CMAKE_VERBOSE_MAKEFILE ON)
# The version number.
set (freeRTOSBBB_VERSION_MAJOR 1)
set (freeRTOSBBB_VERSION_MINOR 0)
#Workaround for Removing '[rollbar] Unable to automatically determine compiler'
set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER} CACHE FILEPATH "Compiler")
set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER} CACHE FILEPATH "Compiler")

set(gcc 1)
set(TARGET "armv7a")
set(DEVICE "am335x")
set(EVM "beaglebone")
set(COMPILER "gcc")
# Below are Handled in Arguments now XXX_makefile_args.py
# set(BOOT "MMCSD")
# set(CONSOLE "UARTCONSOLE")

include(ProjectIncludes.cmake)
#Continue
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a8  -march=armv7-a -marm -mfloat-abi=hard -mfpu=neon")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mlong-calls -fdata-sections -funsigned-char -ffunction-sections -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Dgcc -D${DEVICE} -D${EVM} -DSUPPORT_UNALIGNED -DMMCSD -DUARTCONSOLE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -gdwarf-3 -gstrict-dwarf -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-value")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=\"nosys.specs\"")

set(CMAKE_ASM_FLAGS ${CMAKE_C_FLAGS})


set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -e Entry -Wl,-T,${PROJECT_SOURCE_DIR}/bbb.lds -Wl,-u,__aeabi_uidiv -Wl,-u,__aeabi_idiv")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map,\"${PROJECT_NAME}.map\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-output-format")

message(STATUS "By Abhinav Tripathi with alot of help from Saket Gupta")

# For POST BUILD SETUIP
if ( APPLE )
    message(STATUS "Apple")
    set( ENV_LIB_PATHS ENV DYLD_LIBRARY_PATH )
elseif ( UNIX )
    message(STATUS "JUNIX")
    set(TOOLS_TI "${CMAKE_CURRENT_BINARY_DIR}/../tools/ti/tiimage")
elseif ( WIN32 )    
    message(STATUS "WINDOZE 32")
    set(TOOLS_TI "${CMAKE_CURRENT_BINARY_DIR}/../lib/third_party/tools/ti/tiimage.exe")
else ()    
    message(STATUS "OTHERS")
    set( ENV_LIB_PATHS "" )
endif()
set(TOOLS_TI "${CMAKE_CURRENT_BINARY_DIR}/../lib/third_party/tools/ti/tiimage.exe")
#   <executable file name> <load address> <boot mode> <input image path/name>   <output image path/name>
set (ELF_INPUT ${PROJECT_NAME}.${BUILD_EXT})
set(ELF_TO_APP_LOAD_ADDRESS "0x80000000")
set(ELF_TO_APP_BOOT_MODE    "NONE")
# set(MAKE_APP_COMMAND_ARGS "  0x80000000      NONE       ./${PROJECT_NAME}.elf     ./app.${PROJECT_NAME}.elf.rename")
set(ELF_TO_APP_INPUT  "\"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf\"")

set(ELF_TO_APP_OUTPUT "\"${CMAKE_CURRENT_BINARY_DIR}/app.${PROJECT_NAME}.bin\"")

set(ELF_TO_BIN_ARGS "-v -I elf32-littlearm -O binary -B arm -S ${ELF_TO_APP_INPUT} ${ELF_TO_APP_OUTPUT}")

set(SD_CARD_FILE_NAME  "\"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sdcard\"")

set(MAKE_APP_COMMAND_ARGS "${ELF_TO_APP_LOAD_ADDRESS} ${ELF_TO_APP_BOOT_MODE} ${ELF_TO_APP_OUTPUT} ${SD_CARD_FILE_NAME}" )

#Create BIN from ELF
add_custom_command(TARGET ${PROJECT_NAME}.${BUILD_EXT} POST_BUILD
                   COMMAND ${OBJCOPY} "${ELF_TO_BIN_ARGS}"
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "\n
                        Building app for SD Card .... invoking :
                            Working Dir --> ${CMAKE_CURRENT_BINARY_DIR}
                            OBJCOOPY TI --> ${OBJCOPY}
                            Args        --> ${ELF_TO_BIN_ARGS} 
                            Command Invoked
                            ${OBJCOPY} ${ELF_TO_BIN_ARGS}
                            By Abhinav Tripathi ELF --> BIN")
# build TIIMAGE EXEC
# add_custom_command(TARGET ${PROJECT_NAME}.${BUILD_EXT} POST_BUILD    
#                             COMMAND "gcc ${CMAKE_CURRENT_BINARY_DIR}/../cftnav-bbb-starterware/tools/ti_image/tiimage.c -o ${TOOLS_TI}"
#                             WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
#                             COMMENT "\n
#                         Building ti_image.exe :
#                             Working Dir --> ${CMAKE_CURRENT_BINARY_DIR}
#                             Tool TI     --> ${TOOLS_TI}
                            
#                             Command Invoked
#                             gcc ${CMAKE_CURRENT_BINARY_DIR}/../cftnav-bbb-starterware/tools/ti_image/tiimage.c -o ${TOOLS_TI}
#                             By Abhinav Tripathi tiimage.c --> tiimage.exe")
#Create TIIMAGE from BIN
add_custom_command(TARGET ${PROJECT_NAME}.${BUILD_EXT} POST_BUILD    
                   COMMAND ${TOOLS_TI} "${MAKE_APP_COMMAND_ARGS}"
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "\n
                        Building app for SD Card .... invoking :
                            Working Dir --> ${CMAKE_CURRENT_BINARY_DIR}
                            Tool TI     --> ${TOOLS_TI}
                            Args        --> ${MAKE_APP_COMMAND_ARGS} 
                            Command Invoked
                            ${TOOLS_TI} ${MAKE_APP_COMMAND_ARGS}
                            By Abhinav Tripathi BIN --> APP")



# set(TARGET_ELF_COPY_DIR "D:\\CCS8_WorkSpace\\trt\\Debug")
# set(ELF_COPY_DBG_NAME  "${TARGET_ELF_COPY_DIR}\\trt.out")
# add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#                    COMMAND cp ${ELF_TO_APP_INPUT} ${ELF_COPY_DBG_NAME}
#                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                    COMMENT "\n
#                         Copy ELF for Debug in CCS .... invoking:
#                             cp ${ELF_TO_APP_INPUT} ${ELF_COPY_DBG_NAME}
#                             Input Filename     --> ${ELF_COPY_DBG_NAME}
#                             By Abhinav Tripathi ELF --> CCSDBG")  