// ----------------------------------------------------------------------------
// 'SEnergyCorrelator.h'
// Derek Anderson
// 01.20.2023
//
// A module to implement Peter Komiske's
// EEC library in the sPHENIX software
// stack.
// ----------------------------------------------------------------------------

#ifndef SENERGYCORRELATOR_H
#define SENERGYCORRELATOR_H

// standard c includes
#include <cmath>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <utility>
// root includes
#include "TH1.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TString.h"
#include "TVector3.h"  // TODO update to XYZvector
#include "TDirectory.h"
// f4a include
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllHistoManager.h>
// phool includes
#include <phool/phool.h>
#include <phool/getClass.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHCompositeNode.h>
// fastjet includes
#include <fastjet/PseudoJet.hh>
// eec include
#include "/sphenix/user/danderson/eec/EnergyEnergyCorrelators/eec/include/EECLongestSide.hh"

using namespace std;
using namespace fastjet;



// SEnergyCorrelator definition -----------------------------------------------

class SEnergyCorrelator : public SubsysReco {

  public:

    // ctor/dtor
    SEnergyCorrelator(const string &name = "SEnergyCorrelator", const bool isComplex = false, const bool doDebug = false, const bool inBatch = false);
    ~SEnergyCorrelator() override;

    // F4A methods
    int Init(PHCompositeNode*)          override;
    int process_event(PHCompositeNode*) override;
    int End(PHCompositeNode*)           override;

    // standalone-only methods
    void Init();
    void Analyze();
    void End();

    // setters (inline)
    void SetVerbosity(const int verb)           {m_verbosity   = verb;}
    void SetInputNode(const string &iNodeName)  {m_inNodeName  = iNodeName;}
    void SetInputFile(const string &iFileName)  {m_inFileName  = iFileName;}
    void SetOutputFile(const string &oFileName) {m_outFileName = oFileName;}

    // setters (*.io.h)
    void SetInputTree(const string &iTreeName, const bool isTruthTree = false, const bool isEmbedTree = false);
    void SetJetParameters(const vector<pair<double, double>> &pTjetBins, const double minEta, const double maxEta);
    void SetConstituentParameters(const double minMom, const double maxMom, const double minDr, const double maxDr, const bool applyCstCuts = false);
    void SetCorrelatorParameters(const uint32_t nPointCorr, const uint64_t nBinsDr, const double minDr, const double maxDr);
    void SetSubEventsToUse(const uint16_t subEvtOpt = 0, const vector<int> vecSubEvtsToUse = {});

    // system getters
    int      GetVerbosity()        {return m_verbosity;}
    bool     GetInDebugMode()      {return m_inDebugMode;}
    bool     GetInBatchMode()      {return m_inBatchMode;}
    bool     GetInComplexMode()    {return m_inComplexMode;}
    bool     GetInStandaloneMode() {return m_inStandaloneMode;}
    bool     GetIsInputTreeTruth() {return m_isInputTreeTruth;}
    bool     GetIsInputTreeEmbed() {return m_isInputTreeEmbed;}
    bool     GetApplyCstCuts()     {return m_applyCstCuts;}
    bool     GetSelectSubEvts()    {return m_selectSubEvts;}
    string   GetModuleName()       {return m_moduleName;}
    string   GetInputFileName()    {return m_inFileName;}
    string   GetInputNodeName()    {return m_inNodeName;}
    string   GetInputTreeName()    {return m_inTreeName;}
    string   GetOutputFileName()   {return m_outFileName;}
    uint16_t GetSubEvtOpt()        {return m_subEvtOpt;}

    // correlator getters
    double   GetMinDrBin()   {return m_drBinRange[0];}
    double   GetMaxDrBin()   {return m_drBinRange[1];}
    double   GetMinJetPt()   {return m_ptJetRange[0];}
    double   GetMaxJetPt()   {return m_ptJetRange[1];}
    double   GetMinJetEta()  {return m_etaJetRange[0];}
    double   GetMaxJetEta()  {return m_etaJetRange[1];}
    double   GetMinCstMom()  {return m_momCstRange[0];}
    double   GetMaxCstMom()  {return m_momCstRange[1];}
    double   GetMinCstDr()   {return m_drCstRange[0];}
    double   GetMaxCstDr()   {return m_drCstRange[1];}
    size_t   GetNBinsJetPt() {return m_nBinsJetPt;}
    uint32_t GetNPointCorr() {return m_nPointCorr;}
    uint64_t GetNBinsDr()    {return m_nBinsDr;}
    

  private:

    // constants
    enum CONST {
      NRANGE   = 2,
      NPARTONS = 2
    };

    // io methods (*.io.h)
    void GrabInputNode();
    void OpenInputFile();
    void OpenOutputFile();
    void SaveOutput();
    void CloseInputFile();
    void CloseOutputFile();

    // system methods (*.sys.h)
    void    InitializeMembers();
    void    InitializeHists();
    void    InitializeCorrs();
    void    InitializeTree();
    void    PrintMessage(const uint32_t code, const uint64_t nEvts = 0, const uint64_t event = 0);
    void    PrintDebug(const uint32_t code);
    void    PrintError(const uint32_t code, const size_t nDrBinEdges = 0, const size_t iDrBin = 0);
    bool    CheckCriticalParameters();
    int64_t LoadTree(const uint64_t entry);
    int64_t GetEntry(const uint64_t entry);

    // analysis methods (*.ana.h)
    void     DoCorrelatorCalculation();
    void     ExtractHistsFromCorr();
    bool     ApplyJetCuts(const double ptJet, const double etaJet);
    bool     ApplyCstCuts(const double momCst, const double drCst);
    bool     CheckIfSubEvtGood(const int embedID);
    uint32_t GetJetPtBin(const double ptJet);

    // io members
    TFile*        m_outFile = NULL;
    TFile*        m_inFile  = NULL;
    TTree*        m_inTree  = NULL;
    vector<TH1D*> m_outHistDrAxis;
    vector<TH1D*> m_outHistLnDrAxis;

    // system members
    int      m_fCurrent         = 0;
    int      m_verbosity        = 0;
    bool     m_inDebugMode      = false;
    bool     m_inBatchMode      = false;
    bool     m_inComplexMode    = false;
    bool     m_inStandaloneMode = false;
    bool     m_isInputTreeTruth = false;
    bool     m_isInputTreeEmbed = false;
    bool     m_applyCstCuts     = false;
    bool     m_selectSubEvts    = false;
    string   m_moduleName       = "";
    string   m_inFileName       = "";
    string   m_inNodeName       = "";
    string   m_inTreeName       = "";
    string   m_outFileName      = "";
    uint16_t m_subEvtOpt        = 0;

    // jet, cst, correlator parameters
    uint32_t                     m_nPointCorr                 = 0;
    uint64_t                     m_nBinsDr                    = 0;
    size_t                       m_nBinsJetPt                 = 0;
    double                       m_drBinRange[CONST::NRANGE]  = {-999., -999.};
    double                       m_ptJetRange[CONST::NRANGE]  = {-999., -999.};
    double                       m_etaJetRange[CONST::NRANGE] = {-999., -999.};
    double                       m_momCstRange[CONST::NRANGE] = {-999., -999.};
    double                       m_drCstRange[CONST::NRANGE]  = {-999., -999.};
    vector<int>                  m_subEvtsToUse;
    vector<PseudoJet>            m_jetCstVector;
    vector<pair<double, double>> m_ptJetBins;

    // correlators
    vector<contrib::eec::EECLongestSide<contrib::eec::hist::axis::log>*> m_eecLongSide;

    // input truth tree address members
    int                  m_evtNumChrgPars              = -999;
    int                  m_partonID[CONST::NPARTONS]   = {-999,  -999};
    double               m_partonMomX[CONST::NPARTONS] = {-999., -999.};
    double               m_partonMomY[CONST::NPARTONS] = {-999., -999.};
    double               m_partonMomZ[CONST::NPARTONS] = {-999., -999.};
    double               m_evtSumPar                   = -999.;
    vector<vector<int>>* m_cstID                       = NULL;
    vector<vector<int>>* m_cstEmbedID                  = NULL;
    // input reco. tree address members
    int                  m_evtNumTrks = -999;
    double               m_evtSumECal = -999.;
    double               m_evtSumHCal = -999.;
    vector<vector<int>>* m_cstMatchID = NULL;

    // generic input tree address members
    int                     m_evtNumJets = -999;
    double                  m_evtVtxX    = -999.;
    double                  m_evtVtxY    = -999.;
    double                  m_evtVtxZ    = -999.;
    vector<unsigned long>*  m_jetNumCst  = NULL;
    vector<unsigned int>*   m_jetID      = NULL;
    vector<unsigned int>*   m_jetTruthID = NULL;
    vector<double>*         m_jetEnergy  = NULL;
    vector<double>*         m_jetPt      = NULL;
    vector<double>*         m_jetEta     = NULL;
    vector<double>*         m_jetPhi     = NULL;
    vector<double>*         m_jetArea    = NULL;
    vector<vector<double>>* m_cstZ       = NULL;
    vector<vector<double>>* m_cstDr      = NULL;
    vector<vector<double>>* m_cstEnergy  = NULL;
    vector<vector<double>>* m_cstJt      = NULL;
    vector<vector<double>>* m_cstEta     = NULL;
    vector<vector<double>>* m_cstPhi     = NULL;

    // input truth tree branch members
    TBranch* m_brPartonID[CONST::NPARTONS]   = {NULL, NULL};
    TBranch* m_brPartonMomX[CONST::NPARTONS] = {NULL, NULL};
    TBranch* m_brPartonMomY[CONST::NPARTONS] = {NULL, NULL};
    TBranch* m_brPartonMomZ[CONST::NPARTONS] = {NULL, NULL};
    TBranch* m_brEvtSumPar                   = NULL;
    TBranch* m_brCstID                       = NULL;
    TBranch* m_brCstEmbedID                  = NULL;
    // input reco. tree branch members
    TBranch* m_brEvtNumTrks = NULL;
    TBranch* m_brEvtSumECal = NULL;
    TBranch* m_brEvtSumHCal = NULL;
    TBranch* m_brCstMatchID = NULL;

    // generic input tree branch members
    TBranch* m_brEvtNumJets = NULL;
    TBranch* m_brEvtVtxX    = NULL;
    TBranch* m_brEvtVtxY    = NULL;
    TBranch* m_brEvtVtxZ    = NULL;
    TBranch* m_brJetNumCst  = NULL;
    TBranch* m_brJetID      = NULL;
    TBranch* m_brJetEnergy  = NULL;
    TBranch* m_brJetPt      = NULL;
    TBranch* m_brJetEta     = NULL;
    TBranch* m_brJetPhi     = NULL;
    TBranch* m_brJetArea    = NULL;
    TBranch* m_brCstZ       = NULL;
    TBranch* m_brCstDr      = NULL;
    TBranch* m_brCstEnergy  = NULL;
    TBranch* m_brCstJt      = NULL;
    TBranch* m_brCstEta     = NULL;
    TBranch* m_brCstPhi     = NULL;

};

#endif

// end ------------------------------------------------------------------------
