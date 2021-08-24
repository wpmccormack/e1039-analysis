#include "TreeData.h"
using namespace std;

EventData::EventData()
  : event_id(0)
  , trig_bits(0)
  , rec_stat(0)
  , weight(1.0)
  , n_h1x(0)
  , n_h2x(0)
  , n_h3x(0)
  , n_h4x(0)
{
  ;
}

TrackData::TrackData() 
  : charge(0)
{
  ;
}
  
DimuonData::DimuonData() 
{
  ;
}
