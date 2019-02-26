#include "SL_V2XCommunication.hpp"
#include <iostream>
using namespace LTEv;
int main(){
  std::vector<int> v;
  SL_V2XCommunication sl;
  printf("Objeto creado\n");
  sl.getV2XCommResourcePool();
  printf("Resource pool obtenido\n");

  sl.setTransmissionFormat();
  printf("Formato de transmision\n");
}
