#!/usr/bin/env python
import sys
import re

erp_filename = sys.argv[1]
num_slaves = int(sys.argv[2])
assert erp_filename[-4:] == '.erp'
target_filename = erp_filename[:-4]

erp_file = open(erp_filename, 'r')
target_lines = []

gen = False

for line in erp_file:
    match = re.search(r'--\s+EVAL(.*)$', line)
    if match:
        eval(match.group(1))
        next
    if gen:
        if re.search(r'--\s+END', line):
            for index in gen_range:
                for gen_line in gen_lines:
                    target_lines.append(gen_line.format(**locals()))
            gen = False
        else:
            gen_lines.append(line)
    else:
        match = re.search(r'--\s+BEGIN\s+(.*)$', line)
        if not match:
            target_lines.append(line.format(**locals()))
        else:
            gen_range = eval(match.group(1))
            gen_lines = []
            gen = True

erp_file.close()

target_file = open(target_filename, 'w')
target_file.write(''.join(target_lines))
target_file.close()
