################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../proj/common/breakpoint.c \
../proj/common/compatibility.c \
../proj/common/list.c \
../proj/common/log.c \
../proj/common/mempool.c \
../proj/common/mmem.c \
../proj/common/printf.c \
../proj/common/qsort.c \
../proj/common/sdk_version.c \
../proj/common/selection_sort.c \
../proj/common/tstring.c \
../proj/common/tutility.c 

OBJS += \
./proj/common/breakpoint.o \
./proj/common/compatibility.o \
./proj/common/list.o \
./proj/common/log.o \
./proj/common/mempool.o \
./proj/common/mmem.o \
./proj/common/printf.o \
./proj/common/qsort.o \
./proj/common/sdk_version.o \
./proj/common/selection_sort.o \
./proj/common/tstring.o \
./proj/common/tutility.o 


# Each subdirectory must supply rules for building sources it contributes
proj/common/%.o: ../proj/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\proj\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\mijia_ble_api" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\libs" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\sdk_src\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\cfg" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\data_template" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common" -D__telink__ -D__PROJECT_MESH_LPN__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


