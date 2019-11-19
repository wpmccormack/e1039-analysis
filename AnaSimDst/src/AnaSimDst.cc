#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <g4main/PHG4TruthInfoContainer.h>
//#include <g4main/PHG4HitContainer.h>
//#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
//#include <g4main/PHG4HitDefs.h>
#include <g4main/PHG4VtxPoint.h>
//#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQHitVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
//#include <geom_svc/GeomSvc.h>
//#include <UtilAna/UtilSQHit.h>
#include "AnaSimDst.h"
using namespace std;

int AnaSimDst::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::InitRun(PHCompositeNode* topNode)
{
  sqevt  = findNode::getClass<SQEvent               >(topNode, "SQEvent"    );
  sqhit  = findNode::getClass<SQHitVector           >(topNode, "SQHitVector");
  g4true = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  srec   = findNode::getClass<SRecEvent             >(topNode, "SRecEvent"  );
  if (!sqevt || !sqhit || !g4true || !srec) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  file   = new TFile("output.root", "RECREATE");
  tree = new TTree("tree", "Created by AnaSimDst");

  tree->Branch("trigger" , &trigger ,  "trigger/I");
  tree->Branch("rec_stat", &rec_stat, "rec_stat/I");

  tree->Branch("trk_n"      , &trk_n,       "trk_n/I");
  tree->Branch("trk_ch"     ,  trk_ch,      "trk_ch[trk_n]/I");
  tree->Branch("trk_x"      ,  trk_x,       "trk_x[trk_n]/F");
  tree->Branch("trk_y"      ,  trk_y,       "trk_y[trk_n]/F");
  tree->Branch("trk_z"      ,  trk_z,       "trk_z[trk_n]/F");
  tree->Branch("trk_px"     ,  trk_px,      "trk_px[trk_n]/F");
  tree->Branch("trk_py"     ,  trk_py,      "trk_py[trk_n]/F");
  tree->Branch("trk_pz"     ,  trk_pz,      "trk_pz[trk_n]/F");
  tree->Branch("trk_reco_id",  trk_reco_id, "trk_reco_id[trk_n]/I");
  tree->Branch("trk_reco_x" ,  trk_reco_x,  "trk_reco_x[trk_n]/F");
  tree->Branch("trk_reco_y" ,  trk_reco_y,  "trk_reco_y[trk_n]/F");
  tree->Branch("trk_reco_z" ,  trk_reco_z,  "trk_reco_z[trk_n]/F");
  tree->Branch("trk_reco_px",  trk_reco_px, "trk_reco_px[trk_n]/F");
  tree->Branch("trk_reco_py",  trk_reco_py, "trk_reco_py[trk_n]/F");
  tree->Branch("trk_reco_pz",  trk_reco_pz, "trk_reco_pz[trk_n]/F");

  tree->Branch("dim_n",        &dim_n,             "dim_n/I");
  tree->Branch("dim_x",         dim_x,             "dim_x[dim_n]/F");
  tree->Branch("dim_y",         dim_y,             "dim_y[dim_n]/F");
  tree->Branch("dim_z",         dim_z,             "dim_z[dim_n]/F");
  tree->Branch("dim_px",        dim_px,            "dim_px[dim_n]/F");
  tree->Branch("dim_py",        dim_py,            "dim_py[dim_n]/F");
  tree->Branch("dim_pz",        dim_pz,            "dim_pz[dim_n]/F");
  tree->Branch("dim_mass",      dim_mass,          "dim_mass[dim_n]/F");
  tree->Branch("dim_eta",       dim_eta,           "dim_eta[dim_n]/F");
  tree->Branch("dim_phi",       dim_phi,           "dim_phi[dim_n]/F");
  tree->Branch("dim_x1" ,       dim_x1 ,           "dim_x1[dim_n]/F");
  tree->Branch("dim_x2" ,       dim_x2 ,           "dim_x2[dim_n]/F");
  tree->Branch("dim_reco_id",   dim_reco_id,       "dim_reco_id[trk_n]/I");
  tree->Branch("dim_reco_x",    dim_reco_x,        "dim_reco_x[dim_n]/F");
  tree->Branch("dim_reco_y",    dim_reco_y,        "dim_reco_y[dim_n]/F");
  tree->Branch("dim_reco_z",    dim_reco_z,        "dim_reco_z[dim_n]/F");
  tree->Branch("dim_reco_px",   dim_reco_px,       "dim_reco_px[dim_n]/F");
  tree->Branch("dim_reco_py",   dim_reco_py,       "dim_reco_py[dim_n]/F");
  tree->Branch("dim_reco_pz",   dim_reco_pz,       "dim_reco_pz[dim_n]/F");
  tree->Branch("dim_reco_mass", dim_reco_mass,     "dim_reco_mass[dim_n]/F");
  tree->Branch("dim_reco_eta",  dim_reco_eta,      "dim_reco_eta[dim_n]/F");
  tree->Branch("dim_reco_phi",  dim_reco_phi,      "dim_reco_phi[dim_n]/F");
  tree->Branch("dim_reco_x1" ,  dim_reco_x1 ,      "dim_reco_x1[dim_n]/F");
  tree->Branch("dim_reco_x2" ,  dim_reco_x2 ,      "dim_reco_x2[dim_n]/F");

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::process_event(PHCompositeNode* topNode)
{
  const double M_MU = 0.1056583745; // GeV

  trigger  = sqevt->get_trigger();
  rec_stat = srec->getRecStatus();

  ///
  /// Construct the true track and dimuon info
  ///
  m_list_trk.clear();
  m_list_dim.clear();

  for (auto it = g4true->GetPrimaryParticleRange().first; it != g4true->GetPrimaryParticleRange().second; ++it) {
    PHG4Particle* par = it->second;
    int pid = par->get_pid();
    if (abs(pid) != 13) continue; // not muon
    SQTrack trk;
    trk.vtx_id = par->get_vtx_id();
    PHG4VtxPoint* vtx = g4true->GetVtx(trk.vtx_id);
    trk.charge = pid < 0 ? +1: -1; // -13 = mu+
    trk.pos_vtx.SetXYZ(vtx->get_x(), vtx->get_y(), vtx->get_z());
    trk.mom_vtx.SetXYZM(par->get_px(), par->get_py(), par->get_pz(), M_MU);
    m_list_trk.push_back(trk);
  }

  unsigned int n_trk = m_list_trk.size();
  for (unsigned int i1 = 0; i1 < n_trk; i1++) {
    SQTrack* trk1 = &m_list_trk[i1];
    if (trk1->charge <= 0) continue;
    for (unsigned int i2 = 0; i2 < n_trk; i2++) {
      SQTrack* trk2 = &m_list_trk[i2];
      if (trk2->charge >= 0) continue;
      if (trk1->vtx_id != trk2->vtx_id) continue;
      SQDimuon dim;
      //dim.id = 0;
      dim.pos = trk1->pos_vtx;
      dim.mom = trk1->mom_vtx + trk2->mom_vtx;
      dim.mom_pos = trk1->mom_vtx;
      dim.mom_neg = trk2->mom_vtx;
      dim.track_idx_pos = i1;
      dim.track_idx_neg = i2;
      m_list_dim.push_back(dim);
    }
  }

  ///
  /// Find the relation between true and reconstructed objects
  ///
  typedef std::map<int, int> IdMap_t; // For now the key is not ID but index.
  IdMap_t id_trk_t2r; // true to reco
  IdMap_t id_dim_t2r; // true to reco

  for (unsigned int i_true = 0; i_true < m_list_trk.size(); i_true++) {
    SQTrack* trk_true = &m_list_trk[i_true];
    int     ch_true = trk_true->charge;
    double mom_true = trk_true->mom_vtx.Mag();

    int i_reco_best = -1;
    double mom_diff_best;
    for (int i_reco = 0; i_reco < srec->getNTracks(); i_reco++) {
      SRecTrack* trk_reco = &srec->getTrack(i_reco);
      if (trk_reco->getCharge() != ch_true) continue;
      double mom_diff = fabs(trk_reco->getMomentumVertex().Mag() - mom_true);
      if (i_reco_best < 0 || mom_diff < mom_diff_best) {
        i_reco_best   = i_reco;
        mom_diff_best = mom_diff;
      }
    }
    id_trk_t2r[i_true] = i_reco_best;
  }

  for (unsigned int i_true = 0; i_true < m_list_dim.size(); i_true++) {
    SQDimuon* dim_true = &m_list_dim[i_true];
    double mass_true = dim_true->mom.M();

    int i_reco_best = -1;
    double mass_diff_best;
    for (int i_reco = 0; i_reco < srec->getNDimuons(); i_reco++) {
      SRecDimuon dim_reco = srec->getDimuon(i_reco);
      double mass_diff = fabs(dim_reco.mass - mass_true);
      if (i_reco_best < 0 || mass_diff < mass_diff_best) {
        i_reco_best   = i_reco;
        mass_diff_best = mass_diff;
      }
    }
    id_dim_t2r[i_true] = i_reco_best;
  }


  ///
  /// Fill the output trees
  ///
  trk_n = m_list_trk.size();
  for (int ii = 0; ii < trk_n; ii++) {
    SQTrack* trk = &m_list_trk[ii];
    trk_ch[ii] = trk->charge;
    trk_x [ii] = trk->pos_vtx.X();
    trk_y [ii] = trk->pos_vtx.Y();
    trk_z [ii] = trk->pos_vtx.Z();
    trk_px[ii] = trk->mom_vtx.X();
    trk_py[ii] = trk->mom_vtx.Y();
    trk_pz[ii] = trk->mom_vtx.Z();
    trk_reco_id[ii] = id_trk_t2r[ii];
    if (id_trk_t2r[ii] >= 0) {
      SRecTrack* trk_reco = &srec->getTrack(id_trk_t2r[ii]);
      TVector3       pos = trk_reco->getVertex();
      TLorentzVector mom = trk_reco->getMomentumVertex();
      trk_reco_x [ii] = pos.X();
      trk_reco_y [ii] = pos.Y();
      trk_reco_z [ii] = pos.Z();
      trk_reco_px[ii] = mom.X();
      trk_reco_py[ii] = mom.Y();
      trk_reco_pz[ii] = mom.Z();
    } else {
      trk_reco_x [ii] = 0;
      trk_reco_y [ii] = 0;
      trk_reco_z [ii] = 0;
      trk_reco_px[ii] = 0;
      trk_reco_py[ii] = 0;
      trk_reco_pz[ii] = 0;
    }
  }

  dim_n = m_list_dim.size();
  for (int ii = 0; ii < dim_n; ii++) {
    SQDimuon* dim = &m_list_dim[ii];
    dim_x   [ii] = dim->pos.X();
    dim_y   [ii] = dim->pos.Y();
    dim_z   [ii] = dim->pos.Z();
    dim_px  [ii] = dim->mom.X();
    dim_py  [ii] = dim->mom.Y();
    dim_pz  [ii] = dim->mom.Z();
    dim_mass[ii] = dim->mom.M();
    dim_eta [ii] = dim->mom.Eta();
    dim_phi [ii] = dim->mom.Phi();
    EvalX1X2(dim, dim_x1[ii], dim_x2[ii]);
    dim_reco_id[ii] = id_dim_t2r[ii];
    if (id_dim_t2r[ii] >= 0) {
      SRecDimuon dim_reco = srec->getDimuon(id_dim_t2r[ii]);
      TVector3       pos = dim_reco.vtx;
      TLorentzVector mom = dim_reco.p_pos + dim_reco.p_neg;
      dim_reco_x   [ii] = pos.X();
      dim_reco_y   [ii] = pos.Y();
      dim_reco_z   [ii] = pos.Z();
      dim_reco_px  [ii] = mom.X();
      dim_reco_py  [ii] = mom.Y();
      dim_reco_pz  [ii] = mom.Z();
      dim_reco_mass[ii] = dim_reco.mass; // mom.M();
      dim_reco_eta [ii] = mom.Eta();
      dim_reco_phi [ii] = mom.Phi();
      dim_reco_x1  [ii] = dim_reco.x1;
      dim_reco_x2  [ii] = dim_reco.x2;
    } else {
      dim_reco_x   [ii] = 0;
      dim_reco_y   [ii] = 0;
      dim_reco_z   [ii] = 0;
      dim_reco_px  [ii] = 0;
      dim_reco_py  [ii] = 0;
      dim_reco_pz  [ii] = 0;
      dim_reco_mass[ii] = 0;
      dim_reco_eta [ii] = 0;
      dim_reco_phi [ii] = 0;
      dim_reco_x1  [ii] = 0;
      dim_reco_x2  [ii] = 0;
    }
  }

  tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::End(PHCompositeNode* topNode)
{
//  ostringstream oss;
//  TCanvas* c1 = new TCanvas("c1", "");
//  c1->SetGrid();
//  for (unsigned int i_det = 0; i_det < list_det_id.size(); i_det++) {
//    h1_ele[i_det]->Draw();
//    oss.str("");
//    oss << h1_ele[i_det]->GetName() << ".png";
//    c1->SaveAs(oss.str().c_str());
//
//    h1_nhit[i_det]->Draw();
//    oss.str("");
//    oss << h1_nhit[i_det]->GetName() << ".png";
//    c1->SaveAs(oss.str().c_str());
//  }
//  delete c1;

  file->cd();
  file->Write();
  file->Close();
  
  return Fun4AllReturnCodes::EVENT_OK;
}

void AnaSimDst::EvalX1X2(const SQDimuon* dim, float& x1, float& x2)
{
  const double M_P    = 0.938;
  const double E_BEAM = 120.0;
  const TLorentzVector p_beam  (0, 0, sqrt(E_BEAM*E_BEAM - M_P*M_P), E_BEAM);
  const TLorentzVector p_target(0, 0, 0, M_P);
  const TLorentzVector p_cms = p_beam + p_target;
  TLorentzVector p_sum = dim->mom_pos + dim->mom_neg;
  x1 = (p_target*p_sum)/(p_target*p_cms);
  x2 = (p_beam  *p_sum)/(p_beam  *p_cms);
}
