CC = cc
CFLAGS = $(include_dirs) $(use_c99)
include_dirs = -Iinclude
use_c99 = -std=c99
objects = $(patsubst %.c,%.o,$(wildcard src/*.c))

funk: obj $(objects)
	$(CC) $(CFLAGS) -o $@ $(objects)

obj:
	mkdir obj

$(objects) : include/*.h

clean:
	rm -rf ./obj
	rm -f ./funk
	rm -f ./src/*~
	rm -f ./*~
