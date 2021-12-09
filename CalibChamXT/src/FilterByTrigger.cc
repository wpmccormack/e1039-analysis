#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/getClass.h>
#include <interface_main/SQEvent.h>
#include <TFile.h>
#include <TH1D.h>
#include "FilterByTrigger.h"
using namespace std;

FilterByTrigger::FilterByTrigger()
  : m_trig_bits(0)
  , m_fn_out("")
  , m_file_out(0)
  , m_h1_evt_cnt(0)
  , m_evt      (0)
{
  ;
}

FilterByTrigger::~FilterByTrigger()
{
  ;
}

int FilterByTrigger::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int FilterByTrigger::InitRun(PHCompositeNode* topNode)
{
  m_evt = findNode::getClass<SQEvent>(topNode, "SQEvent");
  if (!m_evt) return Fun4AllReturnCodes::ABORTEVENT;

  if (m_fn_out != "") {
    m_file_out = new TFile(m_fn_out.c_str(), "RECREATE");
    m_h1_evt_cnt = new TH1D("h1_evt_cnt", "", 10, 0.5, 10.5);
    m_h1_evt_cnt->GetXaxis()->SetBinLabel(1, "All");
    m_h1_evt_cnt->GetXaxis()->SetBinLabel(2, "Trigger OK");
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int FilterByTrigger::process_event(PHCompositeNode* topNode)
{
  if (m_file_out) m_h1_evt_cnt->Fill(1);

  if ((m_evt->get_trigger() & m_trig_bits) == 0) return Fun4AllReturnCodes::ABORTEVENT;

  if (m_file_out) m_h1_evt_cnt->Fill(2);
  return Fun4AllReturnCodes::EVENT_OK;
}

int FilterByTrigger::End(PHCompositeNode* topNode)
{
  if (m_file_out) {
    m_file_out->cd();
    m_file_out->Write();
    m_file_out->Close();
  }
  return Fun4AllReturnCodes::EVENT_OK;
}
