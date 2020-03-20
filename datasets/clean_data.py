import sys
from string import ascii_letters

if len(sys.argv) != 2:
    print('Usage: {} <filename>'.format(sys.argv[0]))
    exit(1)

formatted_output = ''
with open(sys.argv[1], 'r') as input_file:
    for ch in input_file.read():
        if ch in ascii_letters:
            formatted_output += ch.lower()
        elif ch == '\n' or ch == ' ':
            if formatted_output[len(formatted_output)-1] != ' ':
                formatted_output += ' '

with open(sys.argv[1], 'w') as output_file:
    output_file.write(formatted_output.lower())
