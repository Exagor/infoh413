#!/bin/bash

# Path to the "instances" folder
instances_folder="instances"

# Loop through all files in the "instances" folder
for file in "${instances_folder}"/*150*; do # "/*150*" if we want only 150 size instances
    if [[ -f "${file}" ]]; then
        for algo_option in "--meme"; do #"--ils"
            ./lop -i "${file}" "${algo_option}"
        done
    fi
done