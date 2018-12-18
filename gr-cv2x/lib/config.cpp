#include <string>
#include <cstring>
#include <cstdlib>
//// Configuration
// common for Tx/Rx: (reflect 36.331 - 6.3.8, sidelink-related IEs)
// SL Basic Operation Parameters: common for communication and broadcast (if present)
#define SFN_MAX 10240
#define TAM_sl_Subframe_r14 20
#define TAM_PSCCH 2
#define sizeSubchannel_r14 5                      // Indicates the number of PRBs of each subchannel in the corresponding resource pool: {n4, n5, n6, n8, n9, n10, n12, n15, n16, n18, n20, n25, n30, n48, n50, n72, n75, n96, n100}

int NSLRB                           = 25;                     // Sidelink bandwidth configuration in RBs: 6, 15, 25, 50, 100 (default : 25)
int NSLID                           = 301;                    // SLSSID: Physical layer sidelink synchronization identity: 0..335 (default 0)
int slMode                          = 3;                      // Sidelink Mode: 3 (V2V-scheduled) or 4 (V2V autonomous sensing)
// Sync
int syncOffsetIndicator             = 0;                      // SL-SyncConfig: Offset indicator for sync subframe with respect to subframe #0: 0..159 (default : 0)
bool syncTxPeriodic                  = true;                      // SL-SyncConfig: single-shot or periodic sync (default: 1)
int syncPeriod                      = 160;
// SL V2X COMMUNICATION Resources Pool Configuration
int sl_OffsetIndicator_r14          = 0;                      // Indicates the offset of the first subframe of a resource pool: {0..10239}
bool sl_Subframe_r14[TAM_sl_Subframe_r14];    // Determines PSSCH subframe pool: bitmap with acceptable sizes {16,20,100}

int adjacencyPSCCH_PSSCH_r14        = true;                   // Indicates if PSCCH and PSSCH are adjacent in the frequecy domain {true,false}
int numSubchannel_r14               = 5;                      // Indicates the number of subchannels in the corresponding resource pool: {n1, n3, n5, n10, n15, n20}
int startRB_Subchannel_r14          = 0;                      // Indicates the lowest RB index of the subchannel with the lowest index: {0..99}
int startRB_PSCCH_Pool_r14          = 14;                     // Indicates the lowest RB index of the PSCCH pool. This field is irrelevant if a UE always transmits control and data in adjacent RBs in the same subframe: {0..99}
// SL V2X COMMUNICATION UE-specific Resource Allocation Configuration
// Common for both modes
int sduSize                         = 10;                 // SDU size (coming from MAC)
//Nota: Si el gap es 0 no hay retransmision
//Nota: V2x commm solo permite una retransmision
//Nota: El gap implica buscar el siguiente pool donde retransmitir
int SFgap                           = 0;                   // Time gap between initial transmission and retransmission. Either set through DCI5A or Higher Layers or Preconfigured.
// fully controlled(Only for mode 3)
int Linit                           = 0;                   // 1st transmission opportunity frequency offset: from DCI5A "Lowest index of the subchannel allocation to the initial transmission" --> ceil(log2(numSubchannel_r14) bits, here in integer form. Either set through DCI5A or Higher Layers or Preconfigured.
int nsubCHstart                     = 0;                   // (relevant if SFgap not zero) 2nd transmission opportunity frequency offset: from "Frequency Resource Location of the initial transmission and retransmission" --> ceil(log2(numSubchannel_r14) bits, here in integer form.  This is actually configured using "RIV". Here we provide the corresponding subchannel directly. Either set through DCI5A or Higher Layers or Preconfigured.

// recovery processing
std::string decodingType            = "Soft";                // Decoding type for SL-BCH/PSBCH recovery. Pick from 'Soft' or 'Hard' (default : 'Soft')
std::string chanEstMethod           = "LS";                  // Channel estimation method. Currently 'LS' and 'mmse-direct' are fully supported (default : 'LS')
int timeVarFactor                   = 0;                     // Channel time variance factor. Multiple of 1/(sampling rate). Typical values: 0 for static, 50 for highly time-variant (default : 0)

//non changeable parameters
std::string cp_Len_r12           = "Normal";                  // SL-CP-Len: Cyclic Prefix: For V2V only 'Normal' is allowed
int NRBsc                            = 12;                        // resource block size in the frequency domain, expressed as a number of subcarriers
int sci1_TBsize                      = 32;                        // length of SCI Format 1 message (36.212-5.4.3.1.2)
int Msc_PSCCH                        = 24;                        // bandwidth of SL PSCCH in # subcarriers (2 PRBs in one subframe)
int NSLsymb                          = 7;                         // Number of SL symbols per slot, depending on CPconf
int PSCCH_DMRS_symbloc_perSubframe[4]   = {2, 5, 8, 11};               // PSCCH DMRS Symbol locations per subframe for PSCCH
int PSCCH_symbloc_perSubframe[]        = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13}; // PSCCH Symbol locations per subframe for PSCCH
int PSSCH_DMRS_symbloc_perSubframe[]   = {2, 5, 8, 11};               // PSSCH DMRS Symbol locations per subframe for PSCCH
int PSSCH_symbloc_perSubframe[]        = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13}; // PSSCH Symbol locations per subframe for PSCCH
int pscch_c_init                     = 510;                       // 36.211, 9.4.1: initializer for PSCCH scrambling sequence
int acceptable_NPRBsizes[] =                                        // Acceptable sizes for NRB_PSSCH
    {1,2,3,4,5,6,8,9,10,12,15,16,18,20,24,25,27,30,32,36,40,45,48,50,54,60,72,75,80,81,90,96,100};

int mcs_r14_MAX = 11;                                             // Assume for the time being that only QPSK is allowed
// mac parametrization
int slschSubHeader_Len = 6;                                       // Length of SL-SCH Subheader in bytes (36.321, 6.1.6)
int macPDU_Len = 2;                                               // Length of MAC PDU Subheader in bytes (36.321, 6.1.6)
int maxTBSize = 1256;                                             // this is internal: MAX TB Size (due to turbo coding)
int NFFT;
double chanSRate;                                                  //Si no es necesario se borra
