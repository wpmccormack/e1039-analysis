/** Fun4CODA.C:  Fun4all macro to run full reconstruction chain from CODA files. 
 * 
 * To run this macro on a local computer, you need copy Coda file and also
 *  mapping files.  You can use the following commands;
     RUN=28700
     WORK_DIR=<some-working-dir>
     
     mkdir -p $WORK_DIR/runs
     RUN6=$(printf '%06i' $RUN)
     scp -p  e906-gat6.fnal.gov:/data3/data/mainDAQ/run_$RUN6.dat $WORK_DIR
     scp -pr e906-gat6.fnal.gov:/seaquest/production/runs/run_$RUN6  $WORK_DIR/runs
 */
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <phool/recoConsts.h>
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/HepMCNodeReader.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <g4detectors/PHG4DetectorSubsystem.h>
#include <g4detectors/DPDigitizer.h>
#include <decoder_maindaq/CalibXT.h>
#include <decoder_maindaq/CalibInTime.h>
#include <phpythia8/PHPythia8.h>
#include <g4eval/PHG4DSTReader.h>
#include <jobopts_svc/JobOptsSvc.h>
#include <geom_svc/GeomSvc.h>
#include <module_example/TrkEval.h>

#include <TSystem.h>

#include "G4_SensitiveDetectors.C"
#include "G4_Target.C"

R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libonlmonserver)
R__LOAD_LIBRARY(libdecoder_maindaq)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4testbench)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
#endif

int Fun4CODA(
const int nevent = 0,
const int run = 24172,
const std::string e1030_resource = "/e906/app/software/osg/users/yuhw/e1039/resource/")
{
  gSystem->Load("libdecoder_maindaq.so");
  gSystem->Load("libonlmonserver.so");

  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors");
  gSystem->Load("libg4testbench");
  gSystem->Load("libg4eval");
  gSystem->Load("libktracker.so");

  //const char* coda_dir  = "/data3/data/mainDAQ/";
  //const char* para_dir  = "/seaquest/production/runs/";

  const char* coda_dir  = "./";
  const char* para_dir  = "./";

  const char* out_dir   = "./";

  ostringstream oss;
  oss << setfill('0') 
      << coda_dir << "/run_" << setw(6) << run << ".dat";
  string coda_file = oss.str();
  oss.str("");
  oss << out_dir << "/run_" << setw(6) << run << ".root";
  string out_dst = oss.str();

  Fun4AllServer* se = Fun4AllServer::instance();
  se->Verbosity(99);

  const double FMAGSTR = -1.044;//-1.054;
  const double KMAGSTR = -1.025;//-0.951;

  recoConsts *rc = recoConsts::instance();
  rc->set_DoubleFlag("FMAGSTR", FMAGSTR);
  rc->set_DoubleFlag("KMAGSTR", KMAGSTR);
  rc->Print();

  JobOptsSvc *jobopt_svc = JobOptsSvc::instance();
  jobopt_svc->init("e906_data.opts");

  GeomSvc *geom_svc = GeomSvc::instance();

  // Fun4All G4 module
  PHG4Reco *g4Reco = new PHG4Reco();
  //PHG4Reco::G4Seed(123);
  //g4Reco->set_field(5.);
  g4Reco->set_field_map(
      jobopt_svc->m_fMagFile+" "+
      jobopt_svc->m_kMagFile+" "+
      Form("%f",FMAGSTR) + " " +
      Form("%f",KMAGSTR) + " " +
      "5.0",
      PHFieldConfig::RegionalConst);
  // size of the world - every detector has to fit in here
  g4Reco->SetWorldSizeX(1000);
  g4Reco->SetWorldSizeY(1000);
  g4Reco->SetWorldSizeZ(5000);
  // shape of our world - it is a tube
  g4Reco->SetWorldShape("G4BOX");
  // this is what our world is filled with
  g4Reco->SetWorldMaterial("G4_AIR"); //G4_Galactic, G4_AIR
  // Geant4 Physics list to use
  g4Reco->SetPhysicsList("FTFP_BERT");

  // insensitive elements of the spectrometer
  PHG4E1039InsensSubsystem* insens = new PHG4E1039InsensSubsystem("Insens");
  g4Reco->registerSubsystem(insens);

  // collimator, targer and shielding between target and FMag
  gROOT->LoadMacro("G4_Target.C");
  SetupTarget(g4Reco);

  // sensitive elements of the spectrometer
  gROOT->LoadMacro("G4_SensitiveDetectors.C");
  SetupSensitiveDetectors(g4Reco);

  se->registerSubsystem(g4Reco);

  std:string cali_db_conf = e1030_resource + "/db_conf/my.cnf";
  std::cout << "ClibConf: " << cali_db_conf << std::endl;

  CalibInTime* cali_intime = new CalibInTime();
  cali_intime->SetDBConf(cali_db_conf);
  se->registerSubsystem(cali_intime);

  CalibXT* cali_xt = new CalibXT();
  cali_xt->SetDBConf(cali_db_conf);
  se->registerSubsystem(cali_xt);

  // trakcing module
  gSystem->Load("libktracker.so");
  KalmanFastTrackingWrapper *ktracker = new KalmanFastTrackingWrapper();
  ktracker->Verbosity(99);
  ktracker->set_enable_event_reducer(true);
  ktracker->set_DS_level(0);
  se->registerSubsystem(ktracker);

  Fun4AllEVIOInputManager *in = new Fun4AllEVIOInputManager("CODA");
  in->Verbosity(1);
  in->EventSamplingFactor(20);
  in->DirParam(para_dir);
  in->fileopen(coda_file);
  se->registerInputManager(in);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", out_dst);
  se->registerOutputManager(out);

  se->run(nevent);
  se->End();

  se->PrintTimer();
  
  delete se;
  cout << "Fun4CODA Done!" << endl;

  gSystem->Exit(0);
  return 0;
}
