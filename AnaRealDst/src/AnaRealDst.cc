#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQHitVector.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <geom_svc/GeomSvc.h>
#include "UtilSQHit.h"
#include "AnaRealDst.h"
using namespace std;

const vector<string> AnaRealDst::list_det_name = { "H1T", "H1B", "H2T", "H2B" };

int AnaRealDst::Init(PHCompositeNode* topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::InitRun(PHCompositeNode* topNode)
{
  f_out = new TFile("output.root", "RECREATE");
  tree  = new TTree("tree", "Created by AnaRealDst");
  tree->Branch("det_name", &b_det_name, "det_name/C");
  tree->Branch("det"     , &b_det     ,      "det/I");
  tree->Branch("ele"     , &b_ele     ,      "ele/I");
  tree->Branch("time"    , &b_time    ,     "time/D");

  ostringstream oss;
  GeomSvc* geom = GeomSvc::instance();
  for (unsigned int i_det = 0; i_det < list_det_name.size(); i_det++) {
    string name = list_det_name[i_det];
    int id = geom->getDetectorID(name);
    if (id <= 0) {
      cerr << "!ERROR!  AnaRealDst::InitRun():  Invalid ID (" << id << ").  Probably the detector name that you specified in 'list_det_name' (" << name << ") is not valid.  Abort." << endl;
      exit(1);
    }
    list_det_id.push_back(id);
    int n_ele = geom->getPlaneNElements(id);
    cout << "  " << setw(6) << name << " = " << id << endl;

    oss.str("");
    oss << "h1_ele_" << name;
    h1_ele[i_det] = new TH1D(oss.str().c_str(), "", n_ele, 0.5, n_ele+0.5);
    oss.str("");
    oss << name << ";Element ID;Hit count";
    h1_ele[i_det]->SetTitle(oss.str().c_str());

    oss.str("");
    oss << "h1_nhit_" << name;
    h1_nhit[i_det] = new TH1D(oss.str().c_str(), "", 10, -0.5, 9.5);
    oss.str("");
    oss << name << ";N of hits/plane/event;Hit count";
    h1_nhit[i_det]->SetTitle(oss.str().c_str());
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::process_event(PHCompositeNode* topNode)
{
  SQEvent* event       = findNode::getClass<SQEvent    >(topNode, "SQEvent");
  SQHitVector* hit_vec = findNode::getClass<SQHitVector>(topNode, "SQHitVector");
  if (!event || !hit_vec) return Fun4AllReturnCodes::ABORTEVENT;
  //int spill_id = event->get_spill_id();
  //int event_id = event->get_event_id();

  ///
  /// Event selection
  ///
  if (! event->get_trigger(SQEvent::NIM2)) {
    return Fun4AllReturnCodes::EVENT_OK;
  }

  static int did_h3t = 0;
  static int did_h3b = 0;
  if (did_h3t == 0) {
    GeomSvc* geom = GeomSvc::instance();
    did_h3t = geom->getDetectorID("H3T");
    did_h3b = geom->getDetectorID("H3B");
    cout << "H3T = " << did_h3t << ", H3B = " << did_h3b << endl;
  }

  SQHitVector* hv_h3t = UtilSQHit::FindHits(hit_vec, did_h3t);
  SQHitVector* hv_h3b = UtilSQHit::FindHits(hit_vec, did_h3b);
  if (hv_h3t->size() + hv_h3b->size() != 1) return Fun4AllReturnCodes::EVENT_OK;

  ///
  /// Get & fill the hit info
  ///
  for (unsigned int i_det = 0; i_det < list_det_name.size(); i_det++) {
    strncpy(b_det_name, list_det_name[i_det].c_str(), sizeof(b_det_name));
    b_det = list_det_id[i_det];
    SQHitVector* hv = UtilSQHit::FindHits(hit_vec, b_det);
    for (SQHitVector::ConstIter it = hv->begin(); it != hv->end(); it++) {
      b_ele  = (*it)->get_element_id();
      b_time = (*it)->get_tdc_time  ();
      tree->Fill();

      h1_ele[i_det]->Fill(b_ele);
    }
    h1_nhit[i_det]->Fill(hv->size());
    delete hv;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

int AnaRealDst::End(PHCompositeNode* topNode)
{
  ostringstream oss;
  TCanvas* c1 = new TCanvas("c1", "");
  c1->SetGrid();
  for (unsigned int i_det = 0; i_det < list_det_id.size(); i_det++) {
    h1_ele[i_det]->Draw();
    oss.str("");
    oss << h1_ele[i_det]->GetName() << ".png";
    c1->SaveAs(oss.str().c_str());

    h1_nhit[i_det]->Draw();
    oss.str("");
    oss << h1_nhit[i_det]->GetName() << ".png";
    c1->SaveAs(oss.str().c_str());
  }
  delete c1;

  f_out->cd();
  f_out->Write();
  f_out->Close();
  
  return Fun4AllReturnCodes::EVENT_OK;
}
