import sys
from base64 import b64encode

def hashing(word: str):
    """
    Convert a string to its base64 representation after converting it to lowercase.
    This is a really weak hash function as it is easily reversible and case-insensitive (will generate collisions beetween 'AA','aa', 'Aa' and 'aA').
    
    Example:
        >>> hashing("Appl3")
        'YXBwbDM='  # Process: "Appl3" -> "appl3" -> bytes -> base64
    
    """
    return b64encode(word.lower().encode()).decode()

if len(sys.argv) < 2:
    print("Error: No word specified.")
    sys.exit(1)

print(hashing(" ".join(sys.argv[1:])))
