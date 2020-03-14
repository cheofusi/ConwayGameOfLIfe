CC = g++
CFLAGS = -std=c++17 
LDFLAGS = -lglfw -lGL -lGLEW 
OBJFILES = Main.o conwaysGame.o Shaders/shader.o Camera/camera.o 
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)


clean:
	rm -f  $(OBJFILES) $(TARGET)