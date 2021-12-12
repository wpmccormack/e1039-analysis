R__LOAD_LIBRARY(libGenRoadset)
using namespace std;

/// Macro to generate a roadset based on the S/N FoM.
void GenerateRoad()
{
  /// 102: 4.0, 5.0, 1200, 0.010
  /// 103: 2.5, 3.5, 1200, 0.040
  /// 104: 2.5, 3.5, 1200, 0.033
  /// 105: 7.0, 8.0, 1200, 0.010
  /// 113: 2.5, 3.5, 1200, 0.040
  const string    rs_id = "113";
  const double  mass_lo = 2.5;
  const double  mass_hi = 3.5;
  const int    inte_cut = 1200;
  const double frac_cut = 0.040;

  AnaSignal* ana_sig = new AnaSignal();
  ana_sig->SetInputTreeName  ("signal_tree");
  ana_sig->SetInputBranchName("signal_data");
  ana_sig->SetMassRange(mass_lo, mass_hi);
  ana_sig->Init();
  ana_sig->ReadEventsFromFileList("list_signal.txt");
  ana_sig->Analyze();
  ana_sig->End();

  AnaBG* ana_bg = new AnaBG();
  ana_bg->SetInputTreeName  ("bg_tree");
  ana_bg->SetInputBranchName("bg_data");
  ana_bg->SetInteCut(inte_cut);
  ana_bg->SetRoads(ana_sig);
  ana_bg->Init();
  ana_bg->ReadEventsFromFileList("list_bg.txt");
  ana_bg->Analyze();
  ana_bg->End();

  GenRoadset* gen_rs = new GenRoadset("gen_rs" + rs_id);
  gen_rs->SetMap(ana_sig);
  gen_rs->GenerateRoadSetByFoM(frac_cut, rs_id);

  delete gen_rs;
  delete ana_bg;
  delete ana_sig;
  cout << "E N D" << endl;
  exit(0);
}

/// Macro to generate a roadset based on the GMC signal yield.
void GenerateRoadBySignal()
{
  const string    rs_id = "901"; // "103"; // "101";
  const double  mass_lo = 4.0;
  const double  mass_hi = 5.0;
  const double frac_cut = 0.05;

  AnaSignal* ana_sig = new AnaSignal();
  ana_sig->SetInputTreeName  ("signal_tree");
  ana_sig->SetInputBranchName("signal_data");
  ana_sig->SetMassRange(mass_lo, mass_hi);
  ana_sig->Init();
  ana_sig->ReadEventsFromFileList("list_signal.txt");
  ana_sig->Analyze();

  GenRoadset* gen_rs = new GenRoadset("gen_rs" + rs_id);
  gen_rs->SetMap(ana_sig);
  gen_rs->GenerateRoadSetBySignal(frac_cut, rs_id);

  ana_sig->End();

  delete gen_rs;
  delete ana_sig;
  cout << "E N D" << endl;
  exit(0);
}
