RAYLIB_INCLUDE := -I/usr/local/include -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic 

GCCFLAGS := $(RAYLIB_INCLUDE) -I. -Wall

SOURCES  := $(patsubst %.c,%.o,$(shell find -P -wholename "**/*.c" -type f))	
OUT	 := music.out

build: $(OUT)

$(OUT) : $(SOURCES)
	@echo Edition de lien de $@ avec $(GCCFLAGS) $(args)
	@gcc -o $(OUT) $^ $(GCCFLAGS)

%.o : %.c
	@echo Compilation de $@ avec $(GCCFLAGS) $(args)
	@gcc -c $< -o $@ $(GCCFLAGS)

clean:
	@echo Deleting files: 
	@find -P -wholename "**/*.o" -delete -print

run:
	@./$(OUT)
