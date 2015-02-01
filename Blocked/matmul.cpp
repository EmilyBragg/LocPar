#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

// define the matrix dimensions A is MxP, B is PxN, and C is MxN
#define M 512
#define N 512
#define P 512
#define cache_size_order 200 //in floats



struct pair { int row; int column; };
struct boundary{ pair begin; pair end; };

// calculate C = A x B
void block_matmul( float **A, float **B, float **C, boundary a, boundary b, boundary c )
{
  float sum;
  int   i;
  int   j;
  int   k;
    
  for ( i = c.begin.row; i <= c.end.row; i++ )
  {
    // for each row of C
    for ( j = c.begin.column; j <= c.end.column; j++ )
    {
      // for each column of C
      for ( k = a.begin.column; k <= a.end.column; k++ )
      {
        // dot product of row from A and column from B
        C[ i ][ j ] += A[ i ][ k ] * B[ k ][ j ];
      }
    }
  }
}

void matmul_rec( float **A, float **B, float **C, boundary a, boundary b, boundary c )
{
    if( (( (a.end.row - a.begin.row + 1) * (a.end.row - a.begin.row + 1) ) +
         ( (b.end.row - b.begin.row + 1) * (b.end.row - b.begin.row + 1) ) +
         ( (c.end.row - c.begin.row + 1) * (c.end.row - c.begin.row + 1) ) ) <= cache_size_order )
    {
        block_matmul( A, B, C, a, b, c );
    }
    else
    {
        if( ( (a.end.row - a.begin.row + 1) >= (b.end.column - b.begin.column + 1) ) &&
            ( (a.end.row - a.begin.row + 1) >= (b.end.row - b.begin.row + 1)) ) //m is biggest
        {
            boundary temp_a;
            temp_a.begin.row = a.begin.row;
            temp_a.begin.column = a.begin.column;
            temp_a.end.row = a.end.row / 2;
            temp_a.end.column = a.end.column;
            
            boundary temp_c;
            temp_c.begin.row = c.begin.row;
            temp_c.begin.column = c.begin.column;
            temp_c.end.row = c.end.row / 2;
            temp_c.end.column = c.end.column;
            matmul_rec( A, B, C, temp_a, b, temp_c );
            ///
            temp_a.begin.row = temp_a.end.row + 1;
            temp_a.begin.column = a.begin.column;
            temp_a.end.row = a.end.row;
            temp_a.end.column = a.end.column;
        
            temp_c.begin.row = temp_c.end.row + 1;
            temp_c.begin.column = c.begin.column;
            temp_c.end.row = c.end.row;
            temp_c.end.column = c.end.column;
            matmul_rec( A, B, C, temp_a, b, temp_c );
        }
        else if( ( (b.end.column - b.begin.column + 1) >= (a.end.row - a.begin.row + 1) ) &&
                 ( (b.end.column - b.begin.column + 1) >= (b.end.row - b.begin.row + 1) ) )//n is biggest
        {
            boundary temp_b;
            temp_b.begin.row = b.begin.row;
            temp_b.begin.column = b.begin.column;
            temp_b.end.row = b.end.row;
            temp_b.end.column = b.end.column / 2;
            
            boundary temp_c;
            temp_c.begin.row = c.begin.row;
            temp_c.begin.column = c.begin.column;
            temp_c.end.row = c.end.row;
            temp_c.end.column = c.end.column / 2;
            matmul_rec( A, B, C, a, temp_b, temp_c );
            ///
            temp_b.begin.row = b.begin.row;
            temp_b.begin.column = temp_b.end.column + 1;
            temp_b.end.row = b.end.row;
            temp_b.end.column = b.end.column;
            
            temp_c.begin.row = c.begin.row;
            temp_c.begin.column = temp_c.end.column + 1;
            temp_c.end.row = c.end.row;
            temp_c.end.column = c.end.column;
            matmul_rec( A, B, C, a, temp_b, temp_c );
        }
        else
        {
            boundary temp_a;
            temp_a.begin.row = a.begin.row;
            temp_a.begin.column = a.begin.column;
            temp_a.end.row = a.end.row;
            temp_a.end.column = a.end.column / 2;
            
            boundary temp_b;
            temp_b.begin.row = b.begin.row;
            temp_b.begin.column = b.begin.column;
            temp_b.end.row = b.end.row / 2;
            temp_b.end.column = b.end.column;
            matmul_rec( A, B, C, temp_a, temp_b, c );
            ///
            temp_a.begin.row = a.begin.row;
            temp_a.begin.column = temp_a.end.column + 1;
            temp_a.end.row = a.end.row;
            temp_a.end.column = a.end.column;
            
            temp_b.begin.row = temp_b.end.row + 1;
            temp_b.begin.column = b.begin.column;
            temp_b.end.row = b.end.row;
            temp_b.end.column = b.end.column;
            matmul_rec( A, B, C, temp_a, temp_b, c );
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

  T = ( float** ) calloc ( m, sizeof( float* ) );
  for ( i = 0; i < m; i++ )
  {
     T[i] = ( float* ) calloc ( n, sizeof( float ) );
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

    /*for( int i = 0; i < 16; i++ )
    {
        for( int j = 0; j < 16; j++ )
        {
            A[ i ][ j ] = i * j + 1;
            cout << A[ i ][ j ] << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    for( int i = 0; i < 16; i++ )
    {
        for( int j = 0; j < 16; j++ )
        {
            B[ i ][ j ] = i + j + 1;
            cout << B[ i ][ j ] << " ";
        }
        cout << endl;
    }
    
    cout << endl;*/
    
    boundary a;
    a.begin.row = 0;
    a.begin.column = 0;
    a.end.row = M - 1;
    a.end.column = P - 1;
    
    boundary b;
    b.begin.row = 0;
    b.begin.column = 0;
    b.end.row = P - 1;
    b.end.column = N - 1;
    
    boundary c;
    c.begin.row = 0;
    c.begin.column = 0;
    c.end.row = M - 1;
    c.end.column = N - 1;
    
    matmul_rec( A, B, C, a, b, c );

    /*for( int i = 0; i < 16; i++ )
    {
        for( int j = 0; j < 16; j++ )
        {
            cout << C[ i ][ j ] << " ";
        }
        cout << endl;
    }*/
    
    return (0);
}
