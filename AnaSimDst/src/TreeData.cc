#include "TreeData.h"
using namespace std;

EventData::EventData()
  : proc_id(0), trig_bits(0), rec_stat(0), n_dim_reco(0)
{
  for (int ii = 0; ii < 4; ii++) {
    par_id [ii] = 0;
    par_mom[ii].SetXYZT(0, 0, 0, 0);
  }
}

TrackData::TrackData()
  : charge(0), x(0), y(0), z(0), px(0), py(0), pz(0)
{
  ;
}
  
DimuonData::DimuonData()
  : x(0), y(0), z(0), px(0), py(0), pz(0),
    mass(0), eta(0), phi(0), x1(0), x2(0)
{
  ;
}
