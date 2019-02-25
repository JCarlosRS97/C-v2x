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
/* 2.- Declaración de la clase 1 */
#ifndef SL_V2XCOMMUNICATION
#define SL_V2XCOMMUNICATION
namespace LTEv{
   class SL_V2XCommunication
   {
   protected:
   /* 2.1.- Declaración de atributos privados */
   /* constantes del estandar*/
   const std::string cp_Len_r12 = "Normal"; // SL-CP-Len: Cyclic Prefix: For V2V only 'Normal' is allowed
   static const int NRBsc = 12; // resource block size in the frequency domain, expressed as a number of subcarriers
   static const int sci1_TBsize = 32; // length of SCI Format 1 message (36.212-5.4.3.1.2)
   static const int Msc_PSCCH = 24; // bandwidth of SL PSCCH in # subcarriers (2 PRBs in one subframe)
   static const int NSLsymb = 7; // Number of SL symbols per slot,  depending on CPconf
   static constexpr int PSCCH_DMRS_symbloc_perSubframe[4] = {2, 5, 8, 11}; // PSCCH DMRS Symbol locations per subframe for PSCCH
   static constexpr int PSCCH_symbloc_perSubframe[] = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13}; // PSCCH Symbol locations per subframe for PSCCH
   static constexpr int PSSCH_DMRS_symbloc_perSubframe[] = {2, 5, 8, 11}; // PSSCH DMRS Symbol locations per subframe for PSCCH
   static constexpr int PSSCH_symbloc_perSubframe[] = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13}; // PSSCH Symbol locations per subframe for PSCCH
   static const int pscch_c_init = 510; // 36.211, 9.4.1: initializer for PSCCH scrambling sequence
   static constexpr int acceptable_NPRBsizes[] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36, 40, 45, 48, 50, 54, 60, 72, 75, 80, 81, 90, 96, 100}; // Acceptable sizes for NRB_PSSCH
   static const int mcs_r14_MAX = 11; // Assume for the time being that only QPSK is allowed
   static const int Size_PSCCH = 2; // Tamaño en PRB de PSCCH
   // mac parametrization
   static const int slschSubHeader_Len = 6; // Length of SL-SCH Subheader in bytes (36.321, 6.1.6)
   static const int macPDU_Len = 2; // Length of MAC PDU Subheader in bytes (36.321, 6.1.6)
   static const int maxTBSize = 1256; // this is internal: MAX TB Size (due to turbo coding)
   // SyncConfig
   static const int syncPeriod = 160; // sync subframe period # subframes. Fixed to 160 for V2V.


   /* Atributos de inicializacion */
   static const int NSLRB = 25; // Sidelink bandwidth configuration in RBs: 6, 15, 25, 50, 100 (default : 25)
   static const int NSLID = 301; // SLSSID: Physical layer sidelink synchronization identity: 0..335 (default 0)
   static const int slMode = 3; // Sidelink Mode: 3 (V2V-scheduled) or 4 (V2V autonomous sensing)
   // SyncConfig
   static const int syncOffsetIndicator = 0; // SL-SyncConfig: Offset indicator for sync subframe with respect to subframe #0: 0..159 (default : 0)
   static const bool syncTxPeriodic = true; // SL-SyncConfig: single-shot or periodic sync (default: 1)
   // SL V2X COMMUNICATION Resources Pool Configuration
   static const int sl_OffsetIndicator_r14 = 0; // Indicates the offset of the first subframe of a resource pool: {0..10239}
   static const int TAM_sl_Subframe_r14 = 14; //bitmap with acceptable sizes {16,20,100}
   bool sl_Subframe_r14[TAM_sl_Subframe_r14]; // Determines PSSCH subframe pool

   static const int adjacencyPSCCH_PSSCH_r14 = 1; // Indicates if PSCCH and PSSCH are adjacent in the frequecy domain {true,false}
   static const int SizeSubchannel_r14 = 5; // Indicates the number of PRBs of each subchannel in the corresponding resource pool: {n4, n5, n6, n8, n9, n10, n12, n15, n16, n18, n20, n25, n30, n48, n50, n72, n75, n96, n100}
   static const int numSubchannel_r14 = 5; // Indicates the number of subchannels in the corresponding resource pool: {n1, n3, n5, n10, n15, n20}
   static const int startRB_Subchannel_r14 = 0; // Indicates the lowest RB index of the subchannel with the lowest index: {0..99}
   static const int startRB_PSCCH_Pool_r14 = 14; // Indicates the lowest RB index of the PSCCH pool. This field is irrelevant if a UE always transmits control and data in adjacent RBs in the same subframe: {0..99}

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

   /* Atributos internos de la clase */

   int ms_PSCCH_RP[][SizeSubchannel_r14]; // PSCCH Resource Blocks Pool
   int ms_PSSCH_RP[][Size_PSCCH];         // PSSCH Resource Blocks Pool
   std::vector<int> ls_PSXCH_RP;          // PSXCH Subframe Pool
   /* 2.2.- Declaración de métodos privados */
   void initialize_data();

   public:
   /* 2.3.- Declaración de atributos públicos */

   /* 2.4.- Declaración de métodos públicos */
   SL_V2XCommunication();
   void GetV2XCommResourcePool ();
   void SetTransmissionFormat(); // determina mcs tbSize y N_PRB
   };
   /* 7.- Declaración de tipos de datos */
}
#endif
