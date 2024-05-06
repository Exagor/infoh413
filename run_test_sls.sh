#!/bin/bash

# Path to the "instances" folder
instances_folder="instances"

# Loop through all algo options
for algo_option in "--meme"; do #"--ils" or "--meme"
    # Loop through all files in the "instances" folder
    for file in "${instances_folder}"/*150*; do # "/*150*" if we want only 150 size instances
        if [[ -f "${file}" ]]; then
            ./lop2 -i "${file}" "${algo_option}"
        fi
    done
done