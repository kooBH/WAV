# WAV  
c++ WAV class  
 
 
 # Output
 ```c++
 WAV output(channels,samplerate);
 output.NewFile("output.wav");
 
 while(IsProcessing){
    ...
    output.Append(short_type_buf,shift_size*channels);
 }
 
 output.Finish()
 ```
 
 # Input 
 ```c++
 WAV input();
 input.OpenFile("input.wav");
 
 while(!input.IsEOF()){
    input.ReadUnit(short_type_buffer,shift_size*channels);
    ...
 }
 
 input.Finish();
 
 ```
