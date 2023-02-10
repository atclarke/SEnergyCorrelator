// 'SEnergyCorrelator.cc'
// Derek Anderson
// 01.20.2023
//
// A module to implement Peter Komiske's
// EEC library in the sPHENIX software
// stack.

#define SENERGYCORRELATOR_CC

// user includes
#include "SEnergyCorrelator.h"
#include "SEnergyCorrelator.io.h"
#include "SEnergyCorrelator.sys.h"

using namespace std;
using namespace fastjet;



// ctor/dtor ------------------------------------------------------------------

//SEnergyCorrelator::SEnergyCorrelator(const string &name, const bool isComplex, const bool doDebug) : SubsysReco(name) {
SEnergyCorrelator::SEnergyCorrelator(const string &name, const bool isComplex, const bool doDebug, const bool inBatch) {

  // initialize internal variables
  InitializeMembers();

  // set standalone/complex mode
  if (isComplex) {
    m_inComplexMode    = true;
    m_inStandaloneMode = false; 
  } else {
    m_inComplexMode    = false;
    m_inStandaloneMode = true;
  }

  // set verbosity in complex mode
/* TODO add back in complex mode
  if (m_inComplexMode) {
    m_verbosity = Verbosity();
  }
*/

  // set debug/batch mode & print debug statement
  m_inDebugMode = doDebug;
  m_inBatchMode = inBatch;
  if (m_inDebugMode) PrintDebug(1);

  // set module name & announce start of calculation
  m_moduleName = name;
  if (m_inStandaloneMode) PrintMessage(0);

}  // end ctor(string, bool, bool)



SEnergyCorrelator::~SEnergyCorrelator() {

  // print debug statement
  if (m_inDebugMode) PrintDebug(14);

  // delete pointers to files
  if (!m_inTree) {
    delete m_inFile;
    delete m_outFile;
  }

  // delete pointers to correlators
  for (size_t iPtBin = 0; iPtBin < m_nBinsJetPt; iPtBin++) {
    delete m_eecLongSide.at(iPtBin);
  }
  m_eecLongSide.clear();
  m_ptJetBins.clear();

}  // end dtor



// F4A methods ----------------------------------------------------------------

/* TODO F4A methods will go here */



// standalone-only methods ----------------------------------------------------

void SEnergyCorrelator::Init() {

  // print debug statement
  if (m_inDebugMode) PrintDebug(10);

  // make sure standalone mode is on & open files
  if (m_inComplexMode) {
    PrintError(5);
    assert(m_inStandaloneMode);
  } else {
    OpenInputFile();
  }
  OpenOutputFile();

  // announce files
  PrintMessage(1);

  // initialize input, output, & correlators
  InitializeTree();
  InitializeHists();
  InitializeCorrs();
  return;

}  // end 'StandaloneInit()'



void SEnergyCorrelator::Analyze() {

  // print debug statement
  if (m_inDebugMode) PrintDebug(12);

  // make sure standalone mode is on
  if (m_inComplexMode) {
    PrintError(8);
    assert(m_inStandaloneMode);
  }

  // load tree and announce start of event loop
  const uint64_t nEvts = 5;
  PrintMessage(7, nEvts);

  /* event loop goes here */
  for (uint64_t iEvt = 0; iEvt < nEvts; iEvt++) {
    PrintMessage(8, nEvts, iEvt);
  }

  // announce end of analysis
  PrintMessage(9);
  return;

}  // end 'StandaloneAnalyze()'



void SEnergyCorrelator::End() {

  // print debug statement
  if (m_inDebugMode) PrintDebug(13);

  // make sure standalone mode is on & save output
  if (m_inComplexMode) {
    PrintError(9);
    assert(m_inStandaloneMode);
  } else {
    SaveOutput();
  }

  // announce end
  PrintMessage(11);
  return;

}  // end 'StandaloneEnd()'

// end ------------------------------------------------------------------------
