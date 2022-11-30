#include <bits/stdc++.h>
#include "kernel.cuh"
#include <sdsl/suffix_arrays.hpp>
#include <omp.h>

using namespace std;


int main(){
	sdsl::bit_vector_il<512> BL_il;
	ifstream is2("text2.txt");
    stringstream buffer;
    buffer << is2.rdbuf();
    unsigned char *fTxt;
	string s = buffer.str();
	int size = s.size();
   	fTxt = (unsigned char *) s.c_str();
	unsigned char **pattern;
	pattern = new unsigned char*[1000];
	
	string line;
	ifstream input ("patterns.txt");
	int i = 0, j = 0;
	int m = 10;
	while(input.good() && i < 1000){
		getline(input, line);
		pattern[i] = new unsigned char[m+1];
		for(j = 0; j < m; ++j){
			pattern[i][j] = line.at(j);
		}	
		pattern[i][m] = '\0';
		i++;
	}
	input.close();

	dvc dev = load(fTxt, size, pattern, m);
	cout << "correct\n";
	int *occs;
	occs = (int *) malloc(size*sizeof(int));
	float avg_time = 0;
	long c = 0;
	for (int i = 0; i < 1000; ++i){
		
		memset(occs, size, size*sizeof(int));
		float time = 0;
		calling(dev.text, dev.pattern[i], occs, size, m, time, c);
		avg_time += time;
		cout << i << " " <<  (char *) pattern[i] << " " << avg_time*1000 <<endl;
	}
	cout << avg_time*1000 << endl;
	cout << c << endl;
	cout << ((avg_time*1000)/c) << " microS" << endl;

	omp_set_num_threads(4);
	#pragma omp parallel
	{
		int t = omp_get_thread_num();
		cout <<  t << endl;
	}
	return 0;
}
