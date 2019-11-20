#include <iomanip>
#include <phhepmc/PHHepMCGenEventMap.h>
#include <g4main/PHG4TruthInfoContainer.h>
//#include <g4main/PHG4HitContainer.h>
//#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
//#include <g4main/PHG4HitDefs.h>
#include <g4main/PHG4VtxPoint.h>
//#include <interface_main/SQRun.h>
//#include <interface_main/SQEvent.h>
//#include <interface_main/SQHitVector.h>
//#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
//#include <geom_svc/GeomSvc.h>
//#include <UtilAna/UtilSQHit.h>
#include "SQTrueEvent.h"
#include "SQTrack.h"
#include "SQDimuon.h"
#include "TrueNodeMaker.h"
using namespace std;

TrueNodeMaker::TrueNodeMaker()
  : m_evt(0), m_vec_trk(0), m_vec_dim(0)
{
  ;
}

TrueNodeMaker::~TrueNodeMaker()
{
  if (! m_evt    ) delete m_evt;
  if (! m_vec_trk) delete m_vec_trk;
  if (! m_vec_dim) delete m_vec_dim;
}

int TrueNodeMaker::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int TrueNodeMaker::InitRun(PHCompositeNode* topNode)
{
  int ret = GetNodes(topNode);
  if (ret != Fun4AllReturnCodes::EVENT_OK) return ret;
  ret = MakeNodes(topNode);
  if (ret != Fun4AllReturnCodes::EVENT_OK) return ret;
  return Fun4AllReturnCodes::EVENT_OK;
}

int TrueNodeMaker::process_event(PHCompositeNode* topNode)
{
  const double M_MU = 0.1056583745; // GeV

  ///
  /// Extract the key event info
  ///
  if (genevtmap->size() != 1) {
    cout << "TrueNodeMaker::process_event(): size != 1 unexpectedly." << endl;
  }
  for (PHHepMCGenEventMap::Iter iter = genevtmap->begin(); iter != genevtmap->end(); ++iter) {
    PHHepMCGenEvent *genevt = iter->second;
    if (! genevt) {
      cout << "No PHHepMCGenEvent object." << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }
    HepMC::GenEvent *evt = genevt->getEvent();
    if (! evt) {
      cout << "No HepMC::GenEvent object." << endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }
    m_evt->proc_id = evt->signal_process_id();

    //HepMC::GenVertex* vtx = evt->signal_process_vertex(); // Return 0 as of 2019-11-19.
    HepMC::GenEvent::particle_const_iterator it = evt->particles_begin();
    it++; // Skip the 1st beam particle.
    for (int ii = 0; ii < 4; ii++) {
      it++;
      const HepMC::GenParticle* par = *it;
      const HepMC::FourVector * mom = &par->momentum();
      m_evt->par_id[ii] = (*it)->pdg_id();
      m_evt->par_mom[ii].SetPxPyPzE(mom->px(), mom->py(), mom->pz(), mom->e());
      //cout << "  P " << par->pdg_id() << " " << mom->pz() << endl;
    }
  }

  ///
  /// Construct the true track and dimuon info
  ///
  m_vec_trk->clear();
  m_vec_dim->clear();

  //for (auto it = g4true->GetParticleRange().first; it != g4true->GetParticleRange().second; ++it) {
  //  PHG4Particle* par = it->second;
  //  cout << "X " << par->get_pid() << " " << par->get_name() << " " << par->get_parent_id() << " " << par->get_primary_id() << endl;
  //}

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
    m_vec_trk->push_back(trk);
  }

  unsigned int n_trk = m_vec_trk->size();
  for (unsigned int i1 = 0; i1 < n_trk; i1++) {
    SQTrack* trk1 = &m_vec_trk->at(i1);
    if (trk1->charge <= 0) continue;
    for (unsigned int i2 = 0; i2 < n_trk; i2++) {
      SQTrack* trk2 = &m_vec_trk->at(i2);
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
      m_vec_dim->push_back(dim);
    }
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int TrueNodeMaker::End(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int TrueNodeMaker::GetNodes(PHCompositeNode* topNode)
{
  genevtmap = findNode::getClass<PHHepMCGenEventMap    >(topNode, "PHHepMCGenEventMap");
  g4true    = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  if (!genevtmap || !g4true) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int TrueNodeMaker::MakeNodes(PHCompositeNode* topNode)
{
  PHNodeIterator iter(topNode);
  PHCompositeNode* node_dst = static_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST"));
  if (! node_dst) {
    cout << "No DST node!?" << endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  m_evt     = new SQTrueEvent();
  m_vec_trk = new SQTrackVector();
  m_vec_dim = new SQDimuonVector();

  node_dst->addNode(new PHIODataNode<PHObject>(m_evt    , "SQTrueEvent"       , "PHObject"));
  node_dst->addNode(new PHIODataNode<PHObject>(m_vec_trk, "SQTrueTrackVector" , "PHObject"));
  node_dst->addNode(new PHIODataNode<PHObject>(m_vec_dim, "SQTrueDimuonVector", "PHObject"));

  return Fun4AllReturnCodes::EVENT_OK;
}
