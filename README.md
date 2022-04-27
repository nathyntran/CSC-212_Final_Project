# CSC-212_Final_Project

### Instructions

#### Main Project - program files used to retrieve our data

To run the program, please download all the corresponding files from the GitHub. To compile the program, please open the Makefile and at the bottom, where "run:" is, please edit the line with "./output" using this format:

"./output (text file name) (pattern file name) (search size)"
  
Text file name is the original file which is being compared to, and the pattern file name is the text file that contains the pattern that is being compared to the text file.
  
Search size is the amount of characters each sub-string of the pattern file is going to be. If search size is 0, it will search the entire pattern against the text file. The search size should be greater than 6 for good search against the file. 

After editing the Makefile, please type the following to run the code:

"make run"
  
To compile the code without the make file, you can follow this outline:
  
"g++ main.cpp boyer.cpp rabin.cpp -o run && ./run (text file name) (pattern file name) (search size)"

#### Presentation - program files used for presentation with GUI

The first thing you need to do is make sure to add all the files to a folder  repository  called GUI_Rabin-BoyerFiles. After this anyone is able to download or clone the repository file. 

To run the program you should make sure whatever your IDE adds the following components: ASP.NET and web development, .NET desktop development , Desktop development with C++. 

After this you should be able to run the FinalProject2.sln file. Note: the first time you build the program to run probably takes a few minutes.        
