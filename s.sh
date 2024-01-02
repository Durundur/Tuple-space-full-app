#!/bin/bash

gcc src/tuple_space.c src/server_main.c src/server.c src/protocol.c -o server


if [ $? -eq 0 ]; then
    ./server
else
    echo "error"
fi