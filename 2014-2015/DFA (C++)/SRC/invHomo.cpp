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
   int numStates, alphaSizeOut, alphaSizeIn, acceptSize;
   vector<int> acceptingStates;
   char *alphabetOut;
   char *alphabetIn;
   char *stateLine;
   int count = 0;
   int currentState = 0;
   vector<vector<int> > stateVectorOut;
   vector<int> stateLineOut;
   vector<vector<char> > stateVectorIn;
   vector<char> stateLineIn;
   vector<vector<int> > outVector;
   vector<int> outLine;
   
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
   while(!inFile1.eof())
   {
      getline(inFile1, inLine);
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
         alphaSizeOut = alphaLine.size();
         alphabetOut = new char [alphaSizeOut+1];
         strcpy (alphabetOut, alphaLine.c_str());
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
            stateLineOut.push_back(n);
         }
         stateVectorOut.push_back(stateLineOut);
         stateLineOut.clear();
      }
   } 

   /* Goes through the homomorphic image file line by line */
   while(getline(inFile2,inLine))
   {
      // Stores the new alphabet into a character array
      if(inLine.compare(0, 16, "Input alphabet: ") == 0)
      {
         alphaLine = inLine.substr(16);
         alphaSizeIn = alphaLine.size();
         alphabetIn = new char [alphaSizeIn+1];
         strcpy(alphabetIn, alphaLine.c_str());
      }
      /* Since the output alphabet is the same as the current
         alphabet, no work nees to be done. */
      else if(inLine.compare(0, 17, "Output alphabet: ") == 0)
      {
         // do nothing
      }
      /* Lastly, it adds the new transitions iteratively */
      else
      {
         /* Since the empty string is a viable transition,
            there must be a check to see if the new transition
            is empty. */
         if(inLine.size() != 0)
         {
            // Copies the string into a character array
            stateLine = new char [inLine.size()+1];
            strcpy(stateLine, inLine.c_str());
            // Push the character onto the vector
            for(int i = 0; i < inLine.size()+1; i++)
            {
               stateLineIn.push_back(stateLine[i]);
            }
            /* Pushes the vector onto the 2d vector.
               Characters are used since the homomorphism
               can have characters in the transition */
            stateVectorIn.push_back(stateLineIn);
            stateLineIn.clear();
            delete stateLine;
         }
         else
         {
            /* If the new transition is empty, the '\0' character
               is used to substitute this because it does not
               fall within the acceptable code for new transitions */
            stateLineIn.push_back('\0');
            stateVectorIn.push_back(stateLineIn);
            stateLineIn.clear();
         }
      }
   }

   /* Goes through each state of the DFA to find the new transition table */
   for(int k = 0; k < stateVectorOut.size()-1; k++)
   {
      int newNextState = k;
      /* Goes through each element of the new alphabet */
      for(int i = 0; i < stateVectorIn.size(); i++)
      {
         //Checks to see if the empty string was used for this character
         if((stateVectorIn[i].size()-1) == 0)
         {
            // do nothing
         }
         //Parses the new transition state
         else
         {
            for(int j = 0; j < stateVectorIn[i].size()-1; j++)
            {
               bool found = false;
               int l = 0;
               while(!found)
               {
                  if(alphabetOut[l] == stateVectorIn[i][j])
                  {
                     found = true;
                  }
                  else
                  {
                     l++;
                  }
               } 
               int n = newNextState;
               newNextState = stateVectorOut[n][l];
            } 
         } 
         outLine.push_back(newNextState);
         newNextState = k;
      }
      outVector.push_back(outLine);
      outLine.clear();
   }

   cout << "Number of states: " << numStates << endl;
   cout << "Accepting states: ";
   for(int i = 0; i < acceptSize; i++)
   {
      cout << acceptingStates[i] << " ";
   }
   cout << endl;
   cout << "Alphabet: ";
   for(int i = 0; i < alphaSizeIn; i++)
   {
      cout << alphabetIn[i];
   }
   cout << endl;

   for(int i = 0; i < outVector.size(); i++)
   {  
      for(int j = 0; j < outVector[i].size(); j++)
      {   
         cout << outVector[i][j] << " ";
      }
      cout << endl;
   }
   
   delete alphabetOut;
   delete alphabetIn;
   inFile1.close();
   inFile2.close(); 

   return 0;
}
