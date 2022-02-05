main: main.cc cpp_utils/*
	g++ -std=c++17 main.cc -o main -O2

main_ga: main_ga.cc cpp_utils/*
	g++ -std=c++17 main_ga.cc -o main_ga -O2

main_debug: main.cc cpp_utils/*
	g++ -std=c++17 main.cc -o main -O0 -g
	
test: test.cc cpp_utils/*
	g++ std=c++17 test.cc -o test

zip: main.cc *.py optimizer/* cpp_utils/* 
	zip code.zip -r main.cc *.py optimizer cpp_utils

clean:
	rm -f main
	rm -f main_debug
	rm -f test

ultraclean:
	rm -f main
	rm -f main_debug
	rm -f outputs/*
	rm -f out_temp.txt
	rm -f code.zip
	rm -f test