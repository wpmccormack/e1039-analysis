#include <TSystem.h>

R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)

/*
This macro takes severl external input files to run:
1. geom.root is the standard geometry dump from running the Fun4Sim macro;
2. data.root should be the DST file generated either by decoder or by 

This is an example script intended to demonstrate how to run SQReco in a minimalistic fashion, it is NOT
suitable for production use and users should develop their own reconstruction macro for their own analysis.
*/

int RecoE1039Data(const int nEvents = 1)
{
  const double FMAGSTR = -1.054;
  const double KMAGSTR = -0.951;

  recoConsts* rc = recoConsts::instance();
  rc->set_DoubleFlag("FMAGSTR", FMAGSTR);
  rc->set_DoubleFlag("KMAGSTR", KMAGSTR);
  rc->Print();

  Fun4AllServer* se = Fun4AllServer::instance();
  se->Verbosity(100);

  JobOptsSvc* jobopt_svc = JobOptsSvc::instance();
  jobopt_svc->init("support/e1039_sim.opts");  //at this time the simulation and data uses same configuration

  GeomSvc::UseDbSvc(true);  
  GeomSvc* geom_svc = GeomSvc::instance();

  SQReco* reco = new SQReco();
  reco->Verbosity(100);
  reco->set_geom_file_name("support/geom.root");
  reco->set_enable_KF(true); //Kalman filter not needed for the track finding, disabling KF saves a lot of initialization time
  reco->setInputTy(SQReco::E1039);    //options are SQReco::E906 and SQReco::E1039
  reco->setFitterTy(SQReco::KFREF);  //not relavant for the track finding
  reco->set_evt_reducer_opt("aoce"); //if not provided, event reducer will be using JobOptsSvc to intialize; to turn off, set it to "none"
  reco->set_enable_eval(true);
  reco->set_eval_file_name("eval.root");
  se->registerSubsystem(reco);

  Fun4AllInputManager* in = new Fun4AllDstInputManager("DSTIN");
  in->Verbosity(0);
  in->fileopen("data.root");
  se->registerInputManager(in);

  Fun4AllDstOutputManager* out = new Fun4AllDstOutputManager("DSTOUT", "result.root");
  se->registerOutputManager(out);

  se->run(nEvents);
  se->End();

  delete se;
  gSystem->Exit(0);
  return 0;
}