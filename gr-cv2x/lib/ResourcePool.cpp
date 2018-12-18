#include "config.cpp"
#include <vector>
#include <list>
#include <iostream>
using namespace std;
void initialize_data(){
  std::memset(sl_Subframe_r14, 0, TAM_sl_Subframe_r14);
  sl_Subframe_r14[2] = true;
  switch (NSLRB){
      case 6:   NFFT = 128;  chanSRate = 1.92e6; break;
      case 15:  NFFT = 256;  chanSRate = 3.84e6; break;
      case 25:  NFFT = 512;  chanSRate = 7.68e6; break;
      case 50:  NFFT = 1024; chanSRate = 15.36e6; break;
      case 75:  NFFT = 1536; chanSRate = 23.04e6; break;
      case 100: NFFT = 2048; chanSRate = 30.72e6; break;
    }
}
void GetV2XCommResourcePool (std::vector<int> &v, int ms_PSSCH_RP[][sizeSubchannel_r14], int ms_PSCCH_RP[][TAM_PSCCH]){
  //Calculate v2x communication resource pools for given input configuration (36.213 - 14.1.5 (PSSCH) & 14.2.4 (PSCCH))

  // ------------------ SUBFRAMES (common for PSSCH/PSSCH, 14.1.5) ----------------------
  // identify subframes where SLSS is configured within an SFN
  //Nota: el timimg del mib solo permite contar hasta 10240
  //subframe(10 bits de SFN)

  std::list<int> ls_PSXCH_RP;
  //Se inicializa la lista eliminando ya los SLSS
  int k = syncOffsetIndicator;
  int Nslss;
  if(syncTxPeriodic){
    Nslss = 0;
    for(int i = 0; i < SFN_MAX; i++){
      if(i == k){
        Nslss++;
        k += syncPeriod;
      }else{
        ls_PSXCH_RP.push_back(i);
      }

    }
  }else{
    for(int i = 0; i < SFN_MAX; i++){
      if(i != k){
        ls_PSXCH_RP.push_back(i);
      }
    }
    Nslss = 1;
  }


  //Ahora se calculan los subframes reservados y son eliminados
  int Nreserved = (SFN_MAX - Nslss) % TAM_sl_Subframe_r14;

  k = 0; // contador de los reservados
  std::list<int>::iterator it = ls_PSXCH_RP.begin();
	while (it != ls_PSXCH_RP.end()) {
		// Remove elements while iterating
		if (k < Nreserved && int(k*(SFN_MAX - Nslss)/Nreserved) == (*it)) {
			// erase() makes the passed iterator invalid
			// But returns the iterator to the next of deleted element
			it = ls_PSXCH_RP.erase(it);
      k++;
		} else{
			it++;
    }
	}

  //calcula los que est�n acorde al bitmap y los pasa a un vector que
  //funcionará de forma más rapida para ser recorrido
  it = ls_PSXCH_RP.begin();
  k = 0; // contador bitmap
  while (it != ls_PSXCH_RP.end()) {
		// Remove elements while iterating
		if (sl_Subframe_r14[k] == true) {
      v.push_back((*it));
		}
    k++;
    if(k >= TAM_sl_Subframe_r14){
      k = 0;
    }
    it++;
	}
  // int ms_PSCCH_RP[numSubchannel_r14][TAM_PSCCH = 2]
  // int ms_PSSCH_RP[numSubchannel_r14][sizeSubchannel_r14]
	for(int i = 0; i < numSubchannel_r14; i++){
		for(int j = 0; j < sizeSubchannel_r14; j++){
			ms_PSSCH_RP[i][j] = startRB_Subchannel_r14 + sizeSubchannel_r14 * i + j;
      if(j < 2){// hasta aqui son los PRB's de sci
        if(adjacencyPSCCH_PSSCH_r14){
          ms_PSCCH_RP[i][j] =  ms_PSSCH_RP[i][j];
        }else{
          ms_PSCCH_RP[i][j] = startRB_PSCCH_Pool_r14 + 2*i + j;
        }
      }
		}
	}
}

int main(){
  int ms_PSCCH_RP[numSubchannel_r14][TAM_PSCCH];
  int ms_PSSCH_RP[numSubchannel_r14][sizeSubchannel_r14];
  std::vector<int> v;
  initialize_data();
  GetV2XCommResourcePool(v, ms_PSSCH_RP, ms_PSCCH_RP);
  for(int i = 0; i< v.size(); i++){
    std::cout<<v[i]<<" ";
  }
  std::cout<<std::endl;
  for(int i = 0; i < numSubchannel_r14; i++){
    for(int j = 0; j < sizeSubchannel_r14; j++){
      std::cout<<ms_PSSCH_RP[i][j]<<" ";
    }
    std::cout<<endl;
    for(int j = 0; j < TAM_PSCCH; j++){
      std::cout<<ms_PSCCH_RP[i][j]<<" ";
    }
    std::cout<<endl;
  }
}
