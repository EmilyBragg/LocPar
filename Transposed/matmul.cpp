#include <stdlib.h>

// define the matrix dimensions A is MxP, B is PxN, and C is MxN
#define M 512
#define N 512
#define P 512

// calculate C = AxB
void matmul(float **A, float **B, float **C)
{
  float sum;
  int   i;
  int   j;
  int   k;

  for ( i = 0; i < M; i++ )
  {
    // for each row of C
    for ( j = 0; j < N; j++ )
    {
      // for each column of C
      sum = 0.0f; // temporary value
      for ( k = 0; k < P; k++ )
      {
        // dot product of row from A and column from B
        //NOTE: B is B_T here!! => look at the indices.
        sum += A[ i ][ k ] * B[ j ][ k ];
      }
      C[ i ][ j ] = sum;
    }
  }
}

// function to allocate a matrix on the heap
// creates an mXn matrix and returns the pointer.
//
// the matrices are in row-major order.
void create_matrix(float*** A, int m, int n)
{
  float **T = 0;
  int i;

  T = (float**) malloc( m * sizeof( float* ));
    
  for ( i = 0; i < m; i++ )
  {
     T[ i ] = (float*) malloc ( n * sizeof( float ) );
  }
    
  *A = T;
}

void transpose( float** A, int m, int n )
{
    float temp;
    for( int i = 0; i < m; i++ )
    {
        for( int j = 0; j < n; j++ )
        {
            if( j >= i )
            {
                temp = A[ i ][ j ];
                A[ i ][ j ] = A[ j ][ i ];
                A[ j ][ i ] = temp;
            }
        }
    }
}

int main()
{
    float** A;
    float** B;
    float** C;

    create_matrix( &A, M, P );
    create_matrix( &B, P, N );
    create_matrix( &C, M, N );

    // assume some initialization of A and B
    // think of this as a library where A and B are
    // inputs in row-major format, and C is an output
    // in row-major.

    transpose( B, P, N );
    matmul( A, B, C );
    
    return (0);
}
