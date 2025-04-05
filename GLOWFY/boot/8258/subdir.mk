################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../boot/8258/cstartup_8258_RET_16K.S 

OBJS += \
./boot/8258/cstartup_8258_RET_16K.o 


# Each subdirectory must supply rules for building sources it contributes
boot/8258/%.o: ../boot/8258/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -Xassembler"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware" -DMCU_STARTUP_8258_RET_16K -D__PROJECT_MESH_LPN__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor" -I"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\vendor\common" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


