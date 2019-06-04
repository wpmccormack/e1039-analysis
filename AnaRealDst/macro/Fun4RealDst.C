/// Fun4MainDaq.C:  Fun4all macro to decode the MainDAQ data.
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
R__LOAD_LIBRARY(libana_real_dst)
#endif

int Fun4RealDst(const int run=46, const int nevent=0)
{
  gSystem->Load("libana_real_dst.so");

  const char* dir_in  = "/data2/analysis/kenichi/e1039/codadata";
  ostringstream oss;
  oss << setfill('0') << dir_in << "/run_" << setw(6) << run << "_spin.root";
  string fn_in = oss.str();

  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("RealDst");
  in->fileopen(fn_in);
  se->registerInputManager(in);

  SubsysReco* ana = new AnaRealDst();
  se->registerSubsystem(ana);

  se->run(nevent);
  se->End();
  delete se;
  return 0;
}
