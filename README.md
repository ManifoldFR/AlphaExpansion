# AlphaExpansion

An implementation of alpha-expansion and graph cut in C++. This is 

We use the [Boost Graph library](https://www.boost.org/doc/libs/1_72_0/libs/graph/doc/index.html).

## Building

```bash
mkdir build && cd build/
cmake ../
make
```
or if you are using the command line on Linux:
```bash
./build.sh
```

## Use

Run the executable from the command line with
```bash
./Main path/to/folder
```
where the folder should contain the node potentials and edges as
```
nodes.txt
edges.txt
```



## References
* https://hal.inria.fr/file/index/docid/617524/filename/sample2e.pdf
* http://akira.ruc.dk/~keld/teaching/algoritmedesign_f03/Artikler/08/Goldberg88.pdf 
