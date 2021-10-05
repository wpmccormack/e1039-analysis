#ifndef __CALIB_XT_V2_H__
#define __CALIB_XT_V2_H__
#include <fun4all/SubsysReco.h>
class SQHitVector;
class CalibParamXT;
class CalibParamInTimeTaiwan;

class CalibXTv2: public SubsysReco {
  SQHitVector* m_vec_hit;
  CalibParamXT* m_cal_xt;
  CalibParamInTimeTaiwan* m_cal_int;

 public:
  CalibXTv2(const std::string &name = "CalibXTv2");
  virtual ~CalibXTv2();
  int Init(PHCompositeNode *topNode);
  int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);
};

#endif // __CALIB_XT_V2_H__
