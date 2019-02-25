# Workspace configure utility

This repository presents workspace configure utility, that provide fast and simple setting your workspace;

Utility have been wrote using C language and ncurses library;

## Build

First of all you need install ncurses library using apt-get or another package manager:

```
$ sudo apt-get install -y libncurses5-dev libncurses5 libcurl-dev
```

Download and build libarchive:
```
$ curl -LO https://github.com/libarchive/libarchive/archive/v3.3.1.tar.gz
```
```
$ tar -xvf v3.3.1.tar.gz && cd v3.3.1
```

```
$ ./configure && make install
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

Clean temp directory:
```
$ ./bin/wscu --remove
```

Run utility using config:
```
$ ./bin/wscu --config="config.yaml"
```

```
$ ./bin/wscu -c ./config/config.yaml
```

Generate workflow configuration file:
```
$ ./bin/wscu --configure
```

Use help to print usage information:
```
$ ./bin/wscu --help
```

(will be updated)

## License

GPL-3.0