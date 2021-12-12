//R__LOAD_LIBRARY(libfun4all)
//R__LOAD_LIBRARY(libg4detectors)
//R__LOAD_LIBRARY(libg4testbench)
//R__LOAD_LIBRARY(libg4eval)
//R__LOAD_LIBRARY(libg4dst)
//R__LOAD_LIBRARY(libSQPrimaryGen)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libGenRoadset)

int Fun4All(const int run, const string fname, const int nevent=0)
{
  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);

  //const bool use_trigger_hit = false;

  Fun4AllSRawEventInputManager* in = new Fun4AllSRawEventInputManager("SRawEventIM");
  in->Verbosity(0);
  in->enable_E1039_translation();
  se->registerInputManager(in);

  SubsysRecoNim3* sr_nim3 = new SubsysRecoNim3();
  se->registerSubsystem(sr_nim3);

  in->fileopen(fname);
  se->run(nevent);
  se->End();
  delete se;
  return 0;
}
