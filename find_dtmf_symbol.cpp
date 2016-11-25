// Work by Lin, Tzu-Heng
// W42, 2014011054
// Dept. of Electronic Engineering, Tsinghua University
// DSP Course Work

// Decipher
char find_dtmf_symbol(double a, double b)
{
	char symbol[]	 =  {'1','2','3','A',
						 '4','5','6','B',
						 '7','8','9','C',
						 '*','0','#','D'};
	double freqs1[] = 				 		{697,
									  		 770,
									  		 852,
									  		 941};
	double freqs2[] = {1209,1336,1477,1633};

	// switch if a > b, making sure a < b
	if (a > b) {
		int c = a;
		a = b;
		b = c;
	}

	// get a pos: x
	int x = 100;
	for (int i = 0; i < 4; ++i)
	{
		if ( abs(freqs1[i] - a) <= 10 )
		{
			x = i;
			break;
		}
	}

	// get b pos: y
	int y = 100;
	for (int i = 0; i < 4; ++i)
	{
		if ( abs(freqs2[i] - b) <= 10 )
		{
			y = i;
			break;
		}
	}

	if (x == 100 || y == 100)
	{
		char error_char = 'x';
		return error_char;
	}
	// get return symbol
	char return_symbol = symbol[ x*4 + y ];

	return return_symbol;
}