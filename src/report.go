// report.go

package main

import (
    "fmt"
    "os"
    "sync"
    "time"
    "strings"
)

type Report struct {
    startTime     time.Time
    command       string
    threads       int
    wordlist      string
    wordSize      int
    collisions    map[string][]string
    mu            sync.Mutex
    displayMode   bool
    outputFile    *os.File
}

func NewReport(command string, threads int, wordlist string, wordSize int, displayMode bool, outputPath string) (*Report, error) {
    var output *os.File
    var err error
    if outputPath != "" {
        output, err = os.Create(outputPath)
        if err != nil {
            return nil, fmt.Errorf("error creating output file: %v", err)
        }
    }

    return &Report{
        startTime:     time.Now(),
        command:       command,
        threads:       threads,
        wordlist:      wordlist,
        wordSize:      wordSize,
        collisions:    make(map[string][]string),
        displayMode:   displayMode,
        outputFile:    output,
    }, nil
}

func (r *Report) writeHeader() {
    header := fmt.Sprintf(`===============================================================
HashQuake v1.0.0
===============================================================
[+] Runned command: '%s'
[+] Threads:    %d
[+] Wordlist:   %s
[+] Word size:  %d
===============================================================
%s Starting HashQuake
===============================================================

`, r.command, r.threads, 
        func() string {
            if r.wordlist == "" {
                return "No wordlist (using generated words)"
            }
            return r.wordlist
        }(),
        r.wordSize,
        r.startTime.Format("2006/01/02 15:04:05"))

    if r.displayMode {
        fmt.Print(header)
    }
    if r.outputFile != nil {
        r.outputFile.WriteString(header)
    }
}

func (r *Report) writeFooter() {
    totalHashes := len(r.collisions)
    collisions := 0
    for _, words := range r.collisions {
        if len(words) > 1 {
            collisions++
        }
    }

    collisionPercentage := float64(0)
    if totalHashes > 0 {
        collisionPercentage = (float64(collisions) / float64(totalHashes)) * 100
    }

    footer := fmt.Sprintf(`
===============================================================
%s Finished
===============================================================

===============================================================
Statistics:
===============================================================
- Total unique hashes: %d
- Total collisions found: %d
- Collision percentage: %.2f%%
===============================================================
`, time.Now().Format("2006/01/02 15:04:05"),
        totalHashes,
        collisions,
        collisionPercentage)

    if r.displayMode {
        fmt.Print(footer)
    }
    if r.outputFile != nil {
        r.outputFile.WriteString(footer)
    }
}

func (r *Report) recordCollision(hash string, word string) {
    r.mu.Lock()
    defer r.mu.Unlock()

    if existingWords, exists := r.collisions[hash]; exists {
        r.collisions[hash] = append(existingWords, word)
        collisionMsg := fmt.Sprintf("Found: %s  [%s]\n", 
            strings.Join(r.collisions[hash], " -> "), 
            hash)
        
        if r.displayMode {
            fmt.Print(collisionMsg)
        }
        if r.outputFile != nil {
            r.outputFile.WriteString(collisionMsg)
        }
    } else {
        r.collisions[hash] = []string{word}
    }
}

func (r *Report) Close() {
    if r.outputFile != nil {
        r.outputFile.Close()
    }
}