#include <iomanip>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQHitVector.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
#include <UtilAna/UtilSQHit.h>
#include <UtilAna/UtilTrigger.h>
#include "SubsysRecoNim3.h"
using namespace std;

SubsysRecoNim3::SubsysRecoNim3(const std::string &name)
  : SubsysReco(name)
{
  ;
}

int SubsysRecoNim3::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int SubsysRecoNim3::InitRun(PHCompositeNode* topNode)
{
  mi_evt     = findNode::getClass<SQEvent    >(topNode, "SQEvent");
  mi_vec_hit = findNode::getClass<SQHitVector>(topNode, "SQHitVector");
  if (!mi_evt || !mi_vec_hit) return Fun4AllReturnCodes::ABORTEVENT;

  mo_file = new TFile("bg_data.root", "RECREATE");
  mo_tree = new TTree("bg_tree", "Created by SubsysRecoNim3");
  mo_tree->Branch("bg_data", &mo_bg);

  h1_evt_cnt = new TH1D("h1_evt_cnt", "", 20, 0.5, 20.5);

  return Fun4AllReturnCodes::EVENT_OK;
}

int SubsysRecoNim3::process_event(PHCompositeNode* topNode)
{
  h1_evt_cnt->Fill(1);

  static vector<int> list_spill_ok;
  if (list_spill_ok.size() == 0) {
    cout << "Read the good-spill list." << endl;
    ifstream ifs("list_spill_good.txt");
    int sp;
    while (ifs >> sp) list_spill_ok.push_back(sp);
    ifs.close();
    if (list_spill_ok.size() == 0) {
      cout << "  No good spill was found.  Abort." << endl;
      exit(1);
    }
  }
  int spill_id = mi_evt->get_spill_id();
  if (find(list_spill_ok.begin(), list_spill_ok.end(), spill_id) == list_spill_ok.end()) return Fun4AllReturnCodes::EVENT_OK;
  h1_evt_cnt->Fill(2);

  ///
  /// Event selection
  ///
  if (! mi_evt->get_trigger(SQEvent::NIM3)) return Fun4AllReturnCodes::EVENT_OK;
  h1_evt_cnt->Fill(3);

  mo_bg.run        = mi_evt->get_run_id();
  mo_bg.evt        = mi_evt->get_event_id();
  mo_bg.fpga1      = mi_evt->get_trigger(SQEvent::MATRIX1);
  mo_bg.inte_rfp00 = mi_evt->get_qie_rf_intensity(0);

  mo_bg.inte_max = 0;
  for (int ii = -8; ii <= 8; ii++) {
    int inte = mi_evt->get_qie_rf_intensity(ii);
    if (inte > mo_bg.inte_max) mo_bg.inte_max = inte;
  }

  ExtractHits(mi_vec_hit, "H1T", mo_bg.h1t);
  ExtractHits(mi_vec_hit, "H2T", mo_bg.h2t);
  ExtractHits(mi_vec_hit, "H3T", mo_bg.h3t);
  ExtractHits(mi_vec_hit, "H4T", mo_bg.h4t);
  ExtractHits(mi_vec_hit, "H1B", mo_bg.h1b);
  ExtractHits(mi_vec_hit, "H2B", mo_bg.h2b);
  ExtractHits(mi_vec_hit, "H3B", mo_bg.h3b);
  ExtractHits(mi_vec_hit, "H4B", mo_bg.h4b);

  //FindAllRoads(hit_vec, +1, b_roads_top, b_n_top);
  //FindAllRoads(hit_vec, -1, b_roads_bot, b_n_bot);

  mo_tree->Fill();

  return Fun4AllReturnCodes::EVENT_OK;
}

int SubsysRecoNim3::End(PHCompositeNode* topNode)
{
  mo_file->cd();
  mo_file->Write();
  mo_file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}

void SubsysRecoNim3::ExtractHits(const SQHitVector* hit_vec, const std::string det_name, std::vector<int>& list_ele)
{
  list_ele.clear();
  shared_ptr<SQHitVector> hv(UtilSQHit::FindFirstHits(hit_vec, det_name, true));
  for (SQHitVector::Iter it = hv->begin(); it != hv->end(); it++) {
    list_ele.push_back( (*it)->get_element_id() );
  }
}

//void SubsysRecoNim3::FindAllRoads(const SQHitVector* hv, const int tb, std::vector<int>& roads, int* n_hit)
//{
//  if (abs(tb) != 1) return;
//  roads.clear();
//  shared_ptr<SQHitVector> hv_h1(UtilSQHit::FindFirstHits(hv, (tb > 0 ? "H1T" : "H1B"), true));
//  shared_ptr<SQHitVector> hv_h2(UtilSQHit::FindFirstHits(hv, (tb > 0 ? "H2T" : "H2B"), true));
//  shared_ptr<SQHitVector> hv_h3(UtilSQHit::FindFirstHits(hv, (tb > 0 ? "H3T" : "H3B"), true));
//  shared_ptr<SQHitVector> hv_h4(UtilSQHit::FindFirstHits(hv, (tb > 0 ? "H4T" : "H4B"), true));
//  n_hit[0] = hv_h1->size();
//  n_hit[1] = hv_h2->size();
//  n_hit[2] = hv_h3->size();
//  n_hit[3] = hv_h4->size();
//  for (SQHitVector::Iter it1 = hv_h1->begin(); it1 != hv_h1->end(); it1++) {
//    int ele1 = (*it1)->get_element_id();
//    for (SQHitVector::Iter it2 = hv_h2->begin(); it2 != hv_h2->end(); it2++) {
//      int ele2 = (*it2)->get_element_id();
//      for (SQHitVector::Iter it3 = hv_h3->begin(); it3 != hv_h3->end(); it3++) {
//        int ele3 = (*it3)->get_element_id();
//        for (SQHitVector::Iter it4 = hv_h4->begin(); it4 != hv_h4->end(); it4++) {
//          int ele4 = (*it4)->get_element_id();
//          roads.push_back( UtilTrigger::Hodo2Road(ele1, ele2, ele3, ele4, tb) );
//        }
//      }
//    }
//  }
//}
