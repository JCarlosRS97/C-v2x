/*----------------------------------------------------------------------------
  Universidad de Málaga - Grupo de Ingeniería de Comunicaciones
  Proyecto: <proyecto>
  ----------------------------------------------------------------------------
  MODULO:      <nombre>
  ----------------------------------------------------------------------------
  Autor:       <autor>
  ----------------------------------------------------------------------------
  Control de ediciones
     <fecha>   <descripción>
  ----------------------------------------------------------------------------
  Descripción
     xxxxxxxxxxxxxxxxxxxxxxxxx
  ----------------------------------------------------------------------------
  Métodos [rutinas] públicos
     <nombre>          <descripción>
     <nombre>          <descripción>
  ----------------------------------------------------------------------------
  Métodos [rutinas] internos
     <nombre>          <descripción>
     <nombre>          <descripción>
  ----------------------------------------------------------------------------*/


/* 1.- Variables de compilación condicional */


/* 2.- Ficheros include del módulo */
#include "SL_V2XCommunication.hpp"

/* 3.- Ficheros include externos */
#include <list>
#include <memory>
#include <cstring>
#include <iostream>

/* 4.- Ficheros include del proyecto */
#include "3GPP_36213_Table8_6_1_1.cpp"
#include "3GPP_36213_Table7_1_7_2_1_1.cpp"

/* 5.- Definición de literales */


/* 6.- Declaración de rutinas internas */


/* 7.- Implementación de las clases */


/* 7.1. – Clase 1 */
namespace LTEv
{

/*------------------------------------------------------------------------------
                                MÉTODOS PÚBLICOS
/*----------------------------------------------------------------------------*/

SL_V2XCommunication::SL_V2XCommunication(){
   initialize_data();
}

void SL_V2XCommunication::getV2XCommResourcePool (){
   //Calculate v2x communication resource pools for given input configuration (36.213 - 14.1.5 (PSSCH) & 14.2.4 (PSCCH))
   //36.213 14.1.1.4A
   // ------------------ SUBFRAMES (common for PSSCH/PSSCH, 14.1.5) ----------------------
   // identify subframes where SLSS is configured within an SFN
   //Nota: el timimg del mib solo permite contar hasta 10240
   //subframe(10 bits de SFN)

   std::list<int> ls_PSXCH_RP_aux;
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
         ls_PSXCH_RP_aux.push_back(i);
       }
     }
   }else{
     for(int i = 0; i < SFN_MAX; i++){
       if(i != k){
         ls_PSXCH_RP_aux.push_back(i);
       }
     }
     Nslss = 1;
   }

   //Ahora se calculan los subframes reservados y son eliminados
   int Nreserved = (SFN_MAX - Nslss) % Size_sl_Subframe_r14;

   k = 0; // contador de los reservados
   std::list<int>::iterator it = ls_PSXCH_RP_aux.begin();
   while (it != ls_PSXCH_RP_aux.end()) {
     // Remove elements while iterating
     if (k < Nreserved && int(k*(SFN_MAX - Nslss)/Nreserved) == (*it)) {
       // erase() makes the passed iterator invalid
       // But returns the iterator to the next of deleted element
       it = ls_PSXCH_RP_aux.erase(it);
       k++;
     } else{
       it++;
     }
   }


   //calcula los que est�n acorde al bitmap y los pasa a un vector que
   //funcionará de forma más rapida para ser recorrido
   it = ls_PSXCH_RP_aux.begin();
   k = 0; // contador bitmap
   while (it != ls_PSXCH_RP_aux.end()) {
     // Remove elements while iterating
     if (sl_Subframe_r14[k] == true) {
       ls_PSXCH_RP.push_back(*it);
     }
     k++;
     if(k >= Size_sl_Subframe_r14){
       k = 0;
     }
     it++;
   }

   //A continuación se calcula los RB disponibles
   // int ms_PSCCH_RP[numSubchannel_r14][TAM_PSCCH = 2]
   // int ms_PSSCH_RP[numSubchannel_r14][SizeSubchannel_r14]
   for(int i = 0; i < NumSubchannel_r14; i++){
     for(int j = 0; j < SizeSubchannel_r14; j++){
       ms_PSSCH_RP[i][j] = startRB_Subchannel_r14 + SizeSubchannel_r14 * i + j;
       if(j < 2){// hasta aqui son los PRB's de sci
         if(adjacencyPSCCH_PSSCH_r14){
           ms_PSCCH_RP[i][j] =  ms_PSSCH_RP[i][j];
         }else{
           ms_PSCCH_RP[i][j] = startRB_PSCCH_Pool_r14 + 2*i + j;
         }
       }
     }
   }

   //Comprobaciones
   for(int i = 0; i< ls_PSXCH_RP.size(); i++){
     std::cout<<ls_PSXCH_RP[i]<<" ";
   }
   std::cout<<std::endl;
   for(int i = 0; i < NumSubchannel_r14; i++){
     for(int j = 0; j < SizeSubchannel_r14; j++){
       std::cout<<ms_PSSCH_RP[i][j]<<" ";
     }
     std::cout<<std::endl;
     for(int j = 0; j < Size_PSCCH; j++){
       std::cout<<ms_PSCCH_RP[i][j]<<" ";
     }
     std::cout<<std::endl;
  }
}




 void SL_V2XCommunication::PSxCH_Procedures(SL_V2XUEConfig SL_V2XUEConfig, int subframeCounter){
    /*----------------------------------------------------------------------------
      PSxCH_Procedures
      ----------------------------------------------------------------------------
      Descripción:
         Define los parametros de capa física a partir de capas superiores,
         DCI5 o preconfiguraciones previas.
      ----------------------------------------------------------------------------
      Entradas:
        - SL_V2XUEConfig      parametros propios del usuario
        - subframeCounter   numero de subframe SFN
      Salidas:
        - sduSize: obtenido por SL_V2XUEConfig o a partir del resto de parametros
        - SFgap:   obtenido por SL_V2XUEConfig o a partir del resto de parametros
        - Linit: obtenido por SL_V2XUEConfig o a partir del resto de parametros
        - nsubCHstart: obtenido por SL_V2XUEConfig o a partir del resto de parametros
        - LsubCH: obtenido por SL_V2XUEConfig o a partir del resto de parametros
        - mcs_r14: obtenido por SL_V2XUEConfig o a partir del resto de parametros

      Devuelve:

      ----------------------------------------------------------------------------
      Variables/atributos globales:
      Entradas:
         <nombre>  <nombre>  <nombre>...
      Salidas:
         <nombre>  <nombre>  <nombre>...
      Entrada/salida:
         <nombre>  <nombre>  <nombre>...
      ----------------------------------------------------------------------------
      Rutinas/métodos:
          <nombre>   <nombre>...
          <modulo>:   <nombre>   <nombre>...
          <modulo>:   <nombre>   <nombre>...
      ----------------------------------------------------------------------------*/
      //Parametro beta 14.1.1.4C
      int beta = 2*adjacencyPSCCH_PSSCH_r14;

      // Numero de transmisiones
      SFgap = sL_V2XConfig.getSFgap();
      int numTxOp = (SFgap > 0) +1

      //Se obtienen todos los parametros propios del usuario
      if(sL_V2XConfig.isTx()){
         sduSize = sL_V2XConfig.getSduSize();
         Linit = sL_V2XConfig.getLinit();
         nsubCHstart = sL_V2XConfig.getNsubCHstart();
         setTransmissionFormat();
         LsubCH = (N_RB_PSSCH + beta)/sizeSubchannel_r14;
      } else {
         mcs_r14 = sL_V2XConfig.getMsc_r14();
         Linit = sL_V2XConfig.getLinit();
         nsubCHstart = sL_V2XConfig.getNsubCHstart();
         
      }
}


/*------------------------------------------------------------------------------
                                MÉTODOS PRIVADOS
/*----------------------------------------------------------------------------*/

void SL_V2XCommunication::initialize_data(){
   //Aqui tambien se configura el cp_Len_r12
   std::memset(sl_Subframe_r14, 0, Size_sl_Subframe_r14);
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

void SL_V2XCommunication::setTransmissionFormat() {
  /*La función que se muestra a continuacion tiene como salida varios parametros de
  configuracion correspondientes a 36.213 14.1.1.4C
  El procedimiento consiste en limitar las posibilidades al minimo posible y elegir
  en funcion de un criterio
  */
  //Primero se restringen las posibilidades
  //Numeros de PRBs posible segun la limitacion de
  //ancho de banda y tamaños de subcanales
  std::vector<int> N_PRB_space;
  int i = 0;
  while(acceptable_NPRBsizes[i] != NSLRB){
     if((acceptable_NPRBsizes[i] + 2*adjacencyPSCCH_PSSCH_r14)%SizeSubchannel_r14==0){
        N_PRB_space.push_back(acceptable_NPRBsizes[i]);
     }
     i++;
  }

  //mac pdu length accounting for SDU and Headers
  int mac_pdu_len_MIN = (slschSubHeader_Len + macPDU_Len)*8 + sduSize;

  //Los posibles I_TBS son inferiores a este valor
  int Itbs_space_max = _3GPP_36213_Table8_6_1_1[mcs_r14_MAX][2];

  //Para seleccionar uan configuracion siguiendo el criterio de Padding
  //habra 3 columnas Itbs_space N_PRB_space Padding
  std::vector<std::unique_ptr<int[]>> combs;
  std::unique_ptr<int[]> ptr;
  int value;
  int minorPadding = -1;
  for(i = 0; i <= Itbs_space_max; i++){
     for(int j = 0; j < N_PRB_space.size(); j++){
        value = _3GPP_36213_Table7_1_7_2_1_1[i+1][N_PRB_space[j]];
        if((value >= mac_pdu_len_MIN) && (value <= maxTBSize)){
           ptr = std::make_unique<int[]>(3);
           ptr[0] = i;
           ptr[1] = N_PRB_space[j];
           ptr[2] = value - mac_pdu_len_MIN;
           if((minorPadding == -1) || (ptr[2] < minorPadding)){
              minorPadding = ptr[2];
           }
           combs.push_back(std::move(ptr));
        }
     }
  }

  //Resultado
  i = 0;
  while(combs[i][2] != minorPadding){
     i++;
  }

  int N_RB_PSSCH = combs[i][1];
  int pssch_TBsize = _3GPP_36213_Table7_1_7_2_1_1[combs[i][0]+1][combs[i][1]];
  //Se localiza el indice en  la tabla de modulaciones para obtener el mcs
  //y la qprima
  int modtable_index = 0;
  while(combs[i][0] != _3GPP_36213_Table8_6_1_1[modtable_index][2]){
     modtable_index++;
  }
  mcs_r14 = _3GPP_36213_Table8_6_1_1[modtable_index][0];
  pssch_Qprime = _3GPP_36213_Table8_6_1_1[modtable_index][1];

  printf("For SDU size = %3i bits we set: mcs = %2i, N_PRB = %2i, TB Size = %4i, Qprime = %i (Padding = %4i)\n",
      sduSize, mcs_r14, N_RB_PSSCH, pssch_TBsize, pssch_Qprime, minorPadding);
 }

/* 8.- Implementación de las rutinas */



/*------------------------------------------------------------------------------
                                RUTINAS PÚBLICAS
/*----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                RUTINAS INTERNAS
/*----------------------------------------------------------------------------*/
}
