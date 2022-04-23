#ifndef __BOYER_H__
#define __BOYER_H__

#define NO_OF_CHARS 256 // total number of possible characters

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

class Boyer
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

        // functions to change how sub-strings are calculated
        void sub_patterns(int search_size);

        /*
        Pre-processing Algorithms
        */
        
        // creates a table for shifting based on the character *REVISE COMMENT*
        void badCharHeuristic(int size, int badchar[NO_OF_CHARS]);

        // preprocessing for strong good suffix rule
        void preprocess_strong_suffix(int *shift, int *bpos, int m);
        void preprocess_case2(int *shift, int *bpos, int m);

    public:

        // Constructor Function
        Boyer(std::string txt_file, std::string pat_file, int search_size);

        // returns how many occurrences there were of the search
        int search();

        // runs the algorithm against the text using the pattern
        void run();

};

#endif
