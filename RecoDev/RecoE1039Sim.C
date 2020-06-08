#include <TSystem.h>

#include <top/G4_Beamline.C>
#include <top/G4_Target.C>
#include <top/G4_InsensitiveVolumes.C>
#include <top/G4_SensitiveDetectors.C>

R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libPHPythia8)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4testbench)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libg4dst)
R__LOAD_LIBRARY(libdptrigger)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libevt_filter)
R__LOAD_LIBRARY(libmodule_example)

using namespace std;

/*
This is an example script intended to demonstrate how to run SQReco in a minimalistic fashion, it is NOT
suitable for production use and users should develop their own reconstruction macro for their own analysis.
*/

int RecoE1039Sim(const int nevent = 10)
{
  const bool do_collimator = true;
  const bool do_target     = true;
  const bool do_shielding  = true;
  const bool do_fmag       = true;
  const bool do_kmag       = true;
  const bool do_absorber   = true;

  const double collimator_pos_z = -602.36;
  const double target_coil_pos_z = -300.;
  const double target_l = 7.9; //cm
  const double target_z = (7.9-target_l)/2.; //cm

  const double FMAGSTR = -1.054;
  const double KMAGSTR = -0.951;

  recoConsts *rc = recoConsts::instance();
  rc->set_DoubleFlag("FMAGSTR", FMAGSTR);
  rc->set_DoubleFlag("KMAGSTR", KMAGSTR);
  rc->Print();

  JobOptsSvc* jobopt_svc = JobOptsSvc::instance();
  jobopt_svc->init("support/e1039_sim.opts");

  GeomSvc::UseDbSvc(true);
  GeomSvc* geom_svc = GeomSvc::instance();

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  PHPythia8 *pythia8 = new PHPythia8();
  //pythia8->Verbosity(99);
  //pythia8->set_config_file("phpythia8_DY.cfg");
  pythia8->set_config_file("support/phpythia8_DY.cfg");
  pythia8->set_vertex_distribution_mean(0, 0, target_coil_pos_z, 0);
  se->registerSubsystem(pythia8);

  pythia8->set_trigger_AND();

  PHPy8ParticleTrigger* trigger_mup = new PHPy8ParticleTrigger();
  trigger_mup->AddParticles("-13");
  trigger_mup->SetPzHighLow(120, 15); 
  pythia8->register_trigger(trigger_mup);

  PHPy8ParticleTrigger* trigger_mum = new PHPy8ParticleTrigger();
  trigger_mum->AddParticles("13");
  trigger_mum->SetPzHighLow(120, 15); 
  pythia8->register_trigger(trigger_mum);

  HepMCNodeReader* hr = new HepMCNodeReader();
  hr->set_particle_filter_on(true);
  hr->insert_particle_filter_pid(13);
  hr->insert_particle_filter_pid(-13);
  se->registerSubsystem(hr);

  // Fun4All G4 module
  PHG4Reco* g4Reco = new PHG4Reco();
  //PHG4Reco::G4Seed(123);
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
  // shape of our world - it is a box
  g4Reco->SetWorldShape("G4BOX");
  // this is what our world is filled with
  g4Reco->SetWorldMaterial("G4_AIR"); //G4_Galactic, G4_AIR
  // Geant4 Physics list to use
  g4Reco->SetPhysicsList("FTFP_BERT");

  SetupBeamline(g4Reco, do_collimator, collimator_pos_z);
  SetupTarget(g4Reco, target_coil_pos_z, target_l, target_z, 1, 0);
  SetupInsensitiveVolumes(g4Reco, do_shielding, do_fmag, do_kmag, do_absorber);
  SetupSensitiveDetectors(g4Reco, 99);
  se->registerSubsystem(g4Reco);

  // save truth info to the Node Tree
  PHG4TruthSubsystem* truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  // apply in-acceptance cut
  se->registerSubsystem(new RequireParticlesInAcc());

  // digitizer
  DPDigitizer* digitizer = new DPDigitizer("DPDigitizer", 0);
  //digitizer->Verbosity(99);
  se->registerSubsystem(digitizer);

  DPTriggerAnalyzer* dptrigger = new DPTriggerAnalyzer();
  dptrigger->set_hit_container_choice("Vector");
  dptrigger->set_road_set_file_name(gSystem->ExpandPathName("$E1039_RESOURCE/trigger/trigger_67.txt"));
  //dptrigger->Verbosity(99);
  se->registerSubsystem(dptrigger);

  EvtFilter *evt_filter = new EvtFilter();
  //evt_filter->Verbosity(10);
  //evt_filter->set_trigger_req(1<<5);
  se->registerSubsystem(evt_filter);

  // trakcing module
  SQReco* reco = new SQReco();
  reco->Verbosity(0);
  //reco->set_geom_file_name("support/geom.root"); //not needed as it's created on the fly
  reco->set_enable_KF(true);          //Kalman filter not needed for the track finding, disabling KF saves a lot of initialization time
  reco->setInputTy(SQReco::E1039);    //options are SQReco::E906 and SQReco::E1039
  reco->setFitterTy(SQReco::KFREF);   //not relavant for the track finding
  reco->set_evt_reducer_opt("none");  //if not provided, event reducer will be using JobOptsSvc to intialize; to turn off, set it to "none", for normal tracking, set to something like "aoc"
  reco->set_enable_eval(true);        //include final track candidates in eval tree
  reco->set_eval_file_name("eval.root");
  reco->set_enable_eval_dst(true);    //include final track cnadidates in the DST tree
  //reco->add_eval_list(4);             //include back partial tracks in eval tree
  //reco->add_eval_list(3);             //include station-3+/- in eval tree
  //reco->add_eval_list(2);             //include station-2 tracks in eval tree
  se->registerSubsystem(reco);

  //Vertexing is not tested and probably does not work yet
  // VertexFit* vertexing = new VertexFit();
  // se->registerSubsystem(vertexing);

  // input - we need a dummy to drive the event loop
  Fun4AllInputManager *in = new Fun4AllDummyInputManager("DUMMY");
  se->registerInputManager(in);

  ///////////////////////////////////////////
  // Output
  ///////////////////////////////////////////
  // DST output manager, tunred off to save disk by default
  Fun4AllDstOutputManager* out = new Fun4AllDstOutputManager("DSTOUT", "DST.root");
  se->registerOutputManager(out);

  se->run(nevent);
  //PHGeomUtility::ExportGeomtry(se->topNode(),"geom.root");

  // finish job - close and save output files
  se->End();
  se->PrintTimer();
  std::cout << "All done" << std::endl;

  // cleanup - delete the server and exit
  delete se;
  gSystem->Exit(0);

  return 0;
}
