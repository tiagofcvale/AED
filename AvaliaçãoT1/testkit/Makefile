# make              # to compile files and create the executables
# make clean        # to cleanup object files and executables
# make cleanobj     # to cleanup object files only

CFLAGS = -Wall -Wextra -O2 -g

PROGS = imageRGBTest

TESTPROGS = \
        testImageCopy testImageIsEqual \
        testImageRotate90CW testImageRotate180CW \
        testFloodFill testImageSegmentation testAllFunctions

# Default rule: make all programs
all: $(PROGS) $(TESTPROGS)

imageRGBTest: imageRGBTest.o imageRGB.o instrumentation.o error.o \
			  PixelCoords.o PixelCoordsQueue.o PixelCoordsStack.o

imageRGBTest.o: imageRGB.h instrumentation.h error.h \
                PixelCoords.h PixelCoordsQueue.h PixelCoordsStack.h

COMMONOBJ = LoadSave.o \
            imageRGB.o instrumentation.o error.o \
            PixelCoords.o PixelCoordsQueue.o PixelCoordsStack.o

testImageCopy: testImageCopy.o $(COMMONOBJ)

testImageIsEqual: testImageIsEqual.o $(COMMONOBJ)

testImageRotate90CW: testImageRotate90CW.o $(COMMONOBJ)

testImageRotate180CW: testImageRotate180CW.o $(COMMONOBJ)

testFloodFill: testFloodFill.o $(COMMONOBJ)

testImageSegmentation: testImageSegmentation.o $(COMMONOBJ)

testAllFunctions: testAllFunctions.o $(COMMONOBJ)

# Rule to make any .o file dependent upon corresponding .h file
%.o: %.h

# Make uses builtin rule to create .o from .c files.

cleanobj:
	rm -f *.o

clean: cleanobj
	rm -f $(PROGS)
	rm -f $(TESTPROGS)

