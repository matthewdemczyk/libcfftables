[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://matthewdemczyk.github.io/libcfftables/)
![Build and Test](https://github.com/matthewdemczyk/libcfftables/actions/workflows/test.yaml/badge.svg)


## About

This is a C library for constructing $d$-Cover-Free Families (CFFs). A $d$-CFF($t,n$) is a set system where the ground set has $t$ elements, the set system has $n$ subsets, and no subset is contained in the union of any other $d$ subsets.

It's generally desirable to maximize the number of subsets for a given ground set while maintaining the cover-free property. This library will select the best CFF construct that's implemented in the library to do so for the specific $d$ and $t$. There is also an option to provide a specific $d$ and $n$ to minimize $t$.

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

## Licensing

This project is licensed under the MIT License.

It links against FLINT, which is licensed under the GNU Lesser General Public License v3.