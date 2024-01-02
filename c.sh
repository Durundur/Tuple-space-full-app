#!/bin/bash

gcc src/tuple_space.c src/client_main.c src/client.c src/protocol.c -o client


if [ $? -eq 0 ]; then
    ./client
else
    echo "error"
fi