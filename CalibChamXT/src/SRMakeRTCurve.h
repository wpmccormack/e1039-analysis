#ifndef _SR_MAKE_RT_CURVE_H_
#define _SR_MAKE_RT_CURVE_H_
#include <fstream>
#include <fun4all/SubsysReco.h>
class TFile;
class TH1;
class SQEvent;
class SQHitVector;
class TrackletVector;
class CalibParam;
class CalibData;

class SRMakeRTCurve: public SubsysReco {
  typedef enum {
    ST_ID_D3P = 4,
    ST_ID_D3M = 5,
    ST_ID_D23 = 6
  } StationID_t;
  static const int N_RT_PT = 9; // N of points of R-T curve.
  int m_iter;
  std::string m_dir_name_out;

  CalibParam cal_par;
  CalibData  cal_dat;

  unsigned int m_n_evt_all;
  unsigned int m_n_evt_ana;
  unsigned int m_n_trk_all;
  unsigned int m_n_trk_ana;

  SQEvent*        m_evt;
  SQHitVector*    m_hit_vec;
  TrackletVector* m_trklet_vec;

 public:
  SRMakeRTCurve(const int iter, const std::string& name="SRMakeRTCurve");
  virtual ~SRMakeRTCurve() {;}
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

 protected:
  void DrawHistEvent();
  void DrawHistHit();
  void ExtractRT();
  void WriteRT();
  void DrawCalibResult();

  void EvalD0XY(const Tracklet* trk, double& x, double& y);
  void EvalD2XY(const Tracklet* trk, double& x, double& y);
  void EvalD3XY(const Tracklet* trk, double& x, double& y);
};

#endif // _SR_MAKE_RT_CURVE_H_
