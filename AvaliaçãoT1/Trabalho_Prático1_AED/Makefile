# make              # to compile files and create the executables
# make clean        # to cleanup object files and executables
# make cleanobj     # to cleanup object files only

CFLAGS = -Wall -Wextra -O2 -g

PROGS = imageRGBTest ImageIsEqual_Teste

# Default rule: make all programs
all: $(PROGS)

imageRGBTest: imageRGBTest.o imageRGB.o instrumentation.o error.o \
			  PixelCoords.o PixelCoordsQueue.o PixelCoordsStack.o

imageRGBTest.o: imageRGB.h instrumentation.h error.h \
                PixelCoords.h PixelCoordsQueue.h PixelCoordsStack.h

ImageIsEqual_Teste: ImageIsEqual_Teste.o imageRGB.o instrumentation.o error.o \
                    PixelCoords.o PixelCoordsQueue.o PixelCoordsStack.o
	$(CC) $(CFLAGS) ImageIsEqual_Teste.o imageRGB.o instrumentation.o error.o \
	        PixelCoords.o PixelCoordsQueue.o PixelCoordsStack.o -o ImageIsEqual_Teste
					
ImageIsEqual_Teste.o: imageRGB.h instrumentation.h error.h \
                      PixelCoords.h PixelCoordsQueue.h PixelCoordsStack.h

# Rule to make any .o file dependent upon corresponding .h file
%.o: %.h

# Make uses builtin rule to create .o from .c files.
%.o: %.c
	$(CC) $(CFLAGS) -c $<

cleanobj:
	rm -f *.o

clean: cleanobj
	rm -f $(PROGS)

