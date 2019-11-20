#include "SQTrueEvent.h"
using namespace std;

SQTrueEvent::SQTrueEvent() : proc_id(0)
{
  for (int ii = 0; ii < 4; ii++) {
    par_id [ii] = 0;
    par_mom[ii].SetXYZT(0,0,0,0);
  }
}
