#ifndef _SQ_TRUE_EVENT__H_
#define _SQ_TRUE_EVENT__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct SQTrueEvent : public PHObject {
  int proc_id;
  int par_id[4]; // 2 -> 2
  TLorentzVector par_mom[4];

  SQTrueEvent();
  virtual ~SQTrueEvent() {;}

  void identify(std::ostream& os = std::cout) const {;}
  void Reset() { *this = SQTrueEvent(); }
  int isValid() const { return 1; }
  SQTrueEvent* Clone() const { return new SQTrueEvent(*this); }

  ClassDef(SQTrueEvent, 1)
};

#endif /* _SQ_TRUE_EVENT__H_ */
