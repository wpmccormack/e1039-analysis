#include <phool/getClass.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <interface_main/SQEvent.h>
#include "EventFilter.h"
using namespace std;

EventFilter::EventFilter() : SubsysReco("EventFilter")
{
  ;
}

int EventFilter::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int EventFilter::InitRun(PHCompositeNode* topNode)
{
  mi_evt = findNode::getClass<SQEvent>(topNode, "SQEvent");
  if (!mi_evt) return Fun4AllReturnCodes::ABORTEVENT;
  return Fun4AllReturnCodes::EVENT_OK;
}

int EventFilter::process_event(PHCompositeNode* topNode)
{
  if (! mi_evt->get_trigger(SQEvent::NIM3)) return Fun4AllReturnCodes::ABORTEVENT;
  return Fun4AllReturnCodes::EVENT_OK;
}

int EventFilter::End(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}
