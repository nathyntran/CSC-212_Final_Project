#include "rabin.h"

/*
Constructor Function for Rabin Karp Algorithm
*/
Rabin::Rabin(std::string txt_file, std::string pat_file, int search_size)
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

    method_1(search_size);
}

/*
Private Member Functions 
*/

/*
Input File:


*/
void Rabin::input_file(std::string file_name, std::string type)
{
    // opens file
    std::ifstream myFile(file_name);

    // reads each line of file in string format
    std::string line;
    if (myFile.is_open())
    {   

        while (std::getline(myFile, line)) 
        {

            if (type == "txt")
            {
                this->txt.append(line);   
                this->txt.append(" ");

            } 
            else if (type == "pat")
            {
                this->pat.append(line);
                this->pat.append(" ");

            }
            
        }

        myFile.close();

    }
}

void Rabin::output_file(std::vector<float> &time)
{
    std::ofstream myFile;
    myFile.open("Rabin_" + this->outFile + ".csv");
    
    myFile << "input" << "," << "time" << '\n';

    for (int i = 1; i <= time.size(); i++)
    {
        myFile << i << "," << std::fixed << std::setprecision(6) << time[i - 1] << '\n';
    }

    myFile.close();
}

// methods to determine how sub-string is made
void Rabin::method_1(int search_size)
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
                // std::cout << "boop" << std::endl;
                found++;
                // return found;
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

            // std::cout << p << " " << t << std::endl;
       }



    }

    

    return found;

}

void Rabin::test()
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
    float total_time = 0;
    for(int i = 0; i < this->sub_pat.size(); i++){
        c_start = std::clock();
        this->curr_pat = i;
        int check = this->search();

        c_end = std::clock();
        float output = 1.0 * (c_end - c_start) / CLOCKS_PER_SEC;
        time.push_back(output);
        total_time += output;

        if (check > 0){
            found_per_line++;
        }
        
    }

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








