# CSC-212_Final_Project

### Instructions

To run the program, please download all the corresponding files from the GitHub. To compile the program, please open the Makefile and at the bottom, where "run:" is, please edit the line with "./output" using this format:

"./output '<'text file name'>' <pattern file name> <search size>"
  
Text file name is the original file which is being compared to, and the pattern file name is the text file that contains the pattern that is being compared to the text file.
  
Search size is the amount of characters each sub-string of the pattern file is going to be. If search size is 0, it will search the entire pattern against the text file. The search size should be greater than 6 for good search against the file. 
  
To compile the code without the make file, you can follow this outline:
  
"g++ main.cpp boyer.cpp rabin.cpp -o run && ./run <text file name> <pattern file name> <search size>"
