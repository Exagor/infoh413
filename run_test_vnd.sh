#!/bin/bash

# Path to the "instances" folder
instances_folder="instances"

# Loop through all files in the "instances" folder
for file in "${instances_folder}"/*; do # "/*150*" if we want only 150 size instances
    if [[ -f "${file}" ]]; then
        for algo_option in "--vnd1" "--vnd2"; do
            ./lop -i "${file}" "${algo_option}"
        done
    fi
done