#include "construction_internals.h"

#include "../../include/libcfftables/libcfftables.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "../CFF_Internals.h"


void doublingShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Doubling");
}

void doublingLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "Dbl(%hd,%hd)", consParams[0], consParams[1]);
}

void doublingConstructCFF(int d, int t)
{
    // mutually recursive call to get the 1-CFF needed
    cff_t *smallerCFF = cff_table_get_by_t(2, global_tables_array[1]->array[t].consParams[0]);
    // call the construction and assign the cff's pointer to the table
    global_tables_array[d-1]->array[t].cff = cff_doubling(smallerCFF, global_tables_array[1]->array[t].consParams[1]);
}

CFF_Construction_And_Name_Functions doublingConstructionFunctions = {
    .shortSrcFormatter = doublingShortSrcFormatter,
    .longSrcFormatter = doublingLongSrcFormatter,
    .constructionFunction = doublingConstructCFF
};

// the function to fill in a table with the doubling construction parameters
void doublingConstructionFiller(CFF_Table *table_2, CFF_Table *table_1)
{
    unsigned long long n;
    int s;
    for (int t = 2; t < table_2->numCFFs; t++)
    {
        n = table_2->array[t].n;
        s = binarySearchTable(table_1, n);
        if (s != -1)
        {
            updateTable(table_2, t + s + 2 - (s % 2), 2 * n, &doublingConstructionFunctions, t, s, 0, 0, 0);
        }
    }
}

// helper function used in void doublingConstructCFF(int d, int t)
cff_t* cff_doubling(const cff_t *cff, int s)
{
    cff_t *resultCFF;

    if (s % 2 == 1)
    {
        resultCFF = cff_alloc(2, cff->t + s + 1, cff->n * 2);
    } else
    {
        resultCFF = cff_alloc(2, cff->t + s + 2, cff->n * 2);
    }

    // set the top rows, which are two copies of the cff
    for (int r = 0; r < cff->t; r++)
    {
        for (int c = 0; c < cff->n; c++)
        {
            cff_set_value(resultCFF, r, c, cff_get_value(cff, r, c));
            cff_set_value(resultCFF, r, c + cff->n, cff_get_value(cff, r, c));
        }
    }

    int ceil_s_over_2 = (int) ceil(((double) s) / 2.0);
    int subset[ceil_s_over_2];
    int char_vector[s];
    for (int i = 0; i < ceil_s_over_2; i++)
    {
        subset[i] = i;
    }

    int column = 0;
    do
    {
        if (column == cff->n)
        {
            break;
        }
        int j = 0;
        for (int i = 0; i < s; i++)
        {
            if (subset[j] == i)
            {
                char_vector[i] = 1;
                j++;
            } else
            {
                char_vector[i] = 0;
            }
            // avoid reading out of bounds
            if (j == ceil_s_over_2)
            {
                j--;
            }
        }
        for (int i = 0; i < s; i++)
        {
            cff_set_value(resultCFF, i + cff->t, column, char_vector[i]);
            cff_set_value(resultCFF, i + cff->t, column + cff->n, 1 - char_vector[i]);
        }
        column++;
    } while(kSubsetLexSuccessor(s, ceil_s_over_2, subset));

    // add rows of 0s or 1s at the bottom
    if (s % 2 == 1)
    {
        for (int i = 0; i < cff->n; i++)
        {
            cff_set_value(resultCFF, cff->t + s, i, 0);
        }
        for (int i = cff->n; i < cff->n * 2; i++)
        {
            cff_set_value(resultCFF, cff->t + s, i, 1);
        }
    } else
    {
        for (int i = 0; i < cff->n; i++)
        {
            cff_set_value(resultCFF, cff->t + s, i, 1);
            cff_set_value(resultCFF, cff->t + s + 1, i, 0);
        }
        for (int i = cff->n; i < cff->n * 2; i++)
        {
            cff_set_value(resultCFF, cff->t + s, i, 0);
            cff_set_value(resultCFF, cff->t + s+1, i, 1);
        }
    }
    return resultCFF;
}

