#include "WAVw.h"
#include "WAV.h"


void* WAV_construct(short ch, int rate){
    return static_cast<void*>(new WAV(ch,rate));
}
void WAV_release(void* obj){
    delete static_cast<WAV*>(obj);
}
int WAV_newfile(void* obj, const char* file_name){
    return static_cast<WAV*>(obj)->NewFile(file_name);
}
int WAV_openfile(void* obj, const char* file_name){
    return static_cast<WAV*>(obj)->OpenFile(file_name);
}
int WAV_is_eof(void* obj){
    return static_cast<WAV*>(obj)->IsEOF();
}
int WAV_read_unit_short(void* obj, short* dest, int unit){
    return static_cast<WAV*>(obj)->ReadUnit(dest,unit);
}
int WAV_read_unit_int(void* obj, int* dest, int unit){
    return static_cast<WAV*>(obj)->ReadUnit(dest,unit);
}
int WAV_read_unit_float(void* obj, float* dest, int unit){
    return static_cast<WAV*>(obj)->ReadUnit(dest,unit);
}
int WAV_read_unit_double(void* obj, double* dest, int unit){
    return static_cast<WAV*>(obj)->ReadUnit(dest,unit);
}
int WAV_append_short(void* obj, short* app_data, unsigned int app_size){
    return static_cast<WAV*>(obj)->Append(app_data,app_size);
}
int WAV_append_int(void* obj, int* app_data, unsigned int app_size){
    return static_cast<WAV*>(obj)->Append(app_data,app_size);
}
int WAV_append_float(void* obj, float* app_data, unsigned int app_size){
    return static_cast<WAV*>(obj)->Append(app_data,app_size);
}
int WAV_append_double(void* obj, double* app_data, unsigned int app_size){
    return static_cast<WAV*>(obj)->Append(app_data,app_size);
}

void WAV_finish(void* obj){
    static_cast<WAV*>(obj)->Finish();
}
void WAV_rewind(void* obj){
    static_cast<WAV*>(obj)->Rewind();
}

