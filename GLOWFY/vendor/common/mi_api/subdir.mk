################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/mi_api/i2c_msc.c \
../vendor/common/mi_api/on_action_invoke.c \
../vendor/common/mi_api/on_event_occur.c \
../vendor/common/mi_api/on_property_change.c \
../vendor/common/mi_api/on_property_get.c \
../vendor/common/mi_api/on_property_request.c \
../vendor/common/mi_api/on_property_set.c \
../vendor/common/mi_api/telink_sdk_mesh_api.c \
../vendor/common/mi_api/telink_sdk_mible_api.c 

OBJS += \
./vendor/common/mi_api/i2c_msc.o \
./vendor/common/mi_api/on_action_invoke.o \
./vendor/common/mi_api/on_event_occur.o \
./vendor/common/mi_api/on_property_change.o \
./vendor/common/mi_api/on_property_get.o \
./vendor/common/mi_api/on_property_request.o \
./vendor/common/mi_api/on_property_set.o \
./vendor/common/mi_api/telink_sdk_mesh_api.o \
./vendor/common/mi_api/telink_sdk_mible_api.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/mi_api/%.o: ../vendor/common/mi_api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\proj\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\mijia_ble_api" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\libs" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\sdk_src\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\cfg" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\data_template" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common" -D__telink__ -D__PROJECT_MESH_LPN__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


