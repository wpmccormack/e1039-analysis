R__LOAD_LIBRARY(libGenRoadset)
using namespace std;

void ApplyRoadset2BG()
{
  /// 0 or 1200
  const string rs_id = "113";
  const int inte_cut = 0; // Default = 0, no cut.

  RoadMap map_pos_top;
  RoadMap map_pos_bot;
  RoadMap map_neg_top;
  RoadMap map_neg_bot;
  UtilRoad::ReadFromFile(&map_pos_top, &map_pos_bot, &map_neg_top, &map_neg_bot, rs_id);

  ostringstream oss;
  oss << "app_bg_rs" << rs_id << "_inte" << setfill('0') << setw(4) << inte_cut;
  ReAnaBG* ana_bg = new ReAnaBG(oss.str().c_str());
  ana_bg->SetInputTreeName  ("bg_tree");
  ana_bg->SetInputBranchName("bg_data");
  ana_bg->SetInteCut(inte_cut);
  ana_bg->SetEnabledRoads(&map_pos_top, &map_pos_bot, &map_neg_top, &map_neg_bot);
  ana_bg->Init();
  ana_bg->ReadEventsFromFileList("list_bg.txt");
  ana_bg->Analyze();
  ana_bg->End();

  delete ana_bg;
  cout << "E N D" << endl;
  exit(0);
}

// ----  Note  ------------------------------------------------------------
// QIE inhibit threshold = 1200 on average in the last E906 run.
