R__LOAD_LIBRARY(libGenRoadset)
using namespace std;

void ApplyRoadset2Signal()
{
  /// 101, 4.0, 5.0
  /// 102, 4.0, 5.0
  /// 103, 2.5, 3.5
  /// 104, 2.5, 3.5
  /// 105, 7.0, 8.0
  const string   rs_id = "113";
  const double mass_lo = 2.5;
  const double mass_hi = 3.5;

  ReAnaSignal* ana_sig = new ReAnaSignal("app_sig_rs" + rs_id);
  ana_sig->SetInputTreeName  ("signal_tree");
  ana_sig->SetInputBranchName("signal_data");
  ana_sig->SetMassRange(mass_lo, mass_hi);
  ana_sig->Init();
  UtilRoad::ReadFromFile(
    ana_sig->GetRoadMapPosTop(), 
    ana_sig->GetRoadMapPosBot(), 
    ana_sig->GetRoadMapNegTop(), 
    ana_sig->GetRoadMapNegBot(), 
    rs_id
    );
  ana_sig->ReadEventsFromFileList("list_signal.txt");
  ana_sig->Analyze();
  ana_sig->End();

  delete ana_sig;
  cout << "E N D" << endl;
  exit(0);
}

/*

QIE inhibit threshold = 1200 in the last E906 run.

Expected counts per spill = [N of "fired" events] / [N of all events] * [N of filled RFs]

N of filled RFs = 186e6

 */
