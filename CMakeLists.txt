cmake_minimum_required(VERSION 3.15)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY      arm-none-eabi-objcopy)
set(CMAKE_SIZE         arm-none-eabi-size)

project(panaled C ASM)

set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb")
set(C_WARNINGS "-Wall -Wextra -Werror")

set(CMAKE_C_FLAGS "${CPU_FLAGS} ${C_WARNINGS} -O2 -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS
    "${CPU_FLAGS} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map"
)

set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated" CACHE PATH "generated")

set(SOURCES
    src/main.c
    src/startup_stm32f103c8t6.s
    CMSIS/Device/ST/STM32F1xx/Source/system_stm32f1xx.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/CMSIS/Core/Include
    ${CMAKE_SOURCE_DIR}/CMSIS/Device/ST/STM32F1xx/Include
    ${CMAKE_SOURCE_DIR}/src
)

add_definitions(
    -DSTM32F103xB
    -DHSE_VALUE=16000000U
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T ${CMAKE_SOURCE_DIR}/linker.ld
    -nostartfiles
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
)
