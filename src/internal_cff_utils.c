#include "cff_internals.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


/*
    Returns nCk (n choose k) by generating Pascal's triangle.
    Returns 0 if overflow occurrs during addition in the function,
    or if the parameters are invalid (n < 1 OR n < k OR k < 0)
*/
long long choose(int n, int k)
{
    // invalid inputs
    if (n < k || n < 0 || k < 0)
    {
        return 0;
    }

    // only calculate left side
    if (n - k < k)
    {
        k = n - k;
    }

    // 1 cases
    if (n < 2 || k == 0)
    {
        return 1;
    }

    long long a[k+1]; // previous row of pascal's triangle
    long long b[k+1]; // current row of pascal's triangle

    long long *a_ptr, *b_ptr, *swap_ptr;
    a_ptr = a;
    b_ptr = b;

    // the leftmost cell of every row is one:
    a_ptr[0] = 1;
    b_ptr[0] = 1;
    // iterate over the n + 1 rows in pascal's triangle
    for (int row = 1; row < n + 1; row++)
    {
        int col; // (iniitialize outside for loop so its value can be accessed after)
        for (col = 1; col < row && col < k + 1; col++)
        {
            b_ptr[col] = a_ptr[col-1] + a_ptr[col];
            // returns 0 if overflow occurred during the addition in above line
            if (b_ptr[col] < a_ptr[col-1] || b_ptr[col] < a_ptr[col])
            {
                return 0;
            }
        }
        b_ptr[col] = 1;
        swap_ptr = a_ptr;
        a_ptr = b_ptr;
        b_ptr = swap_ptr;
    }
    return a_ptr[k];
}

long factorial(int n)
{
    long result = 1;
    while (n > 0)
    {
        result = result * n;
        n--;
    }
    return result;
}

// Sets T to all zeros
void set_to_all_zeros(int n, int *T)
{
    for(int i = 0; i < n; i++)
    {
        T[i] = 0;
    }
}

// Replaces "buffer" with the next consecutive lexicographic subset of Zn (size k).
bool k_subset_lex_successor(int n, int k, int *buffer)
{
    for (int i = k-1; i > -1; i--)
    {
        if (buffer[i] != n-k+i)
        {
            buffer[i]++;
            for (int x = i+1; x < k; x++)
            {
                buffer[x] = buffer[i] + (x-i);
            }
            return true;
        }
    }
    return false;
}

bool k_tuple_lex_successor(int n, int k, int *buffer)
{
    for (int i = k-1; i > -1; i--)
    {
        if (buffer[i] < n - 1)
        {
            buffer[i]++;
            for (int x = i+1; x < k; x++)
            {
                buffer[x] = 0;
            }
            return true;
        }
    }
    return false;
}

// Integer exponentiation
int ipow(int base, int exp)
{
    if (exp == 0) {return 1;}
    int result = 1;
    for (int i = 0; i < exp; i++)
    {
        result = result  * base;
    }
    return result;
}


// https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
void prime_sieve(int n, bool *prime_array)
{
    // set initial array values
    for (int i = 0; i < n; i++)
    {
        prime_array[i] = true;
    }

    prime_array[0] = false;
    prime_array[1] = false;

    // regular sieve to find all primes up to n
    for (int p = 2; p * p < n; p++)
    {
        if (prime_array[p])
        {
            for (int i = p * p; i < n; i += p)
            {
                prime_array[i] = false;
            }
        }
    }
}