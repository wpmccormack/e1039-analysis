R__LOAD_LIBRARY(AnaRealDst)

/// Main function.
int Fun4RealDst(const int run=1666, const int nevent=0)
{
  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);

  string fn_in = UtilOnline::GetDstFilePath(run);
  cout << "DST file = " << fn_in << endl;
  Fun4AllInputManager *in = new Fun4AllDstInputManager("RealDst");
  in->fileopen(fn_in);
  se->registerInputManager(in);

  se->registerSubsystem(new AnaTriggerHit());
  //se->registerSubsystem(new AnaRealDst());
  //se->registerSubsystem(new AnaEffHodo());
  //se->registerSubsystem(new AnaEffCham(AnaEffCham::D3p));

  se->run(nevent);
  se->End();
  delete se;
  return 0;
}

/// Function to analyze multiple DST files.
/**
 * You first list up runs in "list_run.txt".
 * Then type the following commands to execute this function:
 *   root -b
 *   .L Fun4RealDst.C
 *   Fun4MultiRealDst();
 */
int Fun4MultiRealDst(const char* fn_list_run="list_run.txt")
{
  gSystem->Load("libana_real_dst.so");
  Fun4AllServer* se = Fun4AllServer::instance();
  //se->Verbosity(1);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("RealDst");
  se->registerInputManager(in);

  se->registerSubsystem(new AnaEffHodo());
  //se->registerSubsystem(new AnaEffCham(AnaEffCham::D3p));

  ifstream ifs(fn_list_run);
  int run;
  while (ifs >> run) {
    string fn_in = UtilOnline::GetDstFileDir() + "/" + UtilOnline::RunNum2DstFile(run);
    cout << "Run " << run << ": " << fn_in << endl;
    in->fileopen(fn_in);
    se->run();
  }
  ifs.close();

  se->End();
  delete se;
  return 0;
}
