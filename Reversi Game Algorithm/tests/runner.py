#!/usr/bin/env python3

import argparse
import difflib
import pathlib
import subprocess
import sys

TESTS_DIR = pathlib.Path(__file__).resolve().parent

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def splitlines(s):
    lines = []
    for line in s.splitlines(keepends=True):
        add_newline = line.endswith("\n")
        line = line.strip()
        if add_newline:
            line += "\n"
        lines.append(line)
    return lines

def check_null_byte(lines):
    found = False
    for line in lines:
        if "\0" in line:
            line = line.replace("\0", "")
            if line.endswith("\n"):
                eprint("!ERROR line contains null (0) byte:", line, end='')
            else:
                eprint("!ERROR line contains null (0) byte:", line)
                eprint("\\ No newline at end of file")

            found = True
    if found:
        sys.exit(1)

def run(test, exe):
    kind, name = test.split('/', maxsplit=1)
    input_path = TESTS_DIR / kind / "inputs" / name
    output_path = TESTS_DIR / kind / "outputs" / name
    timeout = 10

    with open(output_path, "r") as f:
        expected = f.read()

    stdin = None
    if input_path.exists():
        with open(input_path, 'r') as f:
            stdin = f.read()
    
    try:
        p = subprocess.run(
            [exe], capture_output=True, input=stdin, timeout=timeout, text=True
        )
    except subprocess.TimeoutExpired:
        eprint("Timeout")
        return 1

    if p.returncode != 0:
        eprint("Your program returned an error")
        return 1

    stdout_lines = splitlines(p.stdout)
    check_null_byte(stdout_lines)
    expected_lines = splitlines(expected)
    has_diff = False
    for line in difflib.unified_diff(stdout_lines, expected_lines,
                                     fromfile='output', tofile='expected', n=5):
        has_diff = True
        if line.endswith("\n"):
            eprint(line, end='')
        else:
            eprint(line)
            eprint("\\ No newline at end of file")
    if has_diff:
        return 1
    return 0

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("test")
    parser.add_argument("exe")
    args = parser.parse_args()

    return run(args.test, args.exe)

if __name__ == "__main__":
    sys.exit(main())
