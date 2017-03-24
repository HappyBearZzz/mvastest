# mvastest

Small test suite to test the functionality of first class virtual address spaces in the Linux kernel.

## build

You can simply build the tests with the following command

```bash
make
```

The tests require that [libmvas](https://github.com/l3nkz/libmvas "libmvas") is installed on the computer.

## run

You can execute the benchmarks by executing the binary directly or via

```bash
make run
```

You will be presented with an interactive menu that you can use to test the various different features of
multiple virtual address spaces or run the different benchmarks.
