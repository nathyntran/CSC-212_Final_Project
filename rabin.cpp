#include "rabin.h"

/*
Rabin-Karp Constructor Function:

Constructs the object using the two file names as well as the size of the specified search size
*/
Rabin::Rabin(std::string txt_file, std::string pat_file, int search_size)
{
    // keeps file name of the pattern file to create outfile using the file name
    this->outFile = pat_file;

    // removes the ".txt" from the string
    for(int i = 0; i < 4; i++)
    {
        this->outFile.pop_back();
    }
    
    // calls input_file function for both the text file and the pattern file
    this->input_file(txt_file, "txt");
    
    this->input_file(pat_file, "pat");

    // removes the last space added by the input file function
    this->txt.pop_back();
    this->pat.pop_back();

    // calls the sub_patterns function to divide pattern
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
void Rabin::input_file(std::string file_name, std::string type)
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
void Rabin::output_file(std::vector<float> &time)
{
    // opens new file
    std::ofstream myFile;
    myFile.open("Rabin_" + this->outFile + ".csv");
    
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
void Rabin::sub_patterns(int search_size)
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
Preprocessing Function:

This function creates the hash value for the pattern and inital text and
other necessities required for the rolling hash implimentation of the
Rabin-Karp algorithm.
*/
void Rabin::preprocess(int &p, int &t, int &h, int m)
{

    // get h value
    for (int i = 0; i < m - 1; i++)
    {
        h = (h * NO_OF_CHARS) % PRIME_NO;
    }

    // Calculate Hash value for pattern and inital text
    for (int i = 0; i < m; i++)
    {
        p = (NO_OF_CHARS * p + this->sub_pat[curr_pat][i]) % PRIME_NO;
        t = (NO_OF_CHARS * t + this->txt[i]) % PRIME_NO;
    }

}

/*
Public Member Functions
*/

/*
Search:

This function does the actual searching of the program. This function takes the current sub-pattern
and runs it against the Rabin-Karp search algorithm. 
*/
int Rabin::search()
{
    // found 
    int found = 0;

    int m = sub_pat[curr_pat].size();
    int n = txt.length();

    // intitalize hash value of pattern and text to zero
    int p = 0; // hash value for pattern
    int t = 0; // hash value for text
    int h = 1;

    // preprocess step for search
    preprocess(p, t, h, m);

    // Slide the pattern over text, shifting by 1
    for (int i = 0; i <= n - m; i++)
    {
        /*
        Check if the hash values of the current window and text
        match. If and only if they match does it check each character
        one by one
        */
       if (p == t)
       {
        //    std::cout << "boop\n";
           // flag to determnine if there is a match
           bool match = true;
           
           // check char by char
            for (int j = 0; j < m; j++)
            {
                // std::cout << this->txt[i + j] << ' ' << this->sub_pat[curr_pat][j] << '\n';

                // if not the same
                if (this->txt[i + j] != this->sub_pat[curr_pat][j])
                {
                    match = false;
                    break;
                }
                
            }

            // if match is still true, increment "found" counter
            if (match)
            {   
                found++;
            }

       }

       // if next window in range, find new hash
       if (i < n - m)
       {

            t = (NO_OF_CHARS * (t - this->txt[i] * h) + this->txt[i + m]) % PRIME_NO;

            // if hash < 0 make hash positive
            if (t < 0)
            {
                t = (t + PRIME_NO);
            }
       }
    }

    // returns number of found sub-patterns in text
    return found;

}

/*
Run:

This function runs the entirety of the program, where it runs the search function and 
records the time it takes for each search of every sub-pattern created, then calls the
output function to create the .csv file
*/
void Rabin::run()
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
    std::cout << std::fixed << std::setprecision(6) << "Rabin Time: " << total_time << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Percentage found for a match: %" << 
                            plagerism << std::endl;
    
    // threshold percentage to determine if it was plagerized:
    if (plagerism > 70) {
        std::cout << "High likelihood of plagerism!\n\n";
    } else {
        std::cout << "Low likelihood of plagerism\n\n";
    }
}








