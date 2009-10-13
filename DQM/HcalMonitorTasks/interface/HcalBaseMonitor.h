#ifndef DQM_HCALMONITORTASKS_HCALBASEMONITOR_H
#define DQM_HCALMONITORTASKS_HCALBASEMONITOR_H

// Define number of eta, phi bins for histogram objects
#define ETABINS 87
#define PHIBINS 72

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCoder.h"

#include "TH1F.h"
#include "TH2F.h"
#include <map>

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "FWCore/Utilities/interface/CPUTimer.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQM/HcalMonitorTasks/interface/HcalEtaPhiHists.h"

#include <iostream>

// Temporary fix:  Add this into base class until I figure why multiple inclusions are a problem -- Jeff, 23 May 2008
#include "CalibFormats/HcalObjects/interface/HcalCalibrationWidths.h"

using namespace std;
/** \class HcalBaseMonitor
  *  
  * $Date: 2009/08/19 19:27:43 $
  * $Revision: 1.32 $
  * \author W. Fisher - FNAL
  */
class HcalBaseMonitor {
public:
  HcalBaseMonitor(); 
  virtual ~HcalBaseMonitor(); 

  virtual void setup(const edm::ParameterSet& ps, DQMStore* dbe);
  virtual void done();
  virtual void clearME();
  virtual void periodicReset();
  

  void setVerbosity(int verb) { fVerbosity = verb; }
  int getVerbosity() const { return fVerbosity; }
  
  void setDiagnostics(bool myval) { makeDiagnostics=myval;}
  bool getDiagnostics() const { return makeDiagnostics;}

  bool vetoCell(HcalDetId& id);
  void hideKnownBadCells();
  
  // Set up vectors of Monitors for individual depths
  // 2-D histograms with eta-phi binning assumed
  void setupDepthHists2D(MonitorElement* &h, std::vector<MonitorElement*> &hh, std::string Name, std::string Units);
  void setupDepthHists2D(std::vector<MonitorElement*> &hh, std::string Name, std::string Units);
  void SetupEtaPhiHists(MonitorElement* &h, EtaPhiHists& hh, std::string Name, std::string Units);
  void SetupEtaPhiHists(EtaPhiHists &hh, std::string Name, std::string Units);


  // Generic 2-D histograms
  void setupDepthHists2D(MonitorElement* &h, std::vector<MonitorElement*> &hh, std::string Name, std::string Units,
			 int nbinsx, int lowboundx, int highboundx,
			 int nbinsy, int lowboundy, int highboundy);
  
  void setupDepthHists2D(std::vector<MonitorElement*> &hh, std::string Name, std::string Units,
			 int nbinsx, int lowboundx, int highboundx,
			 int nbinsy, int lowboundy, int highboundy);

  void setMinMaxHists2D(std::vector<MonitorElement*> &hh, double min, double max);

  // 1-D histograms
  void setupDepthHists1D(MonitorElement* &h, std::vector<MonitorElement*> &hh, std::string Name, std::string Units, int lowbound, int highbound, int Nbins);
  void setupDepthHists1D(std::vector<MonitorElement*> &hh, std::string Name, std::string Units, int lowbound, int highbound, int Nbins);
  void setMinMaxHists1D(std::vector<MonitorElement*> &hh, double min, double max);

  void processEvent();
  void LumiBlockUpdate(int lb);

protected:
  
  int fVerbosity;
  bool showTiming; // controls whether to show timing diagnostic info
  bool dump2database; // controls whether output written to file for database (will eventually write db directly)
  int checkNevents_; // controls when histograms should be updated

  double etaMax_, etaMin_;
  double phiMax_, phiMin_;
  int etaBins_, phiBins_;
  double minErrorFlag_;

  bool checkHB_, checkHE_, checkHO_, checkHF_, checkZDC_;
  int resetNevents_;
  int Nlumiblocks_;

  edm::CPUTimer cpu_timer; // 
    
  bool makeDiagnostics; // controls whether to make diagnostic plots
  bool fillUnphysical_; // controls whether to fill unphysical iphi bins in eta-phi histograms
  
  DQMStore* m_dbe;
  bool Online_; // tracks whether code is run online or offline 
  vector<string> badCells_; // keeps list of bad cells that should be ignored
  string rootFolder_;
  string baseFolder_;

  //static const int binmapd2[];
  //static const int binmapd3[];

  MonitorElement* ProblemCells;
  EtaPhiHists ProblemCellsByDepth;

  int ievt_;
  int tevt_; // total # of events
  MonitorElement* meEVT_;
  MonitorElement* meTOTALEVT_;
  int lumiblock;
  int NumBadHB, NumBadHE, NumBadHO, NumBadHF, NumBadZDC;
  
  MonitorElement* ProblemsVsLB;
  MonitorElement *ProblemsVsLB_HB, *ProblemsVsLB_HE, *ProblemsVsLB_HO, *ProblemsVsLB_HF, *ProblemsVsLB_ZDC;

};

#endif
