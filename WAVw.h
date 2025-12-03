#ifndef _H_WAV_W_
#define _H_WAV_W_

#ifdef _WIN32
#ifdef _EXPORT_
#define DLL_PREFIX __declspec(dllexport)
#else
#define DLL_PREFIX  __declspec(dllimport)
#endif
#else
#define DLL_PREFIX 
#endif

extern "C" DLL_PREFIX void* WAV_construct(short ch, uint32_t rate);
extern "C" DLL_PREFIX void WAV_release(void* obj);
extern "C" DLL_PREFIX int WAV_newfile(void* obj, const char* file_name);
extern "C" DLL_PREFIX int WAV_openfile(void* obj, const char* file_name);

extern "C" DLL_PREFIX int WAV_is_eof(void* obj);

extern "C" DLL_PREFIX int WAV_read_unit_short(void* obj, short* dest, int unit);
extern "C" DLL_PREFIX int WAV_read_unit_int(void* obj, int* dest, int unit);
extern "C" DLL_PREFIX int WAV_read_unit_float(void* obj, float* dest, int unit);
extern "C" DLL_PREFIX int WAV_read_unit_double(void* obj, double* dest, int unit);

extern "C" DLL_PREFIX int WAV_append_short(void* obj, short* app_data, unsigned int app_size);
extern "C" DLL_PREFIX int WAV_append_int(void* obj, int* app_data, unsigned int app_size);
extern "C" DLL_PREFIX int WAV_append_float(void* obj, float* app_data, unsigned int app_size);
extern "C" DLL_PREFIX int WAV_append_double(void* obj, double* app_data, unsigned int app_size);

extern "C" DLL_PREFIX void WAV_finish(void* obj);
extern "C" DLL_PREFIX void WAV_rewind(void* obj);


#endif
