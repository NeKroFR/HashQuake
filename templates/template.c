package main

import (
	"flag"
	"fmt"
)

func main() {
	// Define both short and long versions of flags using flag.Var()
	showHelp := flag.Bool("help", false, "Display this help message")
	showVersion := flag.Bool("version", false, "Display the version of the program")
	wordLength := flag.Int("size", 3, "Specify the length of words to generate (default 3)")
	displayMode := flag.Bool("display", false, "Display the collisions in real time (default: false)")
	wordlistFile := flag.String("wordlist", "", "Specify a wordlist file")
	outputFile := flag.String("output", "", "Specify an output file to store the report")

	// Manually set short flags to match long versions
	flag.CommandLine.BoolVar(showHelp, "h", false, "Display this help message (short)")
	flag.CommandLine.BoolVar(showVersion, "v", false, "Display the version of the program (short)")
	flag.CommandLine.IntVar(wordLength, "s", 3, "Specify the length of words to generate (short)")
	flag.CommandLine.BoolVar(displayMode, "d", false, "Display the collisions in real time (short)")
	flag.CommandLine.StringVar(wordlistFile, "w", "", "Specify a wordlist file (short)")
	flag.CommandLine.StringVar(outputFile, "o", "", "Specify an output file to store the report (short)")

	// Parse the flags
	flag.Parse()

	// Handle help and version flags immediately
	if *showHelp {
		showHelpMessage() // Renamed function call
	}
	if *showVersion {
		showVersionInfo() // Renamed function call
	}

	// Validation: Check if necessary arguments are set
	if flag.NArg() == 0 {
		fmt.Println("Error: Command to run the algorithm must be specified.")
		showHelpMessage() // Renamed function call
	}

	// Example validation: If both wordlistFile and outputFile are empty, it might be invalid
	if *wordlistFile == "" && *outputFile == "" {
		fmt.Println("Error: Both wordlist and output cannot be empty.")
		showHelpMessage() // Renamed function call
	}

	algorithmCommand := flag.Arg(0)

	// Example function calls based on flags
	fmt.Printf("Word Length: %d\n", *wordLength)
	fmt.Printf("Display Mode: %t\n", *displayMode)
	fmt.Printf("Wordlist File: %s\n", *wordlistFile)
	fmt.Printf("Output File: %s\n", *outputFile)
	fmt.Printf("Algorithm Command: %s\n", algorithmCommand)

	// Add your further logic here, e.g., running the collision check
}

// Display help message
func showHelpMessage() { // Renamed function
	fmt.Println("Usage: ./HashQuake [OPTIONS] <command to run the algorithm>")
	fmt.Println("\nDisclaimer:")
	fmt.Println("   The program will run the command and add the word as the last argument.")
	fmt.Println("   The program expects the hash to appear in stdout.")
	fmt.Println("\nOptions:")
	fmt.Println("   -h, --help            Display this help message")
	fmt.Println("   -v, --version         Display the version of the program")
	fmt.Println("   -w, --wordlist <filepath>   Specify a wordlist file")
	fmt.Println("   -s, --size <num>      Specify the length of words to generate (default 3)")
	fmt.Println("   -d --display          Display the collisions in real time")
	fmt.Println("   -o, --output <filepath>      Specify an output file")
	fmt.Println("\nExample:")
	fmt.Println("    ./HashQuake -w dictionary.txt 'python3 md5.py'")
	fmt.Println("    ./HashQuake -s 8 -o report.txt -d './sha256'")
}

// Display version
func showVersionInfo() { // Renamed function
	fmt.Println("HashQuake: 1.0")
}
