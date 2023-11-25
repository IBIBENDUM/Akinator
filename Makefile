.PHONY: clean run full_recompile

CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

LIBS_DIR = Libs
BUILD_DIR = build
EXECUTABLE = akinator

# SORRY
PROGRAM_SRCS = $(wildcard *.cpp */*.cpp */*/*.cpp)
PROGRAM_OBJS = $(patsubst %.cpp, %.o, $(PROGRAM_SRCS))
PROGRAM_OBJS_PATH = $(subst /,\, $(PROGRAM_OBJS))
OBJS = $(PROGRAM_OBJS)

compile: $(BUILD_DIR) $(EXECUTABLE)
compile_and_run: compile run
full_compile_and_run: clean compile run

$(EXECUTABLE): $(OBJS)
	g++ $^ $(CFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	g++ $^ $(CFLAGS) -c -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	del $(PROGRAM_OBJS_PATH)

run:
	.\$(EXECUTABLE) -i Examples/smeshariki.txt
