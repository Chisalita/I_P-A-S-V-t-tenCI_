################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Test2_atmega324PA.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Test2_atmega324PA.o

OBJS_AS_ARGS +=  \
Test2_atmega324PA.o

C_DEPS +=  \
Test2_atmega324PA.d

C_DEPS_AS_ARGS +=  \
Test2_atmega324PA.d

OUTPUT_FILE_PATH +=Test2_atmega324PA.elf

OUTPUT_FILE_PATH_AS_ARGS +=Test2_atmega324PA.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
G:/Ingeniously/I_P-A-S-V-t-tenCI_/Atmel/Librari/MyLibs/PWM_lib/Debug/libPWM_lib.a \
G:/Ingeniously/I_P-A-S-V-t-tenCI_/Atmel/Librari/MyLibs/Sensors/Debug/libSensors.a \
G:/Ingeniously/I_P-A-S-V-t-tenCI_/Atmel/Librari/MyLibs/Communication/Debug/libCommunication.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../../../Librari/MyLibs"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega324pa -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Test2_atmega324PA.map" -Wl,--start-group -Wl,-lm -Wl,-lPWM_lib -Wl,-lSensors -Wl,-lCommunication  -Wl,--end-group -Wl,-L"../../../../Librari/PWM_lib/PWM_lib/Debug" -Wl,-L"G:\Ingeniously\I_P-A-S-V-t-tenCI_\Atmel\Librari\PWM_lib\PWM_lib\Debug" -Wl,-L"G:\Ingeniously\I_P-A-S-V-t-tenCI_\Atmel\Librari\MyLibs\PWM_lib\Debug" -Wl,-L"G:\Ingeniously\I_P-A-S-V-t-tenCI_\Atmel\Librari\MyLibs\Communication\Debug" -Wl,-L"G:\Ingeniously\I_P-A-S-V-t-tenCI_\Atmel\Librari\MyLibs\Sensors\Debug"  -Wl,--gc-sections -mmcu=atmega324pa  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Test2_atmega324PA.elf" "Test2_atmega324PA.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Test2_atmega324PA.elf" "Test2_atmega324PA.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Test2_atmega324PA.elf" > "Test2_atmega324PA.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Test2_atmega324PA.elf" "Test2_atmega324PA.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "Test2_atmega324PA.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Test2_atmega324PA.elf" "Test2_atmega324PA.a" "Test2_atmega324PA.hex" "Test2_atmega324PA.lss" "Test2_atmega324PA.eep" "Test2_atmega324PA.map" "Test2_atmega324PA.srec" "Test2_atmega324PA.usersignatures"
	