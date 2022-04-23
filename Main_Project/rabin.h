#ifndef __RABIN_H__
#define __RABIN_H__

#define PRIME_NO 211 // prime number 
#define NO_OF_CHARS 256 // total number of characcters

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

class Rabin
{

    private:

        // file name
        std::string outFile;

        // original text
        std::string txt;

        // pattern to test
        std::string pat;

        // sub-string of pattern
        std::vector<std::string> sub_pat;
        // current sub-string index
        int curr_pat;

        // input file
        void input_file(std::string file_name, std::string type);

        // output file
        void output_file(std::vector<float> &time);

        // functions to determine sub-string 
        void sub_patterns(int search_size);

        /*
        Pre-processing
        */
        void preprocess(int &p, int &t, int &h, int m);

    public:

        // Constructor Function
        Rabin(std::string txt_file, std::string pat_file, int search_size);

        // returns how many occurrences there were of the search
        int search();
        
        // runs the algorithm against the text using the pattern
        void run();

};



#endif
