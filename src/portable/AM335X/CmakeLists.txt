
target_sources(src_portable_AM335X
                PUBLIC
                    "${CMAKE_CURRENT_SOURCE_DIR}/bsp_platform.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/hal_bspInit.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/hal_mmu.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/ported_am335x_clock_data.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/ported_am335x_interrupt.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/ported_am335x_startup.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/ported_amm335x_init.S"

            )
