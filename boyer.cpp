#include "boyer.h"

/*
Boyer-Moore Constructor Function:

Constructs the object using the two file names as well as the specified search size
*/

Boyer::Boyer(std::string txt_file, std::string pat_file, int search_size)
{
    this->outFile = pat_file;
    for(int i = 0; i < 4; i++)
    {
        this->outFile.pop_back();
    }
    
    this->input_file(txt_file, "txt");

    this->input_file(pat_file, "pat");

    this->txt.pop_back();
    this->pat.pop_back();

    sub_patterns(search_size);

}

/*
Private Member Functions
*/

/*
Input File:

reads the file specified, whether it is the txt file or the pattern file, then inserts them
into the private string variable for it
*/
void Boyer::input_file(std::string file_name, std::string type)
{
    // opens file
    std::ifstream myFile(file_name);

    // reads each line of file in string format
    std::string line;
    if (myFile.is_open())
    {   

        // for each line in text...
        while (std::getline(myFile, line)) 
        {

            // if from text file...
            if (type == "txt")
            {
                this->txt.append(line);   
                this->txt.append(" ");

            } 
            // if from pattern file...
            else if (type == "pat")
            {
                this->pat.append(line);
                this->pat.append(" ");

            }
            
        }

        myFile.close();

    }
}

/*
Output File:

This function takes the vector of run times of each search and compiles them into a .csv file.
The name of the file is created based on the algorithm used and the name of the pattern file
*/
void Boyer::output_file(std::vector<float> &time)
{

    // opens new file
    std::ofstream myFile;
    myFile.open("Boyer_" + this->outFile + ".csv");

    // inputs the first line of the .csv file, to specify input column and time column
    myFile << "input" << "," << "time" << '\n';

    // for each line element in the time vector, input current index and time at that index
    for (int i = 1; i <= time.size(); i++)
    {
        myFile << i << "," << std::fixed << std::setprecision(6) << time[i - 1] << '\n';
    }

    myFile.close();

}

/*
Sub Patterns:

This function is used to divide up the pattern file a vector of strings of character size "search_size"
or less (in the case it does not evenly divide into "search_size"). This function's purpose is to allow
us to individually check that sub-pattern to see if it exists in the current text, to get a percentage
of how many sub-patterns are found in the text, to get an more accurate plagerism check
*/
void Boyer::sub_patterns(int search_size)
{
    // if search_size is not 0...
    if (search_size != 0) {
        
        // if search size is greater than pat size ...
        bool small = false;
        if (search_size > this->pat.size()){

            // add full pattern to substring
            this->sub_pat.push_back(this->pat);
            small = true;
        }

        // increments through all of pattern, incrementing by the search size
        for (int i = 0; i < this->pat.size() && small == false; i += search_size)
        {
            
            // if i + search_size does not go out of bounds...
            if (i + search_size < this->pat.size())
            {

                // add substring of size "search_size" to sub_pat
                this->sub_pat.push_back(this->pat.substr(i, search_size));

            }
            else 
            {
                // add substring that fits the rest of the pattern onto sub-pat
                this->sub_pat.push_back(this->pat.substr(i, this->pat.size() - i));
                break;

            }
        }
    }
    else // if search size IS 0
    {
        this->sub_pat.push_back(this->pat);
    }
}

/*
Bad Character Heuristic:

This function does the preprocessing for the bad character heuristic step of the
Boyer Moore algorithm. This function creates a table to determine how many shifts 
are needed depending on the current character. The table depends on the last 
occurrence of a character to determine the value of the number of shifts.
*/
void Boyer::badCharHeuristic(int size, int badchar[NO_OF_CHARS])
{

    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;
 
    // Fill the actual value of last occurrence of a character
    for (i = 0; i < size; i++)
        badchar[(int) this->sub_pat[this->curr_pat][i]] = i;

}

/*
Preprocess Strong Suffix:

This function determines the location of the border, to determine the shifts
required for this preprocessing step.
*/
void Boyer::preprocess_strong_suffix(int *shift, int *bpos, int m)
{

    // m is the length of pattern 
    int i = m, j = m + 1;
    bpos[i] = j;
  
    while(i > 0)
    {
        /*if character at position i-1 is not equivalent to
          character at j-1, then continue searching to right
          of the pattern for border */
        while(j <= m && this->sub_pat[curr_pat][i - 1] != this->sub_pat[curr_pat][j - 1])
        {
            /* the character preceding the occurrence of t in 
               pattern P is different than the mismatching character in P, 
               we stop skipping the occurrences and shift the pattern
               from i to j */
            if (shift[j] == 0)
                shift[j] = j - i;
  
            //Update the position of next border 
            j = bpos[j];
        }
        /* p[i-1] matched with p[j-1], border is found.
           store the  beginning position of border */
        i--; j--;
        bpos[i] = j; 
    }

}

/*
Preprocess Case2:

This function does another preprocessing step for the good suffix heuristic,
checking if the matching suffix of the pattern also occurs at the beginning
of the pattern, to help determine the number of shifts.
*/
void Boyer::preprocess_case2(int *shift, int *bpos, int m)
{

    int i, j;
    j = bpos[0];
    for(i=0; i<=m; i++)
    {
        /* set the border position of the first character of the pattern
           to all indices in array shift having shift[i] = 0 */ 
        if(shift[i]==0)
            shift[i] = j;
  
        /* suffix becomes shorter than bpos[0], use the position of 
           next widest border as value of j */
        if (i==j)
            j = bpos[j];
    }

}

/*
Public Member Functions
*/

/*
Search:

This function does the actual searching of the program. This function takes the current sub-pattern
and runs it against the Boyer-Moore search algorithm. 
*/
int Boyer::search()
{

    // s is shift of the pattern with respect to text
    int s = 0;
    int m = sub_pat[curr_pat].size();
    int n = txt.length();

    // found
    int found = 0;

    // does preprocessing for badCharHeuristic

    int badchar[NO_OF_CHARS];

    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(m, badchar);

    // does preprocessing for goodSuffix

    int bpos[m+1], shift[m+1];

    //initialize all occurrence of shift to 0
    for(int i=0;i<m+1;i++) shift[i] = 0;

    // preprocessing for goodSuffix rule
    preprocess_strong_suffix(shift, bpos, m);
    preprocess_case2(shift, bpos, m);

    // loop to run through text file against pattern
    while(s <= (n - m))
    {

        int j = m - 1;

        /* Keep reducing index j of pattern while
        characters of pattern and text are
        matching at this shift s */
        while(j >= 0 && this->sub_pat[curr_pat][j] == this->txt[s + j])
        {
            j--;
        }

        /* 
        If the pattern is present at current shift...
        */
        if (j < 0)
        {
            
            // computes the shift change between both parts, and shifts the maximum
            int badCharCheck = (s + m < n)? m-badchar[this->txt[s + m]] : 1;
            int goodSufCheck = shift[0];
            s += std::max(badCharCheck, goodSufCheck);

            found++;

            // return that it was found in the text
            // return found;


        } 
        /* If not present at current shift, determine max shift 
        between bad char and good suffix */
        else 
        {
            /* 
            computes the shift change between both parts, and shifts the maximum

            !!!NOTICE!!! 
            make sure that there are only valid characters in the text, or else the shift for
            for badCharCheck may become garbage
            */
            int badCharCheck = std::max(1, j - badchar[this->txt[s + j]]);
            int goodSufCheck = shift[j+1];
            
            s += std::max(badCharCheck, goodSufCheck);
            

        }

    }

    // return number of indexes
    return found;
}

/*
Run:

This function runs the entirety of the program, where it runs the search function and 
records the time it takes for each search of every sub-pattern created, then calls the
output function to create the .csv file
*/
void Boyer::run()
{
    // timing function
    unsigned long c_start, c_end;
    
    
    /*
    searches through entire text for each substring to see if there is a match, and if 
    there is a match, the counter is incremented by one
    */
    int found_per_line = 0;
    
    // vector to hold all of the output times
    std::vector<float> time;

    // function to keep track of the total time for the entirety of the pattern
    float total_time = 0;

    // for every sub-pattern...
    for(int i = 0; i < this->sub_pat.size(); i++){

        // start timer:
        c_start = std::clock();

        // changes current pattern being checked
        this->curr_pat = i;
        int check = this->search();

        // end timer:
        c_end = std::clock();

        // pushes back time that the search function took to the time vector
        float output = 1.0 * (c_end - c_start) / CLOCKS_PER_SEC;
        time.push_back(output);

        // adds to total time
        total_time += output;

        // if sub-pattern found at least once in the text...
        if (check > 0){

            // increment found per line counter
            found_per_line++;
        }
        
    }

    // calls the output file
    output_file(time);
    

    // outputs the amount of lines matched vs the total amount of lines
    std::cout << found_per_line << " out of " << this->sub_pat.size() << std::endl;

    // outputs the time it took to run all tests and the percentage of matches of each substring
    double plagerism = (double) found_per_line/this->sub_pat.size() * 100;
    std::cout << std::fixed << std::setprecision(6) << "Boyer Time: " << total_time << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Percentage found for a match: %" << 
                            plagerism << std::endl;
    
    // threshold percentage to determine if it was plagerized:
    if (plagerism > 70) {
        std::cout << "High likelihood of plagerism!\n\n";
    } else {
        std::cout << "Low likelihood of plagerism\n\n";
    }
}
