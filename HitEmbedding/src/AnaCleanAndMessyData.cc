#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TH1D.h>
#include <TEfficiency.h>
#include <TCanvas.h>
#include <UtilAna/UtilSQHit.h>
#include <UtilAna/UtilDimuon.h>
#include "AnaCleanAndMessyData.h"
using namespace std;

AnaCleanAndMessyData::AnaCleanAndMessyData()
  : m_cl_file(0)
  , m_cl_tree(0)
  , m_me_file(0)
  , m_me_tree(0)
{
  ;
}

AnaCleanAndMessyData::~AnaCleanAndMessyData()
{
  ;
}

void AnaCleanAndMessyData::Init(const char* fn_clean, const char* fn_messy)
{
  m_cl_file = new TFile(fn_clean);
  m_cl_tree = (TTree*)m_cl_file->Get("tree");
  m_cl_evt      = new EventData ();
  m_cl_trk_true = new TrackList ();
  m_cl_trk_reco = new TrackList ();
  m_cl_dim_true = new DimuonList();
  m_cl_dim_reco = new DimuonList();
  m_cl_tree->SetBranchAddress("evt"     , &m_cl_evt);
  m_cl_tree->SetBranchAddress("trk_true", &m_cl_trk_true);
  m_cl_tree->SetBranchAddress("trk_reco", &m_cl_trk_reco);
  m_cl_tree->SetBranchAddress("dim_true", &m_cl_dim_true);
  m_cl_tree->SetBranchAddress("dim_reco", &m_cl_dim_reco);

  m_me_file = new TFile(fn_messy);
  m_me_tree = (TTree*)m_me_file->Get("tree");
  m_me_evt      = new EventData ();
  m_me_trk_true = new TrackList ();
  m_me_trk_reco = new TrackList ();
  m_me_dim_true = new DimuonList();
  m_me_dim_reco = new DimuonList();
  m_me_tree->SetBranchAddress("evt"     , &m_me_evt);
  m_me_tree->SetBranchAddress("trk_true", &m_me_trk_true);
  m_me_tree->SetBranchAddress("trk_reco", &m_me_trk_reco);
  m_me_tree->SetBranchAddress("dim_true", &m_me_dim_true);
  m_me_tree->SetBranchAddress("dim_reco", &m_me_dim_reco);

  gSystem->mkdir("result", true);
  m_out_file = new TFile("result/output.root", "RECREATE");
  m_h1_trk_pos_cl = new TH1D("h1_trk_pos_cl", ";RF+00;", 20, 0, 1000);
  m_h1_trk_pos_me = new TH1D("h1_trk_pos_me", ";RF+00;", 20, 0, 1000);
  m_h1_trk_neg_cl = new TH1D("h1_trk_neg_cl", ";RF+00;", 20, 0, 1000);
  m_h1_trk_neg_me = new TH1D("h1_trk_neg_me", ";RF+00;", 20, 0, 1000);
  m_h1_dim_cl     = new TH1D("h1_dim_cl"    , ";RF+00;", 20, 0, 1000);
  m_h1_dim_me     = new TH1D("h1_dim_me"    , ";RF+00;", 20, 0, 1000);
}

void AnaCleanAndMessyData::Analyze()
{
  int n_cl_evt = m_cl_tree->GetEntries();
  int n_me_evt = m_me_tree->GetEntries();
  int i_cl_evt = 0;
  int i_me_evt = 0;
  int file_id_cl = 0;
  int file_id_me = 0;

  bool no_event = false;
  while (! no_event) {
    if (i_cl_evt >= n_cl_evt || i_me_evt >= n_me_evt) return;
    m_cl_tree->GetEntry(i_cl_evt);
    m_me_tree->GetEntry(i_me_evt);

    while (file_id_cl < file_id_me) {
      int evt_id_pre = m_cl_evt->event_id;
      i_cl_evt++;
      if (i_cl_evt >= n_cl_evt) return;
      m_cl_tree->GetEntry(i_cl_evt);
      if (m_cl_evt->event_id < evt_id_pre) file_id_cl++;
    }
    while (file_id_cl > file_id_me) {
      int evt_id_pre = m_me_evt->event_id;
      i_me_evt++;
      if (i_me_evt >= n_me_evt) return;
      m_me_tree->GetEntry(i_me_evt);
      if (m_me_evt->event_id < evt_id_pre) file_id_me++;
    }

    bool file_id_changed = false;
    while (m_cl_evt->event_id != m_me_evt->event_id) {
      if (m_cl_evt->event_id < m_me_evt->event_id) {
        int evt_id_pre = m_cl_evt->event_id;
        i_cl_evt++;
        if (i_cl_evt >= n_cl_evt) return;
        m_cl_tree->GetEntry(i_cl_evt);
        if (evt_id_pre > m_cl_evt->event_id) {
          file_id_cl++;
          file_id_changed = true;
          break;
        }
      } else { // >
        int evt_id_pre = m_me_evt->event_id;
        i_me_evt++;
        if (i_me_evt >= n_me_evt) return;
        m_me_tree->GetEntry(i_me_evt);
        if (evt_id_pre > m_me_evt->event_id) {
          file_id_me++;
          file_id_changed = true;
          break;
        }
      }
    }
    if (file_id_changed) continue;

    //cout << "L " << i_cl_evt << "/" << n_cl_evt << " " << file_id_cl << ":" << m_cl_evt->event_id
    //     << "  " << i_me_evt << "/" << n_me_evt << " " << file_id_me << ":" << m_me_evt->event_id << endl;
    AnalyzeEvent();
    i_cl_evt++;
    i_me_evt++;
  }
}

void AnaCleanAndMessyData::End()
{
  if (m_cl_file) m_cl_file->Close();
  if (m_me_file) m_me_file->Close();
  if (m_out_file) DrawAndWriteOutput();
}

void AnaCleanAndMessyData::AnalyzeEvent()
{
  double ww = m_cl_evt->weight;
  int rfp01 = m_me_evt->rfp01;
  int rfp00 = m_me_evt->rfp00;
  int rfm01 = m_me_evt->rfm01;
  int n_h1x = m_me_evt->n_h1x;
  int n_h2x = m_me_evt->n_h2x;
  int n_h3x = m_me_evt->n_h3x;
  int n_h4x = m_me_evt->n_h4x;

  int n_trk_pos_cl = 0;
  int n_trk_neg_cl = 0;
  for (int ii = 0; ii < m_cl_trk_reco->size(); ii++) {
    TrackData* td = &m_cl_trk_reco->at(ii);
    if (td->charge > 0) n_trk_pos_cl++;
    else                n_trk_neg_cl++;
  }

  int n_trk_pos_me = 0;
  int n_trk_neg_me = 0;
  for (int ii = 0; ii < m_me_trk_reco->size(); ii++) {
    TrackData* td = &m_me_trk_reco->at(ii);
    if (td->charge > 0) n_trk_pos_me++;
    else                n_trk_neg_me++;
  }

  int n_dim_cl = m_cl_dim_reco->size();
  int n_dim_me = m_me_dim_reco->size();

  m_h1_trk_pos_cl->Fill(rfp00, n_trk_pos_cl);
  m_h1_trk_pos_me->Fill(rfp00, n_trk_pos_me);
  m_h1_trk_neg_cl->Fill(rfp00, n_trk_neg_cl);
  m_h1_trk_neg_me->Fill(rfp00, n_trk_neg_me);
  m_h1_dim_cl    ->Fill(rfp00, n_dim_cl    );
  m_h1_dim_me    ->Fill(rfp00, n_dim_me    );

  ///
  /// Event info
  ///
//  mo_evt.event_id  = mi_evt->get_event_id();
//  mo_evt.trig_bits = mi_evt->get_trigger();
//  mo_evt.rfp01     = mi_evt->get_qie_rf_intensity(+1);
//  mo_evt.rfp00     = mi_evt->get_qie_rf_intensity( 0);
//  mo_evt.rfm01     = mi_evt->get_qie_rf_intensity(-1);
//
//  if (mi_evt_mc) {
//    mo_evt.weight = mi_evt_mc->get_weight();
//  }
//  if (mi_srec) {
//    mo_evt.rec_stat = mi_srec->getRecStatus();
//  }
//
//  ///
//  /// Truth track/dimuon info
//  ///
//  if (mi_vec_trk) {
//    mo_trk_true.clear();
//    for (unsigned int ii = 0; ii < mi_vec_trk->size(); ii++) {
//      SQTrack* trk = mi_vec_trk->at(ii);
//      TrackData td;
//      td.charge  = trk->get_charge();
//      td.mom_vtx = trk->get_mom_vtx();
//      //cout << " " << trk->get_track_id() << ":" << trk->get_charge();
//      mo_trk_true.push_back(td);
//    }
//  }
//  if (mi_vec_dim) {
//    mo_dim_true.clear();
//    for (unsigned int ii = 0; ii < mi_vec_dim->size(); ii++) {
//      SQDimuon* dim = mi_vec_dim->at(ii);
//      DimuonData dd;
//      dd.mom     = dim->get_mom();
//      dd.mom_pos = dim->get_mom_pos();
//      dd.mom_neg = dim->get_mom_neg();
//      UtilDimuon::CalcVar(dim, dd.mass, dd.pT, dd.x1, dd.x2, dd.xF);
//      mo_dim_true.push_back(dd);
//      //cout << " " << dim->get_dimuon_id();
//    }
//  }
//
//  ///
//  /// Reconstructed track/dimuon info
//  ///
//  if (mi_srec) {
//    mo_trk_reco.clear();
//    for (int ii = 0; ii < mi_srec->getNTracks(); ii++) {
//      SRecTrack* trk = &mi_srec->getTrack(ii);
//      TrackData td;
//      td.charge  = trk->getCharge();
//      td.mom_vtx = trk->getMomentumVertex();
//      mo_trk_reco.push_back(td);
//    }
//    mo_dim_reco.clear();
//    for (int ii = 0; ii < mi_srec->getNDimuons(); ii++) {
//      SRecDimuon* dim = &mi_srec->getDimuon(ii);
//      DimuonData dd;
//      dd.mom     = dim->p_pos + dim->p_neg;
//      dd.mom_pos = dim->p_pos;
//      dd.mom_neg = dim->p_neg;
//      UtilDimuon::CalcVar(dd.mom_pos, dd.mom_neg, dd.mass, dd.pT, dd.x1, dd.x2, dd.xF);
//      mo_dim_reco.push_back(dd);
//    }
//  }
//
//  mo_tree->Fill();
}

void AnaCleanAndMessyData::DrawAndWriteOutput()
{
  m_out_file->cd();
  m_out_file->Write();
  TEfficiency* teff_trk_pos = new TEfficiency(*m_h1_trk_pos_me, *m_h1_trk_pos_cl);
  TEfficiency* teff_trk_neg = new TEfficiency(*m_h1_trk_neg_me, *m_h1_trk_neg_cl);
  TEfficiency* teff_dim     = new TEfficiency(*m_h1_dim_me    , *m_h1_dim_cl    );
  teff_trk_pos->SetName("teff_trk_pos");
  teff_trk_neg->SetName("teff_trk_neg");
  teff_dim    ->SetName("teff_dim");
  teff_trk_pos->SetTitle(";RF+00;Reco. efficiency of #mu^{#plus}");
  teff_trk_neg->SetTitle(";RF+00;Reco. efficiency of #mu^{#minus}");
  teff_dim    ->SetTitle(";RF+00;Reco. efficiency of dimuon");

  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  teff_trk_pos->Draw();
  c1->SaveAs("result/teff_trk_pos.png");
  teff_trk_neg->Draw();
  c1->SaveAs("result/teff_trk_neg.png");
  teff_dim    ->Draw();
  c1->SaveAs("result/teff_dim.png");
  delete c1;

  teff_trk_pos->Write();
  teff_trk_neg->Write();
  teff_dim    ->Write();
  m_out_file->Close();
}
