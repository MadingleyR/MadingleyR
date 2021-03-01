# Compile source code for Mac

To compile the source on Mac the correct compiler is required (e.g. gcc@7, https://formulae.brew.sh/formula/gcc@7). The compiler can for example be installed using Homebrew. More information about Homebrew can be found here: https://brew.sh/index. To install the compiler using Homebrew use the following code from the terminal (bash):

```bash
brew install gcc@7
```

After installing the compiler ```CompileMac.sh``` can be called within the ```SourceCode/madingley``` folder to compile to code from the terminal:

```bash
cd path/to/SourceCode/madingley/
sh CompileMac.sh
```

