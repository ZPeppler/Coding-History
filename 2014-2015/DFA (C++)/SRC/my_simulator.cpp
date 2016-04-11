/**
  * @author: Zach Peppler
  * @email: peppler@email.sc.edu
  * CSCE 355 Project: Task 1
 **/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
   /* Variables used throughout the program */
   string inLine, numLine, alphaLine, finLine, inString;
   ifstream inFile1, inFile2;
   int numStates, alphaSize, acceptSize;
   vector<int> acceptingStates;
   char *alphabet;
   int count = 0;
   char *test;
   int currentState = 0;
   vector<vector<int> > stateVector;
   vector<int> stateLine;
   
   /* Open the first file and check to see if it opens */
   inFile1.open(argv[1]);
   if(inFile1.fail())
   {
      cout << "Error openning the file " << argv[1] << "\n";
      exit(1);
   }
  
   /* Open the second file and check to see if it opens */
   inFile2.open(argv[2]);
   if(inFile2.fail())
   {
      cout << "Error openning the file " << argv[1] << "\n";
      exit(1);
   }

   /* Goes through the first file line by line to store
      the data in its proper location. */
   while(getline(inFile1, inLine))
   {
      // Find the number of states in the DFA
      if(inLine.compare(0, 18, "Number of states: ") == 0)
      {
         numStates = atoi((inLine.substr(18)).c_str());
      }
      // Find the accepting states of the DFA and store them
      // in a vector
      else if(inLine.compare(0, 18, "Accepting states: ") == 0)
      {
         acceptingStates.clear();
         finLine = inLine.substr(18, inLine.size()-1);
         stringstream stream(finLine);
         int n;
         while(stream >> n)
         {
            acceptingStates.push_back(n);
         }
         acceptSize = acceptingStates.size();
      }
      // Find the alphabet of the DFA and store it in a character array
      else if(inLine.compare(0, 10, "Alphabet: ") == 0)
      {
         alphaLine = inLine.substr(10);
         alphaSize = alphaLine.size();
         alphabet = new char [alphaSize+1];
         strcpy (alphabet, alphaLine.c_str());
      }
      /* Otherwise, parse the state transition table.
         Using a one-dimesional int vector to store
         one line of the table and then upload that
         line into a two-dimensional vector */ 
      else
      {
         stringstream stream(inLine);
         int n;
         while(stream >> n)
         {
            stateLine.push_back(n);
         }
         stateVector.push_back(stateLine);
         stateLine.clear();
      }
   } 

   /* Goes through the second file line by line
      to parse the string through the DFA */
   while(getline(inFile2, inString))
   {
      // Turn the line from a string to a character
      // array
      test = new char[inString.size()+1];
      strcpy (test, inString.c_str());

      /* Since the empty string is a viable input,
         the code will test to see if the string is
         empty or not. */
      if(inString.size() != 0)
      {
         /* Iterates through every character of the string */
         for(int j = 0; j < inString.size(); j++)
         {
            bool found = false;
            int i = 0;
            /* Goes through the alphabet to find which character
               in it matches with the input string character. 
               Upon finding the match, it returns the value of
               the iterator of the alphabet */
            while(!found)
            {
               if(alphabet[i] == test[j])
               {
                  found = true;
               }
               else
               {
                 i++;
               }
            }
            // Save the current state for sanity check
            int n = currentState;
            // n is current state, i is the transition
            // this leads to the next state
            currentState = stateVector[n][i];
         }
      }
      int k = 0;
      bool search = false;
      /* Searches through the accepting states to find
         if the current state matches an accepting state */
      while(!search)
      {
         /* This must come before the accepting state check.
            If the iterator reaches the end of the list of 
            accepting states, then it should reject */
         if(k == acceptSize)
         {
            cout << "reject" << endl;
            search = true;
            currentState = 0;
         }
         /* The current state is one of the accepting
            states, thus the string is accepted.  */
         else if(currentState == acceptingStates[k])
         {
            cout << "accept" << endl;
            search = true;
            currentState = 0;
         }
         else
         {
            k++;
         }
      }
   }

   delete alphabet;
   inFile1.close();
   inFile2.close(); 

   return 0;
}
