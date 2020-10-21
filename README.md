# Quine-McCluskey-Algorithm

[![hackmd-github-sync-badge](https://hackmd.io/gK0PZAnDRjyi5IDyI5DGCQ/badge)](https://hackmd.io/gK0PZAnDRjyi5IDyI5DGCQ)




## How to run?



```shell
$ make
gcc -std=c11 -g -Wall -c -o .out/lists.o src/lists.c
gcc -std=c11 -g -Wall -c -o .out/main.o src/main.c
gcc -std=c11 -g -Wall -c -o .out/mccluskey.o src/mccluskey.c
gcc -std=c11 -g -Wall -o hw1.o .out/lists.o .out/main.o .out/mccluskey.o

$ ./hw1.o
Usage: ./hw1.o [input_file_name] [output_file_name]

Quine Mccluskey Algorithm

Arguments:
    input_file_name
    output_file_name
    
$ ./hw1.o sample_input sample_output
```



## My implementation

I used linked-list as my basic data structure instead of array, the reason is the dynamic memory space, because I could not know the exact number of given minterms which might be up to $2^{10}$ when the number of variable is up to $10$.

The insertion of my linked-list is inserted at head, which could reduce $O(n)$ to $O(1)$ complexity.

For the minterm structure, I used the $0^{th}-9^{th}$ bits for recording expression, the $10^{th}-19^{th}$ bits for recording the coressponding variable careable or not and the last bit, and the $31^{th}$ bit, for recording the minterm was merged with other expression or not, the bit map is shown below. 

```txt
+--------------------------------------+
| 31|    30-20    |   19-10  |   9-0   |
+--------------------------------------+
merged  none-used   careable  expression
```



The last part of the `Quine-McCluskey algorithm` is finding a minimal cost implicants set which could cover all the carealbe expression as given input. This problem could be mapped to the minimal set cover problem and it is an `NP-complete` problem.

I used an `approximated-greedy approach` which time complexity is only $O(\log n)$  instead of an exact approach, `Knuth's Algorithm X`, which is much more complex.

