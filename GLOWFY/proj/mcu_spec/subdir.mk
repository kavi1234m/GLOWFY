################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../proj/mcu_spec/adc_8263.c \
../proj/mcu_spec/adc_8266.c \
../proj/mcu_spec/adc_8267.c \
../proj/mcu_spec/gpio_8263.c \
../proj/mcu_spec/gpio_8266.c \
../proj/mcu_spec/gpio_8267.c \
../proj/mcu_spec/pwm_8263.c \
../proj/mcu_spec/pwm_8266.c \
../proj/mcu_spec/pwm_8267.c 

S_UPPER_SRCS += \
../proj/mcu_spec/cstartup_8261.S \
../proj/mcu_spec/cstartup_8263.S \
../proj/mcu_spec/cstartup_8263_ram.S \
../proj/mcu_spec/cstartup_8266.S \
../proj/mcu_spec/cstartup_8266_ram.S \
../proj/mcu_spec/cstartup_8267.S \
../proj/mcu_spec/cstartup_8267_ram.S \
../proj/mcu_spec/cstartup_8269.S \
../proj/mcu_spec/cstartup_8269_ram.S 

OBJS += \
./proj/mcu_spec/adc_8263.o \
./proj/mcu_spec/adc_8266.o \
./proj/mcu_spec/adc_8267.o \
./proj/mcu_spec/cstartup_8261.o \
./proj/mcu_spec/cstartup_8263.o \
./proj/mcu_spec/cstartup_8263_ram.o \
./proj/mcu_spec/cstartup_8266.o \
./proj/mcu_spec/cstartup_8266_ram.o \
./proj/mcu_spec/cstartup_8267.o \
./proj/mcu_spec/cstartup_8267_ram.o \
./proj/mcu_spec/cstartup_8269.o \
./proj/mcu_spec/cstartup_8269_ram.o \
./proj/mcu_spec/gpio_8263.o \
./proj/mcu_spec/gpio_8266.o \
./proj/mcu_spec/gpio_8267.o \
./proj/mcu_spec/pwm_8263.o \
./proj/mcu_spec/pwm_8266.o \
./proj/mcu_spec/pwm_8267.o 


# Each subdirectory must supply rules for building sources it contributes
proj/mcu_spec/%.o: ../proj/mcu_spec/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\proj\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\mijia_ble_api" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\mi_api\libs" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\sdk_src\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\include" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\cfg" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common\llsync\data_template" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common" -D__telink__ -D__PROJECT_MESH_LPN__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

proj/mcu_spec/%.o: ../proj/mcu_spec/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -Xassembler"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware" -DMCU_STARTUP_8258_RET_16K -D__PROJECT_MESH_LPN__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


