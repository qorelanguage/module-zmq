# Qore ZeroMQ module

## Requirements

This module requires ZMQ library version 4.2.2+ and CZMQ library version 4.0.2+.

## Building

```
cd module-zmq
mkdir build
cd build
cmake ..
make
make install
```

Optionally, if the ZMQ or CZMQ libraries are installed in non-standard location, you can use environment variables `ZMQ_DIR` and `CZMQ_DIR` so that cmake can find the libraries:

```
export ZMQ_DIR=/path/to/zmq
export CZMQ_DIR=/path/to/czmq
cmake ..
```