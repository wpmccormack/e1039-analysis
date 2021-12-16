R__LOAD_LIBRARY(libGenRoadset)
using namespace std;

void ApplyRoadset2Signal(const string rs_id="113")
{
  /// You have to make these parameters identical to the ones in `GenerateRoad.C`.
  double  mass_lo;
  double  mass_hi;
  if      (rs_id == "102") { mass_lo=4.0; mass_hi=5.0; }
  else if (rs_id == "103") { mass_lo=2.5; mass_hi=3.5; }
  else if (rs_id == "104") { mass_lo=2.5; mass_hi=3.5; }
  else if (rs_id == "105") { mass_lo=7.0; mass_hi=8.0; }
  else if (rs_id == "113") { mass_lo=2.5; mass_hi=3.5; }

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
