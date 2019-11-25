#ifndef _SQ_TRACK__H_
#define _SQ_TRACK__H_
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct SQTrack : public PHObject {
  int id;
  int charge;
  int n_hits;
  TVector3 pos_vtx;
  TLorentzVector mom_vtx;

  SQTrack();
  virtual ~SQTrack() {;}

  void identify(std::ostream& os = std::cout) const {;}
  void Reset() { *this = SQTrack(); }
  int isValid() const { return 1; }
  SQTrack* Clone() const { return new SQTrack(*this); }

  ClassDef(SQTrack, 1);
};

struct SQTrackVector : public PHObject, public std::vector<SQTrack> {
  SQTrackVector() {;}
  virtual ~SQTrackVector() {;}

  void identify(std::ostream& os = std::cout) const {;}
  void Reset() { *this = SQTrackVector(); }
  int isValid() const { return 1; }
  SQTrackVector* Clone() const { return new SQTrackVector(*this); }

  ClassDef(SQTrackVector, 1);
};

#endif /* _SQ_TRACK__H_ */
