#define SharedFunctions_included
#include "..\SharedHeader.h"

char* getpath(char* arg, char* file); //Get the Path of the File with filename %file% in the same folder as the file with the fullpath %arg%
void copy_double_to_char_array(char* arr, int index, double d);
void copy_long_to_char_array(char* arr, int index, long long l);
int add_before(int* a, int b); //Calculate %a% + %b%, and Save the Answer in %a%. Return the Value of %a% Before the Addition.
long long get_current_time_usec();
char* get_algorithm_name(char algorithm);
void usleep(long long useconds);
void reset_shared_data(SharedData* shared);
