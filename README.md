# Collatz Conjecture Graph Generator

This project is a C++ program that generates Collatz sequences from a set of random seeds. The Collatz conjecture asserts that for any positive integer, the sequence defined by the rule,

    n -> n/2 if n is even
    n -> 3n+1 if n is odd

always reaches the number 1, regardless of the starting value of `n`.

## Description

The program uses boost libraries to generate large integer values and also for generating random numbers. It takes two inputs: `seeds` which is the number of different starting points you want to use for your sequences, and `max_seed` which sets an upper limit on what these starting values can be.

The program produces several outputs:

- A list of all nodes (numbers) found in these generated sequences.
- A hierarchy indicating how each node leads to other nodes in its corresponding sequence.
- Individual Collatz sequences traced from each seed.
- A DOT file, which can be used with Graphviz or similar tools to visualize the tree structure.

## Requirements

To compile and run this program you need:

- A modern C++ compiler supporting C++14 standard.
- Boost library installed.

## Compilation And Execution

Compile this project using a C++ compiler as follows:

```
c++ -std=c++14 -03 -o collatz collatz.cpp
```

Run it providing two command line arguments:
1. Number of seeds (`seeds`)
2. Maximum seed value (`max_seed`)

Example:
```
./collatz 10 1000000 > data.txt
```

This will produce sequences from ten randomly chosen starting points between 4 and one million.

## Output Sample

The output includes information about each individual sequence, nodes in all sequences, and hierarchical relationships among nodes.

A sample output could look like this:

```bash
collatz sequence data v1

nodes: 9
16
22
40
94
130
166
328
1552
1606

hierarchy:
16 40
16 1606
22 130
40 22
40 94
94 166
94 328
166 1552

collatz: 191527 97 66 31
4
8
16
5
10
20
40
13
26
52
17
34
11
22
7
14

...

DOT file generated successfully.
```

The `tree.dot` can be used with Graphviz or similar tools to visualize the tree structure.

## collatz2

I added a simpler version of the program which displays the seed values along with the number
of steps to get to 1 with tab delimeters. It compiles the same way as the main program and has the
same requirements. When run, it looks something like this:

```bash
./collatz2 10 1000
seed	steps
273	29
354	32
396	27
467	84
473	35
572	105
623	131
761	33
783	121
906	15

Min steps: 15	Max steps: 131
```

Enjoy exploring Collatz sequences!

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/Fudmottin/Collatz/blob/main/LICENSE) file for details.

