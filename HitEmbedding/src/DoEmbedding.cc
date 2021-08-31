#include <fstream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <interface_main/SQEvent_v1.h>
#include <interface_main/SQMCEvent_v1.h>
#include <interface_main/SQHit_v1.h>
#include <interface_main/SQHitVector.h>
#include <interface_main/SQTrack_v1.h>
#include <interface_main/SQTrackVector_v1.h>
#include <interface_main/SQDimuon_v1.h>
#include <interface_main/SQDimuonVector_v1.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
#include <UtilAna/UtilSQHit.h>
#include <UtilAna/UtilDimuon.h>
#include "DoEmbedding.h"
using namespace std;

DoEmbedding::DoEmbedding(const std::string name)
  : SubsysReco(name)
  , m_overwrite_rf_info(true)
  , m_hit_id_shift(10000)
  , m_trk_id_shift(10000)
  , m_dim_id_shift(10000)
  , m_idx_emb_file(0)
  , m_idx_emb_evt (0)
  , m_emb_data_has_sim_evt(false)
  , m_emb_data_has_sim_trk(false)
  , m_emb_data_has_sim_dim(false)
  , m_file_emb(0)
  , m_tree_emb(0)
  , m_emb_evt         (0)
  , m_emb_hit_list    (0)
  , m_emb_sim_evt     (0)
  , m_emb_sim_trk_list(0)
  , m_emb_sim_dim_list(0)
{
  ;
}

int DoEmbedding::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int DoEmbedding::InitRun(PHCompositeNode* topNode)
{
  mi_evt     = findNode::getClass<SQEvent       >(topNode, "SQEvent");
  mi_vec_hit = findNode::getClass<SQHitVector   >(topNode, "SQHitVector");
  if (!mi_evt || !mi_vec_hit) {
    cout << PHWHERE << ":  Cannot find SQEvent and/or SQHitVector." << endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  PHNodeIterator iter(topNode);
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST"));
  _evt_emb = findNode::getClass<SQEvent>(topNode, "SQEmbEvent");
  if (! _evt_emb) {
    _evt_emb = new SQEvent_v1();
    dstNode->addNode(new PHIODataNode<PHObject>(_evt_emb, "SQEmbEvent", "PHObject"));
  }
  mi_sim_evt = findNode::getClass<SQMCEvent>(topNode, "SQMCEvent");
  if (! mi_sim_evt) {
    mi_sim_evt = new SQMCEvent_v1();
    dstNode->addNode(new PHIODataNode<PHObject>(mi_sim_evt, "SQMCEvent", "PHObject"));
  }
  mi_sim_vec_trk = findNode::getClass<SQTrackVector>(topNode, "SQTruthTrackVector");
  if (! mi_sim_vec_trk) {
    mi_sim_vec_trk = new SQTrackVector_v1();
    dstNode->addNode(new PHIODataNode<PHObject>(mi_sim_vec_trk, "SQTruthTrackVector", "PHObject"));
  }
  mi_sim_vec_dim = findNode::getClass<SQDimuonVector>(topNode, "SQTruthDimuonVector");
  if (! mi_sim_vec_dim) {
    mi_sim_vec_dim = new SQDimuonVector_v1();
    dstNode->addNode(new PHIODataNode<PHObject>(mi_sim_vec_dim, "SQTruthDimuonVector", "PHObject"));
  }

  m_emb_evt          = new EmbEventData();
  m_emb_hit_list     = new EmbHitList();
  m_emb_sim_evt      = new EmbSimEventData();
  m_emb_sim_trk_list = new EmbSimTrackList();
  m_emb_sim_dim_list = new EmbSimDimuonList();

  return Fun4AllReturnCodes::EVENT_OK;
}

int DoEmbedding::process_event(PHCompositeNode* topNode)
{
  if (Verbosity() > 9) {
    cout << "DoEmbedding::process_event(): Start.\n"
         << "  run " << m_emb_evt->run_id << ", spill " << m_emb_evt->spill_id << ", event " << m_emb_evt->event_id << endl;
  }

  if (! GetNextEmbEvent()) {
    // No embedding data available.  Do nothing.
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  _evt_emb->set_run_id  (m_emb_evt->run_id  );
  _evt_emb->set_spill_id(m_emb_evt->spill_id);
  _evt_emb->set_event_id(m_emb_evt->event_id);
  _evt_emb->set_trigger (m_emb_evt->trig_bits);
  _evt_emb->set_qie_turn_id(m_emb_evt->turn_id);
  _evt_emb->set_qie_rf_id  (m_emb_evt->rf_id  );
  for (int ii = 0; ii < 33; ii++) {
    _evt_emb->set_qie_rf_intensity(ii-16, m_emb_evt->rf_inte[ii]);
  }
  if (m_overwrite_rf_info) {
    mi_evt->set_qie_turn_id(m_emb_evt->turn_id);
    mi_evt->set_qie_rf_id  (m_emb_evt->rf_id  );
    for (int ii = 0; ii < 33; ii++) {
      mi_evt->set_qie_rf_intensity(ii-16, m_emb_evt->rf_inte[ii]);
    }
  }

  if (Verbosity() > 9) cout << "  N ot hits to be embedded: " << m_emb_hit_list->size() << endl;
  for (EmbHitList::iterator it = m_emb_hit_list->begin(); it != m_emb_hit_list->end(); it++) {
    EmbHitData* ehd = &(*it);
    SQHit_v1 hit;
    hit.set_hit_id        (ehd->hit_id + m_hit_id_shift);
    hit.set_detector_id   (ehd->det_id);
    hit.set_element_id    (ehd->ele_id);
    hit.set_tdc_time      (ehd->tdc_time);
    hit.set_drift_distance(ehd->drift_dist);
    hit.set_pos           (ehd->pos);
    mi_vec_hit->push_back(&hit);
  }

  if (m_emb_data_has_sim_evt) {
    if (Verbosity() > 9) cout << "  Event weight: " << m_emb_sim_evt->weight << endl;
    mi_sim_evt->set_weight(m_emb_sim_evt->weight);
  }

  if (Verbosity() > 9) {
    for (SQTrackVector::ConstIter it = mi_sim_vec_trk->begin(); it != mi_sim_vec_trk->end(); it++) {
      SQTrack* trk = *it;
      TLorentzVector mom = trk->get_mom_vtx();
      cout << "  True track: " << trk->get_track_id() << " " << trk->get_charge()
           << " " << mom.X() << " " << mom.Y() << " " << mom.Z() << endl;
    }
  }

  if (m_emb_data_has_sim_trk) {
    if (Verbosity() > 9) cout << "  N of tracks to be embedded: " << m_emb_sim_trk_list->size() << endl;
    for (EmbSimTrackList::iterator it = m_emb_sim_trk_list->begin(); it != m_emb_sim_trk_list->end(); it++) {
      EmbSimTrackData* estd = &(*it);
      SQTrack_v1 trk;
      trk.set_track_id(estd->trk_id + m_trk_id_shift);
      trk.set_charge  (estd->charge );
      trk.set_pos_vtx (estd->pos_vtx);
      trk.set_pos_st1 (estd->pos_st1);
      trk.set_pos_st3 (estd->pos_st3);
      trk.set_mom_vtx (estd->mom_vtx);
      trk.set_mom_st1 (estd->mom_st1);
      trk.set_mom_st3 (estd->mom_st3);
      mi_sim_vec_trk->push_back(&trk);
    }
  }

  if (m_emb_data_has_sim_dim) {
    if (Verbosity() > 9) cout << "  N of dimuons to be embedded: " << m_emb_sim_dim_list->size() << endl;
    for (EmbSimDimuonList::iterator it = m_emb_sim_dim_list->begin(); it != m_emb_sim_dim_list->end(); it++) {
      EmbSimDimuonData* esdd = &(*it);
      SQDimuon_v1 dim;
      dim.set_dimuon_id   (esdd->dim_id    );
      dim.set_track_id_pos(esdd->trk_pos_id);
      dim.set_track_id_neg(esdd->trk_neg_id);
      dim.set_pos         (esdd->pos       );
      dim.set_mom_pos     (esdd->mom_pos   );
      dim.set_mom_neg     (esdd->mom_neg   );
      mi_sim_vec_dim->push_back(&dim);
    }
  }

  if (Verbosity() > 9) cout << "DoEmbedding::process_event(): End." << endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

int DoEmbedding::End(PHCompositeNode* topNode)
{
  CloseEmbDataFile();
  return Fun4AllReturnCodes::EVENT_OK;
}

void DoEmbedding::AddEmbDataFile(const char* fn_root)
{
  m_list_emb_file.push_back(fn_root);
}

void DoEmbedding::AddEmbDataFiles(const char* fn_list)
{
  ifstream ifs(fn_list);
  string fn_root;
  while (ifs >> fn_root) AddEmbDataFile(fn_root.c_str());
  ifs.close();
}

int DoEmbedding::GetNumEmbEvents()
{
  int num = 0;
  for (vector<string>::iterator it = m_list_emb_file.begin(); it != m_list_emb_file.end(); it++) {
    OpenEmbDataFile(it->c_str());
    num += m_tree_emb->GetEntries();
    CloseEmbDataFile();
  }
  return num;
}

void DoEmbedding::OpenEmbDataFile(const char* fn_root)
{
  m_file_emb = new TFile(fn_root);
  if (! m_file_emb->IsOpen()) {
    cout << "ERROR:  Cannot open the embedding-data file, " << fn_root << ".  Abort." << endl;
    exit(1);
  }
  m_tree_emb = (TTree*)m_file_emb->Get("tree");
  if (! m_tree_emb) {
    cout << "ERROR:  Cannot get the embedding-data tree.  Abort." << endl;
    exit(1);
  }
  if (Verbosity() > 0) {
    cout << "DoEmbedding::OpenEmbDataFile(): " << fn_root << ", N = " << m_tree_emb->GetEntries() << "." << endl;
  }
}

void DoEmbedding::CloseEmbDataFile()
{
  if (! m_file_emb) return;
  m_file_emb->Close();
  m_file_emb = 0;
  m_tree_emb = 0;
}

bool DoEmbedding::GetNextEmbEvent()
{
  if (! m_file_emb) { // Need to open an embedding-data file
    if (m_idx_emb_file >= m_list_emb_file.size()) return false; // No file available.
    string fn_root = m_list_emb_file[m_idx_emb_file];
    OpenEmbDataFile(fn_root.c_str());
    m_tree_emb->SetBranchAddress("emb_evt"     , &m_emb_evt     );
    m_tree_emb->SetBranchAddress("emb_hit_list", &m_emb_hit_list);
    m_emb_data_has_sim_evt = (m_tree_emb->FindBranch("emb_sim_evt"     ) != 0);
    m_emb_data_has_sim_trk = (m_tree_emb->FindBranch("emb_sim_trk_list") != 0);
    m_emb_data_has_sim_dim = (m_tree_emb->FindBranch("emb_sim_dim_list") != 0);
    if (m_emb_data_has_sim_evt) m_tree_emb->SetBranchAddress("emb_sim_evt"     , &m_emb_sim_evt     );
    if (m_emb_data_has_sim_trk) m_tree_emb->SetBranchAddress("emb_sim_trk_list", &m_emb_sim_trk_list);
    if (m_emb_data_has_sim_dim) m_tree_emb->SetBranchAddress("emb_sim_dim_list", &m_emb_sim_dim_list);

    m_idx_emb_file++;
    m_idx_emb_evt = 0;
  }

  if (m_idx_emb_evt >= m_tree_emb->GetEntries()) { // No event available.
    CloseEmbDataFile();
    return GetNextEmbEvent();
  }

  m_tree_emb->GetEntry(m_idx_emb_evt++);

  return true;
}
