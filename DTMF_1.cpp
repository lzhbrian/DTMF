// Work by Lin, Tzu-Heng
// W42, 2014011054
// Dept. of Electronic Engineering, Tsinghua University
// DSP Course Work

# include <fstream>
# include <string>
# include <stdio.h>
# include <stdlib.h>
# include <cmath>
# include <ctime>
# include <sstream>
# include <iostream>
# include <cstdlib>
# include <unistd.h>  
# include <dirent.h>  
# include <sys/stat.h>  
# define PI 3.1415926
int const MAX_STR_LEN = 200;  

# include "complex.h"
# include "dif_fft.h"
# include "find_dtmf_symbol.h"

using namespace std;

// FFT Algorithm
int main()
{

	char dir_name[100] = "./txtData1/"; 

    struct dirent * filename;    // return value for readdir()  
    DIR * dir;                   // return value for opendir()  
    dir = opendir( dir_name );  
    /* read all the files in the dir ~ */  
    while( ( filename = readdir(dir) ) != NULL )  
    {  
        // get rid of "." and ".."  
        if( strcmp( filename->d_name , "." ) == 0 ||   
            strcmp( filename->d_name , "..") == 0 )  
            continue;  
        cout << filename ->d_name <<endl;  
    
    	char hhh[MAX_STR_LEN];
    	strcpy(hhh, dir_name);
    	strcat(hhh, (filename->d_name));
		ifstream in( hhh );
		string filename;
		string line;
		
		if(in) // 有该文件  
		{
			double audio[60000];
			int count = 0;
			while (getline (in, line)) // line中不包括每行的换行符
			{
				// audio[count] = atof(const_cast<const char *>(line.c_str()));

				double d;
				stringstream ss(line);
				ss >> d;
				audio[count] = d;

				count = count + 1;
				// cout << audio[count-1] << endl;
			}

			// is 2^k or not, if not add zero
			int add_zero_count = 0;
			for (int i = 1; i < 100; ++i)
			{
				if ( pow(2,i) < count ) {
					continue;
				} else if ( pow(2,i) == count ) {
					break;
				} else {
					add_zero_count = pow(2,i) - count;
					break;
				}
			}

			// add value to input_seq[]
			int total_length = count + add_zero_count;
			complex* input_seq = new complex[total_length];
			for (int i = 0; i < count; i++)
			{
				input_seq[i].re = audio[i];
				input_seq[i].im = 0;
			}
			for (int i = 0; i < add_zero_count; i++)
			{
				input_seq[count + i].re = 0;
				input_seq[count + i].im = 0;
			}

			// FFT
			complex* output_seq = DIF_FFT_reordered(input_seq, total_length);

			// Amp, Find max 1,2 and their positions
			int amp;
			int max1 = 0;
			int max2 = 0;
			int max1_pos = 0;
			int max2_pos = 0;
			for (int i = 0; i < total_length/2; ++i)
			{
				amp = pow(output_seq[i].re,2) + pow(output_seq[i].im,2);
				if (amp > max2)
				{
					if (amp > max1) {
						// max2 = max1
						max2 = max1;
						max2_pos = max1_pos;
						// max1 = new
						max1 = amp;
						max1_pos = i;
					} else {
						max2 = amp;
						max2_pos = i;
					}
				}
			}

			// x-axis
				// 0:f_s/(N-1):f_s
			int fs = 8000;
			int N = total_length;
			double step = double(fs)/(N-1);
			double* x_axis = new double[N];
			for (int i = 0; i < N; ++i)
			{
				x_axis[i] = i * step;
			}

			// Get max1, max2 freq
			cout << "Max1 pos:" << x_axis[max1_pos] << endl;
			cout << "Max2 pos:" << x_axis[max2_pos] << endl;

			// decipher
			char output_symbol = find_dtmf_symbol(x_axis[max1_pos], x_axis[max2_pos]);
			cout << "The symbol for this sound: " << output_symbol << endl;

			cout << endl;

		} else { // fail reading file
			cout << "no such file" << endl;
		}

    }  
	

	return 0;
}

// complex* DIF_FFT_reordered(complex input_seq[], int N);

