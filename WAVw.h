#ifndef _H_WAV_W_
#define _H_WAV_W_

#ifdef _WIN32
#ifdef _EXPORT_
#define DLL_PREFIX extern "C" __declspec(dllexport)
#else
#define DLL_PREFIX extern "C" __declspec(dllimport)
#endif
#else
    #ifdef __cplusplus
    #define DLL_PREFIX extern "C"
    #else
    #define DLL_PREFIX
    #endif
#endif

DLL_PREFIX void* WAV_construct(short ch, int rate);
DLL_PREFIX void WAV_release(void* obj);
DLL_PREFIX int WAV_newfile(void* obj, const char* file_name);
DLL_PREFIX int WAV_openfile(void* obj, const char* file_name);

DLL_PREFIX int WAV_is_eof(void* obj);

DLL_PREFIX int WAV_read_unit_short(void* obj, short* dest, int unit);
DLL_PREFIX int WAV_read_unit_int(void* obj, int* dest, int unit);
DLL_PREFIX int WAV_read_unit_float(void* obj, float* dest, int unit);
DLL_PREFIX int WAV_read_unit_double(void* obj, double* dest, int unit);

DLL_PREFIX int WAV_append_short(void* obj, short* app_data, unsigned int app_size);
DLL_PREFIX int WAV_append_int(void* obj, int* app_data, unsigned int app_size);
DLL_PREFIX int WAV_append_float(void* obj, float* app_data, unsigned int app_size);
DLL_PREFIX int WAV_append_double(void* obj, double* app_data, unsigned int app_size);

DLL_PREFIX void WAV_finish(void* obj);
DLL_PREFIX void WAV_rewind(void* obj);


#endif
