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
#include <cmath>
#include <cassert>

/* 4.- Ficheros include del proyecto */
#include "LTE_Constants.cpp"
#include "3GPP_36213_Table7_1_7_2_1_1.cpp"
#include "3GPP_36213_Table8_6_1_1.cpp"

/* 5.- Definición de literales */


/* 6.- Declaración de rutinas internas */
uint8_t ra_bitmap_resourcealloc_create(int NsubCH, int RBstart, int Lcrbs);
void LoadSCI1TB (uint8_t sciTBs[4], int txOp);

/* 7.- Implementación de las clases */


/* 7.1. – Clase 1 */
namespace LTEv
{

/*------------------------------------------------------------------------------
                                MÉTODOS PÚBLICOS
------------------------------------------------------------------------------*/

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




 void SL_V2XCommunication::PSxCH_Procedures(LTEv::SL_V2XUEConfig sL_V2XUEConfig, int subframeCounter){
/*-----------------------------------------------------------------------------
      PSxCH_Procedures
-------------------------------------------------------------------------------
      Descripción:
         Define los parametros de capa física a partir de capas superiores,
         DCI5 o preconfiguraciones previas.
--------------------------------------------------------------------------------
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

--------------------------------------------------------------------------------
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
------------------------------------------------------------------------------*/
      //Lectura inicial
      Linit = sL_V2XUEConfig.getLinit();
      nsubCHstart = sL_V2XUEConfig.getNsubCHstart();
      SFgap = sL_V2XUEConfig.getSFgap();
/*----------------------------- PSSCH ----------------------------------------*/
      //Parametro beta 14.1.1.4C
      int beta = 2*adjacencyPSCCH_PSSCH_r14;

      // Numero de transmisiones
      int numTxOp = (SFgap > 0) + 1;

      //Se obtienen todos los parametros propios del usuario
      if(sL_V2XUEConfig.isTx()){
         sduSize = sL_V2XUEConfig.getSduSize();
         setTransmissionFormat();
         LsubCH = std::ceil(double(N_RB_PSSCH + beta)/SizeSubchannel_r14);
      } else {
         mcs_r14 = sL_V2XUEConfig.getMsc_r14();
         LsubCH = sL_V2XUEConfig.getLsubCH();
         pssch_Qprime = _3GPP_36213_Table8_6_1_1[mcs_r14][1];
         int I_TBS = _3GPP_36213_Table8_6_1_1[mcs_r14][2];
         //Ahora hay que encontrar pssch_TBsize a partir de I_TBS y N_RB_PSSCH
         int row = 0;
         while(_3GPP_36213_Table7_1_7_2_1_1[row][0] != I_TBS){
            row++;
         }
         int column = 0;
         while(_3GPP_36213_Table7_1_7_2_1_1[0][column] != N_RB_PSSCH){
            column++;
         }
         //TODO controlar condicion de fin
         pssch_TBsize = _3GPP_36213_Table7_1_7_2_1_1[row][column];
      }

      //Ahora se parametriza capa fisica
      Msc_PSSCH = N_RB_PSSCH*NRBsc;
      // length of PSSCH sequence in bits
      pssch_BitCapacity = Msc_PSSCH*Size_PSXCH_symbloc_perSubframe*pssch_Qprime;
      //Index interleaving will be here
      //pssch_muxintlv_indices =  tran_uplink_MuxIntlvDataOnly_getIndices(  pssch_BitCapacity, cmux, pssch_Qprime, 1 );
      std::cout<<"PSSCH ModOrder = "<< pssch_Qprime<< std::endl;
      std::cout<<"PSSCH TBSize = "<<pssch_TBsize<<" (bits)"<< std::endl;
      std::cout<<"PSSCH Num of PRBs = " << N_RB_PSSCH <<std::endl;
      std::cout<<"PSSCH Bit Capacity = " << pssch_BitCapacity << "(bits)"<< std::endl;

      int m[2]; // Frequency offset array of transmision and retransmision
      if(slMode ==3 || (slMode == 4 && !sL_V2XUEConfig.isTx())){ //14.1.1.4C
         m[0] = Linit;
         m[1] = nsubCHstart;
      }else{
         //14.1.1.4.Β, 14.1.1.6
         std::cout << "Modo 4 no soportado actualmente." << std::endl;
         assert(0);
      }

      for(int txOP = 0; txOP < numTxOp; txOP++){
         for(int j = 0; j < N_RB_PSSCH; j++){
            m_PSSCH_selected[txOP].push_back(startRB_Subchannel_r14 +
               m[txOP]* SizeSubchannel_r14 + beta + j);
            v2x_frlbitmap[txOP] = ra_bitmap_resourcealloc_create(NumSubchannel_r14,
                m[txOP], LsubCH);
         }
      }
      //Impresion
      std::cout<<"==================================================\n";
      for(int txOP = 0; txOP < numTxOp; txOP++){
         std::cout << "PSSCH PRBs [txOp = "<< txOP << "]: ";
         for(int j = 0; j < m_PSSCH_selected[txOP].size(); j++){
            std::cout << m_PSCCH_selected[txOP][j] << ' ';
         }
         std::cout<<std::endl;
      }
/*----------------------------- PSCCH ----------------------------------------*/
      // Determine subframe and resource blocks for transmitting SCI Format 1
      //message & Generate Messages.
      // the following are relevant only at tx side where we know the incoming TB size.
      // In Rx we have already the info since we do blind detection of SCIs
      if(sL_V2XUEConfig.isTx()){
         // select 1st available subframe
         int n = 0;
         while((ls_PSXCH_RP.size() > n) && (ls_PSXCH_RP[n] < subframeCounter)){
            n++;
         }
         //En caso de haber pasado el ultimo subframe
         if(ls_PSXCH_RP.size() == n){
            n = 0;
         }
         //36.213 14.1.1.4C
         // for each (potential) transmission opportunity
         for (int txOp = 0; txOp < numTxOp; txOp++){
             // new transmission: 1st available subframe
             // retransmission  : Add SFgap
             if (slMode==3)
                 l_PSXCH_selected[txOp] = ls_PSXCH_RP[(n+(txOp-1)*SFgap-1) % ls_PSXCH_RP.size()];
             else if (slMode==4)
                 // Same as mode-3 (Placeholder. TO BE REVISED)
                 assert(0);

             // get the PRBs based on PSSCH Subchannel Selection
             //Solamente se esta cogiendo un subcanal. Revisar.
             m_PSCCH_selected[txOp] = ms_PSCCH_RP[m(txOp)];
             // create SCI bit sequence
             LoadSCI1TB (sciTBs[txOp], txOp);
         }

      }
}


/*------------------------------------------------------------------------------
                                MÉTODOS PRIVADOS
------------------------------------------------------------------------------*/

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
    pscch_BitCapacity = Msc_PSCCH*Size_PSXCH_symbloc_perSubframe * 2;
    //36.212 - 5.4.3.1.2
    frlbitmap_len = ceil(log2(NumSubchannel_r14*(NumSubchannel_r14 + 1.0f)/2));
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

  N_RB_PSSCH = combs[i][1];
  pssch_TBsize = _3GPP_36213_Table7_1_7_2_1_1[combs[i][0]+1][combs[i][1]];
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
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                RUTINAS INTERNAS
------------------------------------------------------------------------------*/
uint8_t ra_bitmap_resourcealloc_create(int NsubCH, int RBstart, int Lcrbs){
   /*----------------------------------------------------------------------------
     ra_bitmap_resourcealloc_create
     ----------------------------------------------------------------------------
     Descripción:
        Halla el RIV y lo traduce a un uint8_t. Sigue el estandar 36.213 -
         14.1.1.4C.
     ----------------------------------------------------------------------------
     Entradas:
   - NsubCH: numero total de subcanales
   - RBstart: indice del primer resource block
   - Lcrbs: numero de prb reservados

     Salidas:

     Devuelve:
       - v2x_frlbitmap: frequency resource location del SCI
     ----------------------------------------------------------------------------*/
     uint8_t riv;
     if ((Lcrbs-1)<=NsubCH/2){
        riv = NsubCH*(Lcrbs-1)+RBstart;
     } else{
        riv = NsubCH*(NsubCH - Lcrbs + 1) + (NsubCH - 1 - RBstart);
     }
     return riv;
}

void LoadSCI1TB (uint8_t sciTBs[4], int txOp){

}



}
