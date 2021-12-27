#include <iomanip>
#include <sstream>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <THStack.h>
#include <TGraph.h>
#include <TLegend.h>
#include <geom_svc/GeomSvc.h>
#include <UtilAna/UtilHist.h>
#include <ktracker/FastTracklet.h>
#include "CalibParam.h"
#include "CalibData.h"
using namespace std;

CalibData::CalibData()
  : cal_par(0)
{
  ;
}

CalibData::~CalibData()
{
  ;
}

void CalibData::Init(CalibParam* ptr)
{
  cal_par = ptr;

  h1_stat  = new TH1D("h1_stat" ,      ";Reco. status;N of tracks",  25, -24.5, 0.5);

  h1_st_id = new TH1D("h1_st_id",                   ";Station ID;N of tracks",   7,  0.5,   7.5);
  h2_ntrk  = new TH2D("h2_ntrk" , ";N of tracks/event;Station ID;N of tracks", 100, -0.5, 199.5,  7, 0.5, 7.5);
  h2_nhit  = new TH2D("h2_nhit" ,   ";N of hits/track;Station ID;N of tracks",  18,  0.5,  18.5,  7, 0.5, 7.5);
  h2_mom   = new TH2D("h2_mom"  ,    ";Momentum (GeV);Station ID;N of tracks",  22,  0.0, 120.0,  7, 0.5, 7.5);
  h2_rchi2 = new TH2D("h2_rchi2",      ";#chi^{2}/NDF;Station ID;N of tracks",  50,  0.0,  10.0,  7, 0.5, 7.5);
  h2_x0    = new TH2D("h2_x0"   ,        ";x_{0} (cm);Station ID;N of tracks",  100,  -500, 500,  7, 0.5, 7.5);
  h2_y0    = new TH2D("h2_y0"   ,        ";y_{0} (cm);Station ID;N of tracks",  100,  -500, 500,  7, 0.5, 7.5);
  h2_tx    = new TH2D("h2_tx"   ,             ";t_{x};Station ID;N of tracks",  100,  -1.0, 1.0,  7, 0.5, 7.5);
  h2_ty    = new TH2D("h2_ty"   ,             ";t_{y};Station ID;N of tracks",  100,  -1.0, 1.0,  7, 0.5, 7.5);
  h2_x1800 = new TH2D("h2_x1800",     ";x_{1800} (cm);Station ID;N of tracks",  100,  -250, 250,  7, 0.5, 7.5);
  h2_y1800 = new TH2D("h2_y1800",     ";y_{1800} (cm);Station ID;N of tracks",  100,  -250, 250,  7, 0.5, 7.5);

  GeomSvc* geom = GeomSvc::instance();
  ostringstream oss;
  ostringstream oss2;
  oss2 << setfill('0');
  for (int ip = 0; ip < cal_par->GetNumPlanes(); ip++) {
    string det_name = geom->getDetectorName(ip+1);
    double t_min = cal_par->GetTMin(ip);
    double t_max = cal_par->GetTMax(ip);
    double r_max = cal_par->GetRMax(ip);

    const double t_width = 4.0/9.0 * 3; // 4/9 ns * integer
    oss.str("");
    oss << "h1_time_wide_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";TDC time (ns);Yields";
    h1_time_wide[ip] = new TH1D(oss.str().c_str(), oss2.str().c_str(),  2000, 0, 2000*t_width);

    int bin_t_lo = (int)(t_min / t_width);
    int bin_t_hi = (int)(t_max / t_width) + 1;
    oss.str("");
    oss << "h1_time_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";TDC time (ns);Yields";
    h1_time[ip] = new TH1D(oss.str().c_str(), oss2.str().c_str(),  bin_t_hi-bin_t_lo, t_width*(bin_t_lo)-0.01, t_width*(bin_t_hi)-0.01);

    oss.str("");
    oss << "h2_rt_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";TDC time (ns);R (cm);";
    h2_RT[ip] = new TH2D(oss.str().c_str(), oss2.str().c_str(),  T_BIN, t_min, t_max, R_BIN, 0, 1.2*r_max );

    oss.str("");
    oss << "h2_resi_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";TDC time (ns);Residual (cm);";
    h2_Resi[ip] = new TH2D(oss.str().c_str(), oss2.str().c_str(),  T_BIN, t_min, t_max, R_BIN, -0.5, 0.5);

    oss.str("");
    oss << "h2_tr_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";TDC time (ns);R (cm);";
    h2_TR[ip] = new TH2D(oss.str().c_str(), oss2.str().c_str(),  R_BIN, 0, 1.2*r_max, T_BIN, t_min, t_max );

    oss.str("");
    oss << "h2_tr_ex_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";Signed R (cm);TDC time (ns);";
    h2_TR_ex[ip] = new TH2D(oss.str().c_str(), oss2.str().c_str(),  R_BIN*2, -1.2*r_max, 1.2*r_max, T_BIN, t_min, t_max );

    oss.str("");
    oss << "h2_td_dd_" << det_name;
    oss2.str("");
    oss2 << "#" << setw(2) << (ip+1) << ": " << det_name << ";Track distance (cm);Drift distance (cm);Yields";
    h2_td_dd[ip] = new TH2D(oss.str().c_str(), oss2.str().c_str(),  R_BIN*2, -1.2*r_max, 1.2*r_max,    R_BIN, 0, 1.2*r_max);
  }
}

void CalibData::FillEventInfo(const int rec_stat, const std::map<int, int> list_n_trk)
{
  h1_stat->Fill(rec_stat);
  for (std::map<int, int>::const_iterator it = list_n_trk.begin(); it != list_n_trk.end(); it++) {
    h2_ntrk->Fill(it->second, it->first); // (N of tracks, station ID)
  }
}

void CalibData::FillTracklet(const Tracklet* trk)
{
  int st_id = trk->stationID;
  int n_hit = trk->getNHits();
  int ndf = n_hit - (st_id == 7  ?  5  :  4); // Incorrect, when KMag is off
  double rchi2 = ndf > 0  ?  trk->chisq / ndf  :  0;

  double x1800 = trk->x0 + 1800 * trk->tx;
  double y1800 = trk->y0 + 1800 * trk->ty;

  h1_st_id->Fill(st_id);
  h2_nhit ->Fill(n_hit      , st_id);
  h2_mom  ->Fill(1/trk->invP, st_id);
  h2_rchi2->Fill(rchi2      , st_id);
  h2_x0   ->Fill(trk->x0    , st_id);
  h2_y0   ->Fill(trk->y0    , st_id);
  h2_tx   ->Fill(trk->tx    , st_id);
  h2_ty   ->Fill(trk->ty    , st_id);
  h2_x1800->Fill(x1800      , st_id);
  h2_y1800->Fill(y1800      , st_id);
}

/// Fill the hit information.
/**
 * Here "track_dist" is the signed distance.
 */
void CalibData::FillHit(const int det_id, const double drift_dist, const double tdc_time, const double track_dist)
{
  h1_time_wide[det_id-1]->Fill(tdc_time);
  h1_time [det_id-1]->Fill(tdc_time);
  h2_RT   [det_id-1]->Fill(tdc_time, fabs(track_dist));
  h2_Resi [det_id-1]->Fill(tdc_time, fabs(drift_dist) - fabs(track_dist));
  h2_TR   [det_id-1]->Fill(fabs(track_dist), tdc_time);
  h2_TR_ex[det_id-1]->Fill(track_dist, tdc_time);
  h2_td_dd[det_id-1]->Fill(track_dist, drift_dist);
}

void CalibData::DrawHistEvent(const string dir_out)
{
  cout << "DrawHistEvent()" << endl;
  ostringstream oss;
  TCanvas* c1 = new TCanvas("c1", "", 800, 600);
  c1->SetGrid();
  gStyle->SetOptStat(0);
  
  //h1_stat->Draw();
  //oss.str("");
  //oss << dir_out << "/h1_rec_stat.png";
  //c1->SaveAs(oss.str().c_str());

  h1_st_id->Draw();
  oss.str("");
  oss << dir_out << "/h1_st_id.png";
  c1->SaveAs(oss.str().c_str());
  
  const int iy_lo = 4; // 4 = D3p, 5 = D3m, 6 = D23
  const int iy_hi = 6;
  DrawIn1D(h2_ntrk , "ntrk" , dir_out, iy_lo, iy_hi);
  DrawIn1D(h2_nhit , "nhit" , dir_out, iy_lo, iy_hi);
  //DrawIn1D(h2_mom  , "mom"  , dir_out, iy_lo, iy_hi);
  DrawIn1D(h2_rchi2, "rchi2", dir_out, iy_lo, iy_hi);
  //DrawIn1D(h2_x0   , "x0"   , dir_out, iy_lo, iy_hi);
  //DrawIn1D(h2_y0   , "y0"   , dir_out, iy_lo, iy_hi);
  DrawIn1D(h2_tx   , "tx"   , dir_out, iy_lo, iy_hi);
  DrawIn1D(h2_ty   , "ty"   , dir_out, iy_lo, iy_hi);
  DrawIn1D(h2_x1800, "x1800", dir_out, iy_lo, iy_hi);
  DrawIn1D(h2_y1800, "y1800", dir_out, iy_lo, iy_hi);

  delete c1;
}

void CalibData::DrawHistHit(const string dir_out)
{
  cout << "DrawHistHit()" << endl;
  ostringstream oss;
  TCanvas* c1 = new TCanvas("c1", "", 800, 600);
  c1->SetGrid();
  gStyle->SetOptStat(0);
  
  GeomSvc* geom = GeomSvc::instance();
  
  oss << setfill('0');
  for (int ip = 0; ip < cal_par->GetNumPlanes(); ip++) {
    if      (ip <  6) { if (! cal_par->AnaD0 ()) continue; }
    else if (ip < 12) { if (! cal_par->AnaD1 ()) continue; }
    else if (ip < 18) { if (! cal_par->AnaD2 ()) continue; }
    else if (ip < 24) { if (! cal_par->AnaD3p()) continue; }
    else              { if (! cal_par->AnaD3m()) continue; }
    string det_name = geom->getDetectorName(ip+1);
    //cout << "  Plane " << ip+1 << endl;

    UtilHist::AutoSetRange(h1_time_wide[ip]);
    h1_time_wide[ip]->Draw();
    oss.str("");
    oss << dir_out << "/h1_time_wide_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());

    h1_time[ip]->Draw();
    oss.str("");
    oss << dir_out << "/h1_time_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());
    
    h2_TR_ex[ip]->Draw("colz");
    oss.str("");
    oss << dir_out << "/tr_ex_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());
    
    h2_Resi[ip]->Draw("colz");
    oss.str("");
    oss << dir_out << "/resi_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());
    
    h2_td_dd[ip]->Draw("colz");
    oss.str("");
    oss << dir_out << "/h2_td_dd_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());
    
    TH1* h1_td = h2_td_dd[ip]->ProjectionX("h1_td");
    h1_td->Draw();
    oss.str("");
    oss << dir_out << "/h1_td_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());
    delete h1_td;
    
    TH1* h1_dd = h2_td_dd[ip]->ProjectionY("h1_dd");
    h1_dd->Draw();
    oss.str("");
    oss << dir_out << "/h1_dd_" << setw(2) << (ip+1) << "_" << det_name << ".png";
    c1->SaveAs(oss.str().c_str());
    delete h1_dd;
  }
  delete c1;
}

void CalibData::DrawIn1D(TH2* h2, const string label, const string dir_out, int iy_lo, int iy_hi)
{
  if (iy_lo <= 0) iy_lo = 1;
  if (iy_hi <= 0) iy_hi = h2->GetNbinsY();

  ostringstream oss;
  oss << h2->GetTitle() << ";" << h2->GetXaxis()->GetTitle() << ";" << h2->GetZaxis()->GetTitle();
  THStack hs("hs", oss.str().c_str());
  TLegend leg(0.90, 0.70, 0.99, 0.99);
  leg.SetHeader("St. ID");
  for (int iy = iy_lo; iy <= iy_hi; iy++) {
    oss.str("");
    oss << h2->GetName() << "_" << iy;
    TH1* h1 = h2->ProjectionX(oss.str().c_str(), iy, iy);
    h1->SetLineColor(iy);
    hs.Add(h1);
    oss.str("");   
    //oss << h2->GetYaxis()->GetTitle() << h2->GetYaxis()->GetBinCenter(iy);
    oss << h2->GetYaxis()->GetBinCenter(iy);
    leg.AddEntry(h1, oss.str().c_str(), "l");
  }
  
  TCanvas* c1 = new TCanvas("c1_din1d", "", 800, 600);
  c1->SetGrid();
  c1->SetLogy();
  gStyle->SetOptStat(0);
  hs.Draw("nostack");
  leg.Draw();
  
  oss.str("");
  oss << dir_out << "/h1_" << label << ".png";
  c1->SaveAs(oss.str().c_str());
  delete c1;
}
