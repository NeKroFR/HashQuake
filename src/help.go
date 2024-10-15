package main

import (
	"fmt"
	"os"
)

func showHelpMessage() {
	helpMessage := `Usage:
    ./HashQuake [OPTIONS] <command to run the algorithm>
Disclaimer:
   The program will run the command and add the word as the last argument.
   The program expects the hash to appear in stdout.
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
    ./HashQuake -s 8 -o report.txt -d './sha256'`
	fmt.Print(helpMessage)
	os.Exit(0)
}

func showVersionInfo() {
	fmt.Println("HashQuake: 1.0")
	os.Exit(0)
}