#ifndef _DO_EMBEDDING__H_
#define _DO_EMBEDDING__H_
#include <fun4all/SubsysReco.h>
#include "EmbeddingData.h"
class TFile;
class TTree;
class SQEvent;
class SQMCEvent;
class SQHitVector;
class SQTrackVector;
class SQDimuonVector;

/// An example class to analyze the simulated uDST file.
class DoEmbedding: public SubsysReco {
  bool m_overwrite_rf_info;
  int m_hit_id_shift;
  int m_trk_id_shift;
  int m_dim_id_shift;

  /// DST nodes
  SQEvent       * mi_evt;
  SQHitVector   * mi_vec_hit;
  SQMCEvent     * mi_sim_evt;
  SQTrackVector * mi_sim_vec_trk;
  SQDimuonVector* mi_sim_vec_dim;
  SQEvent       * _evt_emb;

  /// Embedding data
  std::vector<std::string> m_list_emb_file;
  int m_idx_emb_file;
  int m_idx_emb_evt;
  bool m_emb_data_has_sim_evt;
  bool m_emb_data_has_sim_trk;
  bool m_emb_data_has_sim_dim;
  TFile* m_file_emb;
  TTree* m_tree_emb;
  EmbEventData*     m_emb_evt;
  EmbHitList*       m_emb_hit_list;
  EmbSimEventData*  m_emb_sim_evt;
  EmbSimTrackList*  m_emb_sim_trk_list;
  EmbSimDimuonList* m_emb_sim_dim_list;

 public:
  DoEmbedding(const std::string name="DoEmbedding");
  virtual ~DoEmbedding() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  void DoOverwriteRfInfo(const bool overwrite) { m_overwrite_rf_info = overwrite; }
  void SetHitIdShift   (const int id_shift) { m_hit_id_shift = id_shift; }
  void SetTrackIdShift (const int id_shift) { m_trk_id_shift = id_shift; }
  void SetDimuonIdShift(const int id_shift) { m_dim_id_shift = id_shift; }

  void AddEmbDataFile(const char* fn_root);
  void AddEmbDataFiles(const char* fn_list);
  int  GetNumEmbEvents();

 protected:
  void OpenEmbDataFile(const char* fn_root);
  void CloseEmbDataFile();
  bool GetNextEmbEvent();
};

#endif // _DO_EMBEDDING__H_
