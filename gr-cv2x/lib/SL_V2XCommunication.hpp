/*----------------------------------------------------------------------------
Universidad de Málaga - Grupo de Ingeniería de Comunicaciones
Proyecto: <proyecto>
----------------------------------------------------------------------------
MODULO:
SL_V2XCommunication
----------------------------------------------------------------------------
Autor:
Juan Carlos Ruiz Sicilia
----------------------------------------------------------------------------
Control de ediciones
25/02/19 Creacion de la clase
----------------------------------------------------------------------------
Descripción
Este modulo configura la comunicacion LTE-V
----------------------------------------------------------------------------
Clases
SL_V2XCommunication Transceptor LTE-V
----------------------------------------------------------------------------
Tipos de datos
<nombre>
<descripción>
<nombre>
<descripción>
----------------------------------------------------------------------------
Métodos [rutinas] públicos
<nombre>
<descripción>
<nombre>
<descripción>
--------------------------------------------------------------------------*/
/* 1.- Ficheros include externos */
#include <vector>
#include <string>
#include <cstdint>
#include "LTE_Constants.cpp"
#include "SL_V2XUEConfig.hpp"

/* 2.- Declaración de la clase 1 */
#ifndef SL_V2XCOMMUNICATION
#define SL_V2XCOMMUNICATION
namespace LTEv{
   class SL_V2XCommunication
   {
   protected:
   /* 2.1.- Declaración de atributos privados */
   /* Atributos de inicializacion */
   static const int NSLRB = 25; // Sidelink bandwidth configuration in RBs: 6, 15, 25, 50, 100 (default : 25)
   static const int NSLID = 301; // SLSSID: Physical layer sidelink synchronization identity: 0..335 (default 0)
   static const int slMode = 3; // Sidelink Mode: 3 (V2V-scheduled) or 4 (V2V autonomous sensing)
   // SyncConfig
   static const int syncOffsetIndicator = 0; // SL-SyncConfig: Offset indicator for sync subframe with respect to subframe #0: 0..159 (default : 0)
   static const bool syncTxPeriodic = true; // SL-SyncConfig: single-shot or periodic sync (default: 1)
   // SL V2X COMMUNICATION Resources Pool Configuration
   static const int sl_OffsetIndicator_r14 = 0; // Indicates the offset of the first subframe of a resource pool: {0..10239}
   static const int Size_sl_Subframe_r14 = 20; //bitmap with acceptable sizes {16,20,100}
   bool sl_Subframe_r14[Size_sl_Subframe_r14]; // Determines PSSCH subframe pool

   static const int adjacencyPSCCH_PSSCH_r14 = 1; // Indicates if PSCCH and PSSCH are adjacent in the frequecy domain {true,false}
   static const int SizeSubchannel_r14 = 5; // Indicates the number of PRBs of each subchannel in the corresponding resource pool: {n4, n5, n6, n8, n9, n10, n12, n15, n16, n18, n20, n25, n30, n48, n50, n72, n75, n96, n100}
   static const int NumSubchannel_r14 = 5; // Indicates the number of subchannels in the corresponding resource pool: {n1, n3, n5, n10, n15, n20}
   static const int startRB_Subchannel_r14 = 0; // Indicates the lowest RB index of the subchannel with the lowest index: {0..99}
   static const int startRB_PSCCH_Pool_r14 = 14; // Indicates the lowest RB index of the PSCCH pool. This field is irrelevant if a UE always transmits control and data in adjacent RBs in the same subframe: {0..99}
   int NFFT;                               // FFT size
   int chanSRate;                          // channel sampling rate
   int mcs_r14;
   int pssch_Qprime;
   int pssch_TBsize;
   // SL V2X COMMUNICATION UE-specific Resource Allocation Configuration
   // Common for both modes
   // Nota: son pasados por PSxCH_Procedures
   int sduSize = 10; // SDU size (coming from MAC)
   //Nota: Si el gap es 0 no hay retransmision
   //Nota: V2x commm solo permite una retransmision
   //Nota: El gap implica buscar el siguiente pool donde retransmitir
   int SFgap = 0; // Time gap between initial transmission and retransmission. Either set through DCI5A or Higher Layers or Preconfigured.
   // fully controlled(Only for mode 3)
   int Linit = 0; // 1st transmission opportunity frequency offset: from DCI5A "Lowest index of the subchannel allocation to the initial transmission" --> ceil(log2(numSubchannel_r14) bits, here in integer form. Either set through DCI5A or Higher Layers or Preconfigured.
   int nsubCHstart = 0; // (relevant if SFgap not zero) 2nd transmission opportunity frequency offset: from "Frequency Resource Location of the initial transmission and retransmission" --> ceil(log2(numSubchannel_r14) bits, here in integer form.  This is actually configured using "RIV". Here we provide the corresponding subchannel directly. Either set through DCI5A or Higher Layers or Preconfigured.
   int LsubCH;

   /* Atributos internos de la clase */

   int ms_PSCCH_RP[NumSubchannel_r14][Size_PSCCH]; // PSCCH Resource Blocks Pool
   int ms_PSSCH_RP[NumSubchannel_r14][SizeSubchannel_r14];         // PSSCH Resource Blocks Pool
   std::vector<int> ls_PSXCH_RP;          // PSXCH Subframe Pool
   int N_RB_PSSCH;
   int Msc_PSSCH;                          // bandwidth of SL PSCCH in # subcarriers
   int pssch_BitCapacity;                  // PSSCH Channel Bit Capacity
   int pscch_BitCapacity;                  // PSCCH Channel Bit Capacity
   int cmux;                               // multiplier for PUSCH/PSCCH Interleaving
   int pscch_muxintlv_indices;             // PUSCH interleaver indices for transport channel processing : pscch
   int pscch_b_scramb_seq;                 // PSCCH scrambling sequence
   int pssch_muxintlv_indices;             // PUSCH interleaver indices for transport channel processing : pssch
   int pscch_dmrs_seq;                     // PSCCH DMRS sequence
   int l_PSXCH_selected[2] ;                  // UE-specific scheduled subframes for PSCCH
   int m_PSCCH_selected[2][Size_PSCCH];      // UE-specific scheduled PRBs for PSCCH
   int m_PSSCH_selected[2][SizeSubchannel_r14];   // UE-specific scheduled PRBs for PSSCH
   int frlbitmap_len;                          // length of v2x frequency resource location field bitmap used in SCI Format 1
   uint8_t v2x_frlbitmap[2];                          // frl bitmap for v2x pscch/pssch
   uint32_t sciTBs[2];                        // sci bit sequences (new tx and retx)
                                             //El SCI tiene 4 * 8 = 32 bits


   /* 2.2.- Declaración de métodos privados */
   void initialize_data();
   void setTransmissionFormat(); // determina mcs tbSize y N_PRB
   uint32_t LoadSCI1TB(int txOp);

   public:
   /* 2.3.- Declaración de atributos públicos */

   /* 2.4.- Declaración de métodos públicos */
   SL_V2XCommunication();
   void getV2XCommResourcePool ();
   void PSxCH_Procedures(LTEv::SL_V2XUEConfig sL_V2XUEConfig, int subframeCounter);
   };
   /* 7.- Declaración de tipos de datos */
}
#endif
