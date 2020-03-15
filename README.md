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


If you have a reference labels file named `label_ref.txt`, you can have the program check the coherence by using the `--check` flag:
```bash
./Main path/to/folder --check
```

## Python bindings

Once this root C++ library is built or installed (e.g. using `make install`), you can install Python bindings using setuptools:
```bash
cd python
python setup.py build_ext  # with option --inplace for a local build
```
This requires Pybind11.

## Application to semantic 3D point cloud segmentation

The data files (unary potentials and list of edges) included in this repository are generated in the context of a 3D point cloud classification challenge: its code can be found at https://github.com/JulesSanchez/npm3d-challenge. The soft labels are generated using the XGBoost algorithm with handcrafted input features (local covariance and shape features).

More details on the challenge can be found here: https://npm3d.fr/benchmark-for-master-course-on-3d-point-clouds 


## References
* https://hal.inria.fr/file/index/docid/617524/filename/sample2e.pdf
* http://akira.ruc.dk/~keld/teaching/algoritmedesign_f03/Artikler/08/Goldberg88.pdf 
* Simon JD Prince, _Computer vision: models, learning, and inference_. URL: http://www0.cs.ucl.ac.uk/external/s.prince/book/Algorithms.pdf