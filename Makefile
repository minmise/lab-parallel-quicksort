std_correctness_compile:
	g++-12 std_sort.cpp test_correctness.cpp test_generator.cpp -o std_test_correctness.out

seq_correctness_compile:
	g++-12 seq_sort.cpp test_correctness.cpp test_generator.cpp -o seq_test_correctness.out

par_correctness_compile:
	g++-12 par_sort.cpp test_correctness.cpp test_generator.cpp -fopenmp -o par_test_correctness.out

std_correctness_run:
	./std_test_correctness.out

seq_correctness_run:
	./seq_test_correctness.out

par_correctness_run:
	./par_test_correctness.out

std_time_compile:
	g++-12 std_sort.cpp test_time.cpp test_generator.cpp -o std_test_time.out

seq_time_compile:
	g++-12 seq_sort.cpp test_time.cpp test_generator.cpp -o seq_test_time.out

par_time_compile:
	g++-12 par_sort.cpp test_time.cpp test_generator.cpp -fopenmp -o par_test_time.out

std_time_run:
	./std_test_time.out

seq_time_run:
	./seq_test_time.out

par_time_run:
	./par_test_time.out
