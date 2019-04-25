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
R__LOAD_LIBRARY(libinterface_main)
R__LOAD_LIBRARY(libonlmonserver)
R__LOAD_LIBRARY(libdecoder_maindaq)
#endif

int Fun4CODA(const int nevent = 0, const int run = 24172)
{
  gSystem->Load("libdecoder_maindaq.so");
  gSystem->Load("libonlmonserver.so");

  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors");
  gSystem->Load("libg4testbench");
  gSystem->Load("libg4eval");
  gSystem->Load("libktracker.so");

  //const char* dir_in  = "/data/e906",
  //const char* dir_in  = "/seaquest/analysis/kenichi/e1039";
  const char* dir_in  = "/data3/data/mainDAQ/";
  //const char* dir_in  = "./";
  const char* dir_out = "./";
  const bool is_online = false;

  ostringstream oss;
  oss << setfill('0') 
      << dir_in << "/run_" << setw(6) << run << ".dat";
  string fn_in = oss.str();
  oss.str("");
  oss << dir_out << "/run_" << setw(6) << run << ".root";
  string fn_out = oss.str();

  Fun4AllServer* se = 0;
  if(is_online) se = OnlMonServer::instance();
  else se = Fun4AllServer::instance();
  //se->Verbosity(0);

  const double FMAGSTR = -1.054;
  const double KMAGSTR = -0.951;

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

  se->registerSubsystem(new CalibInTime());
  se->registerSubsystem(new CalibXT());

  // trakcing module
  gSystem->Load("libktracker.so");
  KalmanFastTrackingWrapper *ktracker = new KalmanFastTrackingWrapper();
  ktracker->Verbosity(100);
  ktracker->set_enable_event_reducer(true);
  ktracker->set_DS_level(0);
  se->registerSubsystem(ktracker);

  Fun4AllEVIOInputManager *in = new Fun4AllEVIOInputManager("CODA");
  in->Verbosity(1);
  in->EventSamplingFactor(100);
  if (is_online) {
    in->PretendSpillInterval(55);
  }
  //in->DirParam("./runs");
  in->DirParam("/seaquest/production/runs");
  //in->DirParam("/data/e906/runs");
  in->fileopen(fn_in);
  se->registerInputManager(in);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", fn_out);
  se->registerOutputManager(out);

  if (is_online) { // Register the online-monitoring clients
    se->StartServer();

    OnlMonClient* ana = new OnlMonCODA();
    se->registerSubsystem(ana);
  }

  se->run(nevent);
  se->End();

  se->PrintTimer();
  
  delete se;
  cout << "Fun4CODA Done!" << endl;
  return 0;
}
