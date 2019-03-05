#ifndef LTE_CONSTANTS
#define LTE_CONSTANTS
/* constantes del estandar*/
static const int SFN_MAX = 10240;
const std::string cp_Len_r12 = "Normal"; // SL-CP-Len: Cyclic Prefix: For V2V only 'Normal' is allowed
static const int NRBsc = 12; // resource block size in the frequency domain, expressed as a number of subcarriers
static const int sci1_TBsize = 32; // length of SCI Format 1 message (36.212-5.4.3.1.2)
static const int Msc_PSCCH = 24; // bandwidth of SL PSCCH in # subcarriers (2 PRBs in one subframe)
static const int NSLsymb = 7; // Number of SL symbols per slot,  depending on CPconf
static constexpr int PSCCH_DMRS_symbloc_perSubframe[] = {2, 5, 8, 11}; // PSCCH DMRS Symbol locations per subframe for PSCCH
static constexpr int PSCCH_symbloc_perSubframe[] = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13}; // PSCCH Symbol locations per subframe for PSCCH
static constexpr int PSSCH_DMRS_symbloc_perSubframe[] = {2, 5, 8, 11}; // PSSCH DMRS Symbol locations per subframe for PSCCH
static const int Size_PSSCH_symbloc_perSubframe = 10;
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
#endif
