# gsip
Simple tool for filtering stdin by matching "similar" lines (instead of precise match like regexp in grep)

# How it works?
Program takes "acceptable similarity ratio" as first arg. Then it compares every line incoming on stdin with every line in `gsip.ignore` file.
If similarity ratio for such comparison is below acceptable ratio, it prints line on stdout.

Similarity calculation for string1 and string2 is basically count of words from string2 that reappears in string
divided by count of all words in string1:
`(float)reappearingCount / tokensInFirstStringCount`

There is also optional delimiter argument, which can specify tokenization delimiters.

# Compilation
`make`

# Usage
```
% ./gsip -h

usage: ./gsip -[fahvrd]

  Options:
    -f [MATCH FILE NAME]            [MANDATORY] name of file with lines to match
    -a [ACCEPTABLE RATIO]           [MANDATORY] acceptable ratio (value between 0.00-1.00)
    -h                              print this help
    -v                              print highest ratio per line
    -r                              reverse match (print lines least matching the match file)
    -d                              set delimiter for tokenization (" \n" is default)
```
# Example usage
`$ somethingThatGeneratesOutput | gsip -a "0.4" -f matchfile`

or

`$ somethingThatGeneratesOutput | gsip -a "0.4" -f matchfile -d " |\n"`
