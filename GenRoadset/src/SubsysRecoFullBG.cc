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
#include "SubsysRecoFullBG.h"
using namespace std;

SubsysRecoFullBG::SubsysRecoFullBG(const std::string &name)
  : SubsysReco(name)
{
  ;
}

int SubsysRecoFullBG::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int SubsysRecoFullBG::InitRun(PHCompositeNode* topNode)
{
  mi_evt     = findNode::getClass<SQEvent    >(topNode, "SQEvent");
  mi_vec_hit = findNode::getClass<SQHitVector>(topNode, "SQHitVector");
  if (!mi_evt || !mi_vec_hit) return Fun4AllReturnCodes::ABORTEVENT;

  mo_file = new TFile("bg_data.root", "RECREATE");
  mo_tree = new TTree("bg_tree", "Created by SubsysRecoFullBG");
  mo_tree->Branch("bg_data", &mo_bg);

  h1_evt_cnt = new TH1D("h1_evt_cnt", "", 20, 0.5, 20.5);

  return Fun4AllReturnCodes::EVENT_OK;
}

int SubsysRecoFullBG::process_event(PHCompositeNode* topNode)
{
  h1_evt_cnt->Fill(1);
  h1_evt_cnt->Fill(2);
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

  mo_tree->Fill();

  return Fun4AllReturnCodes::EVENT_OK;
}

int SubsysRecoFullBG::End(PHCompositeNode* topNode)
{
  mo_file->cd();
  mo_file->Write();
  mo_file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}

void SubsysRecoFullBG::ExtractHits(const SQHitVector* hit_vec, const std::string det_name, std::vector<int>& list_ele)
{
  list_ele.clear();
  shared_ptr<SQHitVector> hv(UtilSQHit::FindFirstHits(hit_vec, det_name, true));
  for (SQHitVector::Iter it = hv->begin(); it != hv->end(); it++) {
    list_ele.push_back( (*it)->get_element_id() );
  }
}
