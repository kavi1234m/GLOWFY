################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/GLOWFY/HAL/hal_adc.c \
../vendor/GLOWFY/HAL/hal_battery_check.c \
../vendor/GLOWFY/HAL/hal_ble_mesh.c \
../vendor/GLOWFY/HAL/hal_clock.c \
../vendor/GLOWFY/HAL/hal_factory_reset.c \
../vendor/GLOWFY/HAL/hal_gpio.c \
../vendor/GLOWFY/HAL/hal_led.c \
../vendor/GLOWFY/HAL/hal_nlc_sensor.c \
../vendor/GLOWFY/HAL/hal_peripheral_init.c \
../vendor/GLOWFY/HAL/hal_sleep.c \
../vendor/GLOWFY/HAL/hal_watchdog.c 

OBJS += \
./vendor/GLOWFY/HAL/hal_adc.o \
./vendor/GLOWFY/HAL/hal_battery_check.o \
./vendor/GLOWFY/HAL/hal_ble_mesh.o \
./vendor/GLOWFY/HAL/hal_clock.o \
./vendor/GLOWFY/HAL/hal_factory_reset.o \
./vendor/GLOWFY/HAL/hal_gpio.o \
./vendor/GLOWFY/HAL/hal_led.o \
./vendor/GLOWFY/HAL/hal_nlc_sensor.o \
./vendor/GLOWFY/HAL/hal_peripheral_init.o \
./vendor/GLOWFY/HAL/hal_sleep.o \
./vendor/GLOWFY/HAL/hal_watchdog.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/GLOWFY/HAL/%.o: ../vendor/GLOWFY/HAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\proj\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\mijia_ble_api" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\libs" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\sdk_src\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\cfg" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\data_template" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common" -D__telink__ -D__PROJECT_MESH_LPN__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


