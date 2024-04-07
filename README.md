# Collatz Conjecture Graph Generator

This program generates sequences based on the Collatz conjecture for a given set of seed values. It then displays the sequences and identifies nodes in the Collatz graph where sequences intersect.

## Usage

Compile the program using a C++ compiler that supports C++11 or later. For example:

```sh
g++ -std=c++11 -o collatz collatz.cpp
```

Run the compiled program with two command line arguments:

```sh
./collatz <seeds> <max_seed>
```

- `<seeds>`: The number of seed values to use.
- `<max_seed>`: The maximum value for the seed. Must be greater than 4.

## Dependencies

- Boost C++ Libraries: The program uses Boost's multiprecision library for handling large integers and the random library for random number generation.

## Example

```sh
./collatz 10 1000
```

This will generate sequences for 10 seed values between 4 and 1000 and display the sequences along with the identified nodes in the Collatz graph.

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/Fudmottin/Collatz/blob/main/LICENSE) file for details.

