CFLAGS := \
-fPIC

LIBS := \
-F/System/Library/PrivateFrameworks \
-framework MultitouchSupport

.PHONY: clean

out/pamtouch.so: out/touch.o out/pam.o
	$(CC) -shared -o $@ $^ $(LIBS)

out/%.o: src/%.c
	@mkdir -p out
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -r -f out
