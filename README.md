# Heuristic Optimization assignment

May 2024\
by Alexandre Achten 000494484

## How to use

To compile the programs (lop and lop2), run the following command:

```shell
make
```

To clean the directory, run the following command:

```shell
make clean
```

### To run on one instance

```shell
./lop -i instances/N-tiw56r72_250
```

or

```shell
./lop2 -i instances/N-tiw56r72_250
```

### Iterated improvement (lop)

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

### VND (lop)

On top of that, you can choose VND algorithms instead of simple permutation with the following parameters :

- VND : --vnd1, --vnd2

Where VND1 : transpose -> exchange -> insert

And VND2 : transpose -> insert -> exchange

The default for VND is --first, --cw

### Example 2

```shell
    ./lop -i instances/N-tiw56r72_250 --vnd1
```

### Stochastic local search (lop2)

To run the memetic algorithm or the iterated local search, the lop2 program must be used.
The parameters to enter are :

- Memetic algorithm (MA) : --meme
- Iterated local search (ILS) : --ils

### Example 3

```shell
    ./lop2 -i instances/N-tiw56r72_250 --meme
```

or

```shell
    ./lop2 -i instances/N-tiw56r72_250 --ils
```

### During runtime results

To save the in-runtime results of the program, add the --runtime parameter. The in-runtime statistics will be saved in the "raw_results/runtimes" folder. It works only on the lop2 program.

### Example 4

```shell
    ./lop2 -i instances/N-tiw56r72_250 --meme --runtime
```

## Results

The results are stored in the .csv file named "raw_results/results.csv" or "raw_results/results2.csv" depending on the program used (lop, lop2).

To calculate the relative deviation, just run the R script called "statistics/rel_dev.r" from the root directory.

```shell
    Rscript statistics/rel_dev.r
```

### Best known solutions

The best known solutions are in best_known/best_known.txt
