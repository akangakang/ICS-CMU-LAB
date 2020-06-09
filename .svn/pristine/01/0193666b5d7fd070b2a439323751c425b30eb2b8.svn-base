/*
    name: Liu Shuchang
    LognID : 518021910789
*/
/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

    /*
     * case : 32*32 
     * one block 8 int , one line in matrix will cost 4 block, one cache can hold 8 matrix-line  
     * so every 8 line will conflict 
     * I use 8*8 blocking
     */
    if(M==32 && N ==32)
    {
        // 8*8 blocking
        for (int i = 0; i < N; i += 8)
        {
            for (int j = 0; j < M; j += 8)
            {
                for (int k = i; k < i + 8; ++k)
                {
                    int tmp0 = A[k][j];
                    int tmp1 = A[k][j+1];
                    int tmp2 = A[k][j+2];
                    int tmp3 = A[k][j+3];
                    int tmp4 = A[k][j+4];
                    int tmp5 = A[k][j+5];
                    int tmp6 = A[k][j+6];
                    int tmp7 = A[k][j+7];
          
                    B[j][k] = tmp0;
                    B[j+1][k] = tmp1;
                    B[j+2][k] = tmp2;
                    B[j+3][k] = tmp3;
                    B[j+4][k] = tmp4;
                    B[j+5][k] = tmp5;
                    B[j+6][k] = tmp6;
                    B[j+7][k] = tmp7;
                }
            }
        }
    }

    /*
     * case : 64*64
     * one block 8 int , one line in matrix will cost 8 block, one cache can hold 4 matrix-line  
     * so every 4 line will conflict 
     * however, if using  4*4 blocking ,will waste half of the space
     * so can divide 8*8 block into 4 part  
     */
    else if(M==64 && N==64)
    {
        for (int i = 0; i < N; i += 8)
        {
            for (int j = 0; j < M; j += 8)
            {
                for (int k = i; k < i + 4; ++k)
                {
                    /*
                        read A(left-top  right-top) to B(left-top right-top)
                        read 8 int to fully uesd block
                    */
                    int tmp0 = A[k][j];
                    int tmp1 = A[k][j+1];
                    int tmp2 = A[k][j+2];
                    int tmp3 = A[k][j+3];
                    int tmp4 = A[k][j+4];
                    int tmp5 = A[k][j+5];
                    int tmp6 = A[k][j+6];
                    int tmp7 = A[k][j+7];
          
                    B[j][k] = tmp0;
                    B[j+1][k] = tmp1;
                    B[j+2][k] = tmp2;
                    B[j+3][k] = tmp3;
                    /*
                        reverse 
                     */
                    B[j][k+4] = tmp7;
                    B[j+1][k+4] = tmp6;
                    B[j+2][k+4] = tmp5;
                    B[j+3][k+4] = tmp4;
                }
            
                for (int l = 0; l < 4; ++l)
                {
                    /* 
                        read A(left-bottom)from right to left 
                        read A(right-bottom) from left to right
                    */
                    int tmp0 = A[i+4][j+3-l];
                    int tmp1 = A[i+5][j+3-l];
                    int tmp2 = A[i+6][j+3-l];
                    int tmp3 = A[i+7][j+3-l];
                    int tmp4 = A[i+4][j+4+l];
                    int tmp5 = A[i+5][j+4+l];
                    int tmp6 = A[i+6][j+4+l];
                    int tmp7 = A[i+7][j+4+l];

                    /* move B(right-top) to B(left-bottom)*/
                    B[j+4+l][i] = B[j+3-l][i+4];
                    B[j+4+l][i+1] = B[j+3-l][i+5];
                    B[j+4+l][i+2] = B[j+3-l][i+6];
                    B[j+4+l][i+3] = B[j+3-l][i+7];
                    /*
                        move  A(left-bottom) to B(right-top)
                        move A(right-bottom) to B(right-bottom)
                     */
                    B[j+3-l][i+4] = tmp0;
                    B[j+3-l][i+5] = tmp1;
                    B[j+3-l][i+6] = tmp2;
                    B[j+3-l][i+7] = tmp3;
                    B[j+4+l][i+4] = tmp4;
                    B[j+4+l][i+5] = tmp5;
                    B[j+4+l][i+6] = tmp6;
                    B[j+4+l][i+7] = tmp7;
                }       
            }
        }
    }

    /*
      * case : 61*67
      * it is not symmetry so will not conflict if using 16*16 block
      * 
    */
    else if( M==61 && N==67)
    {
        for(int i = 0; i < N; i+=16)
        {
            for(int j = 0; j < M; j+=16)
            {
                for(int a = i; a < i+16 && a < N; a++)
                {
                    for(int b = j; b < j+16 && b < M; b++)
                    {
                        B[b][a] = A[a][b];
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

