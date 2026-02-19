#ifndef FFCFF_WRAPPER_HEADER
#define FFCFF_WRAPPER_HEADER

// to remove the FLINT library dependancy from this codebase, reimplement these functions

int populate_finite_field(int p, int k, int *addition_field, int *multiplication_field);
int compute_field_size(int p, int k);
int horner_polynomial_eval_over_fq(
    int polyLength,
    int *polynomialCoefficients,
    int x,
    int q,
    int *addition_field,
    int *multiplication_field
);
void populate_multiplicative_inverses(int p, int k, int *multiplication_field, int *multiplicative_inverses);
void populate_additive_inverses(int p, int k, int *addition_field, int *additive_inverses);

#endif