#ifndef _TRUE_NODE_MAKER__H_
#define _TRUE_NODE_MAKER__H_
//#include <TVector3.h>
//#include <TLorentzVector.h>
#include <fun4all/SubsysReco.h>
class PHHepMCGenEventMap;
class PHG4TruthInfoContainer;
class SQTrueEvent;
class SQTrack;
class SQDimuon;

/// An SubsysReco module to create a set of (compact) nodes for the simulation true info.
class TrueNodeMaker: public SubsysReco {
  PHHepMCGenEventMap* genevtmap;
  PHG4TruthInfoContainer* g4true;

  SQTrueEvent*    m_evt;
  SQTrackVector*  m_vec_trk;
  SQDimuonVector* m_vec_dim;

 public:
  TrueNodeMaker();
  virtual ~TrueNodeMaker();
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 private:
  int  GetNodes(PHCompositeNode *topNode);
  int MakeNodes(PHCompositeNode *topNode);
};

#endif /* _TRUE_NODE_MAKER__H_ */
