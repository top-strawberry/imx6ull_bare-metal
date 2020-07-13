CROSS_COMPILE	?= arm-linux-gnueabihf-
TARGET			?= imax6ull

FLAGS			:= -Wall -nostdlib -fno-builtin -c -O2
SFLAGS			:=
#-std=c99 不能与 -Wa,-mimplicit-it=thumb 同时用？
CFLAGS			:= -Wa,-mimplicit-it=thumb

LIBS			:= -lgcc
LIBPATH 		:= -L /home/i.max6ull/opt/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4


CC			:= ${CROSS_COMPILE}gcc
LD			:= ${CROSS_COMPILE}ld
OBJCOPY		:= ${CROSS_COMPILE}objcopy
OBJDUMP		:= ${CROSS_COMPILE}objdump

OBJDIR		:= ../obj/

INCLUDEDIRS	:= ../imax6ull	\
			   ../utils/stdio/include \
			   ../main		\
			   ../usr	    \
			   ../bsp	    \

SRCDIRS	:= 	   ../imax6ull	\
			   ../utils/stdio/lib \
			   ../main		\
			   ../usr	    \
			   ../bsp	    \


INCLUDE		:= $(patsubst %, -I %, $(INCLUDEDIRS))


SFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES		:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR	:= $(notdir $(SFILES))
CFILENDIR	:= $(notdir $(CFILES))

SOBJS		:= $(patsubst %, $(OBJDIR)%, $(SFILENDIR:.S=.o))
COBJS		:= $(patsubst %, $(OBJDIR)%, $(CFILENDIR:.c=.o))
OBJS		:= $(SOBJS) $(COBJS)

VPATH		:= $(SRCDIRS)


${TARGET}.bin : ${OBJS}
	${LD} -Timax6ull.lds -o ${TARGET}.elf $^ $(LIBS) $(LIBPATH)
	${OBJDUMP} -D ${TARGET}.elf > ${TARGET}.dis
	${OBJCOPY} -O binary -S ${TARGET}.elf $@

$(SOBJS) : $(OBJDIR)%.o : %.S
	$(CC) $(FLAGS) $(SFLAGS) $(INCLUDE) -o $@ $<

$(COBJS) : $(OBJDIR)%.o : %.c
	$(CC) $(FLAGS) $(CFLAGS) $(INCLUDE) -o $@ $<



.PYONE: clean print
print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)

clean:
	#rm -rf ${TARGET}.bin $(OBJDIR)${TARGET}.elf $(OBJDIR)${TARGET}.dis $(OBJDIR)*.o
	rm -rf *.bin *.elf *.dis load.imx $(OBJDIR)*.o
