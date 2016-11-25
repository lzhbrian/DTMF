// Work by Lin, Tzu-Heng
// W42, 2014011054
// Dept. of Electronic Engineering, Tsinghua University
// DSP Course Work

// input x[n]

// output: amp of the targeted 8 freq (freqs)

double* Goertzel(complex input_seq[], int N) {

	cout << "Calculating Goertzel ..." << endl;
	// targeted freqs
	double freqs[] = 				 		{697,
									  		 770,
									  		 852,
									  		 941,
						1209,1336,1477,1633};

	// Calc WN
	complex* WN = new complex[N];
	WN = Calc_WN(N);

	int sampling_rate = 8000;

	// Calc DFT of targeted 8 freqs
	complex targeted_X[8];
	complex* v = new complex[N];

	for (int i = 0; i < 8; ++i)
	{	
		int k = ( N * freqs[i] ) / sampling_rate;
		double w_k = 2 * PI * k / N;

		// init
		v[0] = input_seq[0];
		v[1] = ComplexAdd(input_seq[1], ComplexScalarMul(v[0], 2*cos(w_k)));

		for (int j = 2; j < N; ++j)
		{
			v[j] = ComplexAdd(ComplexAdd(input_seq[j], ComplexScalarMul(v[j-1], 2*cos(w_k))), ReverseComplex(v[j-2]));
		}

		targeted_X[i] = ComplexAdd(v[N-1], ReverseComplex(ComplexMul(WN[k], v[N-2])));
	}

	// Calc amp
	double* amp_targeted_X = new double[8];
	for (int i = 0; i < 8; ++i)
	{
		amp_targeted_X[i] = pow(targeted_X[i].re,2) + pow(targeted_X[i].im,2);
		// cout << amp_targeted_X[i] << endl; // indicator
	}

	return amp_targeted_X; 
}

