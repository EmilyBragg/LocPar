#include <stdlib.h>

// define the matrix dimensions A is MxP, B is PxN, and C is MxN
#define M 512
#define N 512
#define P 512

// calculate C = A x B
void matmul(float **A, float **B, float **C) 
{
  float sum;
  int   i;
  int   j;
  int   k;

  for ( i = 0; i < M; i++ )
  {
    // for each row of C
    for ( k = 0; k < N; k++ )
    {
      // for each column of C
      for ( j = 0; j < P; j++ )
      {
          C[ i ][ j ] += A[ i ][ k ] * B[ k ][ j ];
      }
    }
  }
}

// function to allocate a matrix on the heap
// creates an mXn matrix and returns the pointer.
//
// the matrices are in row-major order.
void create_matrix( float*** A, int m, int n )
{
  float **T = 0;
  int i;

  T = (float**) calloc( m, sizeof( float* ) );
  for ( i = 0; i < m; i++ )
  {
     T[i] = (float*) calloc( n, sizeof( float ) );
  }
  *A = T;
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
    
    matmul( A, B, C );
    
    return (0);
}
