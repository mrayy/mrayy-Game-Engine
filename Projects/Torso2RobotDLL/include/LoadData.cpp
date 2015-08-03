/*============================================================================

                        Load arrayed data from file.

                    file name : LoadData.cpp
                 first author : I.Kawabuchi(info@kawabuchi-lab.com)

============================================================================*/
#include "stdafx.h"
#include "LoadData.h"

///-----[LoadData]------------------------------------------------------------
int LoadData(
ifstream& data_file,
char partition,
...)
{
  char    *data_name[256], temp[200], ctemp;
  double  *data[256];
  va_list argptr;
  int i, check = 0;

  //-----Connect identifire with pointer.
  va_start(argptr, partition);
  for(i=0; i<100; i++){
    data_name[i] = va_arg(argptr, char*);        // Load data names as keywords for the comparison.
    if(!memcmp(data_name[i], "-end-", 5)) break; // Terminate loading at finding "-end-".
    if(!memcmp(data_name[i], "-check-", 7)) {    // Terminate loading at finding "-check-".
      check = 1;
      break;
    }
    data[i] = va_arg(argptr, double*);           // Load pointers of data parameters.
  }

  if(data_file.peek() == EOF) return(-1);        // Refuse an empty file.

  //-----Load data.
  while(data_file.peek() != partition){          // Refuse a line with the partition letter at the head.

    if(data_file.peek() == EOF) break;
    else if(data_file.peek() != '/'              // Refuse a line with '/' at the head.
         && data_file.peek() != ';'              // Refuse a line with ';' at the head.
         && data_file.peek() != ' '              // Refuse a line with ' '(space, \0) at the head.
         && data_file.peek() != '\n')            // Refuse an empty line.
    {
      data_file.get(temp, 200, '=');             // Load sentence as far as finding '='.
      data_file.get(ctemp);                      // Absorb '='.

      for(int j=0; j<i; j++){
        // Compare the head letters of a line with the keywords.
        if(!memcmp(temp, data_name[j], strlen(data_name[j])))
        {
          // Load data into the matching data parameter.
		  if(memcmp(temp + strlen(data_name[j]) - 5, "(HEX)", 5) && memcmp(temp + strlen(data_name[j]) - 5, "(hex)", 5)){
            data_file >> *data[j];
		  }
		  else{
            data_file >> temp;
            *data[j] = strtol(temp, NULL, 0);
          }

          if(check == 1)                         // Display the data.
            cout << data_name[j] << " = " << *data[j] << endl;
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

