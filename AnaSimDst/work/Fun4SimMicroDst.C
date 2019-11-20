/// Fun4SimMicroDst.C:  Fun4all macro to analyze you uDST.
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
R__LOAD_LIBRARY(libana_sim_dst)
#endif

int Fun4SimMicroDst(const char* fn_udst="uDST.root")
{
  gSystem->Load("libana_sim_dst.so");

  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("SimMicroDst");
  se->registerInputManager(in);
  in->fileopen(fn_udst);

  se->registerSubsystem(new AnaSimDst());

  se->run();
  se->End();
  delete se;
  return 0;
}
