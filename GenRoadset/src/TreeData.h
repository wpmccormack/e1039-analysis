#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_
#include <vector>
#include <phool/PHObject.h>
#include <TLorentzVector.h>

struct SignalData {
  double weight;
  double mass;
  double pT;
  double xF;
  double x1;
  double x2;
  double mom;
  double phi;
  double theta;
  int    road_pos;
  int    road_neg;

  SignalData();
  virtual ~SignalData() {;}
  ClassDef(SignalData, 1);
};

struct BgData {
  int    run;
  int    evt;
  bool   fpga1;
  int    inte_rfp00;
  int    inte_max;
  std::vector<int> h1t;
  std::vector<int> h2t;
  std::vector<int> h3t;
  std::vector<int> h4t;
  std::vector<int> h1b;
  std::vector<int> h2b;
  std::vector<int> h3b;
  std::vector<int> h4b;
  //int    b_n_top[4];
  //int    b_n_bot[4];
  //std::vector<int> b_roads_top;
  //std::vector<int> b_roads_bot;

  BgData();
  virtual ~BgData() {;}
  ClassDef(BgData, 1);
};

//struct EventData {
//  int proc_id;
//  int par_id[4]; // 2 -> 2
//  TLorentzVector par_mom[4];
//  double weight;
//  int trig_bits;
//  int rec_stat;
//  int n_dim_true;
//  int n_dim_reco;
//
//  EventData();
//  virtual ~EventData() {;}
//
//  ClassDef(EventData, 1);
//};
//
//struct TrackData {
//  int            charge;
//  TVector3       pos_vtx;
//  TLorentzVector mom_vtx;
//
//  TrackData();
//  virtual ~TrackData() {;}
//
//  ClassDef(TrackData, 1);
//};
//
//struct DimuonData {
//  int            pdg_id;
//  TVector3       pos;
//  TLorentzVector mom;
//  TLorentzVector mom_pos;
//  TLorentzVector mom_neg;
//  double         mass;
//  double         pT;
//  double         x1;
//  double         x2;
//  double         xF;
//  double         costh;
//  double         phi;
//
//  DimuonData();
//  virtual ~DimuonData() {;}
//
//  ClassDef(DimuonData, 1);
//};
//
//typedef std::vector<TrackData > TrackList;
//typedef std::vector<DimuonData> DimuonList;

#endif /* _TREE_DATA__H_ */
