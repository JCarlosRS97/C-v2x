#include "SL_V2XCommunication.hpp"
#include <iostream>
using namespace LTEv;
int main(){
  std::vector<int> v;
  SL_V2XCommunication sl;
  printf("Objeto creado\n");
  sl.getV2XCommResourcePool();
  printf("Resource pool obtenido\n");
  LTEv::SL_V2XUEConfig sL_V2XUEConfig(80, 0, 0, 0);
  sl.PSxCH_Procedures(sL_V2XUEConfig, 0);
  printf("Formato de transmision\n");
}
