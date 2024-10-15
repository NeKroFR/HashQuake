package main

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"
	"strings"
	"sync"
)

const charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
var hashes = make(map[string][]string)
var mu sync.Mutex

func generateWords(word []byte, wordLength int) []string {
	var results []string
	for {
		results = append(results, string(word))
		if !nextWord(word) {
			break
		}
	}
	return results
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

func run_algorithm(word []byte, algorithmCommand string) []byte {
	cmd := exec.Command("bash", "-c", algorithmCommand+" "+string(word))
	var out bytes.Buffer
	cmd.Stdout = &out

	if err := cmd.Run(); err != nil {
		fmt.Printf("Error executing command: %v\n", err)
		return []byte("error")
	}
	return out.Bytes()
}

func worker(words <-chan []byte, algorithmCommand string, displayMode bool, output *os.File, wg *sync.WaitGroup) {
	defer wg.Done()
	for word := range words {
		hash := run_algorithm(word, algorithmCommand)
		hashStr := strings.TrimSpace(string(hash))
		
		mu.Lock()
		if existingWords, exists := hashes[hashStr]; exists {
			existingWords = append(existingWords, string(word))
			hashes[hashStr] = existingWords
			if displayMode {
				outputStr := fmt.Sprintf("Collision found on hash %s: %s", hashStr, strings.Join(existingWords, " -> "))
				fmt.Println(outputStr)
			}
		} else {
			hashes[hashStr] = []string{string(word)}
		}
		mu.Unlock()

		if output != nil {
			output.WriteString(fmt.Sprintf("%s: %s\n", word, hashStr))
		}
	}
}

func collide(wordLength int, displayMode bool, wordlistFile string, outputFile string, threads int, algorithmCommand string) {
	if displayMode {
		fmt.Printf("Word Length: %d\n", wordLength)
		fmt.Printf("Display Mode: %t\n", displayMode)
		fmt.Printf("Wordlist File: %s\n", wordlistFile)
		fmt.Printf("Output File: %s\n", outputFile)
		fmt.Printf("Threads: %d\n", threads)
		fmt.Printf("Algorithm Command: %s\n", algorithmCommand)
		fmt.Println()
	}
	var words []string
	if wordlistFile != "" {
		wordlist, err := os.ReadFile(wordlistFile)
		if err != nil {
			fmt.Printf("Error reading %s: %v\n", wordlistFile, err)
			os.Exit(1)
		}
		lines := strings.Split(string(wordlist), "\n")
		for _, line := range lines {
			if line != "" {
				words = append(words, line)
			}
		}
	} else {
		word := make([]byte, wordLength)
		for i := range word {
			word[i] = charset[0]
		}
		words = generateWords(word, wordLength)
	}
	var output *os.File
	if outputFile != "" {
		var err error
		output, err = os.Create(outputFile)
		if err != nil {
			fmt.Printf("Error creating output %s: %v\n", outputFile, err)
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
	for _, word := range words {
		wordsChan <- []byte(word)
	}
	close(wordsChan)
	wg.Wait()

	totalHashes := len(hashes)
	collisions := 0
	for _, words := range hashes {
		if len(words) > 1 {
			collisions++
		}
	}

	collisionPercentage := (float64(collisions) / float64(totalHashes)) * 100

	fmt.Println("\n--------------------")
	fmt.Printf("Total hashes: %d\n", totalHashes)
	fmt.Printf("Total collisions: %d\n", collisions)
	fmt.Printf("Collision percentage: %.2f%%\n", collisionPercentage)
	fmt.Println("--------------------")
}

