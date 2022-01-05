#include <iomanip>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzRotation.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQMCEvent.h>
#include <interface_main/SQTrackVector.h>
#include <interface_main/SQDimuonVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <UtilAna/UtilDimuon.h>
#include "AnaSimDst.h"
using namespace std;

AnaSimDst::AnaSimDst() : SubsysReco("AnaSimDst")
{
  ;
}

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

  ///
  /// Event info
  ///
  mo_evt.proc_id = mi_evt_true->get_process_id();
  for (int ii = 0; ii < 4; ii++) {
    mo_evt.par_id [ii] = mi_evt_true->get_particle_id      (ii);
    mo_evt.par_mom[ii] = mi_evt_true->get_particle_momentum(ii);
  }
  mo_evt.weight     = 1.0; // mi_evt_true->get_weight();
  mo_evt.nim1       = mi_evt->get_trigger(SQEvent::NIM1);
  mo_evt.fpga1      = mi_evt->get_trigger(SQEvent::MATRIX1);
  mo_evt.trig_bits  = mi_evt->get_trigger();
  mo_evt.rec_stat   = mi_srec->getRecStatus();
  mo_evt.n_dim_true = mi_vec_dim->size();
  mo_evt.n_dim_reco = mi_srec->getNDimuons();

  int pol_sign = (mi_evt->get_event_id() % 2 == 0  ?  +1  : -1);

  m_proc_id_cnt   [mi_evt_true->get_process_id()  ]++;
  m_part_id_1_cnt [mi_evt_true->get_particle_id(0)]++;
  m_part_id_2_cnt [mi_evt_true->get_particle_id(1)]++;
  m_part_id_3_cnt [mi_evt_true->get_particle_id(2)]++;
  m_part_id_4_cnt [mi_evt_true->get_particle_id(3)]++;

  ostringstream oss;
  oss << mi_evt_true->get_particle_id(0) << " " << mi_evt_true->get_particle_id(1);
  m_part_id_12_cnt[oss.str()]++;

  ///
  /// Track info
  ///
//  IdMap_t id_trk_t2r;
//  FindTrackRelation(id_trk_t2r);
//  mo_trk_true.clear();
//  mo_trk_reco.clear();
//  for (unsigned int ii = 0; ii < mi_vec_trk->size(); ii++) {
//    SQTrack* trk = &mi_vec_trk->at(ii);
//    TrackData td;
//    td.charge  = trk->charge;
//    td.pos_vtx = trk->pos_vtx;
//    td.mom_vtx = trk->mom_vtx;
//    mo_trk_true.push_back(td);
//
//    TrackData tdr;
//    if (id_trk_t2r[ii] >= 0) {
//      SRecTrack* trk_reco = &mi_srec->getTrack(id_trk_t2r[ii]);
//      tdr.charge  = trk_reco->getCharge();
//      tdr.pos_vtx = trk_reco->getVertex();
//      tdr.mom_vtx = trk_reco->getMomentumVertex();
//    }
//    mo_trk_reco.push_back(tdr);
//  }

  ///
  /// Dimuon info
  ///
  IdMap_t id_dim_t2r;
  FindDimuonRelation(id_dim_t2r);
  mo_dim_true.clear();
  mo_dim_reco.clear();
  for (unsigned int ii = 0; ii < mi_vec_dim->size(); ii++) {
    SQDimuon* dim = mi_vec_dim->at(ii);
    DimuonData dd;
    //dd.pdg_id  = dim->get_pdg_id();
    dd.pos     = dim->get_pos();
    dd.mom     = dim->get_mom();
    dd.mom_pos = dim->get_mom_pos();
    dd.mom_neg = dim->get_mom_neg();
    //if (fabs(dd.mom.M() - 3.097) < 0.001) dd.pdg_id = 443;
    UtilDimuon::CalcVar(dim, dd.mass, dd.pT, dd.x1, dd.x2, dd.xF);
    UtilDimuon::Lab2CollinsSoper(dim, dd.costh, dd.phi_s);

    //double x1mod, x2mod;
    //double phi_s, phi_s_up;
    //CalcAngle(dd.mom_pos, dd.mom_neg,       +1, x1mod, x2mod, phi_s_up);
    //CalcAngle(dd.mom_pos, dd.mom_neg, pol_sign, x1mod, x2mod, phi_s);
    //if (dd.pT > 2) cout << "X " << dd.x1 << " " << x1mod << " "  << dd.x2 << " " << x2mod << " "  << dd.phi_s << " " << phi_s << endl;
    //dd.x1       = x1mod;
    //dd.x2       = x2mod;
    //dd.phi_s    = phi_s;
    //dd.phi_s_up = phi_s_up;
    mo_dim_true.push_back(dd);

    //double costh_dim, phi_dim;
    //UtilDimuon::Lab2CollinsSoper(dim, costh_dim, phi_dim);
    //cout << "X "<<dd.x1<<" "<<dd.x2<<" "<<x1mod<<" "<<x2mod<<endl;
    //cout << "P "<<phi_s<<" "<<phi_dim<<" "<<costh_dim<<endl;

    DimuonData ddr;
    if (id_dim_t2r[ii] >= 0) {
      SRecDimuon dim_reco = mi_srec->getDimuon(id_dim_t2r[ii]);
      ddr.pos     = dim_reco.vtx;
      ddr.mom     = dim_reco.p_pos + dim_reco.p_neg;
      ddr.mom_pos = dim_reco.p_pos;
      ddr.mom_neg = dim_reco.p_neg;
      ddr.x1      = dim_reco.x1;
      ddr.x2      = dim_reco.x2;
    }
    mo_dim_reco.push_back(ddr);
  }
  tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::End(PHCompositeNode* topNode)
{
  ostringstream oss;
  oss << "Process ID:\n";
  for (map<int, int>::iterator it = m_proc_id_cnt.begin(); it != m_proc_id_cnt.end(); it++) {
    oss << setw(10) << it->first << "\t" << setw(10) << it->second << "\n";
  }
  oss << "Particle-1&2 IDs:\n";
  for (map<string, int>::iterator it = m_part_id_12_cnt.begin(); it != m_part_id_12_cnt.end(); it++) {
    oss << setw(10) << it->first << "\t" << setw(10) << it->second << "\n";
  }
  oss << "Particle-1 ID:\n";
  for (map<int, int>::iterator it = m_part_id_1_cnt.begin(); it != m_part_id_1_cnt.end(); it++) {
    oss << setw(10) << it->first << "\t" << setw(10) << it->second << "\n";
  }
  oss << "Particle-2 ID:\n";
  for (map<int, int>::iterator it = m_part_id_2_cnt.begin(); it != m_part_id_2_cnt.end(); it++) {
    oss << setw(10) << it->first << "\t" << setw(10) << it->second << "\n";
  }
  oss << "Particle-3 ID:\n";
  for (map<int, int>::iterator it = m_part_id_3_cnt.begin(); it != m_part_id_3_cnt.end(); it++) {
    oss << setw(10) << it->first << "\t" << setw(10) << it->second << "\n";
  }
  oss << "Particle-4 ID:\n";
  for (map<int, int>::iterator it = m_part_id_4_cnt.begin(); it != m_part_id_4_cnt.end(); it++) {
    oss << setw(10) << it->first << "\t" << setw(10) << it->second << "\n";
  }
  cout << "\n" << oss.str() << endl;
 
  ofstream ofs("pid_count.txt");
  ofs << oss.str() << endl;
  ofs.close();

  file->cd();
  file->Write();
  file->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaSimDst::GetNodes(PHCompositeNode *topNode)
{
  mi_evt      = findNode::getClass<SQEvent       >(topNode, "SQEvent");
  mi_srec     = findNode::getClass<SRecEvent     >(topNode, "SRecEvent");
  mi_evt_true = findNode::getClass<SQMCEvent     >(topNode, "SQMCEvent");
  mi_vec_trk  = findNode::getClass<SQTrackVector >(topNode, "SQTruthTrackVector");
  mi_vec_dim  = findNode::getClass<SQDimuonVector>(topNode, "SQTruthDimuonVector");
  if (!mi_evt || !mi_srec || !mi_evt_true || !mi_vec_trk || !mi_vec_dim) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

void AnaSimDst::MakeTree()
{
  file = new TFile("sim_tree.root", "RECREATE");
  tree = new TTree("tree", "Created by AnaSimDst");

  //tree->Branch("sqevt"   ,  mi_evt);
  tree->Branch("evt"     , &mo_evt);
  //tree->Branch("trk_true", &mo_trk_true);
  //tree->Branch("trk_reco", &mo_trk_reco);
  tree->Branch("dim_true", &mo_dim_true);
  tree->Branch("dim_reco", &mo_dim_reco);
}

void AnaSimDst::FindTrackRelation(IdMap_t& id_map)
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

void AnaSimDst::FindDimuonRelation(IdMap_t& id_map)
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

/// Convert the momenta of two muons in the lab frame to "phi_s_tf", "xb" and "xt".
/** 
 * Imported from "asymmetry.C" written by Abinash.
 */
void AnaSimDst::CalcAngle(const TLorentzVector& mu0, const TLorentzVector& mu1, const int pol_sign, double& xb, double& xt, double& phi_s_tf)
{
  //Initial 4-vector definition
  const float pro_mass = 0.93827;
  const float beamE = 120.0;
  TLorentzVector hadron_b, hadron_t;
  hadron_b.SetPxPyPzE(0.0,0.0,TMath::Sqrt(beamE*beamE-pro_mass*pro_mass),beamE);
  hadron_t.SetPxPyPzE(0.0,0.0,0.0,pro_mass);
  TLorentzVector initial = hadron_b + hadron_t;

  TLorentzVector spin;
  spin.SetPxPyPzE(0.0, (pol_sign > 0 ? +1.0 : -1.0), 0.0, 0.0);

  TLorentzVector dimu = mu0 + mu1;
  float dimu_gmass = dimu.Mag();
    
  //boost to centre of mass
  TLorentzRotation cm_r;
  TVector3 cm_v = initial.BoostVector();
  cm_r.Boost(-cm_v);
          
  TLorentzVector hadron_b_cm = cm_r * hadron_b;
  TLorentzVector hadron_t_cm = cm_r * hadron_t;
  TLorentzVector mu0_cm = cm_r * mu0;
  TLorentzVector mu1_cm = cm_r * mu1;
  TLorentzVector dimu_cm = cm_r * dimu;
  TLorentzVector spin_cm = cm_r * spin;
          
  float s = 2.0*beamE*pro_mass + 2.0*pro_mass*pro_mass;
  float sqrt_s = TMath::Sqrt(s);
  float dimu_E = dimu_cm.E();
  float dimu_pl = dimu_cm.Pz();
  float dimu_pl_max = sqrt_s/2.0*(1-(dimu_gmass*dimu_gmass)/s);
  float xf = dimu_pl/dimu_pl_max;
  float y = 0.5*TMath::Log((dimu_E+dimu_pl)/(dimu_E-dimu_pl));
  float tau = (dimu_gmass*dimu_gmass)/s;
  xb = 0.5*(TMath::Sqrt(xf*xf+4*tau)+xf);
  xt = 0.5*(TMath::Sqrt(xf*xf+4*tau)-xf);
          
  //boost to dilepton restframe
  TLorentzRotation cm_dimu;
  TVector3 cm_v2 = dimu_cm.BoostVector();
  cm_dimu.Boost(-cm_v2);
          
  TLorentzVector hadron_b_cm_dimu = cm_dimu * hadron_b_cm;
  TLorentzVector hadron_t_cm_dimu = cm_dimu * hadron_t_cm;
  TLorentzVector mu0_cm_dimu = cm_dimu * mu0_cm;
  TLorentzVector mu1_cm_dimu = cm_dimu * mu1_cm;
  TLorentzVector dimu_cm_dimu = cm_dimu * dimu_cm;
  TLorentzVector spin_cm_dimu = cm_dimu * spin_cm;
          
  TVector3 zaxis = (hadron_b_cm_dimu.Vect()).Unit()-(hadron_t_cm_dimu.Vect()).Unit();
  TVector3 zunit = zaxis.Unit();
  TVector3 hadron_plane_norm = zunit.Cross((hadron_b_cm_dimu.Vect()).Unit());
  TVector3 lepton_plane_norm = zunit.Cross((mu0_cm_dimu.Vect()).Unit());
  TVector3 yunit = lepton_plane_norm.Unit();
  TVector3 xunit = (yunit.Cross(zunit)).Unit();

  //below are phi_s_tf from target rest frame. The frame for the Asymmetry calculation
  TVector3 spin_vect_tf = spin.Vect();
  TVector3 z_prime_unit = (hadron_b.Vect()).Unit();
  TVector3 y_prime_unit = (z_prime_unit.Cross(dimu.Vect())).Unit(); 
  TVector3 x_prime_unit = (y_prime_unit.Cross(z_prime_unit)).Unit(); 
  float cosphi_s_tf = (x_prime_unit.Dot(spin_vect_tf))/((x_prime_unit.Mag())*(spin_vect_tf.Mag()));
  //float phi_s_tf;
  if ( acos( spin_vect_tf.Dot(y_prime_unit) / ( (spin_vect_tf).Mag() * y_prime_unit.Mag() ) ) > TMath::Pi()/2 )
  {
    phi_s_tf = -acos(cosphi_s_tf);
  }
  else
  {
    phi_s_tf = acos(cosphi_s_tf);
  } 
}
