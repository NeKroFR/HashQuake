import sys

def hashing(word: str):
    return word.lower()

if len(sys.argv) < 2:
    print("Error: No word specified.")
    sys.exit(1)

print(hashing(" ".join(sys.argv[1:])))
