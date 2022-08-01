#include "../WAV.h"

extern "C"{
    WAV* WAV_new(int n_channel, int samplerate) {
        return new WAV(n_channel,samplerate);
    }
    void WAV_NewFile(WAV* p, char* file_name){
        p->NewFile(file_name);
    }
    void WAV_Finish(WAV*p){
        p->Finish();
    }
    int WAV_Append(WAV*p, short*app_data, unsigned int app_size){
        return p->Append(app_data,app_size);
    }
    void WAV_Print(WAV*p){
        p->Print();
    }
    
}

    