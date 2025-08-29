#!/usr/bin/env bash

root_dir="$(pwd)"

pull=false
attach=false

for arg in "$@"; do
    case $arg in
        --pull)
            pull=true
            shift
            ;;
        --attach)
            attach=true
            shift
            ;;
        *)
            echo "Error: Invalid flag."
            ;;
    esac
done

if  [[ $pull == true ]]; then
    echo "Building image"
    docker build -t http_server .
elif [[ $attach == true ]]; then
    echo "Attaching container"
    docker exec -i http_server bash
else
    echo "Starting HTTP server"
    docker run --rm -it --name="http_server" -v "${root_dir}":/http_server -w="/http_server" -p 8080:8080 http_server
fi
