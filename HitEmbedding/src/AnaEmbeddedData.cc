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
#include <UtilAna/UtilSQHit.h>
#include <UtilAna/UtilDimuon.h>
#include "AnaEmbeddedData.h"
using namespace std;

AnaEmbeddedData::AnaEmbeddedData(const std::string name)
  : SubsysReco(name)
{
  ;
}

int AnaEmbeddedData::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaEmbeddedData::InitRun(PHCompositeNode* topNode)
{
  mi_evt     = findNode::getClass<SQEvent       >(topNode, "SQEvent");
  mi_vec_hit = findNode::getClass<SQHitVector   >(topNode, "SQHitVector");
  if (!mi_evt || !mi_vec_hit) {
    cout << PHWHERE << ":  Cannot find SQEvent and/or SQHitVector." << endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  mi_evt_emb = findNode::getClass<SQEvent       >(topNode, "SQEmbEvent");
  mi_evt_mc  = findNode::getClass<SQMCEvent     >(topNode, "SQMCEvent");
  mi_vec_trk = findNode::getClass<SQTrackVector >(topNode, "SQTruthTrackVector");
  mi_vec_dim = findNode::getClass<SQDimuonVector>(topNode, "SQTruthDimuonVector");
  mi_srec    = findNode::getClass<SRecEvent     >(topNode, "SRecEvent");
  //if (!mi_evt || !mi_evt_mc || !mi_vec_hit || !mi_vec_trk || !mi_vec_dim) return Fun4AllReturnCodes::ABORTEVENT;
  if (!mi_evt_emb) cout << "No SQEmbEvent node." << endl;
  if (!mi_evt_mc ) cout << "No SQMCEvent node." << endl;
  if (!mi_vec_trk) cout << "No SQTruthTrackVector node." << endl;
  if (!mi_vec_dim) cout << "No SQTruthDimuonVector node." << endl;
  if (!mi_srec   ) cout << "No SRecEvent node." << endl;

  mo_file = new TFile("ana_tree.root", "RECREATE");
  mo_tree = new TTree("tree", "Created by AnaEmbeddedData");
  mo_tree->Branch("evt"     , &mo_evt);
  mo_tree->Branch("trk_true", &mo_trk_true);
  mo_tree->Branch("trk_reco", &mo_trk_reco);
  mo_tree->Branch("dim_true", &mo_dim_true);
  mo_tree->Branch("dim_reco", &mo_dim_reco);

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaEmbeddedData::process_event(PHCompositeNode* topNode)
{
  //static unsigned int n_evt = 0;
  //if    (++n_evt % 100000 == 0) cout << n_evt << endl;
  //else if (n_evt %  10000 == 0) cout << " . " << flush;

  ///
  /// Event info
  ///
  mo_evt.event_id  = mi_evt->get_event_id();
  mo_evt.trig_bits = mi_evt->get_trigger();

  if (mi_srec) {
    mo_evt.rec_stat = mi_srec->getRecStatus();
  }
  if (mi_evt_mc) {
    mo_evt.weight = mi_evt_mc->get_weight();
  }

  shared_ptr<SQHitVector> hv_h1t(UtilSQHit::FindHits(mi_vec_hit, "H1T"));
  shared_ptr<SQHitVector> hv_h2t(UtilSQHit::FindHits(mi_vec_hit, "H2T"));
  shared_ptr<SQHitVector> hv_h3t(UtilSQHit::FindHits(mi_vec_hit, "H3T"));
  shared_ptr<SQHitVector> hv_h4t(UtilSQHit::FindHits(mi_vec_hit, "H4T"));
  shared_ptr<SQHitVector> hv_h1b(UtilSQHit::FindHits(mi_vec_hit, "H1B"));
  shared_ptr<SQHitVector> hv_h2b(UtilSQHit::FindHits(mi_vec_hit, "H2B"));
  shared_ptr<SQHitVector> hv_h3b(UtilSQHit::FindHits(mi_vec_hit, "H3B"));
  shared_ptr<SQHitVector> hv_h4b(UtilSQHit::FindHits(mi_vec_hit, "H4B"));
  mo_evt.n_h1x = hv_h1t->size() + hv_h1b->size();
  mo_evt.n_h2x = hv_h2t->size() + hv_h2b->size();
  mo_evt.n_h3x = hv_h3t->size() + hv_h3b->size();
  mo_evt.n_h4x = hv_h4t->size() + hv_h4b->size();

  //const vector<string> list_det = { "H1T", "H2T", "H3T", "H4T", "H1B", "H2B", "H3B", "H4B" };
  //for (vector<string>::const_iterator it = list_det.begin(); it != list_det.end(); it++) {
  //  shared_ptr<SQHitVector> hv(UtilSQHit::FindHits(mi_vec_hit, *it));
  //  cout << " " << *it;
  //  for (SQHitVector::ConstIter it2 = hv->begin(); it2 != hv->end(); it2++) {
  //    cout << ":" << (*it2)->get_hit_id();
  //  }
  //}
  //cout << "\n";

  //map<int, SQHitVector*> vec_hit_det;
  //SeparateHits(mi_vec_hit, vec_hit_det);
  //cout << "H";
  //for (map<int, SQHitVector*>::iterator it = vec_hit_det.begin(); it != vec_hit_det.end(); it++) {
  //  int det_id = it->first;
  //  if (det_id < 31 || det_id > 46) continue; // hodo only
  //  SQHitVector* hv = it->second;
  //  cout << " " << det_id << ":" << hv->size();
  //}
  //cout << "\n";

  if (mi_vec_trk) {
    mo_trk_true.clear();
    for (unsigned int ii = 0; ii < mi_vec_trk->size(); ii++) {
      SQTrack* trk = mi_vec_trk->at(ii);
      TrackData td;
      td.charge  = trk->get_charge();
      td.mom_vtx = trk->get_mom_vtx();
      //cout << " " << trk->get_track_id() << ":" << trk->get_charge();
      mo_trk_true.push_back(td);
    }
  }
  if (mi_vec_dim) {
    mo_dim_true.clear();
    for (unsigned int ii = 0; ii < mi_vec_dim->size(); ii++) {
      SQDimuon* dim = mi_vec_dim->at(ii);
      DimuonData dd;
      dd.mom     = dim->get_mom();
      dd.mom_pos = dim->get_mom_pos();
      dd.mom_neg = dim->get_mom_neg();
      mo_dim_true.push_back(dd);
      //cout << " " << dim->get_dimuon_id();
    }
  }
  if (mi_srec) {
    mo_trk_reco.clear();
    for (int ii = 0; ii < mi_srec->getNTracks(); ii++) {
      SRecTrack* trk = &mi_srec->getTrack(ii);
      TrackData td;
      td.charge  = trk->getCharge();
      td.mom_vtx = trk->getMomentumVertex();
      mo_trk_reco.push_back(td);
    }
    mo_dim_reco.clear();
    for (int ii = 0; ii < mi_srec->getNDimuons(); ii++) {
      SRecDimuon* dim = &mi_srec->getDimuon(ii);
      DimuonData dd;
      dd.mom     = dim->p_pos + dim->p_neg;
      dd.mom_pos = dim->p_pos;
      dd.mom_neg = dim->p_neg;
      mo_dim_reco.push_back(dd);
    }
  }

  //cout << "T";
  //for (unsigned int ii = 0; ii < mi_vec_trk->size(); ii++) {
  //  SQTrack* trk = mi_vec_trk->at(ii);
  //  cout << " " << trk->get_track_id() << ":" << trk->get_charge();
  //}
  //cout << "\n";

  //cout << "D";
  //for (unsigned int ii = 0; ii < mi_vec_dim->size(); ii++) {
  //  SQDimuon* dim = mi_vec_dim->at(ii);
  //  cout << " " << dim->get_dimuon_id();
  //}
  //cout << "\n";
  //
  //if (mi_srec) {
  //  cout << "R " << mi_srec->getRecStatus() << " " << mi_srec->getNTracks() << " " << mi_srec->getNDimuons() << "\n";
  //}

  //cout << endl;

  //if (mi_srec) {
  //  mo_evt.rec_stat   = mi_srec->getRecStatus();
  //  mo_evt.n_dim_reco = mi_srec->getNDimuons();
  //} else {
  //  mo_evt.rec_stat   = 0;
  //  mo_evt.n_dim_reco = 0;
  //}

  ///
  /// Track info
  ///
  //IdMap_t id_trk_t2r;
  //if (mi_srec) FindTrackRelation(id_trk_t2r);
  //mo_trk_true.clear();
  //mo_trk_reco.clear();
  //for (unsigned int ii = 0; ii < mi_vec_trk->size(); ii++) {
  //  SQTrack* trk = &mi_vec_trk->at(ii);
  //  TrackData td;
  //  td.charge  = trk->charge;
  //  td.pos_vtx = trk->pos_vtx;
  //  td.mom_vtx = trk->mom_vtx;
  //  mo_trk_true.push_back(td);
  //
  //  if (mi_srec) {
  //    TrackData tdr;
  //    if (id_trk_t2r[ii] >= 0) {
  //      SRecTrack* trk_reco = &mi_srec->getTrack(id_trk_t2r[ii]);
  //      tdr.charge  = trk_reco->getCharge();
  //      tdr.pos_vtx = trk_reco->getVertex();
  //      tdr.mom_vtx = trk_reco->getMomentumVertex();
  //    }
  //    mo_trk_reco.push_back(tdr);
  //  }
  //}

  ///
  /// Dimuon info
  ///
  //IdMap_t id_dim_t2r;
  //if (mi_srec) FindDimuonRelation(id_dim_t2r);
  //mo_dim_true.clear();
  //mo_dim_reco.clear();
  //for (unsigned int ii = 0; ii < mi_vec_dim->size(); ii++) {
  //  SQDimuon* dim = mi_vec_dim->at(ii);
  //  DimuonData dd;
  //  dd.pdg_id  = dim->get_pdg_id();
  //  dd.pos     = dim->get_pos();
  //  dd.mom     = dim->get_mom();
  //  dd.mom_pos = dim->get_mom_pos();
  //  dd.mom_neg = dim->get_mom_neg();
  //  UtilDimuon::CalcVar(dim, dd.mass, dd.pT, dd.x1, dd.x2, dd.xF, dd.costh, dd.phi);
  //  mo_dim_true.push_back(dd);
  //
  //  if (mi_srec) {
  //    DimuonData ddr;
  //    if (id_dim_t2r[ii] >= 0) {
  //      SRecDimuon dim_reco = mi_srec->getDimuon(id_dim_t2r[ii]);
  //      ddr.pos     = dim_reco.vtx;
  //      ddr.mom     = dim_reco.p_pos + dim_reco.p_neg;
  //      ddr.mom_pos = dim_reco.p_pos;
  //      ddr.mom_neg = dim_reco.p_neg;
  //      ddr.x1      = dim_reco.x1;
  //      ddr.x2      = dim_reco.x2;
  //    }
  //    mo_dim_reco.push_back(ddr);
  //  }
  //}

  mo_tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaEmbeddedData::End(PHCompositeNode* topNode)
{
  mo_file->cd();
  mo_file->Write();
  mo_file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}

void AnaEmbeddedData::SeparateHits(const SQHitVector* vec_in, std::map<int, SQHitVector*>& vec_sep, const bool in_time)
{
  SQHitVector* vec0 = vec_in->Clone();
  vec0->clear();
  for (SQHitVector::ConstIter it = mi_vec_hit->begin(); it != mi_vec_hit->end(); it++) {
    const SQHit* hit = *it;
    if (in_time && ! hit->is_in_time()) continue;
    int det_id = hit->get_detector_id();
    SQHitVector* hv = vec_sep[det_id];
    if (hv == 0) vec_sep[det_id] = hv = vec0->Clone();
    hv->push_back(hit);
  }
  delete vec0;
}

void AnaEmbeddedData::FindTrackRelation(IdMap_t& id_map)
{
  id_map.clear();
  for (unsigned int i_true = 0; i_true < mi_vec_trk->size(); i_true++) {
    SQTrack* trk_true = mi_vec_trk->at(i_true);
    int     ch_true = trk_true->get_charge();
    double mom_true = trk_true->get_mom_vtx().Mag();

    int i_reco_best = -1;
    double mom_diff_best;
    for (int i_reco = 0; i_reco < mi_srec->getNTracks(); i_reco++) {
      SRecTrack* trk_reco = &mi_srec->getTrack(i_reco);
      if (trk_reco->getCharge() != ch_true) continue;
      double mom_diff = fabs(trk_reco->getMomentumVertex().Mag() - mom_true);
      if (i_reco_best < 0 || mom_diff < mom_diff_best) {
        i_reco_best   = i_reco;
        mom_diff_best = mom_diff;
      }
    }
    id_map[i_true] = i_reco_best;
  }
}

void AnaEmbeddedData::FindDimuonRelation(IdMap_t& id_map)
{
  id_map.clear();
  for (unsigned int i_true = 0; i_true < mi_vec_dim->size(); i_true++) {
    SQDimuon* dim_true = mi_vec_dim->at(i_true);
    double mass_true = dim_true->get_mom().M();

    int i_reco_best = -1;
    double mass_diff_best;
    for (int i_reco = 0; i_reco < mi_srec->getNDimuons(); i_reco++) {
      SRecDimuon dim_reco = mi_srec->getDimuon(i_reco);
      double mass_diff = fabs(dim_reco.mass - mass_true);
      if (i_reco_best < 0 || mass_diff < mass_diff_best) {
        i_reco_best   = i_reco;
        mass_diff_best = mass_diff;
      }
    }
    id_map[i_true] = i_reco_best;
  }
}
