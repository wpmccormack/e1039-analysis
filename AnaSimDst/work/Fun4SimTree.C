/// Macro to analyze the simulated tree created by Fun4SimMicroDst.C.
TCanvas* c1;
void DrawDimTrueKin(TTree* tr);
void DrawDimRecoKin(TTree* tr);
void DrawTrkTrueKin(TTree* tr);
void DrawTrueVar(TTree* tr, const std::string varname, const std::string title_x, const int n_x, const double x_lo, const double x_hi);

void Fun4SimTree(const char* fname="sim_tree.root", const char* tname="tree")
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
  tr->Draw("n_dim_reco");
  c1->SaveAs("result/h1_reco_n_dim.png");

  const double PI = TMath::Pi();
  DrawTrueVar(tr, "dim_true.px"  , "True dimuon px (GeV)", 100, -5,   5);
  DrawTrueVar(tr, "dim_true.py"  , "True dimuon py (GeV)", 100, -5,   5);
  DrawTrueVar(tr, "dim_true.pz"  , "True dimuon pz (GeV)", 100,  0, 100);
  DrawTrueVar(tr, "dim_true.mass", "True dimuon mass (GeV)", 100, 0, 5);
  DrawTrueVar(tr, "dim_true.x1"  , "True x1", 50, 0, 1);
  DrawTrueVar(tr, "dim_true.x2"  , "True x2", 50, 0, 1);
  DrawTrueVar(tr, "dim_true.eta" , "True dimuon #eta", 110, 0, 11);
  DrawTrueVar(tr, "dim_true.phi" , "True dimuon #phi", 100, -PI, PI);
}

void DrawDimRecoKin(TTree* tr)
{
  tr->Draw("rec_stat"); // cf. GlobalConsts.h.
  c1->SaveAs("result/h1_rec_stat.png");
  
  tr->Draw("trig_bits", "rec_stat==0");
  c1->SaveAs("result/h1_trig_bits.png");

  tr->Draw("dim_reco.mass", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_mass.png");

  tr->Draw("dim_reco.x1", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_x1.png");

  tr->Draw("dim_reco.x2", "rec_stat==0");
  c1->SaveAs("result/h1_dim_reco_x2.png");
}


void DrawTrkTrueKin(TTree* tr)
{
  DrawTrueVar(tr, "dim_true.pos_px", "True px (GeV) of mu+", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.pos_py", "True py (GeV) of mu+", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.pos_pz", "True pz (GeV) of mu+", 100,  0, 100);
  DrawTrueVar(tr, "dim_true.neg_px", "True px (GeV) of mu-", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.neg_py", "True py (GeV) of mu-", 100, -5, 5);
  DrawTrueVar(tr, "dim_true.neg_pz", "True pz (GeV) of mu-", 100,  0, 100);

  THStack* hs;
  TH1* h1_all = new TH1D("h1_all", "", 100, -1, 1);
  TH1* h1_rec = new TH1D("h1_rec", "", 100, -1, 1);
  tr->Project("h1_all", "(dim_true.pos_pz - dim_true.neg_pz)/(dim_true.pos_pz + dim_true.neg_pz)");
  tr->Project("h1_rec", "(dim_true.pos_pz - dim_true.neg_pz)/(dim_true.pos_pz + dim_true.neg_pz)", "rec_stat==0");
  hs = new THStack("hs", "J/#psi GMC;gpz+gpz (GeV) of tracks;N of tracks");
  hs->Add(h1_all);
  hs->Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_trk_true_pz_asym.png");
}

void DrawTrueVar(TTree* tr, const std::string varname, const std::string title_x, const int n_x, const double x_lo, const double x_hi)
{
  TH1* h1_all = new TH1D("h1_all", "", n_x, x_lo, x_hi);
  TH1* h1_rec = new TH1D("h1_rec", "", n_x, x_lo, x_hi);
  tr->Project("h1_all", varname.c_str());
  tr->Project("h1_rec", varname.c_str(), "rec_stat==0");

  ostringstream oss;
  oss << "J/#psi GMC;" << title_x << ";Yield";
  THStack hs("hs", oss.str().c_str());
  hs.Add(h1_all);
  hs.Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs.Draw("nostack");

  oss.str("");
  oss << "result/h1_" << varname << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_all;
  delete h1_rec;
}
