# MIC_Polytope
A team project done in an advanced algorithms and data structures about the [The Maximum-Impact Coloring Polytope](http://sedici.unlp.edu.ar/handle/10915/59241)

## Usage
To create the executable file, run the `make` command from the `/src` folder. Then the `tp2` executable can be found in the `/build` folder.

The script should use one of the following arguments to work with the desired algorithm:
- `H1`: Heuristic 1
- `H2`: Heuristic 2
- `MH1`: Metaheuristic 1
- `MH2`: Metaheuristic 2

Lastly, for example, to execute the first heuristic over instance `INST1.TXT`, the next command is needed:
```
cat INST1.txt | ./tp2 BT-O
```


