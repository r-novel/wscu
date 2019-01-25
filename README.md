# Workspace configure utility

This repository presents workspace configure utility, that provide fast and simple setting your workspace;

Utility have been wrote using C language and ncurses library;

## Build

First of all you need install ncurses library using apt-get or another package manager:

```
$ sudo apt-get install -y libncurses5-dev libncurses5
```

Then build test program:

```
$ make
```
Clean obj and exec files:

```
$ make clean
```

(will be updated)

## Usage

Create temp dot directory and download tools using default variables:
```
$ ./bin/wscu
```

Make temp directory:
```
$ ./bin/wscu --make=".wscu_custom_dir"
```

Remove temp directory:
```
$ ./bin/wscu --remove=".wscu_custom_dir"
```

Run utility with custom options:
```
$ ./bin/wscu --dir=".wscu"
```

Use help to print usage information:
```
$ ./bin/wscu --help
```

(will be updated)

## License

GPL-3.0