# Quine-McCluskey-Algorithm

## How to run?

```shell
$ make
gcc -g -Wall -c -o .out/lists.o src/lists.c
gcc -g -Wall -c -o .out/main.o src/main.c
gcc -g -Wall -c -o .out/mccluskey.o src/mccluskey.c
gcc -g -Wall -o hw1.o .out/lists.o .out/main.o .out/mccluskey.o

$ ./hw1.o
Usage: ./hw1.o [input_file_name] [output_file_name]

Quine Mccluskey Algorithm

Arguments:
	input_file_name
	output_file_name

$ ./hw1.o sample_input sample_output
```