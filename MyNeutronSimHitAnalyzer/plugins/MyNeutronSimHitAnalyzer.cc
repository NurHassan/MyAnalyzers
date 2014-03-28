// -*- C++ -*-
//
// Package:    MyNeutronSimHitAnalyzer
// Class:      MyNeutronSimHitAnalyzer
// 
/**\class MyNeutronSimHitAnalyzer MyNeutronSimHitAnalyzer.cc MyAnalyzers/MyNeutronSimHitAnalyzer/src/MyNeutronSimHitAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Piet Verwilligen,161 R-006,+41227676292,
//         Created:  Wed Oct 10 17:36:38 CEST 2012
// $Id$
//
//


// system include files
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>


// root include files
#include <TRandom.h>
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "THStack.h"
#include "TLegend.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TDirectoryFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"



// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// why is this one not included by default
#include "FWCore/Framework/interface/ESHandle.h"


// Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"
#include <Geometry/RPCGeometry/interface/RPCRoll.h>
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/GeometrySurface/interface/Surface.h"
#include <DataFormats/GeometrySurface/interface/LocalError.h>
#include <DataFormats/GeometryVector/interface/LocalPoint.h>
#include <DataFormats/GeometryVector/interface/LocalPoint.h>
#include "DataFormats/GeometrySurface/interface/Surface.h"
#include <DataFormats/GeometrySurface/interface/LocalError.h>


#include "DataFormats/Common/interface/Handle.h"
// Simulation
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FastSimulation/Tracking/test/FastTrackAnalyzer.h"
// DetIds
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include "DataFormats/MuonDetId/interface/DTWireId.h"
// Digis
#include "DataFormats/RPCDigi/interface/RPCDigi.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"
// RecHits
#include <DataFormats/RPCRecHit/interface/RPCRecHit.h>
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"
// Candidates
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
// Tracking
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
// Track
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "DataFormats/Math/interface/LorentzVectorFwd.h"



//
// class declaration
//

class MyNeutronSimHitAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MyNeutronSimHitAnalyzer(const edm::ParameterSet&);
      ~MyNeutronSimHitAnalyzer();
  edm::ESHandle <RPCGeometry> rpcGeom;
  edm::ESHandle <CSCGeometry> cscGeom;
  edm::ESHandle <DTGeometry>  dtGeom;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  double getLumi(int, int, int);
  double getPU(double, int, int);

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
  std::string pdfFileNameBase, pdfFileName, rootFileName;
  double bunchspacing;
  double comenergy;
  bool debug;
  TFile * outputfile;


  TDirectoryFile * TDir_Muon_hits_deposits;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TH2F * RPCb_el_hits, * RPCb_mu_hits, * RPCb_pi_hits, * RPCb_ka_hits, * RPCb_p_hits, * RPCb_n_hits, * RPCb_g_hits, * RPCb_N_hits;
  TH2F * RPCf_el_hits, * RPCf_mu_hits, * RPCf_pi_hits, * RPCf_ka_hits, * RPCf_p_hits, * RPCf_n_hits, * RPCf_g_hits, * RPCf_N_hits;
  TH2F * CSC_el_hits,  * CSC_mu_hits,  * CSC_pi_hits,  * CSC_ka_hits,  * CSC_p_hits,  * CSC_n_hits,  * CSC_g_hits,  * CSC_N_hits;
  TH2F * DT_el_hits,   * DT_mu_hits,   * DT_pi_hits,   * DT_ka_hits,   * DT_p_hits,   * DT_n_hits,   * DT_g_hits,   * DT_N_hits;

  TH2F * RPCb_el_deposits, * RPCb_mu_deposits, * RPCb_pi_deposits, * RPCb_ka_deposits, * RPCb_p_deposits, * RPCb_n_deposits, * RPCb_g_deposits, * RPCb_N_deposits;
  TH2F * RPCf_el_deposits, * RPCf_mu_deposits, * RPCf_pi_deposits, * RPCf_ka_deposits, * RPCf_p_deposits, * RPCf_n_deposits, * RPCf_g_deposits, * RPCf_N_deposits;
  TH2F * CSC_el_deposits,  * CSC_mu_deposits,  * CSC_pi_deposits,  * CSC_ka_deposits,  * CSC_p_deposits,  * CSC_n_deposits,  * CSC_g_deposits,  * CSC_N_deposits;
  TH2F * DT_el_deposits,   * DT_mu_deposits,   * DT_pi_deposits,   * DT_ka_deposits,   * DT_p_deposits,   * DT_n_deposits,   * DT_g_deposits,   * DT_N_deposits;

  TH1F * RPCb_el_deps, * RPCb_mu_deps, * RPCb_ha_deps;
  TH1F * RPCf_el_deps, * RPCf_mu_deps, * RPCf_ha_deps;
  TH1F * CSC_el_deps,  * CSC_mu_deps,  * CSC_ha_deps;
  TH1F * DT_el_deps,   * DT_mu_deps,   * DT_ha_deps;

  TH1F * RPCb_hits_tof, * RPCf_hits_tof, * CSC_hits_tof, * DT_hits_tof, * RB4_hits_tof, * RE4_hits_tof, * ME4_hits_tof, * MB4_hits_tof;
  TH1F * RPCb_hits_eta, * RPCf_hits_eta, * CSC_hits_eta, * DT_hits_eta;
  TH1F * RPCb_hits_phi, * RPCf_hits_phi, * CSC_hits_phi, * DT_hits_phi, * RB4_hits_phi, * RE4_hits_phi, * ME4_hits_phi, * MB4_hits_phi;
  TH1F * RPCb_hits_lin, * RPCf_hits_lin, * CSC_hits_lin, * DT_hits_lin;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TCanvas * Canvas_RPCb_hits,     * Canvas_RPCf_hits,     * Canvas_CSC_hits,     * Canvas_DT_hits;
  TCanvas * Canvas_RPCb_deposits, * Canvas_RPCf_deposits, * Canvas_CSC_deposits, * Canvas_DT_deposits;
  TCanvas * Canvas_RPCb_1D_deps,  * Canvas_RPCf_1D_deps,  * Canvas_CSC_1D_deps,  * Canvas_DT_1D_deps;


  TDirectoryFile * TDir_Muon_XY_RZ_views;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TH2F    * RPCb_XY, * RPCb_RZ, * RPCf_XY, * RPCf_RZ, *CSC_XY, * CSC_RZ, * DT_XY, * DT_RZ, * Muon_RZ;
  TH2F    * RPCb_000ns_XY, * RPCb_000ns_RZ, * RPCf_000ns_XY, * RPCf_000ns_RZ, *CSC_000ns_XY, * CSC_000ns_RZ, * DT_000ns_XY, * DT_000ns_RZ, * Muon_000ns_RZ;
  TH2F    * RPCb_250ns_XY, * RPCb_250ns_RZ, * RPCf_250ns_XY, * RPCf_250ns_RZ, *CSC_250ns_XY, * CSC_250ns_RZ, * DT_250ns_XY, * DT_250ns_RZ, * Muon_250ns_RZ;

  TH2F * Muon_000ns_el_RZ, * Muon_000ns_mu_RZ, * Muon_000ns_ha_RZ; // hits with 000 < tof < 250 ns
  TH2F * Muon_250ns_el_RZ, * Muon_250ns_mu_RZ, * Muon_250ns_ha_RZ; // hits with 250 < tof < 10^8 ns

  TH2F * Muon_00ns_el_RZ, * Muon_00ns_mu_RZ, * Muon_00ns_ha_RZ; // hits with 00 < tof < 050 ns
  TH2F * Muon_50ns_el_RZ, * Muon_50ns_mu_RZ, * Muon_50ns_ha_RZ; // hirs with 50 < tof < 250 ns

  TH2F * SimVertices_RZ, * SimVertices_Muon_RZ;
  TH1F * PrimVertices_Z, * PrimVertices_R;

  TH1F * RPC_hits, * RPC_area, * RPC_rates;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TCanvas * Canvas_Muon_RZ, * Canvas_Muon_000ns_RZ, * Canvas_Muon_250ns_RZ, * Canvas_Muon_000ns_Cont_RZ, * Canvas_Muon_250ns_Cont_RZ; 
  TCanvas                   * Canvas_Muon_00ns_RZ,  * Canvas_Muon_50ns_RZ,  * Canvas_Muon_00ns_Cont_RZ,  * Canvas_Muon_50ns_Cont_RZ; 
  TCanvas * Canvas_SimVertices_RZ, * Canvas_SimVertices_Muon_RZ;


  TDirectoryFile * TDir_Muon_hit_info;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TH1F * RPCb_Muons_SHPT,           * RPCf_Muons_SHPT,           * CSC_Muons_SHPT,           *DT_Muons_SHPT;
  TH1F * RPCb_Hadrons_SHPT,         * RPCf_Hadrons_SHPT,         * CSC_Hadrons_SHPT,         *DT_Hadrons_SHPT;
  TH1F * RPCb_Electrons_SHPT,       * RPCf_Electrons_SHPT,       * CSC_Electrons_SHPT,       *DT_Electrons_SHPT;
  TH1F * RPCb_Electrons_000ns_SHPT, * RPCf_Electrons_000ns_SHPT, * CSC_Electrons_000ns_SHPT, *DT_Electrons_000ns_SHPT;
  TH1F * RPCb_Electrons_050ns_SHPT, * RPCf_Electrons_050ns_SHPT, * CSC_Electrons_050ns_SHPT, *DT_Electrons_050ns_SHPT;
  TH1F * RPCb_Electrons_250ns_SHPT, * RPCf_Electrons_250ns_SHPT, * CSC_Electrons_250ns_SHPT, *DT_Electrons_250ns_SHPT;
  // TH1F * RPCb_Electrons_SVPT,       * RPCf_Electrons_SVPT,       * CSC_Electrons_SVPT,       *DT_Electrons_SVPT;
  // hits per chamber (HPC)
  TH1F * RPCb_HPC, * RPCb_el_HPC;
  TH1F * RPCf_HPC, * RPCf_el_HPC;
  TH1F * CSC_HPC,  * CSC_el_HPC;
  TH1F * DT_HPC,   * DT_el_HPC;
  // hits per layer (HPL)
  TH1F * RPCb_el_HPL;  TH1F * RPCb_mu_HPL; TH1F * RPCb_ha_HPL;
  TH1F * RPCf_el_HPL;  TH1F * RPCf_mu_HPL; TH1F * RPCf_ha_HPL;
  TH1F * CSC_el_HPL;   TH1F * CSC_mu_HPL;  TH1F * CSC_ha_HPL;
  TH1F * DT_el_HPL;    TH1F * DT_mu_HPL;   TH1F * DT_ha_HPL;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TCanvas * Canvas_RPCb_Layers, * Canvas_RPCf_Layers, * Canvas_CSC_Layers, * Canvas_DT_Layers; 

  TDirectoryFile * TDir_Muon_rates;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TGraphErrors * gr_RPC_Rates_All, * gr_RPC_Rates_Barrel, * gr_RPC_Rates_Endcap;
  TGraphErrors * gr_RPC_Rates_RE12, * gr_RPC_Rates_RE13, * gr_RPC_Rates_RE22, * gr_RPC_Rates_RE23, * gr_RPC_Rates_RE32, * gr_RPC_Rates_RE33, * gr_RPC_Rates_RE42, * gr_RPC_Rates_RE43;
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------- 
  TCanvas * Canvas_RPC_Rates, * Canvas_RPC_Rates_RE1, * Canvas_RPC_Rates_RE2, * Canvas_RPC_Rates_RE3, * Canvas_RPC_Rates_RE4;
  int    RPC_hits_array[2][4][3];
  double RPC_area_array[2][4][3], RPC_rates_array[2][4][3], InstLumi[50], RPC_rates_Summary[15][50], RPC_uncer_Rate[15][50], RPC_uncer_Lumi[15][50]; 
  double rpc_barrel_area, rpc_endcap_area = 0.0;



};

//
// constants, enums and typedefs
//

int n_tof = 700,  n1_tof = 1,  n2_tof = 8;
int m_tof = 70,   m1_tof = 1,  m2_tof = 8;
int m_eta = 50;   double m1_eta =  0.0,  m2_eta = 2.5;
int m_phi = 63;   double m1_phi = -3.15, m2_phi = 3.15;
int m_lin = 1000, m1_lin = 0,  m2_lin = 100000000;
int n_hits = 20;  double n1_hits = 0.5, n2_hits = 20.5;
int n_lay  = 12;  double n1_lay  = 0.5, n2_lay  = 12.5;

int n_D   = 900,  n1_D   = -3, n2_D   = 6;
int n_E   = 900,  n1_E   = -4, n2_E   = 5;
int n_F   = 90,   n1_F   = -3, n2_F   = 6;

int n_xy_x =  800; double n_xy_x1 = -800;  double n_xy_x2 = +800;
int n_xy_y =  800; double n_xy_y1 = -800;  double n_xy_y2 = +800;
int n_zr_z =  550; double n_zr_z1 =    0;  double n_zr_z2 = 1100;
int n_zr_r =  400; double n_zr_r1 =    0;  double n_zr_r2 =  800;

int n_zrc_z = 1350; double n_zrc_z1 = 0; double n_zrc_z2 = 2700;
int n_zrc_r =  650; double n_zrc_r1 = 0; double n_zrc_r2 = 1300;

int n_pv_z = 50, n1_pv_z = -25, n2_pv_z = 25;
int n_pv_r = 10, n1_pv_r = 0,   n2_pv_r = 2;
int n_cat  = 27; double n1_cat  = 0.5, n2_cat  = 27.5;
int n_pro  = 15; double n1_pro  = 0.5, n2_pro  = 15.5;

std::string allcat[27] = {"All", "Barrel", "W0_RB1", "W0_RB2", "W0_RB3", "W0_RB4", "W1_RB1", "W1_RB2", "W1_RB3", "W1_RB4", "W2_RB1", "W2_RB2", "W2_RB3", "W2_RB4", 
                                 "Endcap", "RE11", "RE12", "RE13", "RE21", "RE22", "RE23", "RE31", "RE32", "RE33", "RE41", "RE42", "RE43"};
std::string endcat[24] = {"RE12C", "RE12B", "RE12A", "RE13C", "RE13B", "RE13A", "RE22C", "RE22B", "RE22A", "RE23C", "RE23B", "RE23A", 
                          "RE32C", "RE32B", "RE32A", "RE33C", "RE33B", "RE33A", "RE42C", "RE42B", "RE42A", "RE43C", "RE43B", "RE43A"};
std::string proc[15] = {"notDefined", "fCoulombScattering", "fIonisation", "fBremsstrahlung", "fPairProdByCharged", "fAnnihilation", "fAnnihilationToMuMu", "fAnnihilationToHadrons", 
                        "fNuclearStopping", "notDefined", "fMultipleScattering", "fRayleigh", "fPhotoElectricEffect", "fComptonScattering", "fGammaConversion"};
//
// static data member definitions
//

//
// constructors and destructor
//
MyNeutronSimHitAnalyzer::MyNeutronSimHitAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  pdfFileNameBase = iConfig.getUntrackedParameter<std::string>("PdfFileNameBase");
  rootFileName    = iConfig.getUntrackedParameter<std::string>("RootFileName");
  bunchspacing    = iConfig.getUntrackedParameter<double>("BunchSpacing");
  comenergy       = iConfig.getUntrackedParameter<double>("COMEnergy");
  debug           = iConfig.getUntrackedParameter<bool>("Debug");

  outputfile      = new TFile(rootFileName.c_str(), "RECREATE" );
  
  TDir_Muon_hits_deposits = new TDirectoryFile("Muon_hits_deposits", "Muon_hits_deposits");
  TDir_Muon_XY_RZ_views   = new TDirectoryFile("Muon_XY_RZ_views",   "Muon_XY_RZ_views");
  TDir_Muon_hit_info      = new TDirectoryFile("Muon_hit_info",      "Muon_hit_info");
  TDir_Muon_rates         = new TDirectoryFile("Muon_rates",         "Muon_rates");

  // Simhit Time vs Ekin
  RPCb_el_hits = new TH2F("RPCb_el_hits", "Simhit time vs E_{kin} :: RPCb :: Electrons", n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_mu_hits = new TH2F("RPCb_mu_hits", "Simhit time vs E_{kin} :: RPCb :: Muons",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_pi_hits = new TH2F("RPCb_pi_hits", "Simhit time vs E_{kin} :: RPCb :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_ka_hits = new TH2F("RPCb_ka_hits", "Simhit time vs E_{kin} :: RPCb :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_p_hits  = new TH2F("RPCb_p_hits",  "Simhit time vs E_{kin} :: RPCb :: Protons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_n_hits  = new TH2F("RPCb_n_hits",  "Simhit time vs E_{kin} :: RPCb :: Neutrons",  n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_g_hits  = new TH2F("RPCb_g_hits",  "Simhit time vs E_{kin} :: RPCb :: Photons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCb_N_hits  = new TH2F("RPCb_N_hits",  "Simhit time vs E_{kin} :: RPCb :: Nuclei",    n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);

  RPCf_el_hits = new TH2F("RPCf_el_hits", "Simhit time vs E_{kin} :: RPCf :: Electrons", n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_mu_hits = new TH2F("RPCf_mu_hits", "Simhit time vs E_{kin} :: RPCf :: Muons",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_pi_hits = new TH2F("RPCf_pi_hits", "Simhit time vs E_{kin} :: RPCf :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_ka_hits = new TH2F("RPCf_ka_hits", "Simhit time vs E_{kin} :: RPCf :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_p_hits  = new TH2F("RPCf_p_hits",  "Simhit time vs E_{kin} :: RPCf :: Protons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_n_hits  = new TH2F("RPCf_n_hits",  "Simhit time vs E_{kin} :: RPCf :: Neutrons",  n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_g_hits  = new TH2F("RPCf_g_hits",  "Simhit time vs E_{kin} :: RPCf :: Photons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  RPCf_N_hits  = new TH2F("RPCf_N_hits",  "Simhit time vs E_{kin} :: RPCf :: Nuclei",    n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);

  CSC_el_hits = new TH2F("CSC_el_hits", "Simhit time vs E_{kin} :: CSC :: Electrons", n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_mu_hits = new TH2F("CSC_mu_hits", "Simhit time vs E_{kin} :: CSC :: Muons",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_pi_hits = new TH2F("CSC_pi_hits", "Simhit time vs E_{kin} :: CSC :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_ka_hits = new TH2F("CSC_ka_hits", "Simhit time vs E_{kin} :: CSC :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_p_hits  = new TH2F("CSC_p_hits",  "Simhit time vs E_{kin} :: CSC :: Protons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_n_hits  = new TH2F("CSC_n_hits",  "Simhit time vs E_{kin} :: CSC :: Neutrons",  n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_g_hits  = new TH2F("CSC_g_hits",  "Simhit time vs E_{kin} :: CSC :: Photons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  CSC_N_hits  = new TH2F("CSC_N_hits",  "Simhit time vs E_{kin} :: CSC :: Nuclei",    n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);

  DT_el_hits = new TH2F("DT_el_hits", "Simhit time vs E_{kin} :: DT :: Electrons", n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  DT_mu_hits = new TH2F("DT_mu_hits", "Simhit time vs E_{kin} :: DT :: Muons",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  DT_pi_hits = new TH2F("DT_pi_hits", "Simhit time vs E_{kin} :: DT :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  DT_ka_hits = new TH2F("DT_ka_hits", "Simhit time vs E_{kin} :: DT :: Pions",     n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  DT_p_hits  = new TH2F("DT_p_hits",  "Simhit time vs E_{kin} :: DT :: Protons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);\
  DT_n_hits  = new TH2F("DT_n_hits",  "Simhit time vs E_{kin} :: DT :: Neutrons",  n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  DT_g_hits  = new TH2F("DT_g_hits",  "Simhit time vs E_{kin} :: DT :: Photons",   n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);
  DT_N_hits  = new TH2F("DT_N_hits",  "Simhit time vs E_{kin} :: DT :: Nuclei",    n_E, n1_E, n2_E, n_tof, n1_tof, n2_tof);

  // Simhit Time vs E deposit
  RPCb_el_deposits = new TH2F("RPCb_el_deposits", "Simhit time vs E_{deposit} :: RPCb :: Electrons", n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_mu_deposits = new TH2F("RPCb_mu_deposits", "Simhit time vs E_{deposit} :: RPCb :: Muons",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_pi_deposits = new TH2F("RPCb_pi_deposits", "Simhit time vs E_{deposit} :: RPCb :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_ka_deposits = new TH2F("RPCb_ka_deposits", "Simhit time vs E_{deposit} :: RPCb :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_p_deposits  = new TH2F("RPCb_p_deposits",  "Simhit time vs E_{deposit} :: RPCb :: Protons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_n_deposits  = new TH2F("RPCb_n_deposits",  "Simhit time vs E_{deposit} :: RPCb :: Neutrons",  n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_g_deposits  = new TH2F("RPCb_g_deposits",  "Simhit time vs E_{deposit} :: RPCb :: Photons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCb_N_deposits  = new TH2F("RPCb_N_deposits",  "Simhit time vs E_{deposit} :: RPCb :: Nuclei",    n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);

  RPCf_el_deposits = new TH2F("RPCf_el_deposits", "Simhit time vs E_{deposit} :: RPCf :: Electrons", n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_mu_deposits = new TH2F("RPCf_mu_deposits", "Simhit time vs E_{deposit} :: RPCf :: Muons",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_pi_deposits = new TH2F("RPCf_pi_deposits", "Simhit time vs E_{deposit} :: RPCf :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_ka_deposits = new TH2F("RPCf_ka_deposits", "Simhit time vs E_{deposit} :: RPCf :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_p_deposits  = new TH2F("RPCf_p_deposits",  "Simhit time vs E_{deposit} :: RPCf :: Protons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_n_deposits  = new TH2F("RPCf_n_deposits",  "Simhit time vs E_{deposit} :: RPCf :: Neutrons",  n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_g_deposits  = new TH2F("RPCf_g_deposits",  "Simhit time vs E_{deposit} :: RPCf :: Photons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  RPCf_N_deposits  = new TH2F("RPCf_N_deposits",  "Simhit time vs E_{deposit} :: RPCf :: Nuclei",    n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);

  CSC_el_deposits = new TH2F("CSC_el_deposits", "Simhit time vs E_{deposit} :: CSC :: Electrons", n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_mu_deposits = new TH2F("CSC_mu_deposits", "Simhit time vs E_{deposit} :: CSC :: Muons",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_pi_deposits = new TH2F("CSC_pi_deposits", "Simhit time vs E_{deposit} :: CSC :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_ka_deposits = new TH2F("CSC_ka_deposits", "Simhit time vs E_{deposit} :: CSC :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_p_deposits  = new TH2F("CSC_p_deposits",  "Simhit time vs E_{deposit} :: CSC :: Protons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_n_deposits  = new TH2F("CSC_n_deposits",  "Simhit time vs E_{deposit} :: CSC :: Neutrons",  n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_g_deposits  = new TH2F("CSC_g_deposits",  "Simhit time vs E_{deposit} :: CSC :: Photons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  CSC_N_deposits  = new TH2F("CSC_N_deposits",  "Simhit time vs E_{deposit} :: CSC :: Nuclei",    n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);

  DT_el_deposits = new TH2F("DT_el_deposits", "Simhit time vs E_{deposit} :: DT :: Electrons", n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_mu_deposits = new TH2F("DT_mu_deposits", "Simhit time vs E_{deposit} :: DT :: Muons",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_pi_deposits = new TH2F("DT_pi_deposits", "Simhit time vs E_{deposit} :: DT :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_ka_deposits = new TH2F("DT_ka_deposits", "Simhit time vs E_{deposit} :: DT :: Pions",     n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_p_deposits  = new TH2F("DT_p_deposits",  "Simhit time vs E_{deposit} :: DT :: Protons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_n_deposits  = new TH2F("DT_n_deposits",  "Simhit time vs E_{deposit} :: DT :: Neutrons",  n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_g_deposits  = new TH2F("DT_g_deposits",  "Simhit time vs E_{deposit} :: DT :: Photons",   n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);
  DT_N_deposits  = new TH2F("DT_N_deposits",  "Simhit time vs E_{deposit} :: DT :: Nuclei",    n_D, n1_D, n2_D, n_tof, n1_tof, n2_tof);

  RPCb_el_deps = new TH1F("RPCb_el_deps", "E_{deposit} :: RPCb :: Electrons", n_F, n1_F, n2_F);
  RPCb_mu_deps = new TH1F("RPCb_mu_deps", "E_{deposit} :: RPCb :: Muons",     n_F, n1_F, n2_F);
  RPCb_ha_deps = new TH1F("RPCb_ha_deps", "E_{deposit} :: RPCb :: Pions",     n_F, n1_F, n2_F);
  RPCf_el_deps = new TH1F("RPCf_el_deps", "E_{deposit} :: RPCf :: Electrons", n_F, n1_F, n2_F);
  RPCf_mu_deps = new TH1F("RPCf_mu_deps", "E_{deposit} :: RPCf :: Muons",     n_F, n1_F, n2_F);
  RPCf_ha_deps = new TH1F("RPCf_ha_deps", "E_{deposit} :: RPCf :: Pions",     n_F, n1_F, n2_F);
  CSC_el_deps  = new TH1F("CSC_el_deps",  "E_{deposit} :: CSC :: Electrons",  n_F, n1_F, n2_F);
  CSC_mu_deps  = new TH1F("CSC_mu_deps",  "E_{deposit} :: CSC :: Muons",      n_F, n1_F, n2_F);
  CSC_ha_deps  = new TH1F("CSC_ha_deps",  "E_{deposit} :: CSC :: Pions",      n_F, n1_F, n2_F);
  DT_el_deps   = new TH1F("DT_el_deps",   "E_{deposit} :: DT :: Electrons",   n_F, n1_F, n2_F);
  DT_mu_deps   = new TH1F("DT_mu_deps",   "E_{deposit} :: DT :: Muons",       n_F, n1_F, n2_F);
  DT_ha_deps   = new TH1F("DT_ha_deps",   "E_{deposit} :: DT :: Pions",       n_F, n1_F, n2_F);

  RPCb_XY = new TH2F("RPCb_XY", "Simhits in XY :: RPCb", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  RPCb_RZ = new TH2F("RPCb_RZ", "Simhits in RZ :: RPCb", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  RPCf_XY = new TH2F("RPCf_XY", "Simhits in XY :: RPCf", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  RPCf_RZ = new TH2F("RPCf_RZ", "Simhits in RZ :: RPCf", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  CSC_XY  = new TH2F("CSC_XY",  "Simhits in XY :: CSC", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  CSC_RZ  = new TH2F("CSC_RZ",  "Simhits in RZ :: CSC", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  DT_XY   = new TH2F("DT_XY",   "Simhits in XY :: DT", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  DT_RZ   = new TH2F("DT_RZ",   "Simhits in RZ :: DT", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_RZ = new TH2F("Muon_RZ", "Simhits in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);

  RPCb_000ns_XY = new TH2F("RPCb_000ns_XY", "Simhits with tof < 250ns in XY :: RPCb", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  RPCb_000ns_RZ = new TH2F("RPCb_000ns_RZ", "Simhits with tof < 250ns in RZ :: RPCb", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  RPCf_000ns_XY = new TH2F("RPCf_000ns_XY", "Simhits with tof < 250ns in XY :: RPCf", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  RPCf_000ns_RZ = new TH2F("RPCf_000ns_RZ", "Simhits with tof < 250ns in RZ :: RPCf", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  CSC_000ns_XY  = new TH2F("CSC_000ns_XY",  "Simhits with tof < 250ns in XY :: CSC", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  CSC_000ns_RZ  = new TH2F("CSC_000ns_RZ",  "Simhits with tof < 250ns in RZ :: CSC", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  DT_000ns_XY   = new TH2F("DT_000ns_XY",   "Simhits with tof < 250ns in XY :: DT", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  DT_000ns_RZ   = new TH2F("DT_000ns_RZ",   "Simhits with tof < 250ns in RZ :: DT", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_000ns_RZ = new TH2F("Muon_000ns_RZ", "Simhits with tof < 250ns in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_000ns_el_RZ = new TH2F("Muon_000ns_el_RZ", "Electron Simhits with tof < 250ns in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_000ns_mu_RZ = new TH2F("Muon_000ns_mu_RZ", "Muon Simhits with tof < 250ns in RZ :: Muon",     n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_000ns_ha_RZ = new TH2F("Muon_000ns_ha_RZ", "Hadron Simhits with tof < 250ns in RZ :: Muon",   n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);

  RPCb_250ns_XY = new TH2F("RPCb_250ns_XY", "Simhits with tof > 250ns in XY :: RPCb", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  RPCb_250ns_RZ = new TH2F("RPCb_250ns_RZ", "Simhits with tof > 250ns in RZ :: RPCb", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  RPCf_250ns_XY = new TH2F("RPCf_250ns_XY", "Simhits with tof > 250ns in XY :: RPCf", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  RPCf_250ns_RZ = new TH2F("RPCf_250ns_RZ", "Simhits with tof > 250ns in RZ :: RPCf", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  CSC_250ns_XY  = new TH2F("CSC_250ns_XY",  "Simhits with tof > 250ns in XY :: CSC", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  CSC_250ns_RZ  = new TH2F("CSC_250ns_RZ",  "Simhits with tof > 250ns in RZ :: CSC", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  DT_250ns_XY   = new TH2F("DT_250ns_XY",   "Simhits with tof > 250ns in XY :: DT", n_xy_x, n_xy_x1, n_xy_x2, n_xy_y, n_xy_y1, n_xy_y2);
  DT_250ns_RZ   = new TH2F("DT_250ns_RZ",   "Simhits with tof > 250ns in RZ :: DT", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_250ns_RZ = new TH2F("Muon_250ns_RZ", "Simhits with tof > 250ns in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_250ns_el_RZ = new TH2F("Muon_250ns_el_RZ", "Electron Simhits with tof > 250ns in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_250ns_mu_RZ = new TH2F("Muon_250ns_mu_RZ", "Muon Simhits with tof > 250ns in RZ :: Muon",     n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_250ns_ha_RZ = new TH2F("Muon_250ns_ha_RZ", "Hadron Simhits with tof > 250ns in RZ :: Muon",   n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);

  Muon_00ns_el_RZ = new TH2F("Muon_00ns_el_RZ", "Electron Simhits with tof < 50ns in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_00ns_mu_RZ = new TH2F("Muon_00ns_mu_RZ", "Muon Simhits with tof < 50ns in RZ :: Muon",     n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_00ns_ha_RZ = new TH2F("Muon_00ns_ha_RZ", "Hadron Simhits with tof < 50ns in RZ :: Muon",   n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_50ns_el_RZ = new TH2F("Muon_50ns_el_RZ", "Electron Simhits with 50 < tof < 250ns in RZ :: Muon", n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_50ns_mu_RZ = new TH2F("Muon_50ns_mu_RZ", "Muon Simhits with 50 < tof < 250ns in RZ :: Muon",     n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);
  Muon_50ns_ha_RZ = new TH2F("Muon_50ns_ha_RZ", "Hadron Simhits with 50 < tof < 250ns in RZ :: Muon",   n_zr_z, n_zr_z1, n_zr_z2, n_zr_r, n_zr_r1, n_zr_r2);

  RPCb_Muons_SHPT     = new TH1F("RPCb_Muons_SHPT", "RPCb :: All Muon Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCf_Muons_SHPT     = new TH1F("RPCf_Muons_SHPT", "RPCf :: All Muon Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  CSC_Muons_SHPT      = new TH1F("CSC_Muons_SHPT",  "CSC :: All Muon Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  DT_Muons_SHPT       = new TH1F("DT_Muons_SHPT",   "DT :: All Muon Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCb_Hadrons_SHPT   = new TH1F("RPCb_Hadrons_SHPT", "RPCb :: All Hadron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCf_Hadrons_SHPT   = new TH1F("RPCf_Hadrons_SHPT", "RPCf :: All Hadron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  CSC_Hadrons_SHPT    = new TH1F("CSC_Hadrons_SHPT",  "CSC :: All Hadron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  DT_Hadrons_SHPT     = new TH1F("DT_Hadrons_SHPT",   "DT :: All Hadron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCb_Electrons_SHPT = new TH1F("RPCb_Electrons_SHPT", "RPCb :: All Electron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCf_Electrons_SHPT = new TH1F("RPCf_Electrons_SHPT", "RPCf :: All Electron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  CSC_Electrons_SHPT  = new TH1F("CSC_Electrons_SHPT",  "CSC :: All Electron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  DT_Electrons_SHPT   = new TH1F("DT_Electrons_SHPT",   "DT :: All Electron Hits :: SimHit Process Type", n_pro, n1_pro, n2_pro);

  RPCb_Electrons_000ns_SHPT = new TH1F("RPCb_Electrons_000ns_SHPT", "RPCb :: Electron Hits with tof < 50 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCf_Electrons_000ns_SHPT = new TH1F("RPCf_Electrons_000ns_SHPT", "RPCf :: Electron Hits with tof < 50 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  CSC_Electrons_000ns_SHPT  = new TH1F("CSC_Electrons_000ns_SHPT",  "CSC ::  Electron Hits with tof < 50 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  DT_Electrons_000ns_SHPT   = new TH1F("DT_Electrons_000ns_SHPT",   "DT ::   Electron Hits with tof < 50 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCb_Electrons_050ns_SHPT = new TH1F("RPCb_Electrons_050ns_SHPT", "RPCb :: Electron Hits with 50 < tof < 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCf_Electrons_050ns_SHPT = new TH1F("RPCf_Electrons_050ns_SHPT", "RPCf :: Electron Hits with 50 < tof < 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  CSC_Electrons_050ns_SHPT  = new TH1F("CSC_Electrons_050ns_SHPT",  "CSC ::  Electron Hits with 50 < tof < 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  DT_Electrons_050ns_SHPT   = new TH1F("DT_Electrons_050ns_SHPT",   "DT ::   Electron Hits with 50 < tof < 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCb_Electrons_250ns_SHPT = new TH1F("RPCb_Electrons_250ns_SHPT", "RPCb :: Electron Hits with tof > 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  RPCf_Electrons_250ns_SHPT = new TH1F("RPCf_Electrons_250ns_SHPT", "RPCf :: Electron Hits with tof > 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  CSC_Electrons_250ns_SHPT  = new TH1F("CSC_Electrons_250ns_SHPT",  "CSC ::  Electron Hits with tof > 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);
  DT_Electrons_250ns_SHPT   = new TH1F("DT_Electrons_250ns_SHPT",   "DT ::   Electron Hits with tof > 250 ns :: SimHit Process Type", n_pro, n1_pro, n2_pro);

  SimVertices_RZ      = new TH2F("SimVertices_RZ",      "Vertices in RZ",                           n_zrc_z, n_zrc_z1, n_zrc_z2, n_zrc_r, n_zrc_r1, n_zrc_r2);
  SimVertices_Muon_RZ = new TH2F("SimVertices_Muon_RZ", "Vertices in RZ in Muon System and Cavern", n_zrc_z, n_zrc_z1, n_zrc_z2, n_zrc_r, n_zrc_r1, n_zrc_r2);
  PrimVertices_Z = new TH1F("PrimVertices_Z", "Primary Vertices distribution along Z", n_pv_z, n1_pv_z, n2_pv_z);
  PrimVertices_R = new TH1F("PrimVertices_R", "Primary Vertices distribution along R", n_pv_r, n1_pv_r, n2_pv_r);

  RPC_hits  = new TH1F("RPC_hits",  "Hits in different parts of the RPC system", n_cat, n1_cat, n2_cat);
  RPC_area  = new TH1F("RPC_area",  "Surface of different parts of the RPC system", n_cat, n1_cat, n2_cat);
  RPC_rates = new TH1F("RPC_rates", "Rates in different parts of the RPC system", n_cat, n1_cat, n2_cat);

  for(int i=0; i<2; ++i) { 
    for(int j=0; j<4; ++j) {
      for(int k=0; k<3; ++k) {
	RPC_hits_array[i][j][k] = 0; 
	RPC_area_array[i][j][k] = 0.0;
	RPC_rates_array[i][j][k]= 0.0;
      }
    }
  }

  RPCb_hits_tof = new TH1F("RPCb_hits_tof", "Simhit time :: RPCb", m_tof, m1_tof, m2_tof);
  RPCb_hits_eta = new TH1F("RPCb_hits_eta", "Simhit time :: RPCb", m_eta, m1_eta, m2_eta);
  RPCb_hits_phi = new TH1F("RPCb_hits_phi", "Simhit time :: RPCb", m_phi, m1_phi, m2_phi);
  RPCb_hits_lin = new TH1F("RPCb_hits_lin", "Simhit time :: RPCb", m_lin, m1_lin, m2_lin);

  RPCf_hits_tof = new TH1F("RPCf_hits_tof", "Simhit time :: RPCf", m_tof, m1_tof, m2_tof);
  RPCf_hits_eta = new TH1F("RPCf_hits_eta", "Simhit time :: RPCf", m_eta, m1_eta, m2_eta);
  RPCf_hits_phi = new TH1F("RPCf_hits_phi", "Simhit time :: RPCf", m_phi, m1_phi, m2_phi);
  RPCf_hits_lin = new TH1F("RPCf_hits_lin", "Simhit time :: RPCf", m_lin, m1_lin, m2_lin);

  CSC_hits_tof  = new TH1F("CSC_hits_tof",  "Simhit time :: CSC",  m_tof, m1_tof, m2_tof);
  CSC_hits_eta  = new TH1F("CSC_hits_eta",  "Simhit time :: CSC",  m_eta, m1_eta, m2_eta);
  CSC_hits_phi  = new TH1F("CSC_hits_phi",  "Simhit time :: CSC",  m_phi, m1_phi, m2_phi);
  CSC_hits_lin  = new TH1F("CSC_hits_lin",  "Simhit time :: CSC",  m_lin, m1_lin, m2_lin);

  DT_hits_tof   = new TH1F("DT_hits_tof",   "Simhit time :: DT",   m_tof, m1_tof, m2_tof);
  DT_hits_eta   = new TH1F("DT_hits_eta",   "Simhit time :: DT",   m_eta, m1_eta, m2_eta);
  DT_hits_phi   = new TH1F("DT_hits_phi",   "Simhit time :: DT",   m_phi, m1_phi, m2_phi);
  DT_hits_lin   = new TH1F("DT_hits_lin",   "Simhit time :: DT",   m_lin, m1_lin, m2_lin);

  RB4_hits_tof = new TH1F("RB4_hits_tof", "Simhit time :: RB4", m_tof, m1_tof, m2_tof);
  RE4_hits_tof = new TH1F("RE4_hits_tof", "Simhit time :: RE4", m_tof, m1_tof, m2_tof);
  MB4_hits_tof = new TH1F("MB4_hits_tof", "Simhit time :: MB4", m_tof, m1_tof, m2_tof);
  ME4_hits_tof = new TH1F("ME4_hits_tof", "Simhit time :: ME4", m_tof, m1_tof, m2_tof);

  RB4_hits_phi = new TH1F("RB4_hits_phi", "Simhit time :: RB4", m_phi, m1_phi, m2_phi);
  RE4_hits_phi = new TH1F("RE4_hits_phi", "Simhit time :: RE4", m_phi, m1_phi, m2_phi);
  MB4_hits_phi = new TH1F("MB4_hits_phi", "Simhit time :: MB4", m_phi, m1_phi, m2_phi);
  ME4_hits_phi = new TH1F("ME4_hits_phi", "Simhit time :: ME4", m_phi, m1_phi, m2_phi);

  RPCb_HPC      = new TH1F("RPCb_el_HPC", "Simhits per Chamber :: RPCb", n_hits, n1_hits, n2_hits);
  RPCf_HPC      = new TH1F("RPCf_el_HPC", "Simhits per Chamber :: RPCf", n_hits, n1_hits, n2_hits);
  CSC_HPC       = new TH1F("CSC_el_HPC",  "Simhits per Chamber :: CSC",  n_hits, n1_hits, n2_hits);
  DT_HPC        = new TH1F("DT_el_HPC",   "Simhits per Chamber :: DT",   n_hits, n1_hits, n2_hits);
  RPCb_el_HPC   = new TH1F("RPCb_el_HPC", "Simhits per Chamber :: RPCb", n_hits, n1_hits, n2_hits);
  RPCf_el_HPC   = new TH1F("RPCf_el_HPC", "Simhits per Chamber :: RPCf", n_hits, n1_hits, n2_hits);
  CSC_el_HPC    = new TH1F("CSC_el_HPC",  "Simhits per Chamber :: CSC",  n_hits, n1_hits, n2_hits);
  DT_el_HPC     = new TH1F("DT_el_HPC",   "Simhits per Chamber :: DT",   n_hits, n1_hits, n2_hits);

  RPCb_el_HPL   = new TH1F("RPCb_el_HPL", "Layers hit by Electron  :: RPCb", n_lay, n1_lay, n2_lay);
  RPCf_el_HPL   = new TH1F("RPCf_el_HPL", "Layers hit by Electron  :: RPCf", n_lay, n1_lay, n2_lay);
  CSC_el_HPL    = new TH1F("CSC_el_HPL",  "Layers hit by Electron  :: CSC",  n_lay, n1_lay, n2_lay);
  DT_el_HPL     = new TH1F("DT_el_HPL",   "Layers hit by Electron  :: DT",   n_lay, n1_lay, n2_lay);
  RPCb_mu_HPL   = new TH1F("RPCb_mu_HPL", "Layers hit by Muon  :: RPCb", n_lay, n1_lay, n2_lay);
  RPCf_mu_HPL   = new TH1F("RPCf_mu_HPL", "Layers hit by Muon  :: RPCf", n_lay, n1_lay, n2_lay);
  CSC_mu_HPL    = new TH1F("CSC_mu_HPL",  "Layers hit by Muon  :: CSC",  n_lay, n1_lay, n2_lay);
  DT_mu_HPL     = new TH1F("DT_mu_HPL",   "Layers hit by Muons :: DT",   n_lay, n1_lay, n2_lay);
  RPCb_ha_HPL   = new TH1F("RPCb_ha_HPL", "Layers hit by Hadrons  :: RPCb", n_lay, n1_lay, n2_lay);
  RPCf_ha_HPL   = new TH1F("RPCf_ha_HPL", "Layers hit by Hadrons  :: RPCf", n_lay, n1_lay, n2_lay);
  CSC_ha_HPL    = new TH1F("CSC_ha_HPL",  "Layers hit by Hadrons  :: CSC",  n_lay, n1_lay, n2_lay);
  DT_ha_HPL     = new TH1F("DT_ha_HPL",   "Layers hit by Hadrons  :: DT",   n_lay, n1_lay, n2_lay);


}


MyNeutronSimHitAnalyzer::~MyNeutronSimHitAnalyzer(){

 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  outputfile->cd();

  TStyle *plain  = new TStyle("Plain","Plain Style (no colours/fill areas)");
  plain->cd();
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPalette(1);
  TCanvas * Dummy = new TCanvas("dummy", "dummy", 600, 600);
  // First Plot for PDF File :: print empty Dummy 
  pdfFileName = pdfFileNameBase + ".pdf[";
  Dummy->Print(pdfFileName.c_str());
  // Name for next plot for PDF File
  pdfFileName = pdfFileNameBase + ".pdf";


  // Legends
  double l1_x1, l1_y1, l1_x2, l1_y2;
  double l2_x1, l2_y1, l2_x2, l2_y2;
  double l3_x1, l3_y1, l3_x2, l3_y2;
  double l4_x1, l4_y1, l4_x2, l4_y2;
  // First version
  // l1_x1 = 0.60; l1_x2 = 0.85; l1_y1 = 0.60; l1_y2 = 0.85;
  // Second version
  l1_x1 = 0.65; l1_x2 = 0.85; l1_y1 = 0.65; l1_y2 = 0.85;
  TLegend *l1 = new TLegend(l1_x1, l1_y1,l1_x2,l1_y2,NULL,"brNDC");
  l1->SetLineColor(0);    l1->SetLineStyle(0);  l1->SetLineWidth(0);
  l1->SetFillColor(4000); l1->SetBorderSize(0); l1->SetNColumns(2);
  // First version, with neutrons and gammas included
  /*
  l1->AddEntry(RPCb_el_hits, "e","p");
  l1->AddEntry(RPCb_p_hits,  "p","p");
  l1->AddEntry(RPCb_mu_hits, "#mu","p");
  l1->AddEntry(RPCb_n_hits,  "n","p");
  l1->AddEntry(RPCb_pi_hits, "#pi","p");
  l1->AddEntry(RPCb_g_hits,  "#gamma","p");
  l1->AddEntry(RPCb_ka_hits, "K","p");
  l1->AddEntry(RPCb_N_hits,  "Nuclei","p");
  */
  // second version 
  l1->AddEntry(RPCb_el_hits, "e","p");
  l1->AddEntry(RPCb_pi_hits, "#pi","p");
  l1->AddEntry(RPCb_mu_hits, "#mu","p");
  l1->AddEntry(RPCb_ka_hits, "K","p");
  l1->AddEntry(RPCb_p_hits,  "p","p");
  l1->AddEntry(RPCb_N_hits,  "Nuclei","p");

  TLegend *l2 = new TLegend(l1_x1, l1_y1,l1_x2,l1_y2,NULL,"brNDC");
  l2->SetLineColor(0);    l2->SetLineStyle(0);  l2->SetLineWidth(0);
  l2->SetFillColor(4000); l2->SetBorderSize(0); l2->SetNColumns(1);
  l2->AddEntry(RPCb_el_deps, "e","l");
  l2->AddEntry(RPCb_mu_deps, "#mu","l");
  l2->AddEntry(RPCb_ha_deps, "had","l");


  // Some visual help and comments
  double log250ns =  log10(250);
  TH1F * line_250ns_hits = new TH1F("line_250ns_hits", "", n_E, n1_E, n2_E); for(int i=0; i<n_E; ++i) { line_250ns_hits->SetBinContent(i+1,  log250ns); }
  TH1F * line_250ns_deps = new TH1F("line_250ns_deps", "", n_D, n1_D, n2_D); for(int i=0; i<n_D; ++i) { line_250ns_deps->SetBinContent(i+1,  log250ns); }
  line_250ns_hits->SetLineColor(kBlack); line_250ns_hits->SetLineStyle(2); line_250ns_hits->SetLineWidth(1);
  line_250ns_deps->SetLineColor(kBlack); line_250ns_deps->SetLineStyle(2); line_250ns_deps->SetLineWidth(1);
  TLatex latex_right;  latex_right.SetNDC();  latex_right.SetTextSize(0.03);   latex_right.SetTextAlign(31);
  TLatex latex_cmslab; latex_cmslab.SetNDC(); latex_cmslab.SetTextSize(0.035); latex_cmslab.SetTextAlign(11);
  TLatex latex_legend; latex_legend.SetNDC(); latex_legend.SetTextSize(0.04);  latex_legend.SetTextAlign(31);



  Canvas_RPCb_hits = new TCanvas("Canvas_RPCb_hits", "Simhit time vs E_{kin} :: RPCb", 600, 600);
  Canvas_RPCf_hits = new TCanvas("Canvas_RPCf_hits", "Simhit time vs E_{kin} :: RPCf", 600, 600);
  Canvas_CSC_hits  = new TCanvas("Canvas_CSC_hits",  "Simhit time vs E_{kin} :: CSC",  600, 600);
  Canvas_DT_hits   = new TCanvas("Canvas_DT_hits",   "Simhit time vs E_{kin} :: DT",   600, 600);

  // Combine histograms in single plot
  Canvas_RPCb_hits->cd();
  RPCb_el_hits->GetXaxis()->SetTitle("^{10}log E_{kin} (MeV)");
  RPCb_el_hits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  RPCb_el_hits->GetXaxis()->SetTitleOffset(1.2);
  RPCb_el_hits->SetTitle("SimHit time vs E_{kin} :: RPCb");
  RPCb_el_hits->SetMarkerStyle(7);  RPCb_el_hits->SetMarkerColor(kBlack);   RPCb_el_hits->SetMarkerSize(1);  RPCb_el_hits->Draw("P");
  RPCb_mu_hits->SetMarkerStyle(24); RPCb_mu_hits->SetMarkerColor(kBlue);    RPCb_mu_hits->SetMarkerSize(1);  RPCb_mu_hits->Draw("PSame");
  RPCb_pi_hits->SetMarkerStyle(33); RPCb_pi_hits->SetMarkerColor(kGreen);   RPCb_pi_hits->SetMarkerSize(1);  RPCb_pi_hits->Draw("PSame");
  RPCb_ka_hits->SetMarkerStyle(5);  RPCb_ka_hits->SetMarkerColor(kOrange);  RPCb_ka_hits->SetMarkerSize(1);  RPCb_ka_hits->Draw("PSame");
  RPCb_p_hits->SetMarkerStyle(26);  RPCb_p_hits->SetMarkerColor(kMagenta);  RPCb_p_hits->SetMarkerSize(1);   RPCb_p_hits->Draw("PSame");
  RPCb_n_hits->SetMarkerStyle(32);  RPCb_n_hits->SetMarkerColor(kViolet);   RPCb_n_hits->SetMarkerSize(1);   RPCb_n_hits->Draw("PSame");
  RPCb_g_hits->SetMarkerStyle(30);  RPCb_g_hits->SetMarkerColor(kCyan);     RPCb_g_hits->SetMarkerSize(1);   RPCb_g_hits->Draw("PSame");
  RPCb_N_hits->SetMarkerStyle(2);   RPCb_N_hits->SetMarkerColor(kRed);      RPCb_N_hits->SetMarkerSize(1);   RPCb_N_hits->Draw("PSame");
  line_250ns_hits->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"RPCb");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.50, 0.025,"#font[12]{1 MeV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_RPCb_hits->SetTicks(1,1);
  Canvas_RPCb_hits->Write();
  Canvas_RPCb_hits->Print(pdfFileName.c_str());

  Canvas_RPCf_hits->cd();
  RPCf_el_hits->GetXaxis()->SetTitle("^{10}log E_{kin} (MeV)");
  RPCf_el_hits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  RPCf_el_hits->GetXaxis()->SetTitleOffset(1.2);
  RPCf_el_hits->SetTitle("SimHit time vs E_{kin} :: RPCf");
  RPCf_el_hits->SetMarkerStyle(7);  RPCf_el_hits->SetMarkerColor(kBlack);   RPCf_el_hits->SetMarkerSize(1);  RPCf_el_hits->Draw("P");
  RPCf_mu_hits->SetMarkerStyle(24); RPCf_mu_hits->SetMarkerColor(kBlue);    RPCf_mu_hits->SetMarkerSize(1);  RPCf_mu_hits->Draw("PSame");
  RPCf_pi_hits->SetMarkerStyle(33); RPCf_pi_hits->SetMarkerColor(kGreen);   RPCf_pi_hits->SetMarkerSize(1);  RPCf_pi_hits->Draw("PSame");
  RPCf_ka_hits->SetMarkerStyle(5);  RPCf_ka_hits->SetMarkerColor(kOrange);  RPCf_ka_hits->SetMarkerSize(1);  RPCf_ka_hits->Draw("PSame");
  RPCf_p_hits->SetMarkerStyle(26);  RPCf_p_hits->SetMarkerColor(kMagenta);  RPCf_p_hits->SetMarkerSize(1);   RPCf_p_hits->Draw("PSame");
  RPCf_n_hits->SetMarkerStyle(32);  RPCf_n_hits->SetMarkerColor(kViolet);   RPCf_n_hits->SetMarkerSize(1);   RPCf_n_hits->Draw("PSame");
  RPCf_g_hits->SetMarkerStyle(30);  RPCf_g_hits->SetMarkerColor(kCyan);     RPCf_g_hits->SetMarkerSize(1);   RPCf_g_hits->Draw("PSame");
  RPCf_N_hits->SetMarkerStyle(2);   RPCf_N_hits->SetMarkerColor(kRed);      RPCf_N_hits->SetMarkerSize(1);   RPCf_N_hits->Draw("PSame");
  line_250ns_hits->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"RPCf");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.50, 0.025,"#font[12]{1 MeV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_RPCf_hits->SetTicks(1,1);
  Canvas_RPCf_hits->Write();
  Canvas_RPCf_hits->Print(pdfFileName.c_str());


  Canvas_CSC_hits->cd();
  CSC_el_hits->GetXaxis()->SetTitle("^{10}log E_{kin} (MeV)");
  CSC_el_hits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  CSC_el_hits->GetXaxis()->SetTitleOffset(1.2);
  CSC_el_hits->SetTitle("SimHit time vs E_{kin} :: CSC");
  CSC_el_hits->SetMarkerStyle(7);  CSC_el_hits->SetMarkerColor(kBlack);   CSC_el_hits->SetMarkerSize(1);  CSC_el_hits->Draw("P");
  CSC_mu_hits->SetMarkerStyle(24); CSC_mu_hits->SetMarkerColor(kBlue);    CSC_mu_hits->SetMarkerSize(1);  CSC_mu_hits->Draw("PSame");
  CSC_pi_hits->SetMarkerStyle(33); CSC_pi_hits->SetMarkerColor(kGreen);   CSC_pi_hits->SetMarkerSize(1);  CSC_pi_hits->Draw("PSame");
  CSC_ka_hits->SetMarkerStyle(5);  CSC_ka_hits->SetMarkerColor(kOrange);  CSC_ka_hits->SetMarkerSize(1);  CSC_ka_hits->Draw("PSame");
  CSC_p_hits->SetMarkerStyle(26);  CSC_p_hits->SetMarkerColor(kMagenta);  CSC_p_hits->SetMarkerSize(1);   CSC_p_hits->Draw("PSame");
  CSC_n_hits->SetMarkerStyle(32);  CSC_n_hits->SetMarkerColor(kViolet);   CSC_n_hits->SetMarkerSize(1);   CSC_n_hits->Draw("PSame");
  CSC_g_hits->SetMarkerStyle(30);  CSC_g_hits->SetMarkerColor(kCyan);     CSC_g_hits->SetMarkerSize(1);   CSC_g_hits->Draw("PSame");
  CSC_N_hits->SetMarkerStyle(2);   CSC_N_hits->SetMarkerColor(kRed);      CSC_N_hits->SetMarkerSize(1);   CSC_N_hits->Draw("PSame");
  line_250ns_hits->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"CSC");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.50, 0.025,"#font[12]{1 MeV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_CSC_hits->SetTicks(1,1);
  Canvas_CSC_hits->Write();
  Canvas_CSC_hits->Print(pdfFileName.c_str());


  Canvas_DT_hits->cd();
  DT_el_hits->GetXaxis()->SetTitle("^{10}log E_{kin} (MeV)");
  DT_el_hits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  DT_el_hits->GetXaxis()->SetTitleOffset(1.2);
  DT_el_hits->SetTitle("SimHit time vs E_{kin} :: DT");
  DT_el_hits->SetMarkerStyle(7);  DT_el_hits->SetMarkerColor(kBlack);   DT_el_hits->SetMarkerSize(1);  DT_el_hits->Draw("P");
  DT_mu_hits->SetMarkerStyle(24); DT_mu_hits->SetMarkerColor(kBlue);    DT_mu_hits->SetMarkerSize(1);  DT_mu_hits->Draw("PSame");
  DT_pi_hits->SetMarkerStyle(33); DT_pi_hits->SetMarkerColor(kGreen);   DT_pi_hits->SetMarkerSize(1);  DT_pi_hits->Draw("PSame");
  DT_ka_hits->SetMarkerStyle(5);  DT_ka_hits->SetMarkerColor(kOrange);  DT_ka_hits->SetMarkerSize(1);  DT_ka_hits->Draw("PSame");
  DT_p_hits->SetMarkerStyle(26);  DT_p_hits->SetMarkerColor(kMagenta);  DT_p_hits->SetMarkerSize(1);   DT_p_hits->Draw("PSame");
  DT_n_hits->SetMarkerStyle(32);  DT_n_hits->SetMarkerColor(kViolet);   DT_n_hits->SetMarkerSize(1);   DT_n_hits->Draw("PSame");
  DT_g_hits->SetMarkerStyle(30);  DT_g_hits->SetMarkerColor(kCyan);     DT_g_hits->SetMarkerSize(1);   DT_g_hits->Draw("PSame");
  DT_N_hits->SetMarkerStyle(2);   DT_N_hits->SetMarkerColor(kRed);      DT_N_hits->SetMarkerSize(1);   DT_N_hits->Draw("PSame");
  line_250ns_hits->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"DT");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.50, 0.025,"#font[12]{1 MeV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_DT_hits->SetTicks(1,1);
  Canvas_DT_hits->Write();
  Canvas_DT_hits->Print(pdfFileName.c_str());

  TDir_Muon_hits_deposits->cd();
  // --------------------------- 
  RPCf_el_hits->Write(); 
  RPCf_mu_hits->Write(); 
  RPCf_pi_hits->Write(); 
  RPCf_ka_hits->Write(); 
  RPCf_p_hits ->Write(); 
  RPCf_n_hits ->Write(); 
  RPCf_g_hits ->Write(); 
  RPCf_N_hits ->Write(); 

  RPCb_el_hits->Write(); 
  RPCb_mu_hits->Write(); 
  RPCb_pi_hits->Write(); 
  RPCb_ka_hits->Write(); 
  RPCb_p_hits ->Write(); 
  RPCb_n_hits ->Write(); 
  RPCb_g_hits ->Write(); 
  RPCb_N_hits ->Write(); 

  CSC_el_hits->Write(); 
  CSC_mu_hits->Write(); 
  CSC_pi_hits->Write(); 
  CSC_ka_hits->Write(); 
  CSC_p_hits ->Write(); 
  CSC_n_hits ->Write(); 
  CSC_g_hits ->Write(); 
  CSC_N_hits ->Write(); 

  DT_el_hits->Write(); 
  DT_mu_hits->Write(); 
  DT_pi_hits->Write(); 
  DT_ka_hits->Write(); 
  DT_p_hits ->Write(); 
  DT_n_hits ->Write(); 
  DT_g_hits ->Write(); 
  DT_N_hits ->Write(); 
  // --------------------------- 
  outputfile->cd();

  Canvas_RPCb_deposits = new TCanvas("Canvas_RPCb_deposits", "Simhit time vs E_{deposit} :: RPCb", 600, 600);
  Canvas_RPCf_deposits = new TCanvas("Canvas_RPCf_deposits", "Simhit time vs E_{deposit} :: RPCf", 600, 600);
  Canvas_CSC_deposits  = new TCanvas("Canvas_CSC_deposits",  "Simhit time vs E_{deposit} :: CSC",  600, 600);
  Canvas_DT_deposits   = new TCanvas("Canvas_DT_deposits",   "Simhit time vs E_{deposit} :: DT",   600, 600);

  // SimHit time vs Energy Deposit
  // Combine histograms in single plot
  Canvas_RPCb_deposits->cd();
  RPCb_el_deposits->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  RPCb_el_deposits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  RPCb_el_deposits->GetXaxis()->SetTitleOffset(1.2);
  RPCb_el_deposits->SetTitle("SimHit time vs E_{deposit} :: RPCb");
  RPCb_el_deposits->SetMarkerStyle(7);  RPCb_el_deposits->SetMarkerColor(kBlack);   RPCb_el_deposits->SetMarkerSize(1);  RPCb_el_deposits->Draw("P");
  RPCb_mu_deposits->SetMarkerStyle(24); RPCb_mu_deposits->SetMarkerColor(kBlue);    RPCb_mu_deposits->SetMarkerSize(1);  RPCb_mu_deposits->Draw("PSame");
  RPCb_pi_deposits->SetMarkerStyle(33); RPCb_pi_deposits->SetMarkerColor(kGreen);   RPCb_pi_deposits->SetMarkerSize(1);  RPCb_pi_deposits->Draw("PSame");
  RPCb_ka_deposits->SetMarkerStyle(5);  RPCb_ka_deposits->SetMarkerColor(kOrange);  RPCb_ka_deposits->SetMarkerSize(1);  RPCb_ka_deposits->Draw("PSame");
  RPCb_p_deposits->SetMarkerStyle(26);  RPCb_p_deposits->SetMarkerColor(kMagenta);  RPCb_p_deposits->SetMarkerSize(1);   RPCb_p_deposits->Draw("PSame");
  RPCb_n_deposits->SetMarkerStyle(32);  RPCb_n_deposits->SetMarkerColor(kViolet);   RPCb_n_deposits->SetMarkerSize(1);   RPCb_n_deposits->Draw("PSame");
  RPCb_g_deposits->SetMarkerStyle(30);  RPCb_g_deposits->SetMarkerColor(kCyan);   RPCb_g_deposits->SetMarkerSize(1);   RPCb_g_deposits->Draw("PSame");
  RPCb_N_deposits->SetMarkerStyle(2);   RPCb_N_deposits->SetMarkerColor(kRed);      RPCb_N_deposits->SetMarkerSize(1);   RPCb_N_deposits->Draw("PSame");
  line_250ns_deps->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"RPCb");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_RPCb_deposits->SetTicks(1,1);
  Canvas_RPCb_deposits->Write();
  Canvas_RPCb_deposits->Print(pdfFileName.c_str());

  Canvas_RPCf_deposits->cd();
  RPCf_el_deposits->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  RPCf_el_deposits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  RPCf_el_deposits->GetXaxis()->SetTitleOffset(1.2);
  RPCf_el_deposits->SetTitle("SimHit time vs E_{deposit} :: RPCf");
  RPCf_el_deposits->SetMarkerStyle(7);  RPCf_el_deposits->SetMarkerColor(kBlack);   RPCf_el_deposits->SetMarkerSize(1);  RPCf_el_deposits->Draw("P");
  RPCf_mu_deposits->SetMarkerStyle(24); RPCf_mu_deposits->SetMarkerColor(kBlue);    RPCf_mu_deposits->SetMarkerSize(1);  RPCf_mu_deposits->Draw("PSame");
  RPCf_pi_deposits->SetMarkerStyle(33); RPCf_pi_deposits->SetMarkerColor(kGreen);   RPCf_pi_deposits->SetMarkerSize(1);  RPCf_pi_deposits->Draw("PSame");
  RPCf_ka_deposits->SetMarkerStyle(5);  RPCf_ka_deposits->SetMarkerColor(kOrange);  RPCf_ka_deposits->SetMarkerSize(1);  RPCf_ka_deposits->Draw("PSame");
  RPCf_p_deposits->SetMarkerStyle(26);  RPCf_p_deposits->SetMarkerColor(kMagenta);  RPCf_p_deposits->SetMarkerSize(1);   RPCf_p_deposits->Draw("PSame");
  RPCf_n_deposits->SetMarkerStyle(32);  RPCf_n_deposits->SetMarkerColor(kViolet);   RPCf_n_deposits->SetMarkerSize(1);   RPCf_n_deposits->Draw("PSame");
  RPCf_g_deposits->SetMarkerStyle(30);  RPCf_g_deposits->SetMarkerColor(kCyan);     RPCf_g_deposits->SetMarkerSize(1);   RPCf_g_deposits->Draw("PSame");
  RPCf_N_deposits->SetMarkerStyle(2);   RPCf_N_deposits->SetMarkerColor(kRed);      RPCf_N_deposits->SetMarkerSize(1);   RPCf_N_deposits->Draw("PSame");
  line_250ns_deps->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"RPCf");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_RPCf_deposits->SetTicks(1,1);
  Canvas_RPCf_deposits->Write();
  Canvas_RPCf_deposits->Print(pdfFileName.c_str());

  Canvas_CSC_deposits->cd();
  CSC_el_deposits->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  CSC_el_deposits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  CSC_el_deposits->GetXaxis()->SetTitleOffset(1.2);
  CSC_el_deposits->SetTitle("SimHit time vs E_{deposit} :: CSC");
  CSC_el_deposits->SetMarkerStyle(7);  CSC_el_deposits->SetMarkerColor(kBlack);   CSC_el_deposits->SetMarkerSize(1);  CSC_el_deposits->Draw("P");
  CSC_mu_deposits->SetMarkerStyle(24); CSC_mu_deposits->SetMarkerColor(kBlue);    CSC_mu_deposits->SetMarkerSize(1);  CSC_mu_deposits->Draw("PSame");
  CSC_pi_deposits->SetMarkerStyle(33); CSC_pi_deposits->SetMarkerColor(kGreen);   CSC_pi_deposits->SetMarkerSize(1);  CSC_pi_deposits->Draw("PSame");
  CSC_ka_deposits->SetMarkerStyle(5);  CSC_ka_deposits->SetMarkerColor(kOrange);  CSC_ka_deposits->SetMarkerSize(1);  CSC_ka_deposits->Draw("PSame");
  CSC_p_deposits->SetMarkerStyle(26);  CSC_p_deposits->SetMarkerColor(kMagenta);  CSC_p_deposits->SetMarkerSize(1);   CSC_p_deposits->Draw("PSame");
  CSC_n_deposits->SetMarkerStyle(32);  CSC_n_deposits->SetMarkerColor(kViolet);   CSC_n_deposits->SetMarkerSize(1);   CSC_n_deposits->Draw("PSame");
  CSC_g_deposits->SetMarkerStyle(30);  CSC_g_deposits->SetMarkerColor(kCyan);     CSC_g_deposits->SetMarkerSize(1);   CSC_g_deposits->Draw("PSame");
  CSC_N_deposits->SetMarkerStyle(2);   CSC_N_deposits->SetMarkerColor(kRed);      CSC_N_deposits->SetMarkerSize(1);   CSC_N_deposits->Draw("PSame");
  line_250ns_deps->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"CSC");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_CSC_deposits->SetTicks(1,1);
  Canvas_CSC_deposits->Write();
  Canvas_CSC_deposits->Print(pdfFileName.c_str());

  Canvas_DT_deposits->cd();
  DT_el_deposits->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  DT_el_deposits->GetYaxis()->SetTitle("^{10}log TOF (ns)");
  DT_el_deposits->GetXaxis()->SetTitleOffset(1.2);
  DT_el_deposits->SetTitle("SimHit time vs E_{deposit} :: DT");
  DT_el_deposits->SetMarkerStyle(7);  DT_el_deposits->SetMarkerColor(kBlack);   DT_el_deposits->SetMarkerSize(1);  DT_el_deposits->Draw("P");
  DT_mu_deposits->SetMarkerStyle(24); DT_mu_deposits->SetMarkerColor(kBlue);    DT_mu_deposits->SetMarkerSize(1);  DT_mu_deposits->Draw("PSame");
  DT_pi_deposits->SetMarkerStyle(33); DT_pi_deposits->SetMarkerColor(kGreen);   DT_pi_deposits->SetMarkerSize(1);  DT_pi_deposits->Draw("PSame");
  DT_ka_deposits->SetMarkerStyle(5);  DT_ka_deposits->SetMarkerColor(kOrange);  DT_ka_deposits->SetMarkerSize(1);  DT_ka_deposits->Draw("PSame");
  DT_p_deposits->SetMarkerStyle(26);  DT_p_deposits->SetMarkerColor(kMagenta);  DT_p_deposits->SetMarkerSize(1);   DT_p_deposits->Draw("PSame");
  DT_n_deposits->SetMarkerStyle(32);  DT_n_deposits->SetMarkerColor(kViolet);   DT_n_deposits->SetMarkerSize(1);   DT_n_deposits->Draw("PSame");
  DT_g_deposits->SetMarkerStyle(30);  DT_g_deposits->SetMarkerColor(kCyan);     DT_g_deposits->SetMarkerSize(1);   DT_g_deposits->Draw("PSame");
  DT_N_deposits->SetMarkerStyle(2);   DT_N_deposits->SetMarkerColor(kRed);      DT_N_deposits->SetMarkerSize(1);   DT_N_deposits->Draw("PSame");
  line_250ns_deps->Draw("][same");
  l1->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"DT");
  latex_legend.DrawLatex(0.850, 0.850,"particle type:");
  latex_right.DrawLatex(0.09, 0.250,"#font[12]{250 ns}");
  latex_right.DrawLatex(0.06, 0.665,"#font[12]{1 ms}");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  latex_right.DrawLatex(0.89, 0.225,"#font[12]{prompt and decay}");
  latex_right.DrawLatex(0.89, 0.275,"#font[12]{neutron background}");
  Canvas_DT_deposits->SetTicks(1,1);
  Canvas_DT_deposits->Write();
  Canvas_DT_deposits->Print(pdfFileName.c_str());

  TDir_Muon_hits_deposits->cd();
  // --------------------------- 
  RPCf_el_deposits->Write(); 
  RPCf_mu_deposits->Write(); 
  RPCf_pi_deposits->Write(); 
  RPCf_ka_deposits->Write(); 
  RPCf_p_deposits ->Write(); 
  RPCf_n_deposits ->Write(); 
  RPCf_g_deposits ->Write(); 
  RPCf_N_deposits ->Write(); 

  RPCb_el_deposits->Write(); 
  RPCb_mu_deposits->Write(); 
  RPCb_pi_deposits->Write(); 
  RPCb_ka_deposits->Write(); 
  RPCb_p_deposits ->Write(); 
  RPCb_n_deposits ->Write(); 
  RPCb_g_deposits ->Write(); 
  RPCb_N_deposits ->Write(); 

  CSC_el_deposits->Write(); 
  CSC_mu_deposits->Write(); 
  CSC_pi_deposits->Write(); 
  CSC_ka_deposits->Write(); 
  CSC_p_deposits ->Write(); 
  CSC_n_deposits ->Write(); 
  CSC_g_deposits ->Write(); 
  CSC_N_deposits ->Write(); 

  DT_el_deposits->Write(); 
  DT_mu_deposits->Write(); 
  DT_pi_deposits->Write(); 
  DT_ka_deposits->Write(); 
  DT_p_deposits ->Write(); 
  DT_n_deposits ->Write(); 
  DT_g_deposits ->Write(); 
  DT_N_deposits ->Write(); 
  // --------------------------- 
  outputfile->cd();

  Canvas_RPCb_1D_deps = new TCanvas("Canvas_RPCb_1D_deps", "E_{deposit} :: RPCb", 600, 600);
  Canvas_RPCf_1D_deps = new TCanvas("Canvas_RPCf_1D_deps", "E_{deposit} :: RPCf", 600, 600);
  Canvas_CSC_1D_deps  = new TCanvas("Canvas_CSC_1D_deps",  "E_{deposit} :: CSC",  600, 600);
  Canvas_DT_1D_deps   = new TCanvas("Canvas_DT_1D_deps",   "E_{deposit} :: DT",   600, 600);

  Canvas_RPCb_1D_deps->cd();
  RPCb_el_deps->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  RPCb_el_deps->GetYaxis()->SetTitle("Hits");
  RPCb_el_deps->SetTitle("E_{deposit} :: RPCb");
  RPCb_el_deps->SetLineStyle(1);  RPCb_el_deps->SetLineColor(kBlack);   RPCb_el_deps->SetLineWidth(1);  RPCb_el_deps->Draw("H");
  RPCb_mu_deps->SetLineStyle(1); RPCb_mu_deps->SetLineColor(kBlue);    RPCb_mu_deps->SetLineWidth(1);  RPCb_mu_deps->Draw("HSame");
  RPCb_ha_deps->SetLineStyle(1); RPCb_ha_deps->SetLineColor(kGreen);   RPCb_ha_deps->SetLineWidth(1);  RPCb_ha_deps->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"RPCb");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  Canvas_RPCb_1D_deps->SetTicks(1,1);
  Canvas_RPCb_1D_deps->Write();
  Canvas_RPCb_1D_deps->Print(pdfFileName.c_str());

  Canvas_RPCf_1D_deps->cd();
  RPCf_el_deps->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  RPCf_el_deps->GetYaxis()->SetTitle("Hits");
  RPCf_el_deps->SetTitle("E_{deposit} :: RPCf");
  RPCf_el_deps->SetLineStyle(1);  RPCf_el_deps->SetLineColor(kBlack);   RPCf_el_deps->SetLineWidth(1);  RPCf_el_deps->Draw("H");
  RPCf_mu_deps->SetLineStyle(1); RPCf_mu_deps->SetLineColor(kBlue);    RPCf_mu_deps->SetLineWidth(1);  RPCf_mu_deps->Draw("HSame");
  RPCf_ha_deps->SetLineStyle(1); RPCf_ha_deps->SetLineColor(kGreen);   RPCf_ha_deps->SetLineWidth(1);  RPCf_ha_deps->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"RPCf");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  Canvas_RPCf_1D_deps->SetTicks(1,1);
  Canvas_RPCf_1D_deps->Write();
  Canvas_RPCf_1D_deps->Print(pdfFileName.c_str());

  Canvas_CSC_1D_deps->cd();
  CSC_el_deps->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  CSC_el_deps->GetYaxis()->SetTitle("Hits");
  CSC_el_deps->SetTitle("E_{deposit} :: CSC");
  CSC_el_deps->SetLineStyle(1);  CSC_el_deps->SetLineColor(kBlack);   CSC_el_deps->SetLineWidth(1);  CSC_el_deps->Draw("H");
  CSC_mu_deps->SetLineStyle(1); CSC_mu_deps->SetLineColor(kBlue);    CSC_mu_deps->SetLineWidth(1);  CSC_mu_deps->Draw("HSame");
  CSC_ha_deps->SetLineStyle(1); CSC_ha_deps->SetLineColor(kGreen);   CSC_ha_deps->SetLineWidth(1);  CSC_ha_deps->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"CSC");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  Canvas_CSC_1D_deps->SetTicks(1,1);
  Canvas_CSC_1D_deps->Write();
  Canvas_CSC_1D_deps->Print(pdfFileName.c_str());

  Canvas_DT_1D_deps->cd();
  DT_el_deps->GetXaxis()->SetTitle("^{10}log E_{deposit} (keV)");
  DT_el_deps->GetYaxis()->SetTitle("Hits");
  DT_el_deps->SetTitle("E_{deposit} :: DT");
  DT_el_deps->SetLineStyle(1);  DT_el_deps->SetLineColor(kBlack);   DT_el_deps->SetLineWidth(1);  DT_el_deps->Draw("H");
  DT_mu_deps->SetLineStyle(1); DT_mu_deps->SetLineColor(kBlue);    DT_mu_deps->SetLineWidth(1);  DT_mu_deps->Draw("HSame");
  DT_ha_deps->SetLineStyle(1); DT_ha_deps->SetLineColor(kGreen);   DT_ha_deps->SetLineWidth(1);  DT_ha_deps->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.850,"DT");
  latex_right.DrawLatex(0.40, 0.025,"#font[12]{1 keV}");
  Canvas_DT_1D_deps->SetTicks(1,1);
  Canvas_DT_1D_deps->Write();
  Canvas_DT_1D_deps->Print(pdfFileName.c_str());

  TDir_Muon_hits_deposits->cd();
  // --------------------------- 
  RPCb_el_deps->Write();
  RPCb_mu_deps->Write();
  RPCb_ha_deps->Write();
  RPCf_el_deps->Write();
  RPCf_mu_deps->Write();
  RPCf_ha_deps->Write();
  CSC_el_deps->Write();
  CSC_mu_deps->Write();
  CSC_ha_deps->Write();
  DT_el_deps->Write();
  DT_mu_deps->Write();
  DT_ha_deps->Write();
  // --------------------------- 
  outputfile->cd();

  Canvas_Muon_RZ = new TCanvas("Canvas_Muon_RZ", "RZ-view of SimHits :: Muon", 600, 600);
  Muon_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_RZ->SetTitle("RZ-view of SimHits :: Muon");
  Muon_RZ->Draw("colz");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"All SimHits");
  Canvas_Muon_RZ->SetTicks(1,1);
  Canvas_Muon_RZ->Write(); 
  Canvas_Muon_RZ->Print(pdfFileName.c_str());

  Canvas_Muon_000ns_RZ = new TCanvas("Canvas_Muon_000ns_RZ", "RZ-view of SimHits with tof < 250 ns :: Muon", 600, 600);
  Muon_000ns_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_000ns_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_000ns_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_000ns_RZ->SetTitle("RZ-view of SimHits with tof < 250 ns :: Muon");
  Muon_000ns_RZ->Draw("colz");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"SimHits TOF < 250 ns");
  Canvas_Muon_000ns_RZ->SetTicks(1,1);
  Canvas_Muon_000ns_RZ->Write(); 
  Canvas_Muon_000ns_RZ->Print(pdfFileName.c_str());

  Canvas_Muon_250ns_RZ = new TCanvas("Canvas_Muon_250ns_RZ", "RZ-view of SimHits with tof > 250 ns :: Muon", 600, 600);
  Muon_250ns_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_250ns_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_250ns_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_250ns_RZ->SetTitle("RZ-view of SimHits with tof > 250 ns :: Muon");
  Muon_250ns_RZ->Draw("colz");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"SimHits TOF > 250 ns");
  Canvas_Muon_250ns_RZ->SetTicks(1,1);
  Canvas_Muon_250ns_RZ->Write(); 
  Canvas_Muon_250ns_RZ->Print(pdfFileName.c_str());

  Muon_000ns_el_RZ->SetMarkerColor(kBlack);  Muon_000ns_mu_RZ->SetMarkerColor(kBlue);  Muon_000ns_ha_RZ->SetMarkerColor(kRed);
  l2_x1 = 0.15; l2_x2 = 0.35; l2_y1 = 0.20; l2_y2 = 0.40;
  TLegend *l2a = new TLegend(l2_x1, l2_y1,l2_x2,l2_y2,NULL,"brNDC");
  l2a->SetLineColor(0);    l2a->SetLineStyle(0);  l2a->SetLineWidth(0);
  l2a->SetFillColor(4000); l2a->SetBorderSize(0); l2a->SetNColumns(1);
  l2a->AddEntry(Muon_000ns_el_RZ, "e","p");
  l2a->AddEntry(Muon_000ns_mu_RZ, "#mu","p");
  l2a->AddEntry(Muon_000ns_ha_RZ, "had","p");

  Canvas_Muon_000ns_Cont_RZ = new TCanvas("Canvas_Muon_000ns_Cont_RZ", "RZ-view of SimHits with tof < 250 ns :: Muon", 600, 600);
  Muon_000ns_el_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_000ns_el_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_000ns_el_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_000ns_el_RZ->SetTitle("RZ-view of SimHits with tof < 250 ns :: Muon");
  Muon_000ns_el_RZ->SetMarkerStyle(7); Muon_000ns_el_RZ->SetMarkerColor(kBlack);   Muon_000ns_el_RZ->SetMarkerSize(1);  Muon_000ns_el_RZ->Draw("P");
  Muon_000ns_mu_RZ->SetMarkerStyle(7); Muon_000ns_mu_RZ->SetMarkerColor(kBlue);    Muon_000ns_mu_RZ->SetMarkerSize(1);  Muon_000ns_mu_RZ->Draw("PSame");
  Muon_000ns_ha_RZ->SetMarkerStyle(7); Muon_000ns_ha_RZ->SetMarkerColor(kRed);     Muon_000ns_ha_RZ->SetMarkerSize(1);  Muon_000ns_ha_RZ->Draw("PSame");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"SimHits TOF < 250 ns");
  l2a->Draw();
  Canvas_Muon_000ns_Cont_RZ->SetTicks(1,1);
  Canvas_Muon_000ns_Cont_RZ->Write(); 
  Canvas_Muon_000ns_Cont_RZ->Print(pdfFileName.c_str());

  Canvas_Muon_250ns_Cont_RZ = new TCanvas("Canvas_Muon_250ns_Cont_RZ", "RZ-view of SimHits with tof > 250 ns :: Muon", 600, 600);
  Muon_250ns_el_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_250ns_el_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_250ns_el_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_250ns_el_RZ->SetTitle("RZ-view of SimHits with tof > 250 ns :: Muon");
  Muon_250ns_el_RZ->SetMarkerStyle(7); Muon_250ns_el_RZ->SetMarkerColor(kBlack);   Muon_250ns_el_RZ->SetMarkerSize(1);  Muon_250ns_el_RZ->Draw("P");
  Muon_250ns_mu_RZ->SetMarkerStyle(7); Muon_250ns_mu_RZ->SetMarkerColor(kBlue);    Muon_250ns_mu_RZ->SetMarkerSize(1);  Muon_250ns_mu_RZ->Draw("PSame");
  Muon_250ns_ha_RZ->SetMarkerStyle(7); Muon_250ns_ha_RZ->SetMarkerColor(kRed);     Muon_250ns_ha_RZ->SetMarkerSize(1);  Muon_250ns_ha_RZ->Draw("PSame");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"SimHits TOF > 250 ns");
  l2a->Draw();
  Canvas_Muon_250ns_Cont_RZ->SetTicks(1,1);
  Canvas_Muon_250ns_Cont_RZ->Write(); 
  Canvas_Muon_250ns_Cont_RZ->Print(pdfFileName.c_str());

  TDir_Muon_hits_deposits->cd();
  // --------------------------- 
  Muon_000ns_el_RZ->Write();
  Muon_000ns_mu_RZ->Write();
  Muon_000ns_ha_RZ->Write();
  Muon_250ns_el_RZ->Write();
  Muon_250ns_mu_RZ->Write();
  Muon_250ns_ha_RZ->Write();
  // --------------------------- 
  outputfile->cd();

  Canvas_Muon_00ns_Cont_RZ = new TCanvas("Canvas_Muon_00ns_Cont_RZ", "RZ-view of SimHits with tof < 50 ns :: Muon", 600, 600);
  Muon_00ns_el_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_00ns_el_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_00ns_el_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_00ns_el_RZ->SetTitle("RZ-view of SimHits with tof < 50 ns :: Muon");
  Muon_00ns_el_RZ->SetMarkerStyle(7); Muon_00ns_el_RZ->SetMarkerColor(kBlack);   Muon_00ns_el_RZ->SetMarkerSize(1);  Muon_00ns_el_RZ->Draw("P");
  Muon_00ns_mu_RZ->SetMarkerStyle(7); Muon_00ns_mu_RZ->SetMarkerColor(kBlue);    Muon_00ns_mu_RZ->SetMarkerSize(1);  Muon_00ns_mu_RZ->Draw("PSame");
  Muon_00ns_ha_RZ->SetMarkerStyle(7); Muon_00ns_ha_RZ->SetMarkerColor(kRed);     Muon_00ns_ha_RZ->SetMarkerSize(1);  Muon_00ns_ha_RZ->Draw("PSame");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"SimHits TOF < 50 ns");
  l2a->Draw();
  Canvas_Muon_00ns_Cont_RZ->SetTicks(1,1);
  Canvas_Muon_00ns_Cont_RZ->Write(); 
  Canvas_Muon_00ns_Cont_RZ->Print(pdfFileName.c_str());

  Canvas_Muon_50ns_Cont_RZ = new TCanvas("Canvas_Muon_50ns_Cont_RZ", "RZ-view of SimHits with 50 < tof < 250 ns :: Muon", 600, 600);
  Muon_50ns_el_RZ->GetXaxis()->SetTitle("Z (cm)");
  Muon_50ns_el_RZ->GetYaxis()->SetTitle("R (cm)");
  Muon_50ns_el_RZ->GetYaxis()->SetTitleOffset(1.30);
  Muon_50ns_el_RZ->SetTitle("RZ-view of SimHits with 50 < tof < 250 ns :: Muon");
  Muon_50ns_el_RZ->SetMarkerStyle(7); Muon_50ns_el_RZ->SetMarkerColor(kBlack);   Muon_50ns_el_RZ->SetMarkerSize(1);  Muon_50ns_el_RZ->Draw("P");
  Muon_50ns_mu_RZ->SetMarkerStyle(7); Muon_50ns_mu_RZ->SetMarkerColor(kBlue);    Muon_50ns_mu_RZ->SetMarkerSize(1);  Muon_50ns_mu_RZ->Draw("PSame");
  Muon_50ns_ha_RZ->SetMarkerStyle(7); Muon_50ns_ha_RZ->SetMarkerColor(kRed);     Muon_50ns_ha_RZ->SetMarkerSize(1);  Muon_50ns_ha_RZ->Draw("PSame");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  latex_cmslab.DrawLatex(0.15, 0.15,"SimHits 50 < TOF < 250 ns");
  l2a->Draw();
  Canvas_Muon_50ns_Cont_RZ->SetTicks(1,1);
  Canvas_Muon_50ns_Cont_RZ->Write(); 
  Canvas_Muon_50ns_Cont_RZ->Print(pdfFileName.c_str());

  TDir_Muon_XY_RZ_views->cd();
  // --------------------------- 
  Muon_00ns_el_RZ->Write();
  Muon_00ns_mu_RZ->Write();
  Muon_00ns_ha_RZ->Write();
  Muon_50ns_el_RZ->Write();
  Muon_50ns_mu_RZ->Write();
  Muon_50ns_ha_RZ->Write();
  // --------------------------- 
  outputfile->cd();

  /*
  Canvas_SimVertices_RZ = new TCanvas("Canvas_SimVertices_RZ", "RZ-view of Sim Vertices", 600, 600);
  SimVertices_RZ->GetXaxis()->SetTitle("Z (cm)");
  SimVertices_RZ->GetYaxis()->SetTitle("R (cm)");
  SimVertices_RZ->GetYaxis()->SetTitleOffset(1.30);
  SimVertices_RZ->SetTitle("RZ-view of Sim Vertices");
  SimVertices_RZ->Draw("colz");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_SimVertices_RZ->SetTicks(1,1);
  Canvas_SimVertices_RZ->Write(); 
  Canvas_SimVertices_RZ->Print(pdfFileName.c_str());

  Canvas_SimVertices_Muon_RZ = new TCanvas("Canvas_SimVertices_Muon_RZ", "RZ-view of Sim Vertices in Muon System and Cavern", 600, 600);
  SimVertices_Muon_RZ->GetXaxis()->SetTitle("Z (cm)");
  SimVertices_Muon_RZ->GetYaxis()->SetTitle("R (cm)");
  SimVertices_Muon_RZ->GetYaxis()->SetTitleOffset(1.30);
  SimVertices_Muon_RZ->SetTitle("RZ-view of Sim Vertices in Muon System and Cavern");
  // SimVertices_Muon_RZ->SetMarkerStyle(5);
  // SimVertices_Muon_RZ->SetMarkerColor(kBlack);
  // SimVertices_Muon_RZ->SetMarkerSize(1.25);
  // SimVertices_Muon_RZ->Draw("P");
  SimVertices_Muon_RZ->Draw("colz");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_SimVertices_Muon_RZ->SetTicks(1,1);
  Canvas_SimVertices_Muon_RZ->Write(); 
  Canvas_SimVertices_Muon_RZ->Print(pdfFileName.c_str());
  PrimVertices_Z->Write();
  PrimVertices_R->Write();
  */

  TDir_Muon_XY_RZ_views->cd();
  // --------------------------- 
  RPCb_XY->Write();
  RPCb_RZ->Write();
  RPCf_XY->Write();
  RPCf_RZ->Write();
  CSC_XY->Write();
  CSC_RZ->Write();
  DT_XY->Write();
  DT_RZ->Write();
  Muon_RZ->Write();

  RPCb_000ns_XY->Write();
  RPCb_000ns_RZ->Write();
  RPCf_000ns_XY->Write();
  RPCf_000ns_RZ->Write();
  CSC_000ns_XY->Write();
  CSC_000ns_RZ->Write();
  DT_000ns_XY->Write();
  DT_000ns_RZ->Write();
  Muon_000ns_RZ->Write();

  RPCb_250ns_XY->Write();
  RPCb_250ns_RZ->Write();
  RPCf_250ns_XY->Write();
  RPCf_250ns_RZ->Write();
  CSC_250ns_XY->Write();
  CSC_250ns_RZ->Write();
  DT_250ns_XY->Write();
  DT_250ns_RZ->Write();
  Muon_250ns_RZ->Write();
  // --------------------------- 
  outputfile->cd();

  for(int m=0; m<15; ++m) {
    RPCb_Muons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCf_Muons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    CSC_Muons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    DT_Muons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCb_Hadrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCf_Hadrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    CSC_Hadrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    DT_Hadrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCb_Electrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCf_Electrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    CSC_Electrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    DT_Electrons_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCb_Electrons_250ns_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    RPCf_Electrons_250ns_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    CSC_Electrons_250ns_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
    DT_Electrons_250ns_SHPT->GetXaxis()->SetBinLabel( m+1, proc[m].c_str());
  }

  TDir_Muon_hit_info->cd();
  // --------------------------- 
  RPCb_Muons_SHPT->Write();
  RPCf_Muons_SHPT->Write();
  CSC_Muons_SHPT->Write();
  DT_Muons_SHPT->Write();
  RPCb_Hadrons_SHPT->Write();
  RPCf_Hadrons_SHPT->Write();
  CSC_Hadrons_SHPT->Write();
  DT_Hadrons_SHPT->Write();
  RPCb_Electrons_SHPT->Write();
  RPCf_Electrons_SHPT->Write();
  CSC_Electrons_SHPT->Write();
  DT_Electrons_SHPT->Write();
  RPCb_Electrons_000ns_SHPT->Write();
  RPCf_Electrons_000ns_SHPT->Write();
  CSC_Electrons_000ns_SHPT->Write();
  DT_Electrons_000ns_SHPT->Write();
  RPCb_Electrons_050ns_SHPT->Write();
  RPCf_Electrons_050ns_SHPT->Write();
  CSC_Electrons_050ns_SHPT->Write();
  DT_Electrons_050ns_SHPT->Write();
  RPCb_Electrons_250ns_SHPT->Write();
  RPCf_Electrons_250ns_SHPT->Write();
  CSC_Electrons_250ns_SHPT->Write();
  DT_Electrons_250ns_SHPT->Write();
  // --------------------------- 
  outputfile->cd();

  TDir_Muon_hits_deposits->cd();
  // --------------------------- 
  RPCb_hits_tof->Write();
  RPCb_hits_eta->Write();
  RPCb_hits_phi->Write();
  RPCb_hits_lin->Write();

  RPCf_hits_tof->Write();
  RPCf_hits_eta->Write();
  RPCf_hits_phi->Write();
  RPCf_hits_lin->Write();

  CSC_hits_tof->Write();
  CSC_hits_eta->Write();
  CSC_hits_phi->Write();
  CSC_hits_lin->Write();

  DT_hits_tof->Write();
  DT_hits_eta->Write();
  DT_hits_phi->Write();
  DT_hits_lin->Write();

  RB4_hits_tof->Write();
  RE4_hits_tof->Write();
  MB4_hits_tof->Write();
  ME4_hits_tof->Write();

  RPCb_el_deps->Write();
  RPCb_mu_deps->Write();
  RPCb_ha_deps->Write();
  RPCf_el_deps->Write();
  RPCf_mu_deps->Write();
  RPCf_ha_deps->Write();
  CSC_el_deps->Write();
  CSC_mu_deps->Write();
  CSC_ha_deps->Write();
  DT_el_deps->Write();
  DT_mu_deps->Write();
  DT_ha_deps->Write();
  // --------------------------- 
  outputfile->cd();

  /*
  TDir_Muon_hit_info->cd();
  // ------------------- 
  RPCb_HPC->Write();
  RPCf_HPC->Write();
  CSC_HPC->Write();
  DT_HPC->Write();
  RPCb_el_HPC->Write();
  RPCf_el_HPC->Write();
  CSC_el_HPC->Write();
  DT_el_HPC->Write();
  RPCb_el_HPL->Write();
  RPCf_el_HPL->Write();
  CSC_el_HPL->Write();
  DT_el_HPL->Write();
  RPCb_mu_HPL->Write();
  RPCf_mu_HPL->Write();
  CSC_mu_HPL->Write();
  DT_mu_HPL->Write();
  RPCb_ha_HPL->Write();
  RPCf_ha_HPL->Write();
  CSC_ha_HPL->Write();
  DT_ha_HPL->Write();
  // ------------------- 
  outputfile->cd();
  */
  
  Canvas_RPCb_Layers = new TCanvas("Canvas_RPCb_Layers", "Number of Layers hit :: RPCb", 600, 600);
  Canvas_RPCf_Layers = new TCanvas("Canvas_RPCf_Layers", "Number of Layers hit :: RPCf", 600, 600);
  Canvas_CSC_Layers  = new TCanvas("Canvas_CSC_Layers",  "Number of Layers hit :: CSC",  600, 600);
  Canvas_DT_Layers   = new TCanvas("Canvas_DT_Layers",   "Number of Layers hit :: DT",   600, 600);

  Canvas_RPCb_Layers->cd();
  RPCb_el_HPL->GetXaxis()->SetTitle("Number of Layers hit in RPCb system");
  RPCb_el_HPL->GetYaxis()->SetTitle("Events");
  RPCb_el_HPL->SetTitle("Layers :: RPCb");
  RPCb_el_HPL->SetLineStyle(1);  RPCb_el_HPL->SetLineColor(kBlack);   RPCb_el_HPL->SetLineWidth(1);  RPCb_el_HPL->Draw("H");
  RPCb_mu_HPL->SetLineStyle(1);  RPCb_mu_HPL->SetLineColor(kBlue);    RPCb_mu_HPL->SetLineWidth(1);  RPCb_mu_HPL->Draw("HSame");
  RPCb_ha_HPL->SetLineStyle(1);  RPCb_ha_HPL->SetLineColor(kRed);     RPCb_ha_HPL->SetLineWidth(1);  RPCb_ha_HPL->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPCb_Layers->SetTicks(1,1);
  //Canvas_RPCb_Layers->Write();
  Canvas_RPCb_Layers->Print(pdfFileName.c_str());

  Canvas_RPCf_Layers->cd();
  RPCf_el_HPL->GetXaxis()->SetTitle("Number of Layers hit in RPCf system");
  RPCf_el_HPL->GetYaxis()->SetTitle("Events");
  RPCf_el_HPL->SetTitle("Layers :: RPCf");
  RPCf_el_HPL->SetLineStyle(1);  RPCf_el_HPL->SetLineColor(kBlack);   RPCf_el_HPL->SetLineWidth(1);  RPCf_el_HPL->Draw("H");
  RPCf_mu_HPL->SetLineStyle(1);  RPCf_mu_HPL->SetLineColor(kBlue);    RPCf_mu_HPL->SetLineWidth(1);  RPCf_mu_HPL->Draw("HSame");
  RPCf_ha_HPL->SetLineStyle(1);  RPCf_ha_HPL->SetLineColor(kRed);     RPCf_ha_HPL->SetLineWidth(1);  RPCf_ha_HPL->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPCf_Layers->SetTicks(1,1);
  // Canvas_RPCf_Layers->Write();
  Canvas_RPCf_Layers->Print(pdfFileName.c_str());
  

  Canvas_CSC_Layers->cd();
  CSC_el_HPL->GetXaxis()->SetTitle("Number of Layers hit in CSC system");
  CSC_el_HPL->GetYaxis()->SetTitle("Events");
  CSC_el_HPL->SetTitle("Layers :: CSC");
  CSC_el_HPL->SetLineStyle(1);  CSC_el_HPL->SetLineColor(kBlack);   CSC_el_HPL->SetLineWidth(1);  CSC_el_HPL->Draw("H");
  CSC_mu_HPL->SetLineStyle(1);  CSC_mu_HPL->SetLineColor(kBlue);    CSC_mu_HPL->SetLineWidth(1);  CSC_mu_HPL->Draw("HSame");
  CSC_ha_HPL->SetLineStyle(1);  CSC_ha_HPL->SetLineColor(kRed);     CSC_ha_HPL->SetLineWidth(1);  CSC_ha_HPL->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_CSC_Layers->SetTicks(1,1);
  Canvas_CSC_Layers->Write();
  Canvas_CSC_Layers->Print(pdfFileName.c_str());

  Canvas_DT_Layers->cd();
  DT_el_HPL->GetXaxis()->SetTitle("Number of Layers hit in DT system");
  DT_el_HPL->GetYaxis()->SetTitle("Events");
  DT_el_HPL->SetTitle("Layers :: DT");
  DT_el_HPL->SetLineStyle(1);  DT_el_HPL->SetLineColor(kBlack);   DT_el_HPL->SetLineWidth(1);  DT_el_HPL->Draw("H");
  DT_mu_HPL->SetLineStyle(1);  DT_mu_HPL->SetLineColor(kBlue);    DT_mu_HPL->SetLineWidth(1);  DT_mu_HPL->Draw("HSame");
  DT_ha_HPL->SetLineStyle(1);  DT_ha_HPL->SetLineColor(kRed);     DT_ha_HPL->SetLineWidth(1);  DT_ha_HPL->Draw("HSame");
  l2->Draw();
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_DT_Layers->SetTicks(1,1);
  Canvas_DT_Layers->Write();
  Canvas_DT_Layers->Print(pdfFileName.c_str());


  int rpc_barrel_hits = 0;
  int rpc_endcap_hits = 0;
  for(int j=0; j<4; ++j) {
    for(int k=0; k<3; ++k) {
      rpc_barrel_hits += RPC_hits_array[0][j][k];
      rpc_endcap_hits += RPC_hits_array[1][j][k];
      // std::cout<<"RPC_hits_array[region=0][station="<<j+1<<"][ring="<<k<<"] = "  <<RPC_hits_array[0][j][k]<<" ==> rpc_barrel_hits = "<<rpc_barrel_hits<<std::endl; 
      // std::cout<<"RPC_hits_array[region=1][station="<<j+1<<"][ring="<<k+1<<"] = "<<RPC_hits_array[1][j][k]<<" ==> rpc_endcap_hits = "<<rpc_endcap_hits<<std::endl; 
    }
  }

  // Relation Instantaneous Luminosity and PU interactions

  // calculation ::
  // --------------
  // # collisions / second = L x sigma 
  // # collisions / 25 ns = L x sigma / 40*10^6
  // # collisions / 50 ns = L x sigma / 20*10^6
  // ------------------------------------------
  // Luminosity is in units of cm^-2 s^-1
  // Cross section is in units of barn (10^-28 m^-2
  // 1 mb = 10^3 x 10^-28 m^-2 = 10^3 x 10^-32 cm^-2 = 10^-27 cm^-2
  // ------------------------------------------
  // not all bunches in LHC are colliding
  // 3564 bunches
  // design: 2808 (2760?) filled, 2662 colliding ==> fraction = 0.75
  // 2012:   1380 filled, 1380 colliding         ==> fraction = 0.39

  // 2012
  // 8 TeV
  // sigma_inel = 73 mb ( 8 TeV)
  // L = 10^33  (peak :: 8 * 10^33)
  // L x sigma_inel = 73 x 10^6 s^-1 / 20*10^6 = 3.65 
  // L x sigma_inel = 73 x 10^6 s^-1 / 40*10^6 = 7.30
  // 7.30 / 0.39 = 18.7 (collisions happen only in 40% of all possible 25ns timestamps)

  // int pu = 21; // measured ...
  int pu = 24; // normalized to 5 * 10^33 cm^-2 s^-1

  // 2015
  // com = 14 TeV
  // sigma_inel = 80 mb (14 TeV)
  // L = 10^34 1/cm^2 1/s (design)
  // int pu = 25;

  // old calculation
  int entries = 2500;
  // int bx_space = 25;
  // double corr_fact = pow(10,9) * 1.0/bx_space * 1.0/(entries * 1.0/pu);
  double sg_corr_fact = 2;  // simulation has only single gas volumes implemented instead of double gas volumes
  // std::cout<<"Rate Correction factor =  pow(10,9) * 1.0/(entries * 1.0/pu * bx_space) = "<<pow(10,9) * 1.0/(entries * 1.0/pu * bx_space)<<std::endl;
  // std::cout<<"Rate Correction factor because of simulation of Single Gas Layers = "<<sg_corr_fact<<std::endl;
  // std::cout<<"RPC Hits in Barrel = "<<rpc_barrel_hits<<" || RPC Hits in Endcap = "<<rpc_endcap_hits<<std::endl;
  // std::cout<<"RPC Barrel Active area = "<<rpc_barrel_area<<" cm2 || RPC Endcap Active area = "<<rpc_endcap_area<<" cm2"<<std::endl;

  // new calculation
  // ---------------
  // average amount of hits per event = (hits / entries)
  // ( average amount of hits per event ) / area
  // amount of collisions in 1s = 11245 (revolution frequency) * 1380 (colliding bunches) * pu (no of collisions per crossing)
  // => Rate [Hz/cm^2] = ( average amount of hits per event ) / area * amount of collisions in 1s
  // => Rate [Hz/cm^2] = N /entries / area * 11245 * 1380 * 21

  // getLumi(pu, space, com)
  for(int i=0; i<50; ++i) {
    // std::cout<<"Instantaneous Luminosity for "<<i<<" PU interactions at "<<bunchspacing<<"ns bunch spacing and at "<<comenergy<<"TeV is "<<getLumi(i,bunchspacing,comenergy)<<" * 10^34 cm^-2 s^-1 "<<std::endl;
    InstLumi[i] = getLumi(i,bunchspacing,comenergy);
  }
  // getPU(lumi, space, com)
  for(int i=0; i<60; ++i) {
    // std::cout<<"Pile Up for Instantaneous Luminosity "<<i*1.0/100<<" * 10^34 cm^-2 s^-1 at "<<bunchspacing<<"ns bunch spacing and at "<<comenergy<<"TeV is "<<getPU(i*1.0/100,bunchspacing,comenergy)<<" PU"<<std::endl;
  }

  double bunches;
  if(bunchspacing==50) bunches = 1380;
  if(bunchspacing==25) bunches = 2808;
  double corr_fact = 1.0/entries * 11245 * bunches * pu;

  // Rates as function of pu / instantaneous luminosity
  const int max_pu = 33;
  for(int i=0; i<max_pu; ++i) {
    RPC_rates_Summary[0][i] =  (rpc_barrel_hits+rpc_endcap_hits) * 1.0/(rpc_barrel_area+rpc_endcap_area) * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
    RPC_rates_Summary[1][i] =  (rpc_barrel_hits)                 * 1.0/(rpc_barrel_area)                 * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
    RPC_rates_Summary[2][i] =  (rpc_endcap_hits)                 * 1.0/(rpc_endcap_area)                 * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
    RPC_uncer_Rate[0][i] =  sqrt(rpc_barrel_hits+rpc_endcap_hits) * 1.0/(rpc_barrel_area+rpc_endcap_area) * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
    RPC_uncer_Rate[1][i] =  sqrt(rpc_barrel_hits)                 * 1.0/(rpc_barrel_area)                 * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
    RPC_uncer_Rate[2][i] =  sqrt(rpc_endcap_hits)                 * 1.0/(rpc_endcap_area)                 * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
    RPC_uncer_Lumi[0][i] =  0;
    RPC_uncer_Lumi[1][i] =  0;
    RPC_uncer_Lumi[2][i] =  0;
  }

  gr_RPC_Rates_All    = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[0], RPC_uncer_Lumi[0], RPC_uncer_Rate[0]); 
  gr_RPC_Rates_Barrel = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[1], RPC_uncer_Lumi[1], RPC_uncer_Rate[1]); 
  gr_RPC_Rates_Endcap = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[2], RPC_uncer_Lumi[2], RPC_uncer_Rate[2]); 

  gr_RPC_Rates_All->SetMarkerStyle(21);    gr_RPC_Rates_All->SetMarkerColor(kCyan);     gr_RPC_Rates_All->SetLineColor(kCyan);
  gr_RPC_Rates_Barrel->SetMarkerStyle(21); gr_RPC_Rates_Barrel->SetMarkerColor(kRed);   gr_RPC_Rates_Barrel->SetLineColor(kRed);
  gr_RPC_Rates_Endcap->SetMarkerStyle(21); gr_RPC_Rates_Endcap->SetMarkerColor(kBlack); gr_RPC_Rates_Endcap->SetLineColor(kBlack);

  l3_x1 = 0.20; l3_x2 = 0.45; l3_y1 = 0.65; l3_y2 = 0.85;
  l4_x1 = 0.20; l4_x2 = 0.35; l4_y1 = 0.75; l4_y2 = 0.85;
  TLegend *l3 = new TLegend(l3_x1,l3_y1,l3_x2,l3_y2,NULL,"brNDC");
  l3->SetLineColor(0);    l3->SetLineStyle(0);  l3->SetLineWidth(0);
  l3->SetFillColor(4000); l3->SetBorderSize(0); l3->SetNColumns(1);
  l3->AddEntry(gr_RPC_Rates_Endcap, "Endcap","pl");
  l3->AddEntry(gr_RPC_Rates_All,    "Barrel + Endcap","pl");
  l3->AddEntry(gr_RPC_Rates_Barrel, "Barrel","pl");

  Canvas_RPC_Rates = new TCanvas("Canvas_RPC_Rates", "Rates in RPC System", 600, 600);
  gr_RPC_Rates_Endcap->GetXaxis()->SetTitle("Instantaneous Luminosity #times 10^{34} (cm^{-2}s^{-1})");
  gr_RPC_Rates_Endcap->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  gr_RPC_Rates_Endcap->GetYaxis()->SetTitleOffset(1.30);
  gr_RPC_Rates_Endcap->GetXaxis()->SetRangeUser(0.00,0.68);
  gr_RPC_Rates_Endcap->GetYaxis()->SetRangeUser(0.00,6.00);
  gr_RPC_Rates_Endcap->SetTitle("Rates in RPC System");
  gr_RPC_Rates_Endcap->Draw("PA");
  gr_RPC_Rates_Barrel->Draw("Psame");
  gr_RPC_Rates_All->Draw("Psame");
  l3->Draw("same");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPC_Rates->SetTicks(1,1);
  Canvas_RPC_Rates->Write(); 
  Canvas_RPC_Rates->Print(pdfFileName.c_str());


  // Barrel + Endcap @ 24 pu
  RPC_hits->SetBinContent(1,  rpc_barrel_hits+rpc_endcap_hits);
  RPC_area->SetBinContent(1,  rpc_barrel_area+rpc_endcap_area);
  if((rpc_barrel_hits > 0 || rpc_endcap_hits > 0) && (rpc_barrel_area > 0.0 || rpc_endcap_area > 0.0)) {
    RPC_rates->SetBinContent(1,  (rpc_barrel_hits+rpc_endcap_hits)*1.0/(rpc_barrel_area+rpc_endcap_area)*corr_fact*sg_corr_fact);
    RPC_rates->SetBinError(  1,  sqrt((rpc_barrel_hits+rpc_endcap_hits))*1.0/(rpc_barrel_area+rpc_endcap_area)*corr_fact*sg_corr_fact);
  }
  // Barrel @ 24 pu 
  RPC_hits->SetBinContent(2,  rpc_barrel_hits);
  RPC_area->SetBinContent(2,  rpc_barrel_area);
  if(rpc_barrel_hits > 0 && rpc_barrel_area > 0.0) {
    RPC_rates->SetBinContent(2,  rpc_barrel_hits*1.0/rpc_barrel_area*corr_fact*sg_corr_fact);
    RPC_rates->SetBinError(  2,  sqrt(rpc_barrel_hits)*1.0/rpc_barrel_area*corr_fact*sg_corr_fact);
  }
  for(int k=0; k<3; ++k) {  // Barrel :: ring == wheel
   for(int j=0; j<4; ++j) {
      RPC_hits->SetBinContent(2+4*(k)+(j+1),RPC_hits_array[0][j][k]);
      RPC_area->SetBinContent(2+4*(k)+(j+1),RPC_area_array[0][j][k]);
      // std::cout<<"Bin "<<1+4*(k)+(j+1)<<" filled with RPC_area_array[region="<<0<<"][station"<<j+1<<"]["<<k<<"] = "<<RPC_area_array[0][j][k]<<" cm2"<<std::endl;
      if(RPC_hits_array[0][j][k] >  0 && RPC_area_array[0][j][k] > 0.0) {
	RPC_rates->SetBinContent(2+4*(k)+(j+1), RPC_hits_array[0][j][k]*1.0/RPC_area_array[0][j][k]*corr_fact*sg_corr_fact);
	RPC_rates->SetBinError(  2+4*(k)+(j+1), sqrt(RPC_hits_array[0][j][k])*1.0/RPC_area_array[0][j][k]*corr_fact*sg_corr_fact);
      }
    }
  }
  // Endcap @ 24 pu
  RPC_hits->SetBinContent(15, rpc_endcap_hits);
  RPC_area->SetBinContent(15, rpc_endcap_area);
  if(rpc_endcap_hits > 0 && rpc_endcap_area > 0.0) {
    RPC_rates->SetBinContent(15,  rpc_endcap_hits*1.0/rpc_endcap_area*corr_fact*sg_corr_fact);
    RPC_rates->SetBinError(  15,  sqrt(rpc_endcap_hits)*1.0/rpc_endcap_area*corr_fact*sg_corr_fact);
  }
  for(int j=0; j<4; ++j) {  // Endcap :: station == disk
    for(int k=0; k<3; ++k) {
      RPC_hits->SetBinContent(15+3*j+(k+1),RPC_hits_array[1][j][k]);
      RPC_area->SetBinContent(15+3*j+(k+1),RPC_area_array[1][j][k]);
      // std::cout<<"Bin "<<14+3*j+(k+1)<<" filled with RPC_area_array[region="<<1<<"][station"<<j+1<<"]["<<k+1<<"] = "<<RPC_area_array[1][j][k]<<" cm2"<<std::endl;
      if(RPC_hits_array[1][j][k] >  0 && RPC_area_array[1][j][k] > 0.0) {
	RPC_rates->SetBinContent(15+3*j+(k+1), RPC_hits_array[1][j][k]*1.0/RPC_area_array[1][j][k]*corr_fact*sg_corr_fact);
	RPC_rates->SetBinError(  15+3*j+(k+1), sqrt(RPC_hits_array[1][j][k])*1.0/RPC_area_array[1][j][k]*corr_fact*sg_corr_fact);
      }
    }
  }

  for(int m=0; m<27; ++m) {
    RPC_hits->GetXaxis()->SetBinLabel( m+1, allcat[m].c_str());
    RPC_area->GetXaxis()->SetBinLabel( m+1, allcat[m].c_str());
    RPC_rates->GetXaxis()->SetBinLabel(m+1, allcat[m].c_str());
  }
  /*
  for(int i=0; i<2;++i) {
    for(int j=0; j<4; ++j) {
      for(int k=0; k<3; ++k) {
	int region = i;
        int station = j+1;
        int ring; if(region==0) ring = k; else ring = k+1;
	std::cout<<"RPC_area_array[region="<<region<<"][station="<<station<<"][ring="<<ring<<"] = "<<RPC_area_array[i][j][k]<<" cm2"<<std::endl;
      }
    }
  }
  for(int i=0; i<2;++i) {
    for(int j=0; j<4; ++j) {
      for(int k=0; k<3; ++k) {
	int region = i;
	int station = j+1;
	int ring; if(region==0) ring = k; else ring = k+1;
	std::cout<<"RPC_hits_array[region="<<region<<"][station="<<station<<"][ring="<<ring<<"] = "<<RPC_hits_array[i][j][k]<<" hits"<<std::endl;
      }
    }
  }
  */
  TDir_Muon_rates->cd();
  // ------------------- 
  RPC_hits->Write();
  RPC_area->Write();
  RPC_rates->Write();
  // ------------------- 
  outputfile->cd();

  for(int i=0; i<max_pu; ++i) {
    for(int j=0; j<4; ++j) {  
      for(int k=0; k<3; ++k) {                 /* RPC_hits_array[1][j][k] */                /* RPC_area_array[1][j][k] */
	if(RPC_hits->GetBinContent(15+3*j+(k+1)) > 0.0 || RPC_area->GetBinContent(15+3*j+(k+1))) {
	  RPC_rates_Summary[2+3*j+(k+1)][i] =       RPC_hits->GetBinContent(15+3*j+(k+1))  * 1.0/RPC_area->GetBinContent(15+3*j+(k+1)) * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
	  RPC_uncer_Rate[2+3*j+(k+1)][i]    =  sqrt(RPC_hits->GetBinContent(15+3*j+(k+1))) * 1.0/RPC_area->GetBinContent(15+3*j+(k+1)) * 1.0/entries * 11245 * bunches * i * sg_corr_fact;
	  RPC_uncer_Lumi[2+3*j+(k+1)][i]    =  0;
	}
	else {
	  RPC_rates_Summary[2+3*j+(k+1)][i] = 0;
	  RPC_uncer_Rate[2+3*j+(k+1)][i]    = 0;
	  RPC_uncer_Lumi[2+3*j+(k+1)][i]    = 0;
	}
      }
    }
  }
  // RE11 --> RPC_rates_Summary[3]
  gr_RPC_Rates_RE12 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[4], RPC_uncer_Lumi[4], RPC_uncer_Rate[4]); 
  gr_RPC_Rates_RE13 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[5], RPC_uncer_Lumi[5], RPC_uncer_Rate[5]); 
  gr_RPC_Rates_RE12->SetMarkerStyle(21);    gr_RPC_Rates_RE12->SetMarkerColor(kRed);      gr_RPC_Rates_RE12->SetLineColor(kRed);
  gr_RPC_Rates_RE13->SetMarkerStyle(21);    gr_RPC_Rates_RE13->SetMarkerColor(kCyan);     gr_RPC_Rates_RE13->SetLineColor(kCyan);
  // RE21 --> RPC_rates_Summary[6]
  gr_RPC_Rates_RE22 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[7], RPC_uncer_Lumi[7], RPC_uncer_Rate[7]); 
  gr_RPC_Rates_RE23 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[8], RPC_uncer_Lumi[8], RPC_uncer_Rate[8]); 
  gr_RPC_Rates_RE22->SetMarkerStyle(21);    gr_RPC_Rates_RE22->SetMarkerColor(kRed);      gr_RPC_Rates_RE22->SetLineColor(kRed);
  gr_RPC_Rates_RE23->SetMarkerStyle(21);    gr_RPC_Rates_RE23->SetMarkerColor(kCyan);     gr_RPC_Rates_RE23->SetLineColor(kCyan);
  // RE31 --> RPC_rates_Summary[9]
  gr_RPC_Rates_RE32 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[10], RPC_uncer_Lumi[10], RPC_uncer_Rate[10]); 
  gr_RPC_Rates_RE33 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[11], RPC_uncer_Lumi[11], RPC_uncer_Rate[11]); 
  gr_RPC_Rates_RE32->SetMarkerStyle(21);    gr_RPC_Rates_RE32->SetMarkerColor(kRed);      gr_RPC_Rates_RE32->SetLineColor(kRed);
  gr_RPC_Rates_RE33->SetMarkerStyle(21);    gr_RPC_Rates_RE33->SetMarkerColor(kCyan);     gr_RPC_Rates_RE33->SetLineColor(kCyan);
  // RE41 --> RPC_rates_Summary[12]
  gr_RPC_Rates_RE42 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[13], RPC_uncer_Lumi[13], RPC_uncer_Rate[13]); 
  gr_RPC_Rates_RE43 = new TGraphErrors(max_pu, InstLumi, RPC_rates_Summary[14], RPC_uncer_Lumi[14], RPC_uncer_Rate[14]); 
  gr_RPC_Rates_RE42->SetMarkerStyle(21);    gr_RPC_Rates_RE42->SetMarkerColor(kRed);      gr_RPC_Rates_RE42->SetLineColor(kRed);
  gr_RPC_Rates_RE43->SetMarkerStyle(21);    gr_RPC_Rates_RE43->SetMarkerColor(kCyan);     gr_RPC_Rates_RE43->SetLineColor(kCyan);

  TLegend *l3a = new TLegend(l4_x1,l4_y1,l4_x2,l4_y2,NULL,"brNDC");
  l3a->SetLineColor(0);    l3a->SetLineStyle(0);  l3a->SetLineWidth(0);
  l3a->SetFillColor(4000); l3a->SetBorderSize(0); l3a->SetNColumns(1);
  l3a->AddEntry(gr_RPC_Rates_RE12, "RE1/2","pl");
  l3a->AddEntry(gr_RPC_Rates_RE13, "RE1/3","pl");
  TLegend *l3b = new TLegend(l4_x1,l4_y1,l4_x2,l4_y2,NULL,"brNDC");
  l3b->SetLineColor(0);    l3b->SetLineStyle(0);  l3b->SetLineWidth(0);
  l3b->SetFillColor(4000); l3b->SetBorderSize(0); l3b->SetNColumns(1);
  l3b->AddEntry(gr_RPC_Rates_RE12, "RE2/2","pl");
  l3b->AddEntry(gr_RPC_Rates_RE13, "RE2/3","pl");
  TLegend *l3c = new TLegend(l4_x1,l4_y1,l4_x2,l4_y2,NULL,"brNDC");
  l3c->SetLineColor(0);    l3c->SetLineStyle(0);  l3c->SetLineWidth(0);
  l3c->SetFillColor(4000); l3c->SetBorderSize(0); l3c->SetNColumns(1);
  l3c->AddEntry(gr_RPC_Rates_RE12, "RE3/2","pl");
  l3c->AddEntry(gr_RPC_Rates_RE13, "RE3/3","pl");
  TLegend *l3d = new TLegend(l4_x1,l4_y1,l4_x2,l4_y2,NULL,"brNDC");
  l3d->SetLineColor(0);    l3d->SetLineStyle(0);  l3d->SetLineWidth(0);
  l3d->SetFillColor(4000); l3d->SetBorderSize(0); l3d->SetNColumns(1);
  l3d->AddEntry(gr_RPC_Rates_RE12, "RE4/2","pl");
  l3d->AddEntry(gr_RPC_Rates_RE13, "RE4/3","pl");

  Canvas_RPC_Rates_RE1 = new TCanvas("Canvas_RPC_Rates_RE1", "Rates in RPC System :: First Endcap Station", 600, 600);
  gr_RPC_Rates_RE12->GetXaxis()->SetTitle("Instantaneous Luminosity #times 10^{34} (cm^{-2}s^{-1})");
  gr_RPC_Rates_RE12->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  gr_RPC_Rates_RE12->GetYaxis()->SetTitleOffset(1.30);
  gr_RPC_Rates_RE12->GetXaxis()->SetRangeUser(0.00,0.68);
  gr_RPC_Rates_RE12->GetYaxis()->SetRangeUser(0.00,4.00);
  gr_RPC_Rates_RE12->SetTitle("Rates in RPC System");
  gr_RPC_Rates_RE12->Draw("PA");
  gr_RPC_Rates_RE13->Draw("Psame");
  l3a->Draw("same");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPC_Rates_RE1->SetTicks(1,1);
  Canvas_RPC_Rates_RE1->Write(); 
  Canvas_RPC_Rates_RE1->Print(pdfFileName.c_str());

  Canvas_RPC_Rates_RE2 = new TCanvas("Canvas_RPC_Rates_RE2", "Rates in RPC System :: First Endcap Station", 600, 600);
  gr_RPC_Rates_RE22->GetXaxis()->SetTitle("Instantaneous Luminosity #times 10^{34} (cm^{-2}s^{-1})");
  gr_RPC_Rates_RE22->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  gr_RPC_Rates_RE22->GetYaxis()->SetTitleOffset(1.30);
  gr_RPC_Rates_RE22->GetXaxis()->SetRangeUser(0.00,0.68);
  gr_RPC_Rates_RE22->GetYaxis()->SetRangeUser(0.00,15.0);
  gr_RPC_Rates_RE22->SetTitle("Rates in RPC System");
  gr_RPC_Rates_RE22->Draw("PA");
  gr_RPC_Rates_RE23->Draw("Psame");
  l3b->Draw("same");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPC_Rates_RE2->SetTicks(1,1);
  Canvas_RPC_Rates_RE2->Write(); 
  Canvas_RPC_Rates_RE2->Print(pdfFileName.c_str());

  Canvas_RPC_Rates_RE3 = new TCanvas("Canvas_RPC_Rates_RE3", "Rates in RPC System :: First Endcap Station", 600, 600);
  gr_RPC_Rates_RE32->GetXaxis()->SetTitle("Instantaneous Luminosity #times 10^{34} (cm^{-2}s^{-1})");
  gr_RPC_Rates_RE32->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  gr_RPC_Rates_RE32->GetYaxis()->SetTitleOffset(1.30);
  gr_RPC_Rates_RE32->GetXaxis()->SetRangeUser(0.00,0.68);
  gr_RPC_Rates_RE32->GetYaxis()->SetRangeUser(0.00,11.0);
  gr_RPC_Rates_RE32->SetTitle("Rates in RPC System");
  gr_RPC_Rates_RE32->Draw("PA");
  gr_RPC_Rates_RE33->Draw("Psame");
  l3c->Draw("same");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPC_Rates_RE3->SetTicks(1,1);
  Canvas_RPC_Rates_RE3->Write(); 
  Canvas_RPC_Rates_RE3->Print(pdfFileName.c_str());

  Canvas_RPC_Rates_RE4 = new TCanvas("Canvas_RPC_Rates_RE4", "Rates in RPC System :: First Endcap Station", 600, 600);
  gr_RPC_Rates_RE42->GetXaxis()->SetTitle("Instantaneous Luminosity #times 10^{34} (cm^{-2}s^{-1})");
  gr_RPC_Rates_RE42->GetYaxis()->SetTitle("Rate (Hz/cm^{2})");
  gr_RPC_Rates_RE42->GetYaxis()->SetTitleOffset(1.30);
  gr_RPC_Rates_RE42->GetXaxis()->SetRangeUser(0.00,0.68);
  gr_RPC_Rates_RE42->GetYaxis()->SetRangeUser(0.00,15.0);
  gr_RPC_Rates_RE42->SetTitle("Rates in RPC System");
  gr_RPC_Rates_RE42->Draw("PA");
  gr_RPC_Rates_RE43->Draw("Psame");
  l3d->Draw("same");
  latex_cmslab.DrawLatex(0.10, 0.925,"CMS Simulation #sqrt{s} = 8 TeV");
  Canvas_RPC_Rates_RE4->SetTicks(1,1);
  Canvas_RPC_Rates_RE4->Write(); 
  Canvas_RPC_Rates_RE4->Print(pdfFileName.c_str());

  /*
    TDir_Muon_rates->cd();
  // ------------------------ 
  gr_RPC_Rates_RE12->Write();
  gr_RPC_Rates_RE13->Write();
  gr_RPC_Rates_RE22->Write();
  gr_RPC_Rates_RE23->Write();
  gr_RPC_Rates_RE32->Write();
  gr_RPC_Rates_RE33->Write();
  gr_RPC_Rates_RE42->Write();
  gr_RPC_Rates_RE43->Write();
  // ------------------------
  */

  // last plot for PDF File :: print empty Dummy
  pdfFileName = pdfFileNameBase + ".pdf]";
  Dummy->Print(pdfFileName.c_str());

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MyNeutronSimHitAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // ===============================
  //      General Collections
  // ===============================
  
  // SimHits
  // std::cout << " Getting the SimHits " <<std::endl;
  std::vector<edm::Handle<edm::PSimHitContainer> > theSimHitContainers;
  iEvent.getManyByType(theSimHitContainers);
  if(debug) std::cout << " The number of SimHit Containers is  " << theSimHitContainers.size() <<std::endl;
  std::vector<PSimHit> theSimHits;
  for (int i = 0; i < int(theSimHitContainers.size()); ++i) {
    theSimHits.insert(theSimHits.end(),theSimHitContainers.at(i)->begin(),theSimHitContainers.at(i)->end());
  }
  // SimTracks
  std::vector<SimTrack> theSimTracks;
  edm::Handle<edm::SimTrackContainer> SimTk;
  iEvent.getByLabel("g4SimHits",SimTk);
  theSimTracks.insert(theSimTracks.end(),SimTk->begin(),SimTk->end());
  if(debug) std::cout << "This Event has " <<  theSimTracks.size() << " sim tracks " << std::endl;
  // SimVertices
  std::vector<SimVertex> theSimVertices; 
  edm::Handle<edm::SimVertexContainer> SimVtx;
  iEvent.getByLabel("g4SimHits",SimVtx);
  theSimVertices.insert(theSimVertices.end(),SimVtx->begin(),SimVtx->end());
  if(debug) std::cout << "This Event has " <<  theSimVertices.size() << " sim vertices " << std::endl;

  double vtx_r = 0.0, vtx_z = 0.0;
  for (std::vector<SimVertex>::const_iterator iVertex = theSimVertices.begin(); iVertex != theSimVertices.end(); ++iVertex) {
    SimVertex simvertex = (*iVertex);    
    // vtx_x = simvertex.position().x();
    // vtx_y = simvertex.position().y();
    vtx_r = sqrt(pow(simvertex.position().x(),2)+pow(simvertex.position().y(),2));
    vtx_z = simvertex.position().z();
    SimVertices_RZ->Fill(fabs(vtx_z),vtx_r);
    if( vtx_r < 2 && vtx_z < 25 ) {
      PrimVertices_Z->Fill(vtx_z);
      PrimVertices_R->Fill(vtx_r);
    }
    // if(vtx_r > 400 || vtx_z > 500 ) {               // muon + forward
    //   SimVertices_Muon_RZ->Fill(fabs(vtx_z),vtx_r);
    // }
    if(vtx_r > 400 || (vtx_z > 500 && vtx_r > 100) ) { // muon only
      SimVertices_Muon_RZ->Fill(fabs(vtx_z),vtx_r);
    }
  }

  
  // create a map associating geant particle id and position in the 
  // event SimTrack vector
  std::map<unsigned, unsigned> geantToIndex;
  for( unsigned it=0; it<theSimTracks.size(); ++it ) {
    geantToIndex[ theSimTracks[it].trackId() ] = it;
    // std::cout<<"geantToIndex[ "<<theSimTracks[it].trackId()<<", "<<it<<"]"<<std::endl;
  }  



  // ===============================
  //      Loop over the SimHits
  // ===============================

  for (std::vector<PSimHit>::const_iterator iHit = theSimHits.begin(); iHit != theSimHits.end(); ++iHit) {
    DetId theDetUnitId((*iHit).detUnitId());
    DetId simdetid= DetId((*iHit).detUnitId());


    int pid            = (*iHit).particleType();
    int process        = (*iHit).processType();
    double time        = (*iHit).timeOfFlight();
    double log_time    = log10((*iHit).timeOfFlight());
    double log_energy  = log10((*iHit).momentumAtEntry().perp()*1000); // MeV
    double log_deposit = log10((*iHit).energyLoss()*1000000);          // keV



    if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::RPC){ // Only RPCs

      // RPC Geometry
      // ============
      RPCDetId rollId(theDetUnitId);
      RPCGeomServ rpcsrv(rollId);
      const RPCRoll* rollasociated = rpcGeom->roll(rollId);
      const BoundPlane & RPCSurface = rollasociated->surface();
      GlobalPoint RPCGlobalPoint = RPCSurface.toGlobal((*iHit).localPosition());
      if(debug) {
	std::cout<<"RPC SimHit in "<<std::setw(12)<<(int)rollId<< /*" a.k.a. "<<std::setw(24)<<rpcsrv.name()<<" details: "<<*/ std::setw(24)<<rollId;
	std::cout<<" | time t = "<<std::setw(12)<<(*iHit).timeOfFlight()<<" | z = "<<std::setw(12)<<RPCGlobalPoint.z();
	std::cout<<" | r = "<<std::setw(12)<<RPCGlobalPoint.mag()<<" | phi = "<<std::setw(12)<<RPCGlobalPoint.phi()<<" | eta = "<<std::setw(12)<<RPCGlobalPoint.eta();
	std::cout<<" | global position = "<<RPCGlobalPoint<<std::endl;
      }
      double RPC_GlobalPoint_R = sqrt(pow(RPCGlobalPoint.x(),2)+pow(RPCGlobalPoint.y(),2));

      // SIMHIT Investigations
      // =====================
      /*
      // get track
      int trackId = (*iHit).trackId();
      const SimTrack& track = theSimTracks[trackId];
      // get vertex
      int vertexId = track.vertIndex();
      const SimVertex& vertex = theSimVertices[vertexId];
      // get mother
      int motherId = -1;
      if( !vertex.noParent() ) { // there is a parent to this vertex
	// geant id of the mother
	unsigned motherGeantId =   vertex.parentIndex(); 
	std::map<unsigned, unsigned >::iterator association = geantToIndex.find( motherGeantId );
	if(association != geantToIndex.end() ) {
	  motherId = association->second;
	}
      }
      // int originId = motherId == - 1 ? -1 : myTracks[motherId];
      std::cout<<"TrackId = "<<trackId<<" VertexId = "<<vertexId<<" OriginId = "<<motherId<<std::endl;
      */

      // count hits
      // ----------
      int region = abs(rollId.region());
      int station = abs(rollId.station())-1;
      int ring = (region==0)? abs(rollId.ring()) : abs(rollId.ring())-1;
      ++RPC_hits_array[region][station][ring];


      // RPCb
      // ----
      if(rollId.region() == 0) {
	if(abs(pid)==11)      { RPCb_Electrons_SHPT->Fill(process);        RPCb_el_deps->Fill(log_deposit);}
	else if(abs(pid)==13) { RPCb_Muons_SHPT->Fill(process);            RPCb_mu_deps->Fill(log_deposit);}
	else                  { RPCb_Hadrons_SHPT->Fill(process);          RPCb_ha_deps->Fill(log_deposit);}
	RPCb_XY->Fill(RPCGlobalPoint.x(), RPCGlobalPoint.y());             
	RPCb_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));  
	Muon_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));  

	if((*iHit).timeOfFlight()<250) {
	  if(abs(pid)==11)      { Muon_000ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  else if(abs(pid)==13) { Muon_000ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  else                  { Muon_000ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  if((*iHit).timeOfFlight()<50) {
	    if(abs(pid)==11)      { Muon_00ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); RPCb_Electrons_000ns_SHPT->Fill(process);}
	    else if(abs(pid)==13) { Muon_00ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	    else                  { Muon_00ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  }
	  if((*iHit).timeOfFlight()>50) {
	    if(abs(pid)==11)      { Muon_50ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); RPCb_Electrons_050ns_SHPT->Fill(process);}
	    else if(abs(pid)==13) { Muon_50ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	    else                  { Muon_50ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  }
	  RPCb_000ns_XY->Fill(RPCGlobalPoint.x(), RPCGlobalPoint.y());
	  RPCb_000ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));
	  Muon_000ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); 
	}
	if((*iHit).timeOfFlight()>250) {
	  if(abs(pid)==11)      { Muon_250ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); RPCb_Electrons_250ns_SHPT->Fill(process);}
	  else if(abs(pid)==13) { Muon_250ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  else {                  Muon_250ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  RPCb_250ns_XY->Fill(RPCGlobalPoint.x(), RPCGlobalPoint.y());
	  RPCb_250ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));
	  Muon_250ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); 
	}

	// std::cout<<"RPCb :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and deposit deposit "<<(*iHit).energyLoss()<< " [GeV]";
	// std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [MeV]"<<std::endl;
	if(abs(pid)==2212)      {RPCb_p_hits->Fill(log_energy,log_time); RPCb_p_deposits->Fill(log_deposit,log_time);}
	else if(abs(pid)==2112) {RPCb_n_hits->Fill(log_energy,log_time); RPCb_n_deposits->Fill(log_deposit,log_time);}
	else { 
	  switch (abs(pid)%1000) {
	    // leptons
	  case 11:  RPCb_el_hits->Fill(log_energy,log_time); RPCb_el_deposits->Fill(log_deposit,log_time); break;
	  case 13:  RPCb_mu_hits->Fill(log_energy,log_time); RPCb_mu_deposits->Fill(log_deposit,log_time); break;
	    // Pions
	  case 111: RPCb_pi_hits->Fill(log_energy,log_time); RPCb_pi_deposits->Fill(log_deposit,log_time); break;
	  case 211: RPCb_pi_hits->Fill(log_energy,log_time); RPCb_pi_deposits->Fill(log_deposit,log_time); break;
	  case 130: RPCb_pi_hits->Fill(log_energy,log_time); RPCb_pi_deposits->Fill(log_deposit,log_time); break;
	    // Kaons
	  case 310: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 311: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 321: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 313: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 323: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 315: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 325: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 317: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 327: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 319: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	  case 329: RPCb_ka_hits->Fill(log_energy,log_time); RPCb_ka_deposits->Fill(log_deposit,log_time); break;
	    // Protons
	    // case 2212: RPCb_p_hits->Fill(log_energy,log_time); RPCb_p_deposits->Fill(log_deposit,log_time); break;
	    // Neutrons
	    // case 2112: RPCb_n_hits->Fill(log_energy,log_time); RPCb_n_deposits->Fill(log_deposit,log_time); break;
	    // Photons
	  case 22:   RPCb_g_hits->Fill(log_energy,log_time); RPCb_g_deposits->Fill(log_deposit,log_time); break;
	    // Nucleons
	  default:   {
	    RPCb_N_hits->Fill(log_energy,log_time); RPCb_N_deposits->Fill(log_deposit,log_time);
	    std::cout<<"RPCb :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and deposit deposit "<<(*iHit).energyLoss()<< " [GeV]";
	    std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [keV]"<<std::endl;
	    break;
	  }
	  }
	}
	RPCb_hits_tof->Fill(log_time);
	RPCb_hits_eta->Fill(fabs(RPCGlobalPoint.eta()));
	RPCb_hits_phi->Fill(RPCGlobalPoint.phi());
	RPCb_hits_lin->Fill(time);
	if(rollId.station() == 4) { RB4_hits_tof->Fill(log_time); RB4_hits_phi->Fill(RPCGlobalPoint.phi()); }
      }
      if(rollId.region() != 0) {
	if(abs(pid)==11)      { RPCf_Electrons_SHPT->Fill(process);        RPCf_el_deps->Fill(log_deposit);}
	else if(abs(pid)==13) { RPCf_Muons_SHPT->Fill(process);            RPCf_mu_deps->Fill(log_deposit);}
	else                  { RPCf_Hadrons_SHPT->Fill(process);          RPCf_ha_deps->Fill(log_deposit);}
	RPCf_XY->Fill(RPCGlobalPoint.x(), RPCGlobalPoint.y());             
	RPCf_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));  
	Muon_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));  

	if((*iHit).timeOfFlight()<250) {
	  if(abs(pid)==11)      { Muon_000ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  else if(abs(pid)==13) { Muon_000ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  else                  { Muon_000ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  if((*iHit).timeOfFlight()<50) {
	    if(abs(pid)==11)      { Muon_00ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); RPCf_Electrons_000ns_SHPT->Fill(process);}
	    else if(abs(pid)==13) { Muon_00ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	    else                  { Muon_00ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  }
	  if((*iHit).timeOfFlight()>50) {
	    if(abs(pid)==11)      { Muon_50ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); RPCf_Electrons_050ns_SHPT->Fill(process);}
	    else if(abs(pid)==13) { Muon_50ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	    else                  { Muon_50ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  }
	  RPCf_000ns_XY->Fill(RPCGlobalPoint.x(), RPCGlobalPoint.y());
	  RPCf_000ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));
	  Muon_000ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); 
	}
	if((*iHit).timeOfFlight()>250) {
	  if(abs(pid)==11)      { Muon_250ns_el_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); RPCf_Electrons_250ns_SHPT->Fill(process);}
	  else if(abs(pid)==13) { Muon_250ns_mu_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  else                  { Muon_250ns_ha_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));}
	  RPCf_250ns_XY->Fill(RPCGlobalPoint.x(), RPCGlobalPoint.y());
	  RPCf_250ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R));
	  Muon_250ns_RZ->Fill(fabs(RPCGlobalPoint.z()), fabs(RPC_GlobalPoint_R)); 
	}
	// std::cout<<"RPCf :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and energy deposit "<<(*iHit).energyLoss()<<" [GeV]";
	// std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [MeV]"<<std::endl;
	if(abs(pid)==2212)      {RPCf_p_hits->Fill(log_energy,log_time); RPCf_p_deposits->Fill(log_deposit,log_time);}
	else if(abs(pid)==2112) {RPCf_n_hits->Fill(log_energy,log_time); RPCf_n_deposits->Fill(log_deposit,log_time);}
	else { 
	  switch (abs(pid)%1000) {
	    // leptons
	  case 11:  RPCf_el_hits->Fill(log_energy,log_time); RPCf_el_deposits->Fill(log_deposit,log_time); break;
	  case 13:  RPCf_mu_hits->Fill(log_energy,log_time); RPCf_mu_deposits->Fill(log_deposit,log_time); break;
	    // Pions
	  case 111: RPCf_pi_hits->Fill(log_energy,log_time); RPCf_pi_deposits->Fill(log_deposit,log_time); break;
	  case 211: RPCf_pi_hits->Fill(log_energy,log_time); RPCf_pi_deposits->Fill(log_deposit,log_time); break;
	  case 130: RPCf_pi_hits->Fill(log_energy,log_time); RPCf_pi_deposits->Fill(log_deposit,log_time); break;
	    // Kaons
	  case 310: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 311: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 321: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 313: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 323: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 315: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 325: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 317: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 327: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 319: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	  case 329: RPCf_ka_hits->Fill(log_energy,log_time); RPCf_ka_deposits->Fill(log_deposit,log_time); break;
	    // Protons
	    // case 2212: RPCf_p_hits->Fill(log_energy,log_time); RPCf_p_deposits->Fill(log_deposit,log_time); break;
	    // Neutrons
	    // case 2112: RPCf_n_hits->Fill(log_energy,log_time); RPCf_n_deposits->Fill(log_deposit,log_time); break;
	    // Photons
	  case 22:   RPCf_g_hits->Fill(log_energy,log_time); RPCf_g_deposits->Fill(log_deposit,log_time); break;
	    // Nucleons
	  default:   {
	    RPCf_N_hits->Fill(log_energy,log_time); RPCf_N_deposits->Fill(log_deposit,log_time); 
	    std::cout<<"RPCf :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and energy deposit "<<(*iHit).energyLoss()<<" [GeV]";
	    std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [keV]"<<std::endl;
	    break;
	  }
	  }
	}
	RPCf_hits_tof->Fill(log_time);
	RPCf_hits_eta->Fill(fabs(RPCGlobalPoint.eta()));
	RPCf_hits_phi->Fill(RPCGlobalPoint.phi());
	RPCf_hits_lin->Fill(time);
	if(rollId.station() == 4) { RE4_hits_tof->Fill(log_time); RE4_hits_phi->Fill(RPCGlobalPoint.phi()); }
      }
    }

    if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::CSC){ // Only CSCs
      CSCDetId rollId(theDetUnitId);
      GlobalPoint CSCGlobalPoint = cscGeom->idToDet(rollId)->toGlobal((*iHit).localPosition());
      if (debug) {
       std::cout<<"CSC SimHit in "<<std::setw(24)<<rollId<<" | time t = "<<std::setw(12)<<(*iHit).timeOfFlight()<<" | z = "<<std::setw(12)<<CSCGlobalPoint.z();
       std::cout<<" | r = "<<std::setw(12)<<CSCGlobalPoint.mag()<<" | phi = "<<std::setw(12)<<CSCGlobalPoint.phi()<<" | eta = "<<std::setw(12)<<CSCGlobalPoint.eta();
       std::cout<<" | global position = "<<CSCGlobalPoint<<std::endl;
      }
      double CSC_GlobalPoint_R = sqrt(pow(CSCGlobalPoint.x(),2)+pow(CSCGlobalPoint.y(),2));
      if(abs(pid)==11)      { CSC_Electrons_SHPT->Fill(process);        CSC_el_deps->Fill(log_deposit);}
      else if(abs(pid)==13) { CSC_Muons_SHPT->Fill(process);            CSC_mu_deps->Fill(log_deposit);}
      else                  { CSC_Hadrons_SHPT->Fill(process);          CSC_ha_deps->Fill(log_deposit);}
      CSC_XY->Fill(CSCGlobalPoint.x(), CSCGlobalPoint.y());             
      CSC_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));  
      Muon_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));  
      
      if((*iHit).timeOfFlight()<250) {
	if(abs(pid)==11)      { Muon_000ns_el_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	else if(abs(pid)==13) { Muon_000ns_mu_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	else                  { Muon_000ns_ha_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	if((*iHit).timeOfFlight()<50) {
	  if(abs(pid)==11)      { Muon_00ns_el_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R)); CSC_Electrons_000ns_SHPT->Fill(process);}
	  else if(abs(pid)==13) { Muon_00ns_mu_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	  else                  { Muon_00ns_ha_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	}
	if((*iHit).timeOfFlight()>50) {
	  if(abs(pid)==11)      { Muon_50ns_el_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R)); CSC_Electrons_050ns_SHPT->Fill(process);}
	  else if(abs(pid)==13) { Muon_50ns_mu_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	  else                  { Muon_50ns_ha_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	}
	CSC_000ns_XY->Fill(CSCGlobalPoint.x(), CSCGlobalPoint.y());
	CSC_000ns_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));
	Muon_000ns_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R)); 
      }
      if((*iHit).timeOfFlight()>250) {
	if(abs(pid)==11)      { Muon_250ns_el_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R)); CSC_Electrons_250ns_SHPT->Fill(process);}
	else if(abs(pid)==13) { Muon_250ns_mu_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	else                  { Muon_250ns_ha_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));}
	CSC_250ns_XY->Fill(CSCGlobalPoint.x(), CSCGlobalPoint.y());
	CSC_250ns_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R));
	Muon_250ns_RZ->Fill(fabs(CSCGlobalPoint.z()), fabs(CSC_GlobalPoint_R)); 
      }
      
      // int pid           = (*iHit).particleType();
      // int process       = (*iHit).processType();
      if(debug) std::cout<<"SimHit from particle = "<<pid<<" created in process = "<<process<<" aka "<<proc[process]<<std::endl;
      /*
      // get track
      int trackIdG = (*iHit).trackId();                     // GEANT TRACK ID
      int trackIdC = geantToIndex.find(trackIdG)->second;   // CMSSW VECTOR ID
      const SimTrack& track = theSimTracks[trackIdC];
      std::cout<<"TrackId Geant = "<<std::setw(7)<<trackIdG<<" TrackId CMSSW = "<<std::setw(5)<<trackIdC<<" track = "<<&track;
      std::cout<<" SimHit Particle Type = "<<std::setw(5)<<pid<<" SimTrack Particle Type = "<<std::setw(5)<<track.type()<<std::endl;
      */
      /*
      // get vertex        
      int vertexId = track.vertIndex();
      std::cout<<" VertexId = "<<vertexId<<std::endl;
      const SimVertex& vertex = theSimVertices[vertexId];
      process = vertex.processType();
      std::cout<<" VertexId = "<<vertexId<<" Vertex = "<<&vertex<<" Process = "<<process<<" aka "<<proc[process]<<std::endl;
      */
      // get mother
      /*
      int motherId = -1;
      if( !vertex.noParent() ) { // there is a parent to this vertex
	// geant id of the mother
	unsigned motherGeantId =   vertex.parentIndex(); 
	std::map<unsigned, unsigned >::iterator association = geantToIndex.find( motherGeantId );
	if(association != geantToIndex.end() ) {
	  motherId = association->second;
	}
      }
      // int originId = motherId == - 1 ? -1 : myTracks[motherId];
      std::cout<<"TrackId = "<<trackIdG<<" VertexId = "<<vertexId<<" OriginId = "<<motherId<<std::endl;
      const SimTrack& mother = theSimTracks[motherId];
      int motherGenId =  mother.genpartIndex();
      std::cout<<"TrackId = "<<trackIdG<<" VertexId = "<<vertexId<<" OriginId = "<<motherId<<" GenParicleId = "<<motherGenId<<std::endl;
      */

      // std::cout<<"CSC :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and energy deposit "<<(*iHit).energyLoss()<< " [GeV]";
      // std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [MeV]"<<std::endl;
      if(abs(pid)==2212)      {CSC_p_hits->Fill(log_energy,log_time); CSC_p_deposits->Fill(log_deposit,log_time);}
      else if(abs(pid)==2112) {CSC_n_hits->Fill(log_energy,log_time); CSC_n_deposits->Fill(log_deposit,log_time);}
      else {
	switch (abs(pid)%1000) {
	  // leptons
	case 11:  CSC_el_hits->Fill(log_energy,log_time); CSC_el_deposits->Fill(log_deposit,log_time); break;
	case 13:  CSC_mu_hits->Fill(log_energy,log_time); CSC_mu_deposits->Fill(log_deposit,log_time); break;
	  // Pions
	case 111: CSC_pi_hits->Fill(log_energy,log_time); CSC_pi_deposits->Fill(log_deposit,log_time); break;
	case 211: CSC_pi_hits->Fill(log_energy,log_time); CSC_pi_deposits->Fill(log_deposit,log_time); break;
	case 130: CSC_pi_hits->Fill(log_energy,log_time); CSC_pi_deposits->Fill(log_deposit,log_time); break;
	  // Kaons
	case 310: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 311: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 321: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 313: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 323: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 315: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 325: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 317: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 327: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 319: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	case 329: CSC_ka_hits->Fill(log_energy,log_time); CSC_ka_deposits->Fill(log_deposit,log_time); break;
	  // Protons
	  // case 2212: CSC_p_hits->Fill(log_energy,log_time); CSC_p_deposits->Fill(log_deposit,log_time); break;
	  // Neutrons
	  // case 2112: CSC_n_hits->Fill(log_energy,log_time); CSC_n_deposits->Fill(log_deposit,log_time); break;
	  // Photons
	case 22:   CSC_g_hits->Fill(log_energy,log_time); CSC_g_deposits->Fill(log_deposit,log_time); break;
	  // Nucleons
	default:   {
	  CSC_N_hits->Fill(log_energy,log_time); CSC_N_deposits->Fill(log_deposit,log_time); 
	  std::cout<<"CSC :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and energy deposit "<<(*iHit).energyLoss()<< " [GeV]";
	  std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [keV]"<<std::endl;
	  break;
	}
	}
      }
      CSC_hits_tof->Fill(log_time);
      CSC_hits_eta->Fill(fabs(CSCGlobalPoint.eta()));
      CSC_hits_phi->Fill(CSCGlobalPoint.phi());
      CSC_hits_lin->Fill(time);
      if(rollId.station() == 4) { ME4_hits_tof->Fill(log_time); ME4_hits_phi->Fill(CSCGlobalPoint.phi()); }

      // Count the amount of layers within a single chamber that have a hit
      // ------------------------------------------------------------------
      // Approach :: count the number of hits in a single chamber for each simtrack
      // Assumption :: each simtrack gives only rise to one hit / chamber
      // Cannot work because electron (pid = 11) simhits are due to hadron tracks (321, 2212, ...)
      // ------------------------------------------------------------------
      // Approach :: use ordering of SimHits in CSC SimHit containter
      // Cannot work, it is possible that 1 muon radiates an electron in each gas layer of CSC
      // ------------------------------------------------------------------
      // have to ask Tim Cox for smart idea
      // ------------------------------------------------------------------
      // int trackIdG = (*iHit).trackId();                     // GEANT TRACK ID
      // int trackIdC = geantToIndex.find(trackIdG)->second;   // CMSSW VECTOR ID
      // const SimTrack& track = theSimTracks[trackIdC]; 
      // ------------------------------------------------------------------


    }
    if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::DT){
      DTWireId wireId(theDetUnitId);
      GlobalPoint DTGlobalPoint = dtGeom->idToDet(wireId)->toGlobal((*iHit).localPosition());
      // dtGeometry->idToDet(id)->surface().toGlobal(LocalPoint(point->displacement.x(), point->displacement.y(), point->displacement.z()));
      if (debug) {
       std::cout<<"DT SimHit in "<<std::setw(24)<<wireId<<" | time t = "<<std::setw(12)<<(*iHit).timeOfFlight()<<" | z = "<<std::setw(12)<<DTGlobalPoint.z();
       std::cout<<" | r = "<<std::setw(12)<<DTGlobalPoint.mag()<<" | phi = "<<std::setw(12)<<DTGlobalPoint.phi()<<" | eta = "<<std::setw(12)<<DTGlobalPoint.eta();
       std::cout<<" | global position = "<<DTGlobalPoint<<std::endl;
      }
      double DT_GlobalPoint_R = sqrt(pow(DTGlobalPoint.x(),2)+pow(DTGlobalPoint.y(),2));
      if(abs(pid)==11)      { DT_Electrons_SHPT->Fill(process);        DT_el_deps->Fill(log_deposit);}
      else if(abs(pid)==13) { DT_Muons_SHPT->Fill(process);            DT_mu_deps->Fill(log_deposit);}
      else                  { DT_Hadrons_SHPT->Fill(process);          DT_ha_deps->Fill(log_deposit);}
      DT_XY->Fill(DTGlobalPoint.x(), DTGlobalPoint.y());             
      DT_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));  
      Muon_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));  
      
      if((*iHit).timeOfFlight()<250) {
	if(abs(pid)==11)      { Muon_000ns_el_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	else if(abs(pid)==13) { Muon_000ns_mu_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	else                  { Muon_000ns_ha_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	if((*iHit).timeOfFlight()<50) {
	  if(abs(pid)==11)      { Muon_00ns_el_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R)); DT_Electrons_000ns_SHPT->Fill(process);}
	  else if(abs(pid)==13) { Muon_00ns_mu_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	  else                  { Muon_00ns_ha_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	}
	if((*iHit).timeOfFlight()>50) {
	  if(abs(pid)==11)      { Muon_50ns_el_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R)); DT_Electrons_050ns_SHPT->Fill(process);}
	  else if(abs(pid)==13) { Muon_50ns_mu_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	  else                  { Muon_50ns_ha_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	}
	DT_000ns_XY->Fill(DTGlobalPoint.x(), DTGlobalPoint.y());
	DT_000ns_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));
	Muon_000ns_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R)); 
      }
      if((*iHit).timeOfFlight()>250) {
	if(abs(pid)==11)      { Muon_250ns_el_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R)); DT_Electrons_250ns_SHPT->Fill(process);}
	else if(abs(pid)==13) { Muon_250ns_mu_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	else                  { Muon_250ns_ha_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));}
	DT_250ns_XY->Fill(DTGlobalPoint.x(), DTGlobalPoint.y());
	DT_250ns_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R));
	Muon_250ns_RZ->Fill(fabs(DTGlobalPoint.z()), fabs(DT_GlobalPoint_R)); 
      }

      // int pid             = (*iHit).particleType();
      // int trackid         = (*iHit).trackId();
      // std::cout<<"TrackId = "<<trackid<<std::endl;

      // std::cout<<"DT :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and energy deposit "<<(*iHit).energyLoss()<< " [GeV]";
      // std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [MeV]"<<std::endl;
      if(abs(pid)==2212)      {DT_p_hits->Fill(log_energy,log_time); DT_p_deposits->Fill(log_deposit,log_time);}
      else if(abs(pid)==2112) {DT_n_hits->Fill(log_energy,log_time); DT_n_deposits->Fill(log_deposit,log_time);}
      else {
	switch (abs(pid)%1000) {
	  // leptons
	case 11:  DT_el_hits->Fill(log_energy,log_time); DT_el_deposits->Fill(log_deposit,log_time); break;
	case 13:  DT_mu_hits->Fill(log_energy,log_time); DT_mu_deposits->Fill(log_deposit,log_time); break;
	  // Pions
	case 111: DT_pi_hits->Fill(log_energy,log_time); DT_pi_deposits->Fill(log_deposit,log_time); break;
	case 211: DT_pi_hits->Fill(log_energy,log_time); DT_pi_deposits->Fill(log_deposit,log_time); break;
	case 130: DT_pi_hits->Fill(log_energy,log_time); DT_pi_deposits->Fill(log_deposit,log_time); break;
	  // Kaons
	case 310: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 311: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 321: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 313: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 323: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 315: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 325: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 317: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 327: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 319: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	case 329: DT_ka_hits->Fill(log_energy,log_time); DT_ka_deposits->Fill(log_deposit,log_time); break;
	  // Protons
	  // case 2212: DT_p_hits->Fill(log_energy,log_time); DT_p_deposits->Fill(log_deposit,log_time); break;
	  // Neutrons
	  // case 2112: DT_n_hits->Fill(log_energy,log_time); DT_n_deposits->Fill(log_deposit,log_time); break;
	  // Photons
	case 22:   DT_g_hits->Fill(log_energy,log_time); DT_g_deposits->Fill(log_deposit,log_time); break;
	  // Nucleons
	default:   {
	  DT_N_hits->Fill(log_energy,log_time); DT_N_deposits->Fill(log_deposit,log_time); 
	  std::cout<<"DT :: SimHit from Particle id "<<pid<<" with time of flight "<<(*iHit).timeOfFlight()<<" [ns] and energy deposit "<<(*iHit).energyLoss()<< " [GeV]";
	  std::cout<<" 10 log (tof) = "<<log_time<<" [ns] and 10 log (E) = "<<log_deposit<<" [keV]"<<std::endl;
	  break;
	}
	}
      }
      DT_hits_tof->Fill(log_time);
      DT_hits_eta->Fill(fabs(DTGlobalPoint.eta()));
      DT_hits_phi->Fill(DTGlobalPoint.phi());
      DT_hits_lin->Fill(time);
      if(wireId.station() == 4) { MB4_hits_tof->Fill(log_time); MB4_hits_phi->Fill(DTGlobalPoint.phi()); }
    }
  }
}

double
MyNeutronSimHitAnalyzer::getLumi(int pu, int space, int com)
{
  double rev   = 11245.0;
  double scale = 1.0e34;
  std::map<int,double> cross;
  std::map<int,double> bunches;

  cross[7]=7.31e-26;  // TOTEM Measurement
  cross[8]=7.47e-26;  // TOTEM Measurement
  cross[13]=8.0e-26;

  bunches[25]=2808.0;
  bunches[50]=1380.0;
 
  double instlumi = pu*bunches[space]*rev/cross[com]/scale; 
  return instlumi;
}

double MyNeutronSimHitAnalyzer::getPU(double lumi, int space, int com)
{
  double rev   = 11245.0;
  double scale = 1.0e34;
  std::map<int,double> cross;
  std::map<int,double> bunches;

  cross[7]=7.31e-26;  // TOTEM Measurement
  cross[8]=7.47e-26;  // TOTEM Measurement
  cross[13]=8.0e-26;

  bunches[25]=2808.0;
  bunches[50]=1380.0;

  double pu = lumi/bunches[space]/rev*cross[com]*scale;
  return pu;
}


// ------------ method called once each job just before starting event loop  ------------
void 
MyNeutronSimHitAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MyNeutronSimHitAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
MyNeutronSimHitAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
  iSetup.get<MuonGeometryRecord>().get(cscGeom);
  iSetup.get<MuonGeometryRecord>().get(dtGeom);

  // Loop over RPC Geometry
  // std::cout <<"Analyze RPC Geometry :: Loop over RPC Chambers"<<std::endl;
  for (TrackingGeometry::DetContainer::const_iterator it=rpcGeom->dets().begin(); it<rpcGeom->dets().end(); ++it) {
    if( dynamic_cast< RPCChamber* >( *it ) != 0 ){
      RPCChamber* ch = dynamic_cast< RPCChamber* >( *it );
      std::vector< const RPCRoll*> rolls = (ch->rolls());
      for(std::vector<const RPCRoll*>::const_iterator r = rolls.begin();r != rolls.end(); ++r) {
	RPCDetId rpcId = (*r)->id();
	int n_strips=(*r)->nstrips();
	RPCGeomServ rpcsrv(rpcId);

	int region = abs(rpcId.region());
	int station = abs(rpcId.station())-1;
	int ring = (region==0)? abs(rpcId.ring()) : abs(rpcId.ring())-1; // region == 0 ? Barrel : Endcap

	if (region == 0) {
	  const RectangularStripTopology* top_= dynamic_cast<const RectangularStripTopology*> (&((*r)->topology()));
	  float stripl = top_->stripLength();
	  float stripw = top_->pitch();
	  rpc_barrel_area += stripw*stripl*n_strips;
	  RPC_area_array[region][station][ring] += stripw*stripl*n_strips;
	  // std::cout<<(int) rpcId<<" aka "<<rpcId<<" = "<< stripw*stripl*n_strips <<" cm2";
	  // std::cout<<"==> RPC_area_array[region="<<region<<"][station="<<station+1<<"][ring="<<ring<<"] = "<<RPC_area_array[region][station][ring]<<" cm2"<<std::endl;
	}
	if(region == 1) {
	  const TrapezoidalStripTopology* top_= dynamic_cast<const TrapezoidalStripTopology*> (&((*r)->topology()));
	  float stripl = top_->stripLength();
	  float stripw = top_->pitch();
	  rpc_endcap_area += stripw*stripl*n_strips;
	  RPC_area_array[region][station][ring] += stripw*stripl*n_strips;
	  // std::cout<<(int) rpcId<<" aka "<<rpcId<<" = "<< stripw*stripl*n_strips <<" cm2";
	  // std::cout<<" ==> RPC_area_array[region="<<region<<"][station="<<station+1<<"][ring"<<ring+1<<"] = "<<RPC_area_array[region][station][ring]<<" cm2"<<std::endl;
	}
      }
    }
  }
  // std::cout<<"RPC Barrel Active area = "<<rpc_barrel_area<<" cm2 || RPC Endcap Active area = "<<rpc_endcap_area<<" cm2"<<std::endl;
}


// ------------ method called when ending the processing of a run  ------------
void 
MyNeutronSimHitAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{

}

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MyNeutronSimHitAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MyNeutronSimHitAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyNeutronSimHitAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyNeutronSimHitAnalyzer);
