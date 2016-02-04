float getTemperatureCalibration(){
  if (strcmp(cloudConfig.id, "WeMos01") == 0) {return -0.5;}
  if (strcmp(cloudConfig.id, "WeMos02") == 0) {return -1.2;}
  if (strcmp(cloudConfig.id, "WeMos03") == 0) {return -3.3;}
  if (strcmp(cloudConfig.id, "NodeMCU02") == 0) {return -0.5;}
  if (strcmp(cloudConfig.id, "Thing01") == 0) {return -2.25;}
  return 0.0;
}
