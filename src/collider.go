package main

import (
	"bytes"
	"context"
	"fmt"
	"math/rand"
	"os"
	"os/exec"
	"strings"
	"sync"
	"time"
)

// TODO: display current word in bottom
const charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

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

func worker(ctx context.Context, cancel context.CancelFunc, words <-chan []byte, algorithmCommand string, report *Report, wg *sync.WaitGroup) {
	defer wg.Done()
	for {
		select {
		case <-ctx.Done():
			return
		case word, ok := <-words:
			if !ok {
				return
			}
			select {
			case <-ctx.Done():
				return
			default:
			}

			hash := run_algorithm(word, algorithmCommand)
			hashStr := strings.TrimSpace(string(hash))

			if report.recordCollision(hashStr, string(word)) {
				cancel()
				return
			}
		}
	}
}

func randomWord(randSrc *rand.Rand, length int) []byte {
	w := make([]byte, length)
	for i := 0; i < length; i++ {
		w[i] = charset[randSrc.Intn(len(charset))]
	}
	return w
}

func birthdayGenerator(ctx context.Context, words chan<- []byte, length int) {
	randSrc := rand.New(rand.NewSource(time.Now().UnixNano()))
	for {
		select {
		case <-ctx.Done():
			return
		default:
		}

		w := randomWord(randSrc, length)

		select {
		case <-ctx.Done():
			return
		case words <- w:
			// sent
		default:
			// channel full: yield and try again
			time.Sleep(100 * time.Microsecond)
		}
	}
}

func collide(wordLength int, displayMode bool, wordlistFile string, outputFile string, threads int, algorithmCommand string) {
	report, err := NewReport(algorithmCommand, threads, wordlistFile, wordLength, displayMode, outputFile)
	if err != nil {
		fmt.Printf("Error creating report: %v\n", err)
		os.Exit(1)
	}
	defer report.Close()

	report.writeHeader()

	var wordsList []string
	useBirthday := false

	if wordlistFile != "" {
		wordlist, err := os.ReadFile(wordlistFile)
		if err != nil {
			fmt.Printf("Error reading %s: %v\n", wordlistFile, err)
			os.Exit(1)
		}
		lines := strings.Split(string(wordlist), "\n")
		for _, line := range lines {
			if line != "" {
				wordsList = append(wordsList, line)
			}
		}
	} else {
		useBirthday = true
	}

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	wordsChan := make(chan []byte, 1024)
	var wg sync.WaitGroup

	for i := 0; i < threads; i++ {
		wg.Add(1)
		go worker(ctx, cancel, wordsChan, algorithmCommand, report, &wg)
	}

	if useBirthday {
		generators := threads
		if generators < 1 {
			generators = 1
		}
		var genWg sync.WaitGroup
		for g := 0; g < generators; g++ {
			genWg.Add(1)
			go func() {
				defer genWg.Done()
				birthdayGenerator(ctx, wordsChan, wordLength)
			}()
		}

		// Polling here is mostly redundant because the discovering worker calls cancel(),
		// but we keep it as a safety net for any other code path.
		for {
			if report.HasFoundCollision() {
				cancel()
				break
			}
			time.Sleep(10 * time.Millisecond)
		}

		genWg.Wait()
		close(wordsChan)
		wg.Wait()
	} else {
	feedLoop:
		for _, w := range wordsList {
			select {
			case <-ctx.Done():
				break feedLoop
			default:
				wordsChan <- []byte(w)
				if report.HasFoundCollision() {
					cancel()
					break feedLoop
				}
			}
		}
		close(wordsChan)
		wg.Wait()
	}

	report.writeFooter()
}
