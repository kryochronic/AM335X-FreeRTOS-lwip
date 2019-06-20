#adding entries for FreeRTOS
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel/include")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/FreeRTOS/portable/GCC/ARM_CA8_amm335x")
#adding entries for ti_starterware
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/include")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/include/armv7a")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/include/armv7a/am335x")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/include/hw")
#adding entries for application
	include_directories("${PROJECT_SOURCE_DIR}/src/inc")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/AM335X/inc")
	include_directories("${PROJECT_SOURCE_DIR}/src/inc/config_files")
	include_directories("${PROJECT_SOURCE_DIR}/src/app/lwip_app/ports/cpsw/include")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/AM335X/inc")
#adding entries for lwip
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/portable")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/lwip/ports/cpsw/include")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/lwip")
#adding entries for lib_third_party_ti_drivers
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/drivers")
	add_library(lib_third_party_ti_drivers "")
	target_compile_definitions(lib_third_party_ti_drivers 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/drivers")
#adding entries for lib_third_party_ti_utils
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/utils")
	add_library(lib_third_party_ti_utils "")
	target_compile_definitions(lib_third_party_ti_utils 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/utils")
#adding entries for lib_third_party_ti_mmcsdlib
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/mmcsdlib")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/mmcsdlib/include")
	add_library(lib_third_party_ti_mmcsdlib "")
	target_compile_definitions(lib_third_party_ti_mmcsdlib 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/mmcsdlib")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/mmcsdlib/include")
#adding entries for lib_third_party_ti_platform_beaglebone
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/platform/beaglebone")
	add_library(lib_third_party_ti_platform_beaglebone "")
	target_compile_definitions(lib_third_party_ti_platform_beaglebone 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/platform/beaglebone")
#adding entries for lib_third_party_ti_nandlib
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/nandlib")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/nandlib/include")
	add_library(lib_third_party_ti_nandlib "")
	target_compile_definitions(lib_third_party_ti_nandlib 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/nandlib")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/nandlib/include")
#adding entries for lib_third_party_ti_system_config_armv7a
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a/am335x")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a/am335x/gcc")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a/gcc")
	add_library(lib_third_party_ti_system_config_armv7a "")
	target_compile_definitions(lib_third_party_ti_system_config_armv7a 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a/am335x")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a/am335x/gcc")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/ti/system_config/armv7a/gcc")
#adding entries for lib_third_party_amazon_libraries_3rdparty_lwip_src
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/api")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/core")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/core/ipv4")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip/apps")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip/priv")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip/prot")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/netif")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/posix")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/posix/sys")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/netif")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/portable/arch")
	add_library(lib_third_party_amazon_libraries_3rdparty_lwip_src "")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/api")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/core")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/core/ipv4")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip/apps")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip/priv")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/lwip/prot")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/netif")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/posix")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/include/posix/sys")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/netif")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/libraries/3rdparty/lwip/src/portable/arch")
#adding entries for src_portable_lwip_ports_cpsw_netif
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/lwip/ports/cpsw/netif")
	add_library(src_portable_lwip_ports_cpsw_netif "")
	subdirs("${PROJECT_SOURCE_DIR}/src/portable/lwip/ports/cpsw/netif")
#adding entries for lib_third_party_amazon_freertos_kernel
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel")
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel/include")
	add_library(lib_third_party_amazon_freertos_kernel "")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel/include")
#adding entries for src_portable_FreeRTOS_portable_GCC_ARM_CA8_amm335x
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/FreeRTOS/portable/GCC/ARM_CA8_amm335x")
	add_library(src_portable_FreeRTOS_portable_GCC_ARM_CA8_amm335x "")
	subdirs("${PROJECT_SOURCE_DIR}/src/portable/FreeRTOS/portable/GCC/ARM_CA8_amm335x")
#adding entries for lib_third_party_amazon_freertos_kernel_portable_MemMang
	include_directories("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel/portable/MemMang")
	add_library(lib_third_party_amazon_freertos_kernel_portable_MemMang "")
	subdirs("${PROJECT_SOURCE_DIR}/lib/third_party/amazon/freertos_kernel/portable/MemMang")
#adding entries for src_portable_AM335X
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/AM335X")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/AM335X/inc")
	add_library(src_portable_AM335X "")
	target_compile_definitions(src_portable_AM335X 
		PRIVATE -DBOOT=MMCSD -DCONSOLE=UARTCONSOLE
	)
	subdirs("${PROJECT_SOURCE_DIR}/src/portable/AM335X")
	subdirs("${PROJECT_SOURCE_DIR}/src/portable/AM335X/inc")
#adding entries for src_portable_ported_aws_bufpool
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/ported_aws_bufpool")
	include_directories("${PROJECT_SOURCE_DIR}/src/portable/ported_aws_bufpool/inc")
	add_library(src_portable_ported_aws_bufpool "")
	subdirs("${PROJECT_SOURCE_DIR}/src/portable/ported_aws_bufpool")
	subdirs("${PROJECT_SOURCE_DIR}/src/portable/ported_aws_bufpool/inc")
#adding entries for src_application
	include_directories("${PROJECT_SOURCE_DIR}/src/application")
	add_library(src_application "")
	subdirs("${PROJECT_SOURCE_DIR}/src/application")

target_link_libraries (${PROJECT_NAME}.elf
	 
	lib_third_party_ti_drivers
	lib_third_party_ti_utils
	lib_third_party_ti_mmcsdlib
	lib_third_party_ti_platform_beaglebone
	lib_third_party_ti_nandlib
	lib_third_party_ti_system_config_armv7a
	lib_third_party_amazon_libraries_3rdparty_lwip_src
	src_portable_lwip_ports_cpsw_netif
	lib_third_party_amazon_freertos_kernel
	src_portable_FreeRTOS_portable_GCC_ARM_CA8_amm335x
	lib_third_party_amazon_freertos_kernel_portable_MemMang
	src_portable_AM335X
	src_portable_ported_aws_bufpool
	src_application
)