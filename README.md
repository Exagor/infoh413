# Heuristic Optimization assignment

Adapted by Jérémie Dubois-Lacoste from the ILSLOP implementation
of Tommaso Schiavinotto.

Completed by Alexandre Achten



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

If no parameters are entered, the default is --first --exchange --random

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

### Best known solutions:
The best known solutions are in best_known/best_known.txt
