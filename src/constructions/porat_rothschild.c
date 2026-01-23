#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>

#include "../CFF_Internals.h"
#include "finite_fields_wrapper.h"


// will be returned, completely filled, by the porat construction
typedef struct GeneratorMatrixStruct {
    int q;
    int k;
    int r;
    int m;
    double Hq;
    double delta;
    int* generatorMatrix;
    int* code;
    int numCodewords;
    int minDistance;
} GeneratorMatrixStruct;

cff_t* genmatrixToCFF(GeneratorMatrixStruct gs)
{
    cff_t* cff = cff_alloc(
        (gs.m - 1 ) / (gs.m - (gs.minDistance)),// d
        gs.m*gs.q, // t
        (int)pow(gs.q, gs.k) //n
    );
    int (*code)[cff->n] = (int (*)[cff->n]) gs.code;
    for (int codewordIndex = 0; codewordIndex < cff->n; codewordIndex++)
    {
        for (int codewordPosition = 0; codewordPosition < gs.m; codewordPosition++)
        {
            int codewordLetter = code[codewordPosition][codewordIndex];
            cff_set_value(cff, (codewordPosition * gs.q) + codewordLetter, codewordIndex, 1);
        }
    }
    return cff;
}

double poratEntropyFunction(double q, double r)
{
    return (
        (
            (r - 1) / r
            * log( ((q - 1) * r) / (r - 1) )
            / log(q)
        )
        + (
            1 / r
            * log(r)
            / log(q)
        )
    );
}

void addPoratCodes(CFF_Table* table, int cff_d, int t_max, bool* prime_array)
{
    long long cff_n;
    int cff_t, r, k, m;
    double Hq;
    r = cff_d + 1;
    for (int p = 2; p < 4 * r && p < t_max; p++)
    {
        if (prime_array[p])
        {
            for (int e = 1, q = p; q >= 2 * r && q < 4 * r && q < t_max; e++, q *= p)
            {
                k = 1;
                cff_n = q;
                Hq = poratEntropyFunction((double) q, (double) r);
                m = ceil( ((double) (k)) / (1.0 - Hq) );
                cff_t = m * q;
                while (cff_t <= t_max && cff_n != 0)
                {
                    updateTable(table, cff_t, cff_n, CFF_CONSTRUCTION_ID_PORAT_ROTHSCHILD, p,e,k,m,r);
                    // change array to test next
                    k++;
                    m = (int) ceil( ((double) (k)) / (1.0 - Hq) );
                    cff_n = pow(q, k);
                    cff_t = m * q;
                }
            }
        }
    }
}

// reverse (index revere, not total order reverse) lexicographic k-tuple successor
bool nextReverseLex(int n, int k, int buffer[k])
{
    for (int i = 0; i < k; i++)
    {
        if (buffer[i] < n - 1)
        {
            buffer[i]++;
            for (int x = i-1; x > -1; x--)
            {
                buffer[x] = 0;
            }
            return true;
        }
    }
    return false;
}

void freeGeneratorMatrixStruct(GeneratorMatrixStruct genMatrixStruct)
{
    free(genMatrixStruct.generatorMatrix);
    free(genMatrixStruct.code);
}

// Porat construction
GeneratorMatrixStruct poratRothschildConstructCode(int p, int a, int k, int r, int m)
{
    int q = compute_field_size(p,a);
    int add[q][q];
    int mult[q][q];
    int mult_inverses[q];
    int add_inverses[q];

    populateFiniteField(p,a,(int *)add,(int *)mult);
    populateAdditiveInverses(p, a,(int *) add, add_inverses); //remove this linear search
    populateMultiplicativeInverses(p, a,(int *) mult, mult_inverses);
    if (q < 2 * r || q >= 4 * r)
    {
        printf("Parameters do not match thm requirments. q not in [2r, 4r)\n");
    }
    // determine other parameters from q,k,d (cff's d)
    double delta = (((double) r) - 1.0) / ((double) r);
    double Hq = poratEntropyFunction((double) q, (double) r);
    if (m == 0)
    {
        m = (int) ceil(((double) (k)) / (1.0 - Hq));
    }
    int D = (int) floor(delta*m);

    // store q^k, since it's in a for loop bound
    int q_to_the_k = (int) pow(q, k);

    printf("Starting porat cons with: q=%d k=%d r=%d m=%d Hq(δ)=%f δ=%f Distance=%d\n",q,k,r,m,Hq,delta,D);
    printf("This gives a: %d-CFF(%d,%d)\n", r-1, m*q, q_to_the_k);

    // allocate memory for the generator matrix
    int* g_pointer = malloc(sizeof(int) * k * m);
    // declare 2d array pointer ([m * k] size), call it g (to access the generator matrix easier) and assign it g_pointer's address
    int (*G)[k] = (int (*)[k]) g_pointer;

    // allocate memory for the full code
    int* code_pointer = malloc(sizeof(int) * q_to_the_k * m);
    // 2d ptr to access more easily:
    int (*code)[q_to_the_k] = (int (*)[q_to_the_k]) code_pointer;

    // set zero codeword
    for(int i = 0; i < m; i++)
    {
        code[i][0] = 0;
    }

    // A[x] is the amount of codeword letters fixed to zero for codeword x
    int A[q_to_the_k];
    // set a to all zeros, except for the zero codeword (first codeword)
    A[0] = m;
    for (int i = 1; i < q_to_the_k; i++)
    {
        A[i] = 0;
    }
    // y = all lexicographic k-tuples of Fq
    int y[q_to_the_k][k];
    // now fill y in with a the successor algorithm
    // (but first fill k_tuple_buffer with the 1st lexicographic element)
    int k_tuple_buffer[k];
    for (int i = 0; i < k; i++)
    {
        k_tuple_buffer[i] = 0;
    }
    int h = 0; //h is the current index of y to fill in the following loop
    do
    {
        for (int i = 0; i < k ; i++)
        {
            y[h][i] = k_tuple_buffer[i];
        }
        h++;
    } while (nextReverseLex(q, k, k_tuple_buffer));

    // loop over each cell in generator matrix, start filling it in
    int c, v;
    double m_i_choose_x, one_minus_power, one_over_power;
    double E_best;
    double W[q];
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= k; j++)
        {
            printf("filling G[i=%d][j=%d]\n",i,j);
            for (int b = 0; b < q; b++)
            {
                W[b] = 0;
            }
            for (int l = (int) pow(q, j-1); l < (int) pow(q, j); l++)
            {
                c = i - A[l];
                v = mult
                    [code[i-1][(l%((int)pow(q,j-1)))]]
                    [add_inverses[mult_inverses[y[l][j-1]]]];
                if ( (D-c)>-1)
                {
                    m_i_choose_x = (double) choose(m - i, D-c);
                    one_minus_power = pow(1.0 - (1.0 / ((double) q)), (double) (D-c));
                    one_over_power = pow(1.0 / ((double) q), (double) ((m - i) - (D-c)));
                    W[v] -= m_i_choose_x * one_minus_power * one_over_power;
                }
            }
            E_best = W[0];
            int v_best = 0;
            for (int b = 0; b < q; b++)
            {
                printf("\tW[%d] = %.17g\n", b, W[b]);
                if (W[b] > E_best)
                {
                    v_best = b;
                    E_best = W[b];
                }
            }
            printf("\tLetter %d was chosen as best\n", v_best);
            G[i-1][j-1] = v_best;
            for (int l = (int) pow(q, j-1); l < (int) pow(q, j); l++)
            {
                v = add
                        [mult
                            [G[i-1][j-1]]
                            [y[l][j-1]]
                        ]
                        [
                            code[i-1][(l%((int)pow(q,j-1)))]
                        ];
                if (v == 0)
                {
                    A[l]++;
                }
                code[i-1][l] = v;
            }
        }
    }

    GeneratorMatrixStruct genMatrixStruct;
    genMatrixStruct.q = q;
    genMatrixStruct.k = k;
    genMatrixStruct.r = r;
    genMatrixStruct.m = m;

    genMatrixStruct.Hq = Hq;
    genMatrixStruct.delta = delta;

    genMatrixStruct.generatorMatrix = g_pointer;
    genMatrixStruct.code = code_pointer;

    genMatrixStruct.numCodewords = q_to_the_k;
    genMatrixStruct.minDistance = D;

    return genMatrixStruct;
}

cff_t* cff_porat_rothschild(int p, int a, int k, int r, int m)
{
    GeneratorMatrixStruct gs = poratRothschildConstructCode(p,a,k,r,m);
    cff_t *cff = genmatrixToCFF(gs);
    freeGeneratorMatrixStruct(gs);
    return cff;
}