package main

import (
	"flag"
	"fmt"
	"os"
	"runtime"
)

func main() {
	showHelp := flag.Bool("help", false, "Display this help message")
	showVersion := flag.Bool("version", false, "Display the version of the program")
	wordLength := flag.Int("size", 3, "Specify the length of words to generate (default 3)")
	displayMode := flag.Bool("display", false, "Display the collisions in real time (default: false)")
	wordlistFile := flag.String("wordlist", "", "Specify a wordlist file")
	outputFile := flag.String("output", "", "Specify an output file to store the report")
	threads := flag.Int("threads", 1, "Specify the number of threads to use (default 1)")

	flag.CommandLine.BoolVar(showHelp, "h", false, "Display this help message (short)")
	flag.CommandLine.BoolVar(showVersion, "v", false, "Display the version of the program (short)")
	flag.CommandLine.IntVar(wordLength, "s", 3, "Specify the length of words to generate (short)")
	flag.CommandLine.BoolVar(displayMode, "d", false, "Display the collisions in real time (short)")
	flag.CommandLine.StringVar(wordlistFile, "w", "", "Specify a wordlist file (short)")
	flag.CommandLine.StringVar(outputFile, "o", "", "Specify an output file to store the report (short)")
	flag.CommandLine.IntVar(threads, "t", 1, "Specify the number of threads to use (short)")
	flag.Parse()
	algorithmCommand := flag.Arg(0)

	if *showHelp {
		showHelpMessage()
		os.Exit(0)
	}
	if *showVersion {
		showVersionInfo()
		os.Exit(0)
	}

	if *wordLength < 1 { 
		fmt.Println("Error: Word length must be greater than 0.")
		os.Exit(1)
	}

	if *threads < 1 {
		fmt.Println("Error: Number of threads must be greater than 0.")
		os.Exit(1)
	}

	if flag.NArg() == 0 {
		fmt.Println("Error: Command to run the algorithm must be specified.")
		showHelpMessage()
		os.Exit(1)
	}

	if *wordlistFile != "" {
		if _, err := os.Stat(*wordlistFile); os.IsNotExist(err) {
			fmt.Printf("Error: Wordlist file '%s' does not exist.\n", *wordlistFile)
			os.Exit(1)
		} else if err != nil {
			fmt.Printf("Error: Unable to access wordlist file '%s': %v\n", *wordlistFile, err)
			os.Exit(1)
		}
	}

	maxThreads := runtime.NumCPU()
	if *threads > maxThreads {
		*threads = maxThreads
	}
	fmt.Printf("Colliding with %d threads...\n", *threads)
	collide(*wordLength, *displayMode, *wordlistFile, *outputFile, *threads, algorithmCommand)
	fmt.Printf("Done.\n")
}
