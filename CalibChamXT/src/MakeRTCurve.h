#ifndef _MAKE_RT_CURVE__H_
#define _MAKE_RT_CURVE__H_
#include <map>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <fun4all/SubsysReco.h>
class CalibParam;
class CalibData;

class MakeRTCurve {
  static const int N_RT_PT = 9; // N of points of R-T curve.
  int m_iter;
  std::string m_dir_name_out;
  
  CalibParam cal_par;
  CalibData  cal_dat;

 public:
  MakeRTCurve(const int iter);
  virtual ~MakeRTCurve();

  void Init();
  void AnalyzeListOfFiles(const char* fn_list);
  void AnalyzeFile(const char* fname);
  //void ExtractRT();

  void DrawHistEvent();
  void DrawHistHit();
  void WriteRT();
  void DrawCalibResult();
  
 private:

};

#endif // _MAKE_RT_CURVE__H_
