float getTemperatureCalibration(){
  if (strcmp(cloud.id, "WeMos01") == 0) {return -0.5;}
  if (strcmp(cloud.id, "Balcony") == 0) {return -0.5;}
  if (strcmp(cloud.id, "WeMos02") == 0) {return -1.2;}
  if (strcmp(cloud.id, "WeMos03") == 0) {return -3.3;}
  if (strcmp(cloud.id, "NodeMCU02") == 0) {return -0.5;}
  if (strcmp(cloud.id, "Thing01") == 0) {return -2.25;}
  return 0.0;
}
