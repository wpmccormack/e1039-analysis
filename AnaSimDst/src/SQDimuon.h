#ifndef _SQ_DIMUON__H_
#define _SQ_DIMUON__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct SQDimuon : public PHObject {
  int id;
  int pdg_id;
  int track_id_pos;
  int track_id_neg;
  TVector3 pos;
  TLorentzVector mom;
  TLorentzVector mom_pos;
  TLorentzVector mom_neg;

  SQDimuon();
  virtual ~SQDimuon() {;}

  void identify(std::ostream& os = std::cout) const {;}
  void Reset() { *this = SQDimuon(); }
  int isValid() const { return 1; }
  SQDimuon* Clone() const { return new SQDimuon(*this); }

  ClassDef(SQDimuon, 1);
};

struct SQDimuonVector : public PHObject, public std::vector<SQDimuon> {
  SQDimuonVector() {;}
  virtual ~SQDimuonVector() {;}

  void identify(std::ostream& os = std::cout) const {;}
  void Reset() { *this = SQDimuonVector(); }
  int isValid() const { return 1; }
  SQDimuonVector* Clone() const { return new SQDimuonVector(*this); }

  ClassDef(SQDimuonVector, 1);
};

#endif /* _SQ_DIMUON__H_ */
