#include "EmbeddingData.h"
using namespace std;

EmbEventData::EmbEventData()
  : run_id   (0)
  , spill_id (0)
  , event_id (0)
  , trig_bits(0)
  , turn_id  (0)
  , rf_id    (0)
{
  memset(rf_inte, 0, sizeof(rf_inte));
}

EmbSimEventData::EmbSimEventData()
  : weight(1.0)
{
  ;
}

EmbHitData::EmbHitData()
  : hit_id    (0)
  , det_id    (0)
  , ele_id    (0)
  , tdc_time  (0.0)
  , drift_dist(0.0)
  , pos       (0.0)
{
  ;
}

EmbSimTrackData::EmbSimTrackData() 
  : trk_id(0)
  , charge(0)
{
  ;
}
  
EmbSimDimuonData::EmbSimDimuonData() 
  : dim_id    (0)
  , trk_pos_id(0)
  , trk_neg_id(0)
{
  ;
}
