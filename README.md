# Heuristic Optimization assignment

April 2024\
by Alexandre Achten 000494484

## How to use:

To compile the program, run the following command:

```shell
make
```

To clean the directory, run the following command:

```shell
make clean
```

### To run on one instance:

```shell
./lop -i instances/N-tiw56r72_250
```

### Parameters

You can run an instance choosing the parameters :

- Improvement method : --first or --best
- Permutation type : --transpose, --exchange or --insert
- Initial solution : --random or --cw

If no parameters are entered, the default is --first --exchange --random.
The order of the parameters does not matter.

### Example
    
```shell
    ./lop -i instances/N-tiw56r72_250 --best --exchange --cw
```

### VND

On top of that, you can choose VND algorithms instead of simple permutation with the following parameters :
- VND : --vnd1, --vnd2

Where VND1 : transpose -> exchange -> insert

And VND2 : transpose -> insert -> exchange

The default for VND is --first, --cw

### Example 2

```shell
    ./lop -i instances/N-tiw56r72_250 --vnd1
```

## Results

The results are stored in the .csv file named "raw_results/results.csv".

To calculate the relative deviation, just run the R script called "statistics/rel_dev.r" from the root directory.

```shell
    Rscript statistics/rel_dev.r
```

### Best known solutions:
The best known solutions are in best_known/best_known.txt
