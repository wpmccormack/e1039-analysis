#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQMCEvent.h>
#include <interface_main/SQHitVector.h>
#include <interface_main/SQTrackVector.h>
#include <interface_main/SQDimuonVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
#include <UtilAna/UtilSQHit.h>
#include <UtilAna/UtilDimuon.h>
#include "GenEmbeddingData.h"
using namespace std;

GenEmbeddingData::GenEmbeddingData(const std::string name)
  : SubsysReco(name)
  , m_name_file("embedding_data.root")
  , m_name_tree("tree")
{
  ;
}

int GenEmbeddingData::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int GenEmbeddingData::InitRun(PHCompositeNode* topNode)
{
  mi_evt     = findNode::getClass<SQEvent       >(topNode, "SQEvent");
  mi_vec_hit = findNode::getClass<SQHitVector   >(topNode, "SQHitVector");
  if (!mi_evt || !mi_vec_hit) {
    cout << PHWHERE << ":  Cannot find SQEvent and/or SQHitVector." << endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  mi_sim_evt     = findNode::getClass<SQMCEvent     >(topNode, "SQMCEvent");
  mi_sim_vec_trk = findNode::getClass<SQTrackVector >(topNode, "SQTruthTrackVector");
  mi_sim_vec_dim = findNode::getClass<SQDimuonVector>(topNode, "SQTruthDimuonVector");
  //if (!mi_evt || !mi_sim_evt || !mi_vec_hit || !mi_vec_trk) return Fun4AllReturnCodes::ABORTEVENT;

  mo_file = new TFile(m_name_file.c_str(), "RECREATE");
  mo_tree = new TTree(m_name_tree.c_str(), "Created by GenEmbeddingData");
  mo_tree->Branch("emb_evt"     , &mo_evt     );
  mo_tree->Branch("emb_hit_list", &mo_hit_list);
  if (mi_sim_evt    ) mo_tree->Branch("emb_sim_evt"     , &mo_sim_evt     );
  if (mi_sim_vec_trk) mo_tree->Branch("emb_sim_trk_list", &mo_sim_trk_list);
  if (mi_sim_vec_dim) mo_tree->Branch("emb_sim_dim_list", &mo_sim_dim_list);

  return Fun4AllReturnCodes::EVENT_OK;
}

int GenEmbeddingData::process_event(PHCompositeNode* topNode)
{
  //static unsigned int n_evt = 0;
  //if    (++n_evt % 100000 == 0) cout << n_evt << endl;
  //else if (n_evt %  10000 == 0) cout << " . " << flush;

  mo_evt.run_id    = mi_evt->get_run_id();
  mo_evt.spill_id  = mi_evt->get_spill_id();
  mo_evt.event_id  = mi_evt->get_event_id();
  mo_evt.trig_bits = mi_evt->get_trigger();
  mo_evt.turn_id   = mi_evt->get_qie_turn_id();
  mo_evt.rf_id     = mi_evt->get_qie_rf_id  ();
  for (int ii = 0; ii < 33; ii++) {
    mo_evt.rf_inte[ii] = mi_evt->get_qie_rf_intensity(ii-16);
  }

  mo_hit_list.clear();
  for (SQHitVector::ConstIter it = mi_vec_hit->begin(); it != mi_vec_hit->end(); it++) {
    SQHit* hit = *it;
    int det_id = hit->get_detector_id();
    //string det_name = GeomSvc::instance()->getDetectorName(det_id);
    //if (det_name[0] == 'H' && det_name[2] != 'T' && det_name[2] != 'B') continue; // Skip Y-hodo.

    EmbHitData ehd;
    ehd.hit_id     = hit->get_hit_id();
    ehd.det_id     = det_id;
    ehd.ele_id     = hit->get_element_id();
    ehd.tdc_time   = hit->get_tdc_time();
    ehd.drift_dist = hit->get_drift_distance();
    ehd.pos        = hit->get_pos();
    mo_hit_list.push_back(ehd);
  }

  if (mi_sim_evt) {
    mo_sim_evt.weight = mi_sim_evt->get_weight();
  }
  if (mi_sim_vec_trk) {
    mo_sim_trk_list.clear();
    for (unsigned int ii = 0; ii < mi_sim_vec_trk->size(); ii++) {
      SQTrack* trk = mi_sim_vec_trk->at(ii);
      EmbSimTrackData estd;
      estd.trk_id  = trk->get_track_id();
      estd.charge  = trk->get_charge();
      estd.pos_vtx = trk->get_pos_vtx();
      estd.pos_st1 = trk->get_pos_st1();
      estd.pos_st3 = trk->get_pos_st3();
      estd.mom_vtx = trk->get_mom_vtx();
      estd.mom_st1 = trk->get_mom_st1();
      estd.mom_st3 = trk->get_mom_st3();
      mo_sim_trk_list.push_back(estd);
    }
  }
  if (mi_sim_vec_dim) {
    mo_sim_dim_list.clear();
    for (unsigned int ii = 0; ii < mi_sim_vec_dim->size(); ii++) {
      SQDimuon* dim = mi_sim_vec_dim->at(ii);
      EmbSimDimuonData esdd;
      esdd.dim_id     = dim->get_dimuon_id();
      esdd.trk_pos_id = dim->get_track_id_pos();
      esdd.trk_neg_id = dim->get_track_id_neg();
      esdd.pos        = dim->get_pos();
      esdd.mom_pos    = dim->get_mom_pos();
      esdd.mom_neg    = dim->get_mom_neg();
    }
  }

  mo_tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int GenEmbeddingData::End(PHCompositeNode* topNode)
{
  mo_file->cd();
  mo_file->Write();
  mo_file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}
