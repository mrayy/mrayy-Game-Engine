/*============================================================================

                        Load arrayed data from file.

                        file name : loaddata.cpp
                     first author : I.Kawabuchi(ichiro_kawabuchi@kuramae.ne.jp)
                first publication : 1997.2/5(Wed)
                      version 1.1 : 2003.8/30(Mon)

             Usage : LoadData((istreama&)data_file, (char)partition,
                              (char*)"data_name1", (double*)&data1,
                              (char*)"data_name2", (double*)&data2,
                              ...,
                              (char*)"-end-"); // Terminator.

           „¡„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ[Note]„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„¢
           „  1.The type of parameters is restricted as <<double>> „ 
           „    only.                                              „ 
           „  2.Be careful at arranging data names that include a  „ 
           „    same spelling at the heads. Put a longer name      „ 
           „    ahead of any other shorter one. The longer name is „ 
           „    confused with the shortest one, when the arrange-  „ 
           „    ment would be irregular.                           „ 
           „¤„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„£

============================================================================*/

#pragma once

//#ifndef ___LoadData
//#define ___LoadData

#include "stdafx.h"
#include <fstream> // Substituted for <fstream.h>.(2004/09/02)
#include <string.h>
#include <stdarg.h>

//#endif

using namespace std;

///-----[LoadData]------------------------------------------------------------
int LoadData(
ifstream& data_file,
char partition,
...)
{
  char    *data_name[60], temp[200], ctemp;
  double  *data[60];
  va_list argptr;
  int i;

  //-----Connect identifire with pointer.
  va_start(argptr, partition);
  for(i=0; i<100; i++){
    data_name[i] = va_arg(argptr, char*);        // Load data names as keywords for the comparison.
    if(!memcmp(data_name[i], "-end-", 5)) break; // Terminate loading at finding "-end-".
    data[i] = va_arg(argptr, double*);           // Load pointers of data parameters.
  }

  if(data_file.peek() == EOF) return(-1);        // Refuse an empty file.

  //-----Load data.
  while(data_file.peek() != partition){          // Refuse a line with the partition letter at the head.

    if(data_file.peek() == EOF) break;
    else if(data_file.peek() != '/'              // Refuse a line with '/' at the head.
         && data_file.peek() != ' '              // Refuse a line with ' '(space, \0) at the head.
         && data_file.peek() != '\n')            // Refuse an empty line.
    {
      data_file.get(temp, 200, '=');             // Load sentence as far as finding '='.
      data_file.get(ctemp);                      // Absorb '='.

      // Compare the head letters of a line with the keywords.
      for(int j=0; j<i; j++){
        if(!memcmp(temp, data_name[j], strlen(data_name[j])))
        {
          data_file >> *data[j];                 // Load data into the matching data parameter.
          if(data_file.peek() != '\n'){
            data_file.get(temp, 200);            // Absorb comment.
            data_file.get(ctemp);                // Absorb '\n'.
          }
          else data_file.get(ctemp);             // Absorb '\n'.
          break;
        }
      }
    }
    else{
      if(data_file.peek() != '\n'){
        data_file.get(temp, 200);                // Absorb comment.
        data_file.get(ctemp);                    // Absorb '\n'.
      }
      else data_file.get(ctemp);                 // Absorb '\n'.
    }
  }

  return(0);
}

