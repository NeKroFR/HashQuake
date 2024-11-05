# Hash-Collider

Hash-Collider is a CLI for testing and discovering hash collisions.
It allows you to specify your own hashing function and evaluate it for potential collisions.

## Disclaimer

While brute-force attacks are generally ineffective against most modern hashing algorithms, they can still be useful to easily determine if a custom hash function is weak.
Using Hash-Collider, you can evaluate the collision resistance of your own hash implementations and identify potential weaknesses.
In addition, with some analysis and the use of targeted word lists, you may be able to find collisions more efficiently than relying solely on generating random character combinations.

## Usage:

```
Usage:
    ./HashQuake [OPTIONS] <command to run the algorithm>
Options:
   -h, --help            Display this help message
   -v, --version         Display the version of the program
   -w, --wordlist <filepath>   Specify a wordlist file
   -s, --size <num>      Specify the length of words to generate (default 3)
   -t, --threads <num>   Specify the number of threads to use (default 1)
   -d --display          Display the collisions in real time
   -o, --output <filepath>      Specify an output file
Example:
    ./HashQuake -w dictionary.txt 'python3 md5.py'
    ./HashQuake -s 8 -o report.txt -d './sha256'

```

### Examples

- Use a custom wordlist and run the `md5.py` python3 script:

```
./HashQuake -w wordlist.txt 'python3 md5.py'
```

- Generate words with a length of 8 characters, save the output to a file, display collisions in real-time running the `sha256` binary.
```
./HashQuake -s 8 -o report.txt -d './sha256'
```

## Importing Your Own Hash Functions

You can use the templates in the [/templates](https://github.com/NeKroFR/HashQuake/tree/main/templates) directory and edit the hashing function as needed.

If you do not want to use the template or there are no templates for your language, you can create your own.
However, note that the program will run the command and add the word as the last argument, and it expects the hash to appear in the program's stdout (standard output).
