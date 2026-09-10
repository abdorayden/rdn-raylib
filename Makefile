CC = gcc
CFLAGS = -I./src/raylib-6.0_linux_amd64/include/ -I./src/rdn/include/
LDFLAGS = ./src/raylib-6.0_linux_amd64/lib/libraylib.a ./src/rdn/lib/librdn.a -lm -lX11 -lpthread -ldl -lrt

rdnraylib.so: src/rdnraylib.c src/compat.c
	$(CC) -shared -fPIC $(CFLAGS) -o nativelibs/$@ src/rdnraylib.c src/compat.c $(LDFLAGS)


test: rdnraylib.so test.rdn
	rdn test.rdn

clean:
	rm -f nativelibs/rdnraylib.so
