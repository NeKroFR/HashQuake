package main

import (
	"fmt"
	"os"
)

func showHelpMessage() {
	helpMessage := `Usage:
    ./HashQuake [OPTIONS] <command to run the algorithm>

Description:
   If -w/--wordlist is provided, HashQuake iterates the given list.
   If no wordlist is provided, HashQuake performs a birthday attack by
   generating random words of length -s/--size until a collision is found.

   The <command> is a positional argument and will be executed with the
   candidate word appended as the last argument. If your hasher reads from
   stdin, wrap it yourself (e.g., "bash -c 'echo -n \"$1\" | sha1sum'").

Options:
   -h, --help                   Display this help message
   -v, --version                Display the version of the program
   -w, --wordlist <filepath>    Specify a wordlist file
   -s, --size <num>             Specify the length of words to generate (default 3)
   -t, --threads <num>          Specify the number of threads to use (default 1)
   -d, --display                Display the collisions in real time
   -o, --output <filepath>      Specify an output file

Examples:
   # Wordlist mode
   ./HashQuake -w wordlist.txt -t 4 -d 'python3 examples/custom_hash1.py'

   # Birthday attack (no -w)
   ./HashQuake -t 8 -s 6 -d './examples/custom_hash2'
`
	fmt.Print(helpMessage)
	os.Exit(0)
}

func showVersionInfo() {
	fmt.Println("HashQuake: 1.0.1")
	os.Exit(0)
}
