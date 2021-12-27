#include <iomanip>
#include <sstream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TLegend.h>
#include <geom_svc/GeomSvc.h>
//#include <SRecEvent.h>
#include <ktracker/FastTracklet.h>
#include "RTCurve.h"
#include "CalibParam.h"
#include "CalibData.h"
//#include "FitRTDist.h"
#include "MakeRTCurve.h"
using namespace std;

MakeRTCurve::MakeRTCurve(const int iter)
  : m_iter(iter)
  , m_dir_name_out("")
{
  if (iter <= 0) {
    cout << "'iter' must be a positive integer.  Abort." << endl;
    exit(1);
  }
  //gErrorIgnoreLevel = 1111;
  //GeomSvc* geom = GeomSvc::instance();
  //geom->init();
}

MakeRTCurve::~MakeRTCurve()
{
  ;
}

void MakeRTCurve::Init()
{
  ostringstream oss;
  oss << "calib/" << m_iter;
  m_dir_name_out = oss.str();
  gSystem->mkdir(m_dir_name_out.c_str(), kTRUE);

  cal_par.Init(N_RT_PT);
  //cal_par.ReadRTParam(jobopts->m_calibrationsFile);

  //const char* env_str = gSystem->Getenv("FIX_TIME_WINDOW");
  //if (! env_str) {
  //  cout << "!!ERROR!!  FIX_TIME_WINDOW is not defined.  Abort." << endl;
  //  exit(1);
  //}
  //if (strcmp(env_str, "true" ) == 0) cal_par.ReadTimeWindow("calib/time_window.txt");

  cal_dat.Init(&cal_par);
}

void MakeRTCurve::AnalyzeListOfFiles(const char* fn_list)
{
  ifstream ifs(fn_list);
  string fn_file;
  while (ifs >> fn_file) AnalyzeFile(fn_file.c_str());
  ifs.close();
}

void MakeRTCurve::AnalyzeFile(const char* fname)
{
  if (gSystem->AccessPathName(fname)) return;
  cout << "Input: " << fname << endl;
  TFile* dataFile = new TFile(fname, "READ");
  TTree* dataTree = (TTree*)dataFile->Get("eval");

  //SRawEvent*    raw       = new SRawEvent();
  //SRecEvent*    rec       = new SRecEvent();
  TClonesArray* tracklets = new TClonesArray("Tracklet");
  tracklets->Clear();
  //dataTree->SetBranchAddress("rawEvent" , &raw      );
  //dataTree->SetBranchAddress("recEvent" , &rec      );
  dataTree->SetBranchAddress("tracklets", &tracklets);

  int n_evt_ana = 0;
  int n_trk_ana = 0;
  int n_evt = dataTree->GetEntries();
  for (int i_evt = 0; i_evt < n_evt; i_evt++) {
    dataTree->GetEntry(i_evt);
    n_evt_ana++;
    int n_trk = tracklets->GetEntries();

    map<int, int> list_n_trk; // <station ID, N of tracks>
    for (int i_trk = 0; i_trk < n_trk; i_trk++) {
      Tracklet* trk = (Tracklet*)tracklets->At(i_trk);
      //cal_dat.FillTracklet(trk); // To fill all tracklets
      list_n_trk[trk->stationID]++;
    }
    int rec_st = 0; // rec->getRecStatus()
    cal_dat.FillEventInfo(rec_st, list_n_trk);
    if (list_n_trk[5] > 40) continue;
    
    for (int i_trk = 0; i_trk < n_trk; i_trk++) {
      Tracklet* trk = (Tracklet*)tracklets->At(i_trk);
      int st_id = trk->stationID;
      if (st_id != 5) continue; // 5 = D3m

      double x1800 = trk->x0 + 1800 * trk->tx;
      double y1800 = trk->y0 + 1800 * trk->ty;
      int n_hit = trk->getNHits();
      if (n_hit != 6) continue;

      int ndf = n_hit - 4; // Correct only when KMag is off
      double rchi2 = trk->chisq / ndf;
      if (rchi2 < 1.0 || rchi2 > 3.0 ||
          fabs(trk->tx) > 0.3 ||
          fabs(trk->ty) > 0.4 ||
          fabs(x1800)   > 100 ||
          fabs(y1800+75) > 75   ) continue;
      cal_dat.FillTracklet(trk); // To fill only good tracklets
      n_trk_ana++;
      
      for(std::list<SignedHit>::iterator it = trk->hits.begin(); it != trk->hits.end(); ++it) {
        if(it->hit.index < 0) continue;
        int    det_id     = it->hit.detectorID;
        double drift_dist = it->hit.driftDistance;
        double tdc_time   = it->hit.tdcTime;
        double track_dist = trk->getExpPositionW(det_id) - it->hit.pos; // track position - wire position
        //if (det_id == 1) cout << " Z " << trk->getExpPositionW(det_id) << " " << it->hit.pos << " " << r << " " << t << " " << TMAX[det_id-1]-t << endl;
        cal_dat.FillHit(det_id, drift_dist, tdc_time, track_dist);
      }
    }
    
    
    tracklets->Clear();
  }
  dataFile->Close();
  cout << "  N of all events = " << n_evt << ", analyzed events = " << n_evt_ana << ", tracks = " << n_trk_ana << endl;
}

void MakeRTCurve::ExtractRT()
{
  cout << "ExtractRT()" << endl;
//  FitRTDist* fit = new FitRTDist(N_RT_PT);
//  for (int ip = 0; ip < cal_par.GetNumPlanes(); ip++) {
//    if (SKIP_D1 && 6 <= ip && ip < 12) continue;
//    cout << "  Plane " << ip+1 << endl;
//    if (cal_par.TimeWindowIsFixed()) {
//      double t1 = cal_par.GetT1(ip);
//      double t0 = cal_par.GetT0(ip);
//      fit->FixT1T0(t1, t0);
//    }
//    TH2* h2_rt     = cal_dat.GetHistRT  (ip);
//    double r_max   = cal_par.GetRMax    (ip);
//    TGraph* gr_t2r = cal_par.GetGraphT2R(ip);
//    fit->InitInput(h2_rt, r_max, gr_t2r);
//    fit->DoFit(cal_par.GetRTCurve(ip));
//  }
//  delete fit;
}

void MakeRTCurve::DrawHistEvent()
{
  cal_dat.DrawHistEvent(m_dir_name_out);
}

void MakeRTCurve::DrawHistHit()
{
  cal_dat.DrawHistHit(m_dir_name_out);
}

void MakeRTCurve::WriteRT()
{
  cal_par.WriteRTParam(m_dir_name_out);
  cal_par.WriteRTGraph(m_dir_name_out);
}

void MakeRTCurve::DrawCalibResult()
{
  GeomSvc* geom = GeomSvc::instance();
  cout << "DrawCalibResult()" << endl;
  TCanvas* c1 = new TCanvas("c1", "", 800, 600);
  c1->SetGrid();
  gStyle->SetOptStat(0);
  TLegend* leg = new TLegend(0.7, 0.7, 0.9, 0.9);
  leg->SetTextFont(22);
  leg->SetBorderSize(1);
  leg->SetFillColor(0);

  ostringstream oss;
  oss << setfill('0');
  for(int ip = 0; ip < cal_par.GetNumPlanes(); ip++) {
    if      (ip <  6) { if (! cal_par.AnaD0 ()) continue; }
    else if (ip < 12) { if (! cal_par.AnaD1 ()) continue; }
    else if (ip < 18) { if (! cal_par.AnaD2 ()) continue; }
    else if (ip < 24) { if (! cal_par.AnaD3p()) continue; }
    else              { if (! cal_par.AnaD3m()) continue; }
    
    leg->Clear();

    cal_dat.GetHistRT(ip)->Draw("colz");

    TGraph* gr_t2r_in = cal_par.GetGraphT2R(ip);
    TSpline3* spl_t2r = cal_par.GetRTCurve(ip)->GetT2RSpline();
    TGraph* gr_t2dr   = cal_par.GetRTCurve(ip)->GetT2DRGraph();

    spl_t2r->SetMarkerStyle(10);
    spl_t2r->SetLineWidth(2);
    spl_t2r->Draw("sameCP"); // LPE1
    
    gr_t2dr->SetMarkerColor(kGreen);
    gr_t2dr->SetMarkerStyle(10);
    gr_t2dr->SetLineWidth(2);
    gr_t2dr->Draw("sameP");
    
    gr_t2r_in->SetLineStyle(2);
    gr_t2r_in->SetLineWidth(2);
    gr_t2r_in->Draw("sameC"); // LP
    
    leg->AddEntry(spl_t2r  , "Output", "l");
    leg->AddEntry(gr_t2r_in, "Input" , "l");
    leg->Draw();
    
    oss.str("");
    oss << m_dir_name_out << "/rt_result_" << setw(2) << (ip+1) << "_" << geom->getDetectorName(ip+1) << ".png";
    
    c1->SaveAs(oss.str().c_str());
  }
  delete leg;
  
  int n_pl = cal_par.GetNumPlanes();
  TH1* h1_res = new TH1D("h1_res", ";;Resolution (cm)", n_pl, 0, n_pl);
  for (int ip = 0; ip < n_pl; ip++) {
    if      (ip <  6) { if (! cal_par.AnaD0 ()) continue; }
    else if (ip < 12) { if (! cal_par.AnaD1 ()) continue; }
    else if (ip < 18) { if (! cal_par.AnaD2 ()) continue; }
    else if (ip < 24) { if (! cal_par.AnaD3p()) continue; }
    else              { if (! cal_par.AnaD3m()) continue; }
    string det_name = geom->getDetectorName(ip+1);
    h1_res->SetBinContent(ip+1, cal_par.GetRTCurve(ip)->GetRWidth());
    h1_res->GetXaxis()->SetBinLabel(ip+1, det_name.c_str());
  }
  h1_res->GetYaxis()->SetRangeUser(0, 0.08);
  h1_res->SetMarkerColor(kRed);
  h1_res->SetMarkerStyle(10);
  h1_res->GetXaxis()->LabelsOption("v");
  h1_res->Draw("P");
  oss.str("");
  oss << m_dir_name_out << "/h1_res.png";
  c1->SaveAs(oss.str().c_str());
  delete c1;
  
  //const char* NAME_GROUP[4] = {"D1", "D2", "D3p", "D3m"};
  //const double   RES_MAX[4] = {0.1, 0.1, 0.1, 0.1};
  //for (int ig = 0; ig < 4; ig++) {
  //   TH1* fr = c1->DrawFrame(TMIN[ig*6], 0, TMAX[ig*6], RES_MAX[ig], "Resolution vs TDC ;TDC time (ns);Resolution (cm)");
  //   for (int ip = 0; ip < 6; ip++) {
  //      gr_t2dr[ ig*6 +ip ]->SetLineColor  (ip+1);
  //      gr_t2dr[ ig*6 +ip ]->SetMarkerColor(ip+1);
  //      gr_t2dr[ ig*6 +ip ]->Draw("sameCP"); // LPE1
  //
  //      oss.str(""); oss << geom->getDetectorName( ig*6 + ip + 1);
  //   }
  //   c1->SetGrid();
  //   oss.str(""); oss << m_dir_name_out << "/res_" << NAME_GROUP[ig] <<".png";
  //   c1->SaveAs(oss.str().c_str());
  //}
}
