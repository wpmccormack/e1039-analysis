#include <TSystem.h>

R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libsim_ana)

/*
This macro takes severl external input files to run:
1. geom.root is the standard geometry dump from running the Fun4Sim macro;
2. e906_run7.opts is provided
3. digit_028692_009.root is E906 run6 data, can be found at /pnfs/e906/production/digit/R009/02/86

This is an example script intended to demonstrate how to run SQReco in a minimalistic fashion, it is NOT
suitable for production use and users should develop their own reconstruction macro for their own analysis.
*/

int RecoE906Data(const int nEvents = 1)
{
  const double FMAGSTR = -1.054;
  const double KMAGSTR = -0.951;
  
  recoConsts* rc = recoConsts::instance();
  rc->set_DoubleFlag("FMAGSTR", FMAGSTR);
  rc->set_DoubleFlag("KMAGSTR", KMAGSTR);
  rc->set_IntFlag("RUNNUMBER", 6); // To select the plane geometry for E906 Run 6.

  
  //if(electron_tracking){
    rc->set_BoolFlag(
        "TRACK_ELECTRONS",
        true); // track electrons by eliminating certain muon hit requirements
    //}
  //if(do_displaced_tracking){
    rc->set_BoolFlag(
        "TRACK_DISPLACED",
        true); // track displaced particles by removing backwards extrapolation in st2+3 to st1 tracklet connection
    //}
    
  
  rc->Print();

  Fun4AllServer* se = Fun4AllServer::instance();
  se->Verbosity(0);

  SQReco* reco = new SQReco();
  reco->Verbosity(0);
  reco->set_geom_file_name("support/geom.root");
  reco->set_enable_KF(true); //Kalman filter not needed for the track finding, disabling KF saves a lot of initialization time
  reco->setInputTy(SQReco::E906);    //options are SQReco::E906 and SQReco::E1039
  reco->setFitterTy(SQReco::KFREF);  //not relavant for the track finding
  //reco->set_evt_reducer_opt("aoce"); //if not provided, event reducer will be using JobOptsSvc to intialize; to turn off, set it to "none"
  reco->set_evt_reducer_opt("aoc"); //if not provided, event reducer will be using JobOptsSvc to intialize; to turn off, set it to "none"
  reco->set_enable_eval(true);
  reco->set_eval_file_name("eval.root");
  se->registerSubsystem(reco);

  //WPM
  VertexFit* vertexing = new VertexFit();
  vertexing->Verbosity(0);
  se->registerSubsystem(vertexing);
  
  
  DataAna *sim_ana = new DataAna();
  sim_ana->Verbosity(0);
  std::string ofile = "./output906_displaced.root";
  sim_ana->set_out_name(ofile);
  sim_ana->set_legacy_rec_container(true);
  sim_ana->save_secondaries(false); // set to true to save secondaries
  //if(do_analysis){
  se->registerSubsystem(sim_ana);
  //}
  
  
  Fun4AllSRawEventInputManager* in = new Fun4AllSRawEventInputManager("SRawEventIM");
  in->Verbosity(0);
  in->set_tree_name("save");
  in->set_branch_name("rawEvent");
  in->fileopen("support/digit_028692_009.root");
  se->registerInputManager(in);

  Fun4AllDstOutputManager* out = new Fun4AllDstOutputManager("DSTOUT", "result906_displaced.root");
  //out->AddNode("SQEvent");
  //out->AddNode("SRawEvent");
  //out->AddNode("SRecEvent");
  se->registerOutputManager(out);
  // out->AddNode("SRawEvent");
  // out->AddNode("SRecEvent");
  
  se->run(nEvents);
  se->End();

  delete se;
  gSystem->Exit(0);
  return 0;
}
