## Compile source code for Linux

On Linux the source code can be downloaded and extracted using the code below.

```R
# The source code can be downloaded using
src_path = madingley_download_source(madingley_path = "~/Documents")
```

To compile and install the newly compiled source directly into the MadingleyR library use the following:

```R
# src_path and the downloaded files can be used in madingley_compile()
madingley_compile(madingley_path = src_path, install_new_binary = TRUE)
```

By default ```install_new_binary``` is set to ```FALSE```, this will not overwrite the already existing executable in the package library path. Instead, it only compiles the source code and places the newly created executable into a subfolder within the source code path labelled with the compilation date.  