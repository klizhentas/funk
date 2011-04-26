CC = cc
CFLAGS = $(include_dirs) $(use_c99) -g
include_dirs = -Iinclude
use_c99 = -std=c99
objects = $(patsubst %.c,%.o,$(wildcard src/*.c))

funk: $(objects)
	$(CC) $(CFLAGS) -o $@ $(objects)

$(objects) : include/*.h

clean:
	rm -f ./funk
	rm -f ./src/*~
	rm -f ./src/*.o
	rm -f ./*~
