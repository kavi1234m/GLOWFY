################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/user_app/subdir.mk
-include vendor/common/subdir.mk
-include vendor/common/telink_platform/subdir.mk
-include vendor/common/nlc/subdir.mk
-include vendor/common/nl_api/subdir.mk
-include vendor/common/mi_api/subdir.mk
-include vendor/common/mi_api/mijia_ble_api/subdir.mk
-include vendor/common/mi_api/mi_vendor/subdir.mk
-include vendor/common/mi_api/libs/third_party/micro-ecc/subdir.mk
-include vendor/common/mi_api/libs/subdir.mk
-include vendor/common/llsync/sdk_src/utils/subdir.mk
-include vendor/common/llsync/sdk_src/core/subdir.mk
-include vendor/common/llsync/samples/telink/main/subdir.mk
-include vendor/common/llsync/samples/telink/main/hal/subdir.mk
-include vendor/common/llsync/hal/subdir.mk
-include vendor/common/certify_base/subdir.mk
-include vendor/GLOWFY/subdir.mk
-include vendor/GLOWFY/MIDDLEWARE/subdir.mk
-include vendor/GLOWFY/HAL/subdir.mk
-include vendor/GLOWFY/APPLICATION/subdir.mk
-include proj/mcu_spec/subdir.mk
-include proj/mcu/subdir.mk
-include proj/drivers/subdir.mk
-include proj/common/subdir.mk
-include proj/app/subdir.mk
-include drivers/8258/subdir.mk
-include drivers/8258/flash/subdir.mk
-include boot/8258/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
GLOWFY.lst \

FLASH_IMAGE += \

SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

# Main-build Target
main-build: ble_lt_mesh.elf secondary-outputs

# Tool invocations
ble_lt_mesh.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware\proj_lib" -T ../boot.link -o"ble_lt_mesh.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

GLOWFY.lst: ble_lt_mesh.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S ble_lt_mesh.elf  >"GLOWFY.lst"
	@echo 'Finished building: $@'
	@echo ' '

: ble_lt_mesh.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy ble_lt_mesh.elf
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: ble_lt_mesh.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t ble_lt_mesh.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) ble_lt_mesh.elf
	-@echo ' '

pre-build:
	-"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware/getver.sh"
	-@echo ' '

post-build:
	-"D:\TELINK SDK 4.1\tc_ble_mesh-4.1.0.1\firmware/tl_check_fw.sh"   GLOWFY   ble_lt_mesh
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
