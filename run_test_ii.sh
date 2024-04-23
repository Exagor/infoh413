#!/bin/bash

# Path to the "instances" folder
instances_folder="instances"

# Loop through all files in the "instances" folder
for file in "${instances_folder}"/*150*; do
    # Check if the file is a regular file
    if [[ -f "${file}" ]]; then
        # Run the lop program with different parameter combinations
        for algo_option in "--first"; do # "--best" or "--random"
            for permut_option in "--exchange" "--transpose" "--insert"; do
                for init_option in "--cw" "--random"; do
                    ./lop -i "${file}" "${algo_option}" "${permut_option}" "${init_option}"
                done
            done
        done
    fi
done