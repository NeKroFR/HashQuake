package main

import (
    "bytes"
    "fmt"
    "os"
    "os/exec"
    "strings"
    "sync"
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

func worker(words <-chan []byte, algorithmCommand string, report *Report, wg *sync.WaitGroup) {
    defer wg.Done()
    for word := range words {
        hash := run_algorithm(word, algorithmCommand)
        hashStr := strings.TrimSpace(string(hash))
        report.recordCollision(hashStr, string(word))
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

    wordsChan := make(chan []byte)
    var wg sync.WaitGroup

    for i := 0; i < threads; i++ {
        wg.Add(1)
        go worker(wordsChan, algorithmCommand, report, &wg)
    }

    for _, word := range words {
        wordsChan <- []byte(word)
    }
    close(wordsChan)
    wg.Wait()

    report.writeFooter()
}