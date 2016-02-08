int LDR_Pin = A0; //analog pin A0

void getLdrReadings(){
    data.light = map(analogRead(LDR_Pin), 0, 1024, 0, 100);  // convert to a percentage
}

