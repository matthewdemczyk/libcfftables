[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://matthewdemczyk.github.io/libcfftables/)
![Build and Test](https://github.com/matthewdemczyk/libcfftables/actions/workflows/test.yaml/badge.svg)


## About

This is a C library for constructing $d$-Cover-Free Families (CFFs). A $d$-CFF($t,n$) is a set system where the ground set has $t$ elements, the set system has $n$ subsets, and no subset is contained in the union of any other $d$ subsets.

It's generally desirable to maximize the number of subsets for a given ground set while maintaining the cover-free property. This library selects the best available CFF construction implemented in the library to do so. "Best" here means maximizing $n$ for fixed $(d,t)$, or minimizing $t$ for fixed $(d,n)$, based on known constructions.

This library stores cover-free families as $0-1$ incidence matrices, using a bitfield. When a $d$-cover-free family is viewed as an incidence matrix it is equivalent to a $d$-disjunct matrix.

## Basic usage
The main feature of this library is the ability to provide a $(d,n)$ and construct a CFF that minimizes $t$, or provide a $(d,t)$ and construct a CFF that maximizes $n$ from our selection of CFF constructions.

Usage:
* Initialize the CFF tables up to some maximum values, and construct a CFF from these tables
* Read the cells of the CFF's incidence matrix, for use in your application
* Free the CFF and tables when done

```c
#include <libcfftables/libcfftables.h>

int main() {
    // Initialize the tables by providing max d=3, t=100, n=2000 values allowed
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);

    // Construct a CFF
    int d = 3;
    int t = 20;
    cff_t *cff = cff_table_get_by_t(ctx, d, t); //get a CFF with d=3, t=20, and maximum known n
    int n = cff_get_n(cff);

    // ...your CFF application code here!...
    cff_print(cff);

    // Iterate over the rows of the CFF's incidence matrix:
    for (int row = 0; row < t; row++) {
        // Iterate over the columns of the CFF's incidence matrix:
        for (int col = 0; col < n; col++) {
            int cell = cff_get_matrix_value(cff, row, col);
            // ...do something with the cell here...
        }
    }

    // Clean up
    cff_free(cff);
    cff_table_free(ctx);
}
```

## Tables of CFFs used

The tables that the library will generate and use are available here: https://matthewdemczyk.github.io/CFFtables/

These precomputed tables go up to $n=100$ trillion and $d=25$. The library can generate tables for larger values. The tables are not hardcoded and are generated dynamically up to the arguments provided to `cff_table_create()`.

## Documentation

Full API documentation is available at: https://matthewdemczyk.github.io/libcfftables/

To build documentation locally:
```bash
doxygen Doxyfile
# Open docs/html/index.html in your browser
```

## Prerequisites

**Supported Platforms:** Linux, macOS

**Note:** Windows is not currently supported

- CMake 3.15 or later
- C99-compatible compiler (GCC, Clang, etc.)
- [FLINT](https://flintlib.org/) library
- pkg-config (optional, for easier linking)

### Installing FLINT

**Ubuntu/Debian:**
```bash
sudo apt-get install libflint-dev
```

**macOS (Homebrew):**
```bash
brew install flint
```

**From source or conda:**
See [FLINT installation guide](https://flintlib.org/downloads.html), or [libflint on conda-forge](https://anaconda.org/channels/conda-forge/packages/libflint/overview)

## Installation

### From Source
```bash
# Clone the repository
git clone https://github.com/matthewdemczyk/libcfftables.git
cd libcfftables

# Build and install
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build .
sudo cmake --install .
```

Or install to a local directory without sudo:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build .
cmake --install .
```

### Using the Library

#### With CMake:
```cmake
find_package(libcfftables REQUIRED)
add_executable(myprogram myprogram.c)
target_link_libraries(myprogram PRIVATE libcfftables::libcfftables)
```

#### With pkg-config:
```bash
gcc myprogram.c $(pkg-config --cflags --libs libcfftables) -o myprogram
```


### Uninstalling

To uninstall, manually remove the installed files:
```bash
# If installed to /usr/local (with sudo)
sudo rm -rf /usr/local/include/libcfftables
sudo rm -f /usr/local/lib/libcfftables.so*
sudo rm -rf /usr/local/lib/cmake/libcfftables
sudo rm -f /usr/local/lib/pkgconfig/libcfftables.pc

# If installed to ~/.local (without sudo)
rm -rf ~/.local/include/libcfftables
rm -f ~/.local/lib/libcfftables.so*
rm -rf ~/.local/lib/cmake/libcfftables
rm -f ~/.local/lib/pkgconfig/libcfftables.pc
```

Note: The exact files removed depend on your `CMAKE_INSTALL_PREFIX`.

## Licensing

This project is licensed under the MIT License.

It links against FLINT, which is licensed under the GNU Lesser General Public License v3.