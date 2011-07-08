#define BinaryOutputManager_included
#include "..\SharedHeader.h"

class BinaryOutputManager
{
private:
	Body3D* _bodies;
	int _cur_id;
	int _num_of_bodies;
	int _buffer_frames;
	int _frame_size;
	int _buffer_size;
	char* _filename;
	int _curpos_file;
	int _curpos_mem_buffer;
	int _curpos_disk_buffer;
	char* _mem_buffer;
	char* _disk_buffer;

public:
	BinaryOutputManager(Data* data, int buffer_frames, char* filename);
	bool Capture(long long time); //Returns true If Saving Is Needed
	void Save(); //Append
	void Finalize();
};
