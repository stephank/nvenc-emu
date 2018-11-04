PREFIX ?= /opt/nvenc-emu

LIBDIR := ${PREFIX}/lib
CC := gcc
CFLAGS := -m32 --std=gnu99 -fPIC -Wall -pedantic
EMU_LIB := libnvenc-emu.so
CUDA_LIB := libcuda.so
NVENC_LIB := libnvidia-encode.so

${EMU_LIB}: cuda_emu.o nvenc_emu.o
	${CC} ${CFLAGS} -shared $^ -o $@ -lva -lva-x11 -lX11

%.o: %.c
	${CC} ${CFLAGS} -c $^ -o $@

clean:
	rm -f *.o *.so

install: ${EMU_LIB}
	install -m 644 ${EMU_LIB} ${LIBDIR}/${EMU_LIB}.1
	ln -s ${EMU_LIB}.1 ${LIBDIR}/${EMU_LIB}
	
	ln -s ${EMU_LIB}.1 ${LIBDIR}/${CUDA_LIB}.1
	ln -s ${EMU_LIB}.1 ${LIBDIR}/${CUDA_LIB}
	
	ln -s ${EMU_LIB}.1 ${LIBDIR}/${NVENC_LIB}.1
	ln -s ${EMU_LIB}.1 ${LIBDIR}/${NVENC_LIB}

uninstall:
	rm -f \
	  ${LIBDIR}/${EMU_LIB}.1 \
	  ${LIBDIR}/${EMU_LIB} \
	  ${LIBDIR}/${CUDA_LIB}.1 \
	  ${LIBDIR}/${CUDA_LIB} \
	  ${LIBDIR}/${NVENC_LIB}.1 \
	  ${LIBDIR}/${NVENC_LIB}

.PHONY: clean install uninstall
.SUFFIXES:
