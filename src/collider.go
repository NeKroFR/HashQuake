package main

import (
	"fmt"
	"os"
	"strings"
	"sync"

	// "time" // for testing the run_algorithm function
)

const charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"


func run_algorithm(word []byte, algorithmCommand string) []byte {
	//time.Sleep(4 * time.Millisecond)
	hash := "42"
	return []byte(hash)
}

func worker(words <-chan []byte, algorithmCommand string, displayMode bool, output *os.File, wg *sync.WaitGroup) {
	defer wg.Done()
	for word := range words {
		hash := run_algorithm(word, algorithmCommand)
		if displayMode {
			fmt.Printf("Current word: %s, Hash: %x\n", word, hash)
		}

		if output != nil {
			output.WriteString(fmt.Sprintf("%s\n", word))
		}
	}
}

func nextWord(current []byte) bool {
	for i := len(current) - 1; i >= 0; i-- {
		j := strings.IndexRune(charset, rune(current[i]))
		if j == -1 {
			fmt.Printf("Error: Invalid character '%c' in the input string\n", current[i])
			os.Exit(1)
		}

		if j == len(charset)-1 {
			current[i] = charset[0]
		} else {
			current[i] = charset[j+1]
			return true
		}
	}
	return false
}


func collide(wordLength int, displayMode bool, wordlistFile string, outputFile string, threads int, algorithmCommand string) {
	fmt.Printf("Word Length: %d\n", wordLength)
	fmt.Printf("Display Mode: %t\n", displayMode)
	fmt.Printf("Wordlist File: %s\n", wordlistFile)
	fmt.Printf("Output File: %s\n", outputFile)
	fmt.Printf("Threads: %d\n", threads)
	fmt.Printf("Algorithm Command: %s\n", algorithmCommand)

	var word []byte

	if wordlistFile != "" {
		wordlist, err := os.ReadFile(wordlistFile)
		if err != nil {
			fmt.Printf("Error reading wordlist file: %v\n", err)
			os.Exit(1)
		}
		lines := strings.Split(string(wordlist), "\n")
		word = []byte(lines[0])
	} else {
		word = make([]byte, wordLength)
		for i := range word {
			word[i] = charset[0]
		}
	}
	var output *os.File
	if outputFile != "" {
		var err error
		output, err = os.Create(outputFile)
		if err != nil {
			fmt.Printf("Error creating output file: %v\n", err)
			os.Exit(1)
		}
		defer output.Close()
	}

	wordsChan := make(chan []byte)
	var wg sync.WaitGroup

	for i := 0; i < threads; i++ {
		wg.Add(1)
		go worker(wordsChan, algorithmCommand, displayMode, output, &wg)
	}

	if wordlistFile != "" {
		for _, line := range strings.Split(string(word), "\n") {
			wordsChan <- []byte(line)
		}
	} else {
		looping := true
		for looping {
			wordsChan <- word
			looping = nextWord(word)
		}
	}

	close(wordsChan)
	wg.Wait()

	fmt.Println("All words processed.")
}
