/// Macro to analyze the tree created by Fun4SimDst.C.
TCanvas* c1;
void DrawDimTrueKin(TTree* tr);
void DrawDimRecoKin(TTree* tr);
void DrawTrkTrueKin(TTree* tr);
void DrawTrackTrueMom(TTree* tr, const std::string xyz, const int posneg);

void AnaSimTree(const char* fname="output.root", const char* tname="tree")
{
  TFile* file = new TFile(fname);
  TTree* tr = (TTree*)file->Get(tname);

  gSystem->mkdir("result", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();
  c1->SetLogy(true);
  
  DrawDimTrueKin(tr);
  DrawDimRecoKin(tr);

  DrawTrkTrueKin(tr);

  exit(0);
}

///
/// Functions below
///
void DrawDimTrueKin(TTree* tr)
{
  //tr->Draw("gndimu");
  //c1->SaveAs("result/h1_reco_ndimu.png");
  //c1->SetLogy(true);
  THStack* hs;

  TH1* h1_mass_all = new TH1D("h1_mass_all", "", 100, 0, 5);
  TH1* h1_mass_rec = new TH1D("h1_mass_rec", "", 100, 0, 5);
  tr->Project("h1_mass_all", "dim_mass");
  tr->Project("h1_mass_rec", "dim_mass", "rec_stat==0");
  hs = new THStack("hs", "J/#psi GMC;True dimuon mass (GeV);N of events");
  hs->Add(h1_mass_all);
  hs->Add(h1_mass_rec);
  h1_mass_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_dim_true_mass.png");

  TH1* h1_x1_all = new TH1D("h1_x1_all", "", 50, 0, 1);
  TH1* h1_x1_rec = new TH1D("h1_x1_rec", "", 50, 0, 1);
  tr->Project("h1_x1_all", "dim_x1");
  tr->Project("h1_x1_rec", "dim_x1", "rec_stat==0");
  hs = new THStack("hs", "J/#psi GMC;True dimuon x1;N of events");
  hs->Add(h1_x1_all);
  hs->Add(h1_x1_rec);
  h1_x1_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_dim_true_x1.png");

  TH1* h1_x2_all = new TH1D("h1_x2_all", "", 50, 0, 1);
  TH1* h1_x2_rec = new TH1D("h1_x2_rec", "", 50, 0, 1);
  tr->Project("h1_x2_all", "dim_x2");
  tr->Project("h1_x2_rec", "dim_x2", "rec_stat==0");
  hs = new THStack("hs", "J/#psi GMC;True dimuon x2;N of events");
  hs->Add(h1_x2_all);
  hs->Add(h1_x2_rec);
  h1_x2_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_dim_true_x2.png");

  TH1* h1_eta_all = new TH1D("h1_eta_all", "", 110, 0, 11);
  TH1* h1_eta_rec = new TH1D("h1_eta_rec", "", 110, 0, 11);
  tr->Project("h1_eta_all", "dim_reco_eta");
  tr->Project("h1_eta_rec", "dim_reco_eta", "rec_stat==0");
  delete hs;
  hs = new THStack("hs", "J/#psi GMC;True dimuon #eta;N of events");
  hs->Add(h1_eta_all);
  hs->Add(h1_eta_rec);
  h1_eta_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_dim_true_eta.png");

  const double PI = TMath::Pi();
  TH1* h1_phi_all = new TH1D("h1_phi_all", "", 100, -PI, PI);
  TH1* h1_phi_rec = new TH1D("h1_phi_rec", "", 100, -PI, PI);
  tr->Project("h1_phi_all", "dim_reco_phi");
  tr->Project("h1_phi_rec", "dim_reco_phi", "rec_stat==0");
  delete hs;
  hs = new THStack("hs", "J/#psi GMC;True dimuon #phi;N of events");
  hs->Add(h1_phi_all);
  hs->Add(h1_phi_rec);
  h1_phi_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_dim_true_phi.png");

  //c1->SetLogy(false);
}

void DrawDimRecoKin(TTree* tr)
{
  tr->Draw("rec_stat"); // cf. GlobalConsts.h.
  c1->SaveAs("result/h1_rec_stat.png");
  
  tr->Draw("trigger", "rec_stat==0");
  c1->SaveAs("result/h1_trigger.png");

  tr->Draw("dim_reco_mass", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_mass.png");

  tr->Draw("dim_reco_x1", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_x1.png");

  tr->Draw("dim_reco_x2", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_x2.png");
}


void DrawTrkTrueKin(TTree* tr)
{
  tr->Draw("trk_n");
  c1->SaveAs("result/h1_trk_true_n.png");

  DrawTrackTrueMom(tr, "x", +1);
  DrawTrackTrueMom(tr, "x", -1);
  DrawTrackTrueMom(tr, "y", +1);
  DrawTrackTrueMom(tr, "y", -1);
  DrawTrackTrueMom(tr, "z", +1);
  DrawTrackTrueMom(tr, "z", -1);

  //TH1* h1_all = new TH1D("h1_all", "", 100, -1, 1);
  //TH1* h1_rec = new TH1D("h1_rec", "", 100, -1, 1);
  //tr->Project("h1_all", "(gpz[0]-gpz[1])/(gpz[0]+gpz[1])", "n_tracks==2");
  //tr->Project("h1_rec", "(gpz[0]-gpz[1])/(gpz[0]+gpz[1])", "n_tracks==2 && rec_stat==0");
  //THStack hs("hs", "J/#psi GMC;gpz+gpz (GeV) of tracks;N of tracks");
  //hs.Add(h1_all);
  //hs.Add(h1_rec);
  //h1_rec->SetLineColor(kRed);
  //hs.Draw("nostack");
  //c1->SaveAs("result/h1_trk_reco_pz_asym.png");
}

void DrawTrackTrueMom(TTree* tr, const std::string xyz, const int posneg)
{
  double x_lo, x_hi;
  if      (xyz == "x") { x_lo = -5; x_hi =   5; }
  else if (xyz == "y") { x_lo = -5; x_hi =   5; }
  else if (xyz == "z") { x_lo =  0; x_hi = 100; }
  string varname = "trk_p" + xyz;
  string cutpar = posneg > 0 ? "trk_ch>0" : "trk_ch<0";
  string cutpar_rec = cutpar + " && rec_stat==0";

  TH1* h1_all = new TH1D("h1_all", "", 100, x_lo, x_hi);
  TH1* h1_rec = new TH1D("h1_rec", "", 100, x_lo, x_hi);
  tr->Project("h1_all", varname.c_str(), cutpar    .c_str());
  tr->Project("h1_rec", varname.c_str(), cutpar_rec.c_str());
  ostringstream oss;
  oss << "J/#psi GMC;p" << xyz << " (GeV) of " << (posneg > 0 ? "mu+" : "mu-") << ";N of tracks";

  THStack hs("hs", oss.str().c_str());
  hs.Add(h1_all);
  hs.Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs.Draw("nostack");

  oss.str("");
  oss << "result/h1_trk_true_p" << xyz << "_" << (posneg > 0 ? "pos" : "neg") << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_all;
  delete h1_rec;
}
