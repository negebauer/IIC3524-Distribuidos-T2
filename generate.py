import os
from sys import argv
from random import randint

if len(argv) != 3:
    print('Call like: {} <name> <size>'.format(argv[0]))
    print('\t<name> Name for the txt to be put in test/')
    print('\t<size> Size of the problem')

file_path = "test/" + argv[1] + ".txt"
size = int(argv[2])

with open(file_path, 'w') as file:
    file.write('{}\n'.format(size))
    while size > 0:
        for i in range(0, size):
            space = ' ' if i != size - 1 else ''
            file.write('{}{}'.format(randint(1, 9), space))
        file.write('\n')
        size-=1
