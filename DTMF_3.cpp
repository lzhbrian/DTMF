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
# include "goertzel.h"
# include "find_dtmf_symbol.h"

using namespace std;

// Goertzel Algorithm to identify 3
int main()
{

		cout << "Identifying Prob3, a long audio, using Goertzel Algorithm ..." << endl << endl;

		ifstream in("./txtData2/data.txt");
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

			// Stops (in seconds)
			double stops[] = 	{0.4459, 0.6633,
							     0.8642, 1.072,
							     1.27,   1.473,
							     1.7,    1.89,
							     2.09,   2.29,
							     2.5,    2.722,
							     2.915,  3.1,
							     3.34,   3.549,
							     3.782,  4,
							     4.182,  4.398, 
							     4.581,  4.794, 
							     5.013,  5.222, 
							     5.431,  5.641, 
							     5.869,  6.059, 
							     6.258,  6.447};

		    int len_data2 = count;
			double seconds_len_data2 = len_data2/8000.0;

			for (int d = 0; d < 15; ++d)
			{

				int start_index = stops[d*2]/seconds_len_data2*len_data2;
				int end_index = stops[d*2+1]/seconds_len_data2*len_data2;

				// Get this audio
				count = end_index - start_index + 1;
				double* this_audio = new double[count];
				for (int i = 0; i < count; ++i)
				{
					this_audio[i] = audio[i+start_index];
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
					input_seq[i].re = this_audio[i];
					input_seq[i].im = 0;
				}
				for (int i = 0; i < add_zero_count; i++)
				{
					input_seq[count + i].re = 0;
					input_seq[count + i].im = 0;
				}

				// Goertzel, return amp^2
				double* targeted_amp = Goertzel(input_seq, total_length);

				// Amp, Find max 1,2 and their positions
				int amp;
				int max1 = 0;
				int max2 = 0;
				int max1_pos = 0;
				int max2_pos = 0;
				for (int i = 0; i < 8; ++i)
				{
					amp = targeted_amp[i];
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

				// Get max1, max2 freq
				double x_axis[] = 				 		{697,
												  		 770,
												  		 852,
												  		 941,
									1209,1336,1477,1633};

				cout << "Max1 pos:" << x_axis[max1_pos] << endl;
				cout << "Max2 pos:" << x_axis[max2_pos] << endl;

				// decipher
				char output_symbol = find_dtmf_symbol(x_axis[max1_pos], x_axis[max2_pos]);
				cout << "The symbol for this sound: " << output_symbol << endl;

				cout << endl;

			}

		} else { // fail reading file
			cout << "no such file" << endl;
		}
	

	return 0;
}

// complex* DIF_FFT_reordered(complex input_seq[], int N);
// double* Goertzel(complex input_seq[], int N) {
