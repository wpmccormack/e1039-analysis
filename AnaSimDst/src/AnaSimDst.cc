#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <interface_main/SQEvent.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include "SQTrueEvent.h"
#include "SQTrack.h"
#include "SQDimuon.h"
#include "UtilDimuon.h"
#include "AnaSimDst.h"
using namespace std;

int AnaSimDst::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::InitRun(PHCompositeNode* topNode)
{
  int ret = GetNodes(topNode);
  if (ret != Fun4AllReturnCodes::EVENT_OK) return ret;
  MakeTree();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::process_event(PHCompositeNode* topNode)
{
  static unsigned int n_evt = 0;
  if    (++n_evt % 100000 == 0) cout << n_evt << endl;
  else if (n_evt %  10000 == 0) cout << " . " << flush;

  const double M_MU = 0.1056583745; // GeV

  ///
  /// Fill the event info
  ///
  mo_evt.proc_id = mi_evt_true->proc_id;
  for (int ii = 0; ii < 4; ii++) {
    mo_evt.par_id [ii] = mi_evt_true->par_id [ii];
    mo_evt.par_mom[ii] = mi_evt_true->par_mom[ii];
  }
  mo_evt.trig_bits  = mi_evt->get_trigger();
  mo_evt.rec_stat   = mi_srec->getRecStatus();
  mo_evt.n_dim_reco = mi_srec->getNDimuons();

  mo_dim_true.clear();
  mo_dim_reco.clear();

  ///
  /// Find the relation between true and reconstructed objects
  ///
  //IdMap_t id_trk_t2r;
  //FindTrackRelation(id_trk_t2r);
  IdMap_t id_dim_t2r;
  FindDimuonRelation(id_dim_t2r);

  ///
  /// Fill the output trees
  ///
//  trk_n = mi_vec_trk->size();
//  for (int ii = 0; ii < trk_n; ii++) {
//    SQTrack* trk = &mi_vec_trk->at(ii);
//    trk_ch[ii] = trk->charge;
//    trk_x [ii] = trk->pos_vtx.X();
//    trk_y [ii] = trk->pos_vtx.Y();
//    trk_z [ii] = trk->pos_vtx.Z();
//    trk_px[ii] = trk->mom_vtx.X();
//    trk_py[ii] = trk->mom_vtx.Y();
//    trk_pz[ii] = trk->mom_vtx.Z();
//    trk_reco_id[ii] = id_trk_t2r[ii];
//    if (id_trk_t2r[ii] >= 0) {
//      SRecTrack* trk_reco = &mi_srec->getTrack(id_trk_t2r[ii]);
//      TVector3       pos = trk_reco->getVertex();
//      TLorentzVector mom = trk_reco->getMomentumVertex();
//      trk_reco_x [ii] = pos.X();
//      trk_reco_y [ii] = pos.Y();
//      trk_reco_z [ii] = pos.Z();
//      trk_reco_px[ii] = mom.X();
//      trk_reco_py[ii] = mom.Y();
//      trk_reco_pz[ii] = mom.Z();
//    } else {
//      trk_reco_x [ii] = 0;
//      trk_reco_y [ii] = 0;
//      trk_reco_z [ii] = 0;
//      trk_reco_px[ii] = 0;
//      trk_reco_py[ii] = 0;
//      trk_reco_pz[ii] = 0;
//    }
//  }

  int dim_n = mi_vec_dim->size();
  for (int ii = 0; ii < dim_n; ii++) {
    SQDimuon* dim = &mi_vec_dim->at(ii);
    DimuonData dd;
    dd.x    = dim->pos.X();
    dd.y    = dim->pos.Y();
    dd.z    = dim->pos.Z();
    dd.px   = dim->mom.X();
    dd.py   = dim->mom.Y();
    dd.pz   = dim->mom.Z();
    dd.mass = dim->mom.M();
    dd.eta  = dim->mom.Eta();
    dd.phi  = dim->mom.Phi();
    UtilDimuon::GetX1X2(dim, dd.x1, dd.x2);

    SQTrack* trk_pos = &mi_vec_trk->at(dim->track_idx_pos);
    dd.pos_px = trk_pos->mom_vtx.X();
    dd.pos_py = trk_pos->mom_vtx.Y();
    dd.pos_pz = trk_pos->mom_vtx.Z();
    SQTrack* trk_neg = &mi_vec_trk->at(dim->track_idx_neg);
    dd.neg_px = trk_neg->mom_vtx.X();
    dd.neg_py = trk_neg->mom_vtx.Y();
    dd.neg_pz = trk_neg->mom_vtx.Z();

    mo_dim_true.push_back(dd);

    DimuonData ddr;
    if (id_dim_t2r[ii] >= 0) {
      SRecDimuon dim_reco = mi_srec->getDimuon(id_dim_t2r[ii]);
      TVector3       pos = dim_reco.vtx;
      TLorentzVector mom = dim_reco.p_pos + dim_reco.p_neg;
      ddr.x    = pos.X();
      ddr.y    = pos.Y();
      ddr.z    = pos.Z();
      ddr.px   = mom.X();
      ddr.py   = mom.Y();
      ddr.pz   = mom.Z();
      ddr.mass = dim_reco.mass; // mom.M();
      ddr.eta  = mom.Eta();
      ddr.phi  = mom.Phi();
      ddr.x1   = dim_reco.x1;
      ddr.x2   = dim_reco.x2;

      dd.pos_px = dim_reco.p_pos.X();
      dd.pos_py = dim_reco.p_pos.Y();
      dd.pos_pz = dim_reco.p_pos.Z();
      dd.neg_px = dim_reco.p_neg.X();
      dd.neg_py = dim_reco.p_neg.Y();
      dd.neg_pz = dim_reco.p_neg.Z();
    }
    mo_dim_reco.push_back(ddr);
  }

  tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::End(PHCompositeNode* topNode)
{
  file->cd();
  file->Write();
  file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::GetNodes(PHCompositeNode *topNode)
{
  mi_evt      = findNode::getClass<SQEvent       >(topNode, "SQEvent");
  mi_srec     = findNode::getClass<SRecEvent     >(topNode, "SRecEvent");
  mi_evt_true = findNode::getClass<SQTrueEvent   >(topNode, "SQTrueEvent");
  mi_vec_trk  = findNode::getClass<SQTrackVector >(topNode, "SQTrueTrackVector");
  mi_vec_dim  = findNode::getClass<SQDimuonVector>(topNode, "SQTrueDimuonVector");
  if (!mi_evt || !mi_srec || !mi_evt_true || !mi_vec_trk || !mi_vec_dim) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

void AnaSimDst::MakeTree()
{
  file = new TFile("sim_tree.root", "RECREATE");
  tree = new TTree("tree", "Created by AnaSimDst");

  tree->Branch("evt"     , &mo_evt);
  tree->Branch("dim_true", &mo_dim_true);
  tree->Branch("dim_reco", &mo_dim_reco);
}

void AnaSimDst::FindTrackRelation(IdMap_t& id_map)
{
  id_map.clear();
  for (unsigned int i_true = 0; i_true < mi_vec_trk->size(); i_true++) {
    SQTrack* trk_true = &mi_vec_trk->at(i_true);
    int     ch_true = trk_true->charge;
    double mom_true = trk_true->mom_vtx.Mag();

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

void AnaSimDst::FindDimuonRelation(IdMap_t& id_map)
{
  id_map.clear();
  for (unsigned int i_true = 0; i_true < mi_vec_dim->size(); i_true++) {
    SQDimuon* dim_true = &mi_vec_dim->at(i_true);
    double mass_true = dim_true->mom.M();

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

