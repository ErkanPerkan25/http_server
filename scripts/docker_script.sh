#!/usr/bin/env bash

root_dir="$(pwd)"

pull=false
attach=false

for arg in "$@"; do
    case args in
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
    echo "PULL is true"
elif [[ $attach == true ]]; then
    echo "Attach is true"
fi
