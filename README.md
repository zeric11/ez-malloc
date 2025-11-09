# ez-malloc

Practicing custom malloc implementation in C based on [this tutorial](https://danluu.com/malloc-tutorial/).

**NOTE:** Uses the `sbrk` system call which is only available in Unix-based systems. <br>
On MacOS, a warning that `sbrk` is deprecated may appear, but it should still work just fine.


## Usage

Create `build` folder if it does not exist:
```sh
$ mkdir build
```

Build and run executable with `make`:
```sh
$ make
$ ./build/ez_malloc_test
```

Using `valgrind` to test for memory leaks (**LINUX ONLY**):
```sh
$ valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/ez_malloc_test
```

Clean executable:
```sh
$ make clean
```