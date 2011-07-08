#define Threads_included
#include "..\SharedHeader.h"

void Run(char* running_file_name);
void InitializeConsole();
bool WINAPI ConsoleHandler(DWORD c_event);

void GraphicThread(Data* data, SharedData* shared);
void CalculationThread(Engine* engine, Data* data, SharedData* shared);
void SharedCalculationsThread(Engine* engine, Data* data, SharedData* shared);
void BinaryOutputThread(BinaryOutputManager* bom, Data* data, SharedData* shared);
void LogThread(char* filename, Engine* engine, Data* data, SharedData* shared);
void ExitThreads();
