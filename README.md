## libroman
A simple, small roman numerals library in C. It has no dependencies and only requires an ANSI C compiler and appropriate header files.

### Compilation
Use GNU Make to build the sources:
```
$ make
```

There should now be several binaries:
* The static library `libroman.a`
* The dynamic library `libroman.so` (`.dylib` on Mac)
* The sample executable `roman`

### Usage
The `roman` binary takes one or more values as arguments. Any numeric arguments (e.g. "23" or "-101") are converted to their roman numeral forms, and any other strings are converted from roman numeral form to standard arabic numbers.

The API is documented in the man pages provided.

### License
[MIT](LICENSE)

