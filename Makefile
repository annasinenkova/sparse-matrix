CC = g++
CFLAGS = -O2 -ftrapv -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm
LFLAGS = -fsanitize=address,undefined

all: sp_matr_t1 sp_matr_t2 sp_matr_t3 sp_matr_t4 clean_obj

sp_matr_t%: test%.o sparse_matrix.o
	$(CC) $(LFLAGS) $^ -o $@
	
test%.o: test%.cpp sparse_matrix.hpp
	$(CC) $(CFLAGS) -c $< -o $@
	
sparse_matrix.o: sparse_matrix.cpp sparse_matrix.hpp
	$(CC) $(CFLAGS) -c $< -o $@
	
clean_obj:
	rm *.o
	
clean:
	rm sp_matr_t*
