#!/bin/bash

# Path to the "instances" folder
instances_folder="instances"

# Loop through all files in the "instances" folder
for file in "${instances_folder}"/*150*; do
    if [[ -f "${file}" ]]; then
        for algo_option in "--vnd1" "--vnd2"; do
            ./lop -i "${file}" "${algo_option}" "${permut_option}" "${init_option}"
        done
    fi
done