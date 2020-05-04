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
`$./compile.sh`

# Example usage
`$ somethingThatGeneratesOutput | gsip "0.4"`
or
`$ somethingThatGeneratesOutput | gsip "0.4" " |\n"`
