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
#include "FitRTDist.h"
#include "MakeRTCurve.h"
using namespace std;

MakeRTCurve::MakeRTCurve(const int iter)
  : m_iter(iter)
  , m_dir_name_out("")
  , m_n_evt_all(0)
  , m_n_evt_ana(0)
  , m_n_trk_all(0)
  , m_n_trk_ana(0)
  , m_verb     (0)
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

  cal_par.SetAnaPlanes(false, false, false, false, true); // (d0, d1, d2, d3p, d3m)
  cal_par.Init(N_RT_PT);

  oss.str("");
  oss << "calib/" << (m_iter-1) << "/param.tsv";
  cal_par.ReadRTParam(oss.str());

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

  // SRawEvent is not available in the evaluation file.  Need to use DST.
  //static SRawEvent*    raw       = new SRawEvent();
  //static SRecEvent*    rec       = 0; // new SRecEvent();
  static TClonesArray* tracklets = new TClonesArray("Tracklet");
  tracklets->Clear();
  //dataTree->SetBranchAddress("rawEvent" , &raw      );
  //dataTree->SetBranchAddress("recEvent" , &rec      );
  dataTree->SetBranchAddress("tracklets", &tracklets);

  int n_evt_ana = 0;
  int n_trk_all = 0;
  int n_trk_ana = 0;
  int n_evt = dataTree->GetEntries();
  for (int i_evt = 0; i_evt < n_evt; i_evt++) {
    dataTree->GetEntry(i_evt);
    //bool nim1 = raw->isTriggeredBy(SRawEvent::NIM1); // H1234
    //bool nim2 = raw->isTriggeredBy(SRawEvent::NIM2); // H12
    //bool nim4 = raw->isTriggeredBy(SRawEvent::NIM4); // H24
    //if (! nim4) continue;
    n_evt_ana++;

    int n_trk = tracklets->GetEntries();
    n_trk_all += n_trk;

    map<int, int> list_n_trk; // <station ID, N of tracks>
    for (int i_trk = 0; i_trk < n_trk; i_trk++) {
      Tracklet* trk = (Tracklet*)tracklets->At(i_trk);
      //cal_dat.FillTracklet(trk); // Use this to fill all tracklets
      list_n_trk[trk->stationID]++;
    }
    if (list_n_trk[ST_ID_D3M] > 64) continue; // Exclude noisy events

    int rec_st = 0; // rec->getRecStatus()
    cal_dat.FillEventInfo(rec_st, list_n_trk);
    
    int i_trk_best = -1;
    double rchi2_best = 0;
    for (int i_trk = 0; i_trk < n_trk; i_trk++) {
      Tracklet* trk = (Tracklet*)tracklets->At(i_trk);
      int st_id = trk->stationID;
      if (st_id != ST_ID_D3M) continue;

      int n_hit = trk->getNHits();
      int ndf = n_hit - 4; // Correct only when KMag is off
      double rchi2 = trk->chisq / ndf;
      if (n_hit != 6) continue;
      if (rchi2 > 3.0) continue;

      double x_d2, y_d2;
      double x_d3, y_d3;
      EvalD2XY(trk, x_d2, y_d2);
      EvalD3XY(trk, x_d3, y_d3);

      if (fabs(trk->tx) > 0.3 || fabs(trk->ty) > 0.4 ||
          fabs(x_d3)    > 131 || fabs(y_d3+85) > 85    ) continue; // Track in the D3m acceptance, page 15 of doc9856.

      // Cut L1 = st_id & tx & ty & x_d3 & y_d3
      // Cut L2 = L1 & n_hit & n_trk <= 64
      // Cut L3 = L2 & rchi2 < 2

      if (i_trk_best < 0 || rchi2 < rchi2_best) {
        i_trk_best = i_trk;
        rchi2_best = rchi2;
      }
    }

    if (i_trk_best >= 0) {
      Tracklet* trk = (Tracklet*)tracklets->At(i_trk_best);
      cal_dat.FillTracklet(trk); // Use this to fill only good tracklets
      n_trk_ana++;
      
      for(std::list<SignedHit>::iterator it = trk->hits.begin(); it != trk->hits.end(); ++it) {
        if(it->hit.index < 0) continue; // Probably not necessary.
        //int sign = it->hit.sign;
        int    det_id     = it->hit.detectorID;
        int    ele_id     = it->hit.elementID;
        double drift_dist = it->hit.driftDistance;
        double tdc_time   = it->hit.tdcTime;
        double track_pos  = trk->getExpPositionW(det_id);
        double wire_pos   = it->hit.pos; // GeomSvc::instance()->getMeasurement(det_id, ele_id);
        cal_dat.FillHit(det_id, drift_dist, tdc_time, track_pos, wire_pos);
        //cout << "D " << det_id << " " << it->hit.pos << " " << tdc_time << " " << drift_dist << " " << track_dist << endl;
      }
    }
    
    tracklets->Clear();
  }
  dataFile->Close();
  cout << "  N of all events = " << n_evt << ", analyzed events = " << n_evt_ana << ", tracks = " << n_trk_ana << endl;
  m_n_evt_all += n_evt;
  m_n_evt_ana += n_evt_ana;
  m_n_trk_all += n_trk_all;
  m_n_trk_ana += n_trk_ana;
}

void MakeRTCurve::ExtractRT()
{
  cout << "ExtractRT()" << endl;
  FitRTDist* fit = new FitRTDist();
  fit->Verbosity(m_verb);
  for (int ip = 0; ip < cal_par.GetNumPlanes(); ip++) {
    if (! cal_par.GetAnaPlane(ip)) continue;

    cout << "  Plane " << ip+1 << endl;
    if (cal_par.TimeWindowIsFixed()) {
      double t1 = cal_par.GetT1(ip);
      double t0 = cal_par.GetT0(ip);
      fit->FixT1T0(t1, t0);
    }
    TH2* h2_rt     = cal_dat.GetHistRT  (ip);
    double r_max   = cal_par.GetRMax    (ip);
    TGraph* gr_t2r = cal_par.GetGraphT2R(ip);
    fit->DoFit(N_RT_PT, h2_rt, r_max, gr_t2r, cal_par.GetRTCurve(ip));
  }
  delete fit;
}

void MakeRTCurve::DrawHistEvent()
{
  cal_dat.DrawHistEvent(m_dir_name_out);
  string fname = m_dir_name_out + "/info_event.txt";
  ofstream ofs(fname.c_str());
  ofs << m_n_evt_all << "\n"
      << m_n_evt_ana << "\n"
      << m_n_trk_all << "\n"
      << m_n_trk_ana << "\n";
  ofs.close();
}

void MakeRTCurve::DrawHistHit()
{
  cal_dat.DrawHistHit(m_dir_name_out);
}

void MakeRTCurve::WriteRT()
{
  cal_par.WriteRTParam(m_dir_name_out, "param.tsv");
  cal_par.WriteRTGraph(m_dir_name_out, "rt_graph.root");
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
    if (! cal_par.GetAnaPlane(ip)) continue;
    
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
    if (! cal_par.GetAnaPlane(ip)) continue;
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

void MakeRTCurve::EvalD2XY(const Tracklet* trk, double& x, double& y)
{
  x = trk->x0 + 1345 * trk->tx;
  y = trk->y0 + 1345 * trk->ty;
}

void MakeRTCurve::EvalD3XY(const Tracklet* trk, double& x, double& y)
{
  x = trk->x0 + 1900 * trk->tx;
  y = trk->y0 + 1900 * trk->ty;
}

