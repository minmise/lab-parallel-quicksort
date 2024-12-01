correctness_compile:
	g++-12 -O2 seq_sort.cpp par_sort.cpp test_correctness.cpp test_generator.cpp -fopenmp -o correctness.out

correctness_run:
	./correctness.out

time_compile:
	g++-12 -O2 seq_sort.cpp par_sort.cpp test_time.cpp test_generator.cpp -fopenmp -o time.out

time_run:
	./time.out