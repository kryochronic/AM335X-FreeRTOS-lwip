#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Following ignore list for PEP8 for Formatting "--ignore=E501,E126,E241,E221"

""" Run this where build is a subfolder """
import os
import sys

from lib.third_party.c_source_tools.src.listFiles import make_generate_cmake_project_includes as make_generate_cmake_project_includes


class cmake_list_file_args():
    args = None

    def __init__(self, subfolders=None, path=None):
        cmake_list_file_args.args = self.create_args(subfolders=None, path=None)

    def getargs(self):
        return cmake_list_file_args.args

    def create_args(self, subfolders=None, path=None):
        args = {}
        if path is not None:
            if(os.path.isdir(path)):
                root = path
            else:
                return args
        else:
            root = os.getcwd()
            args['prefix'] = 'bbb_freeRTOS'
        args['application_libs'] = {
                                        'FreeRTOS'      :   [   # Important to Keep this on top of Amazon Includes"
                                                                'lib/third_party/amazon/freertos_kernel/include',
                                                                'src/portable/FreeRTOS/portable/GCC/ARM_CA8_amm335x',
                                                            ],

                                        'ti_starterware':   [
                                                                'lib/third_party/ti/include',
                                                                'lib/third_party/ti/include/armv7a',
                                                                'lib/third_party/ti/include/armv7a/am335x',
                                                                'lib/third_party/ti/include/hw',
                                                            ],
                                        'application':      [
                                                                'src/inc',
                                                                'src/portable/AM335X/inc',
                                                                'src/inc/config_files',
                                                                'src/app/lwip_app/ports/cpsw/include',
                                                                'src/portable/AM335X/inc',
                                                            ],
                                        'lwip':             [
                                                                'lib/third_party/amazon/libraries/3rdparty/lwip',
                                                                'lib/third_party/amazon/libraries/3rdparty/lwip/src/include/',
                                                                'lib/third_party/amazon/libraries/3rdparty/lwip/src/portable',
                                                                'src/portable/lwip/ports/cpsw/include',
                                                                'src/portable/lwip',
                                                            ],
                                       
                                    }
        args['suf']     = ''
        args['pattern'] = ['*.c', '*.h', '*.S']
        args['headers'] = ['.h']
        args['sources'] = ['.c','.S']
        args['exclude'] =   [
                            #general exclusions
                            'unused',  # File or Folder
                            'tools/',  # This is not used in our stack
                            'Utils/',  # This is not used in our stack
                            'binary/',  # This is not used in our stack
                            'build/',  # This is not used in our stack
                            'target_config/',  # This is not used in our stack
                            'mdio_interrupt.c',  # This is not used in our stack   

                            #ti Starterware exclusions                           
                            'ewarm',  # This is not used in our stack                             
                            'cgt',  # This is not used in our stack       
                            'lib/third_party/ti/',  # We dont want the complete stack
                            'fatfs/port',  # This is not used in our stack
                            'raster.c',  # This is not used in our stack
                            'cmdline.c',  # This is not used in our stack
                            'startup.c',  # We have our own version in application
                            'init.S',     # We have our own version in application
                            'usbphyGS70.c',  # This is not used in our stack
                            'clk_config_dmtimer.c',  # This is not used in our stack
                            'serial.c',  # This is not used in our stack
                            'mmu_init.c',  # This is not used in our stack
                            'exceptionhandler.S',  # This is not used in our stack
                            'interrupt.c',  # We have our own version in application

                            #Amazon Exclusions
                            'lib/third_party/amazon/',  # This is not used in our stack only some subfolders!
                            
                            #FreeRTOS Exclusions
                            'heap_1.c',  # This is not used in our stack
                            'heap_2.c',  # This is not used in our stack
                            'heap_3.c',  # This is not used in our stack
                            # 'heap_4.c',  # We want to use Simple FreeRTOS heaps
                            'heap_5.c',  # This is not used in our stack
                            'lib/third_party/amazon/freertos_kernel/portable',  # This is not used in our stack

                            #lwip exclusions
                            'mpu_wrappers.c',  # We dont yet use this file
                            'slipif.c',  # We have our own version in application
                            'ipv6',  # This is not used in our stack
                            'ppp',  # This is not used in our stack
                            'portable/mediatek',  # This is not used in our stack
                            'portable/cypress',  # This is not used in our stack
                            'lib/third_party/amazon/libraries/3rdparty/lwip/src/apps',  # This is not used in our stack
                            ]

        args['root']           = root
        args['CmakeIncludes']  = "ProjectIncludes.cmake"
        if subfolders is None:
            args['subfolders'] = [
                                ['lib/third_party/ti/drivers', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  #Starterware from ti
                                ['lib/third_party/ti/utils', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  #Starterware from ti
                                ['lib/third_party/ti/mmcsdlib', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  #Starterware from ti
                                ['lib/third_party/ti/platform/beaglebone', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  #Starterware from ti
                                ['lib/third_party/ti/nandlib', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  #Starterware from ti
                                ['lib/third_party/ti/system_config/armv7a', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  #Starterware from ti
                               
                               

                                # ['src', '-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  # Application
                                #lwip
                                ['lib/third_party/amazon/libraries/3rdparty/lwip/src'],  #lwip from Amazon stack as a third party lib
                                ['src/portable/lwip/ports/cpsw/netif'],

                                # FreeRTOS inclusions
                                ['lib/third_party/amazon/freertos_kernel'],  # using a port of FreeRTOS for AM335x with critical Section nesting baed on CA9
                                ['src/portable/FreeRTOS/portable/GCC/ARM_CA8_amm335x'],
                                ['lib/third_party/amazon/freertos_kernel/portable/MemMang'],

                                #platform - am335X
                                ['src/portable/AM335X','-DBOOT=MMCSD -DCONSOLE=UARTCONSOLE'],  # Our own versions of files changed from Starterware & Hal initialisations built on Starterware functions
                                # misc & Aws etc
                                ['src/portable/ported_aws_bufpool'], 
                                #Application Glues
                                ['src/application'],  #lwip from Amazon stack as a third party lib
                            ]
        return args


default_args = cmake_list_file_args(path=os.getcwd()).args

if __name__ == '__main__':
    make_generate_cmake_project_includes(default_args)