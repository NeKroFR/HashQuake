import sys

def hashing(word):
    return word

if len(sys.argv) < 2:
    print("Error: No word specified.")
    sys.exit(1)

print(hashing(" ".join(sys.argv[1:])))
