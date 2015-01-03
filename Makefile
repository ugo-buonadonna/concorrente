SOURCES=$(shell find . -type f -iname '*.c')

OBJECTS=$(foreach x, $(basename $(SOURCES)), $(x).o)

includes = $(wildcard include/*.h)
TARGET=HWC2

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) ${includes}