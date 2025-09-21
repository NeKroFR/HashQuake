# HashQuake

HashQuake is a CLI for testing and discovering hash collisions.
It allows you to specify your own hashing program and evaluate it for potential collisions.

## Disclaimer

Brute-force collision searching is generally infeasible against strong standard hashes (SHA-2/3, etc.).
This tool is intended to evaluate **custom** or toy hash functions and to help detect obviously weak implementations.

## Installation

### build from source:

First make sure you have [go](https://go.dev/dl/) 1.23.2 or more installed

then you can run:

```
git clone https://github.com/NeKroFR/HashQuake.git
cd HashQuake
make
```

## Usage:

```
Usage:
    ./HashQuake [OPTIONS] <command to run the algorithm>

Description:
   If -w/--wordlist is provided, HashQuake iterates the given list.
   If no wordlist is provided, HashQuake performs a birthday attack by
   generating random words of length -s/--size until a collision is found.

   The <command> is a positional argument and will be executed with the
   candidate word appended as the last argument. The tool expects the hash
   to appear on stdout of your command (one hash per run).

Options:
   -h, --help                   Display this help message
   -v, --version                Display the version of the program
   -w, --wordlist <filepath>    Specify a wordlist file
   -s, --size <num>             Specify the length of words to generate (default 3)
   -t, --threads <num>          Specify the number of threads to use (default 1)
   -d, --display                Display the collisions in real time
   -o, --output <filepath>      Specify an output file
```

## Importing Your Own Hash Functions

You can use the templates in the [/templates](https://github.com/NeKroFR/HashQuake/tree/main/templates) directory and edit the hashing function as needed.

If you do not want to use the template or there are no templates for your language, you can create your own.
However, note that the program will run the command and add the word as the last argument, and it expects the hash to appear in the program's stdout (standard output).
