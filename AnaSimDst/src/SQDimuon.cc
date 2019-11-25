#include "SQDimuon.h"
using namespace std;

SQDimuon::SQDimuon()
  : id(-1), pdg_id(0), track_id_pos(0), track_id_neg(0),
    pos(0,0,0), mom(0,0,0,0), mom_pos(0,0,0,0), mom_neg(0,0,0,0)
{
  ;
}
