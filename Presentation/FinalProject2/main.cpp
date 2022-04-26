#include <iostream>
#include "MainForm.h"
#include "main.h"


using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


# define NO_OF_CHARS 256

// The preprocessing function for Boyer Moore's
// bad character heuristic
void badCharHeuristic(std::string text, int size,int badchar[NO_OF_CHARS])
{
    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
        badchar[(int)text[i]] = i;
}
/* A pattern searching function that uses Bad
Character Heuristic of Boyer Moore Algorithm */
void search(std::string text, std::string pattern)
{
    int m = pattern.size();
    int n = text.size();

    int badchar[NO_OF_CHARS];

    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(pattern, m, badchar);

    int s = 0; // s is shift of the pattern with
               // respect to text
    while (s <= (n - m))
    {
        int j = m - 1;

        /* Keep reducing index j of pattern while
        characters of pattern and text are
        matching at this shift s */
        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        /* If the pattern is present at current
        shift, then index j will become -1 after
        the above loop */
        if (j < 0)
        {
            //cout << "pattern occurs at shift = " << s << endl;
            MessageBox::Show("pattern occurs at shift =  ",
                "", MessageBoxButtons::OK);

            /* Shift the pattern so that the next
            character in text aligns with the last
            occurrence of it in pattern.
            The condition s+m < n is necessary for
            the case when pattern occurs at the end
            of text */
            s += (s + m < n) ? m - badchar[text[s + m]] : 1;

        }

        else
            /* Shift the pattern so that the bad character
            in text aligns with the last occurrence of
            it in pattern. The max function is used to
            make sure that we get a positive shift.
            We may get a negative shift if the last
            occurrence of bad character in pattern
            is on the right side of the current
            character. */
            s += max(1, j - badchar[text[s + j]]);
    }
}

/* Driver code
int main()
{
    //std::string text = "ABAAABCD";
    //std::string pattern = "ABC";
    //search(text, pattern);
    return 0;
    
}*/



