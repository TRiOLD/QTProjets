#pragma once

/*
* mnk.h
*
*  Created on: 2 2017
*      Author: vova
*      corrected&adapted by TRiOLD
*/

struct dat
{
	double I;
	double x, y;
};


class mnk
{
private:
	int m;
	double ** C;
	double ** Cinv;
	double * Y;
	double * A;
	double * EA;

	void creat_sum( double * L, double y );

public:
	mnk( int mm )
	{
		m = mm;
		A = new double[m];
		EA = new double[m];
		Y = new double[m];
		C = new double *[m];
		Cinv = new double *[m];
		for( int i = 0; i<m; ++i )
		{
			C[i] = new double[m];
			Cinv[i] = new double[m];
		}

		for( int i = 0; i < m; i++ )
		{
			A[i] = 0.0;
			EA[i] = 0.0;
			Y[i] = 0.0;
			for( int j = 0; j < m; j++ )
			{
				C[j][i] = 0.0;
				Cinv[j][i] = 0.0;
			}
		}
	}

	void get_null();
	void get_null_A();
	int put_data( char* FileName, double sigm );
	int put_data( dat *, int, double sigm );
	void inversion();
	void mulip_matrix_vector();
	void calc_errors( double sigm );
	double calc_sigm( char* FileName, double sigm );
	double calc_sigm( dat *, int, double sigm );
	void show_res();
	void show_gausian();

	double tellX(){ return -A[1] / 2.0 / A[2]; }
	double tellY(){	return -A[3] / 2.0 / A[4]; }

	~mnk()
	{
		delete[]A;
		delete[]EA;
		delete[]Y;
		for( int i = 0; i<m; ++i )
		{
			delete[]C[i];
			delete[]Cinv[i];
		}
	}
};

void mnk::get_null()
{
	for( int i = 0; i<m; ++i )
	{
		for( int j = 0; j<m; ++j )
		{
			C[i][j] = 0;
			Cinv[i][j] = 0;
		}
		Y[i] = 0;
		EA[i] = 0;
	}
}

void mnk::get_null_A()
{
	for( int i = 0; i<m; ++i )
		A[i] = 0;

}

int mnk::put_data( char* FileName, double sigm )
{
	FILE * f = fopen( FileName, "r" );
	if( !f )
	{
		printf( "Invalid file dat %s\n", FileName );
		return 0;
	}

	double * L = new double[m];
	double y;
	int count = 0;
	double FUN = 0;
	while( !feof( f ) )
	{
		L[0] = 1.0;
		FUN = A[0];
		for( int i = 1; i<m; ++i )
		{
			fscanf( f, "%lf", &L[i] );
			//printf("%lf\t",L[i]);
			FUN += A[i] * L[i];
		}

		fscanf( f, "%lf", &y );
		//printf("%lf\n",y);

		if( fabs( FUN - y ) <= 3.0*sigm )
		{
			creat_sum( L, y );
			count++;
		}
	}
	delete[]L;
	return count;
}
int mnk::put_data( dat * array_i, int n, double sigm )
{
	double * L = new double[m];
	double y;
	int count = 0;
	double FUN = 0;
	for( int i = 0; i<n; ++i )
	{
		L[0] = 1.0;
		L[1] = array_i[i].x;
		L[2] = array_i[i].x*array_i[i].x;
		L[3] = array_i[i].y;
		L[4] = array_i[i].y*array_i[i].y;
		FUN = A[0];
		FUN += A[1] * L[1];
		FUN += A[2] * L[2];
		FUN += A[3] * L[3];
		FUN += A[4] * L[4];

		y = log( array_i[i].I );
		//printf("%lf\n",y);

		if( fabs( FUN - y ) <= 3.0*sigm )
		{
			creat_sum( L, y );
			count++;
		}
	}

	delete[]L;
	return count;
}

void mnk::creat_sum( double * L, double y )
{
	for( int i = 0; i<m; i++ )
	{
		for( int j = 0; j<m; j++ )
		{
			C[i][j] += L[i] * L[j];
		}
		Y[i] += L[i] * y;
	}
}

void mnk::calc_errors( double sigm )
{
	for( int i = 0; i<m; ++i )
	{
		EA[i] = sigm*sqrt( Cinv[i][i] );
	}
}
double mnk::calc_sigm( char* FileName, double sigm_old )
{
	FILE * f = fopen( FileName, "r" );
	if( !f )
	{
		printf( "Invalid file dat %s\n", FileName );
		return 0;
	}

	double * L = new double[m];
	double y;
	int count = 0;
	double FUN = 0;
	double sigm = 0;
	while( !feof( f ) )
	{
		FUN = A[0];
		for( int i = 1; i<m; ++i )
		{
			fscanf( f, "%lf", &L[i] );
			FUN += A[i] * L[i];
		}

		fscanf( f, "%lf", &y );
		//printf("%lf\n",y);

		if( fabs( FUN - y )<3.0*sigm_old )
		{
			sigm += (FUN - y)*(FUN - y);
			count++;
		}
	}
	delete[]L;
	return sqrt( sigm / (count - m) );
}
double mnk::calc_sigm( dat* array_i, int n, double sigm_old )
{
	double * L = new double[m];
	double y;
	int count = 0;
	double FUN = 0;
	double sigm = 0;
	for( int i = 0; i<n; ++i )
	{
		L[0] = 1.0;
		L[1] = array_i[i].x;
		L[2] = array_i[i].x*array_i[i].x;
		L[3] = array_i[i].y;
		L[4] = array_i[i].y*array_i[i].y;
		FUN = A[0];
		FUN += A[1] * L[1];
		FUN += A[2] * L[2];
		FUN += A[3] * L[3];
		FUN += A[4] * L[4];

		y = log( array_i[i].I );
		//printf("%lf\n",y);


		if( fabs( FUN - y )<3.0*sigm_old )
		{
			sigm += (FUN - y)*(FUN - y);
			count++;
		}
	}
	delete[]L;
 	return sqrt( sigm / (count - m) );
}

void mnk::inversion()
{
	double temp = 0;
	for( int i = 0; i < m; i++ )
	{
		for( int j = 0; j < m; j++ )
		{
			if( i == j )
				Cinv[i][j] = 1.0;
			else
				Cinv[i][j] = 0;
		}
	}

	for( int k = 0; k < m; k++ )
	{
		temp = C[k][k];
		for( int j = 0; j < m; j++ )
		{
			C[k][j] /= temp;
			Cinv[k][j] /= temp;
		}

		for( int i = k + 1; i < m; i++ )
		{
			temp = C[i][k];
			for( int j = 0; j < m; j++ )
			{
				C[i][j] -= C[k][j] * temp;
				Cinv[i][j] -= Cinv[k][j] * temp;
			}
		}
	}
	for( int k = m - 1; k > 0; k-- )
	{
		for( int i = k - 1; i >= 0; i-- )
		{
			temp = C[i][k];
			for( int j = 0; j < m; j++ )
			{
				C[i][j] -= C[k][j] * temp;
				Cinv[i][j] -= Cinv[k][j] * temp;
			}
		}
	}

}

void mnk::mulip_matrix_vector()
{
	for( int i = 0; i<m; ++i )
		for( int j = 0; j<m; ++j )
			A[i] += Cinv[i][j] * Y[j];
}

void mnk::show_res()
{
	for( int i = 0; i<m; ++i )
		printf( "%lf\t+-%lf\t", A[i], EA[i] );
	printf( "\n" );
}


void mnk::show_gausian()
{
	double X0 = -A[1] / 2.0 / A[2];
	double Y0 = -A[3] / 2.0 / A[4];
	double sig_x = -1.0 / 2.0 / A[2];
	double sig_y = -1.0 / 2.0 / A[4];
	fprintf( stderr, "X0=%lf+-%lf\n", X0, sqrt( EA[1] * EA[1] + EA[2] * EA[2] ) );
	fprintf( stderr, "Y0=%lf+-%lf\n", Y0, sqrt( EA[3] * EA[3] + EA[4] * EA[4] ) );
	fprintf( stderr, "sig_x=%lf+-%lf\n", sig_x, EA[2] );
	fprintf( stderr, "sig_y=%lf+-%lf\n", sig_y, EA[4] );
	fprintf( stderr, "I0=%lf+-%lf\n", exp( A[0] + X0*X0 / (2.0*sig_x) + Y0*Y0 / (2.0*sig_y) ), sqrt( EA[0] * EA[0] + EA[1] * EA[1] + EA[2] * EA[2] + EA[3] * EA[3] + EA[4] * EA[4] ) );
}
