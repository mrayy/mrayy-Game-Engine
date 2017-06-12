/*============================================================================

                        Load arrayed data from file.

                    file name : LoadData.h
                 first author : I.Kawabuchi(info@kawabuchi-lab.com)
            first publication : 1997.2/5(Wed)
                  version 1.1 : 2003.8/30(Mon)
                  version 1.2 : 2012.8/5(Sun) // add to read Hexadeciam data

             Usage : LoadData((istreama&)data_file, (char)partition,
                              (char*)"data_name1", (double*)&data1,
                              (char*)"data_name2", (double*)&data2,
                              ...,
                              (char*)"-end-" or "-check-"); // Terminator.

  [Note]
  1.The type of parameters is restricted as <<double>> only.
    When you need anathor type, after LoadData, cast it to the proper one.
  2.The data can be described in both Decimal and Hexadecimal.
    In Hexadecimal description,
    the names of parameter in both function LoadData and the readed data file
    should be addad (HEX) or (hex) following them.
    After LoadData, cast the data to the proper type.
  3.Be careful at arranging data names that include a same spelling at the heads.
    Put a longer name ahead of any other shorter one.
    The longer name is confused with the shortest one,
    when the arrangement is done by incorrect ordering.

============================================================================*/
#ifndef ___LoadData
#define ___LoadData

#include <fstream> // Substituted for <fstream.h>.(2004/09/02)
#include <iostream> /* cout */
#include <string.h>
#include <stdarg.h>

using namespace std;

///-----[LoadData]------------------------------------------------------------
int LoadData(ifstream& data_file, char partition, ...);

#endif
