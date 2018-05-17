#pragma once

/*
* Convolution.h
*
*  Created on: Apr 28, 2017
*      Author: vlad
*/

unsigned short int ** convolution( unsigned short int ** IMAGE, int naxis1, int naxis2, double** Kernel, int ker_size_x, int ker_size_y )
{
	// find center position of kernel (half of kernel size)
	int kCenterX = ker_size_x / 2;
	int kCenterY = ker_size_y / 2;
	int mm, nn, ii, jj;
	double sum = 0;

	double normalization = 0;

	unsigned short int ** out = new unsigned short int*[naxis2];
	for( int h = 0; h < naxis2; h++ )
	{
		out[h] = new unsigned short int[naxis1];
	}


	for( int x = 0; x < ker_size_x; x++ )
	{
		for( int y = 0; y < ker_size_y; y++ )
		{
			normalization += Kernel[x][y];
		}
	}

	for( int i = 0; i < naxis1; ++i )              // rows
	{
		for( int j = 0; j < naxis2; ++j )          // columns
		{
			sum = 0.0;
			for( int m = 0; m < ker_size_y; ++m )     // kernel rows
			{
				mm = ker_size_y - 1 - m;      // row index of flipped kernel

				for( int n = 0; n < ker_size_x; ++n ) // kernel columns
				{
					nn = ker_size_x - 1 - n;  // column index of flipped kernel

											  // index of input signal, used for checking boundary
					ii = i + (m - kCenterY);
					jj = j + (n - kCenterX);

					// ignore input samples which are out of bound
					if( ii >= 0 && ii < naxis2 && jj >= 0 && jj < naxis1 )
					{
                        sum += (IMAGE[jj][ii] * Kernel[mm][nn]);
					}

				}
			}
            out[i][j] = (int)(sum / normalization);
		}
	}
	return out;
}


void FilterInit( double** Filter, int x_size, int y_size, double I )
{
	double sig_x = x_size / 6.0;
	double sig_y = y_size / 6.0;
	for( int i = 0; i < y_size; i++ )
	{
		for( int j = 0; j < x_size; j++ )
		{
			Filter[i][j] = I*exp( -0.5*(i*i / sig_x / sig_x + j*j / sig_y / sig_y) );
		}
	}
}

unsigned short int ** GaussFilter( unsigned short int ** IMAGE, int naxis1, int naxis2, int x_size, int y_size )
{
	double** Filter = new double*[y_size];
	for( int u = 0; u < y_size; u++ )
	{
		Filter[u] = new double[x_size];
	}


	FilterInit( Filter, x_size, y_size, 10 );
	IMAGE = convolution( IMAGE, naxis1, naxis2, Filter, x_size, y_size );

	return IMAGE;
}
