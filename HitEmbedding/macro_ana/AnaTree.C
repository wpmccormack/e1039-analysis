/// AnaTree.C:  Macro to analyze the simulated tree created by Fun4SimMicroDst.C.
R__LOAD_LIBRARY(libana_embedding)

using namespace std;
TFile* file;
TTree* tree;
TCanvas* c1;
void DrawDimTrueKin();
void DrawDimRecoKin();
void DrawTrkTrueKin();
void DrawTrueVar(const string varname, const string title_x, const int n_x, const double x_lo, const double x_hi);
void Draw1D(const string name, const string var, const string title_x, const int n_x, const double x_lo, const double x_hi);

void AnaTree(const char* fname="ana_tree.root", const char* tname="tree")
{
  file = new TFile(fname);
  tree = (TTree*)file->Get(tname);
  gSystem->mkdir("result", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();
  //c1->SetLogy(true);

  Draw1D("n_h1x", "n_h1x", "N of H1X hits", 20, -0.5, 19.5);
  Draw1D("n_h2x", "n_h2x", "N of H2X hits", 20, -0.5, 19.5);
  Draw1D("n_h3x", "n_h3x", "N of H3X hits", 20, -0.5, 19.5);
  Draw1D("n_h4x", "n_h4x", "N of H4X hits", 20, -0.5, 19.5);
  
  Draw1D("n_trk_true", "@trk_true.size()", "N of true tracks" , 10, -0.5, 9.5);
  Draw1D("n_dim_true", "@dim_true.size()", "N of true dimuons", 10, -0.5, 9.5);
  
  Draw1D("n_trk_reco", "@trk_reco.size()", "N of reco. tracks" , 10, -0.5, 9.5);
  Draw1D("n_dim_reco", "@dim_reco.size()", "N of reco. dimuons", 10, -0.5, 9.5);
  
  Draw1D("trk_px_true", "trk_true.mom_vtx.X()", "p_{x} of true tracks", 50, -5, 5);
  Draw1D("trk_py_true", "trk_true.mom_vtx.Y()", "p_{y} of true tracks", 50, -5, 5);
  Draw1D("trk_pz_true", "trk_true.mom_vtx.Z()", "p_{z} of true tracks", 45, 10, 100);

  Draw1D("trk_px_reco", "trk_reco.mom_vtx.X()", "p_{x} of reco. tracks", 50, -5, 5);
  Draw1D("trk_py_reco", "trk_reco.mom_vtx.Y()", "p_{y} of reco. tracks", 50, -5, 5);
  Draw1D("trk_pz_reco", "trk_reco.mom_vtx.Z()", "p_{z} of reco. tracks", 45, 10, 100);

  Draw1D("dim_mass_true", "dim_true.mom.M()", "Mass of true dimuons", 50, 1, 6);
  Draw1D("dim_pz_true"  , "dim_true.mom.Z()", "p_{z} of true dimuons", 45, 30, 120);

  Draw1D("dim_mass_reco", "dim_reco.mom.M()", "Mass of reco. dimuons", 50, 1, 6);
  Draw1D("dim_pz_reco"  , "dim_reco.mom.Z()", "p_{z} of reco. dimuons", 45, 30, 120);

  /// You can use these functions or add new ones.
  //DrawDimTrueKin();
  //DrawDimRecoKin();
  //DrawTrkTrueKin();

  exit(0);
}

///
/// Functions below
///
void DrawDimTrueKin()
{
  tree->Draw("n_dim_true");
  c1->SaveAs("result/h1_true_n_dim.png");

  const double PI = TMath::Pi();
  DrawTrueVar("dim_true.pdg_id"    , "True dimuon PDG ID", 1000, 0, 0);
  DrawTrueVar("dim_true.mom.X()"   , "True dimuon px (GeV)", 100, -5,   5);
  DrawTrueVar("dim_true.mom.Y()"   , "True dimuon py (GeV)", 100, -5,   5);
  DrawTrueVar("dim_true.mom.Z()"   , "True dimuon pz (GeV)", 100,  0, 100);
  DrawTrueVar("dim_true.mom.M()"   , "True dimuon mass (GeV)", 100, 0, 5);
  DrawTrueVar("dim_true.mom.Eta()" , "True dimuon #eta", 110, 0, 11);
  DrawTrueVar("dim_true.mom.Phi()" , "True dimuon #phi", 100, -PI, PI);
  DrawTrueVar("dim_true.x1"        , "True x1", 50, 0, 1);
  DrawTrueVar("dim_true.x2"        , "True x2", 50, 0, 1);
  DrawTrueVar("dim_true.xF"        , "True xF", 50, -1, 1);
  DrawTrueVar("dim_true.costh"     , "True cos#theta", 50, -1, 1);
  DrawTrueVar("dim_true.phi"       , "True #phi"     , 50, -PI, PI);
}

void DrawDimRecoKin()
{
  tree->Draw("n_dim_reco", "weight");
  c1->SaveAs("result/h1_reco_n_dim.png");

  tree->Draw("rec_stat", "weight"); // cf. GlobalConsts.h.
  c1->SaveAs("result/h1_rec_stat.png");
  
  tree->Draw("trig_bits", "weight * (rec_stat==0)");
  c1->SaveAs("result/h1_trig_bits.png");

  tree->Draw("dim_reco.mom.M()", "weight * (rec_stat==0)");
  c1->SaveAs("result/h1_dim_reco_mass.png");

  tree->Draw("dim_reco.x1", "weight * (rec_stat==0)");
  c1->SaveAs("result/h1_dim_reco_x1.png");

  tree->Draw("dim_reco.x2", "weight * (rec_stat==0)");
  c1->SaveAs("result/h1_dim_reco_x2.png");
}


void DrawTrkTrueKin()
{
  DrawTrueVar("dim_true.mom_pos.X()", "True px (GeV) of mu+", 100, -5, 5);
  DrawTrueVar("dim_true.mom_pos.Y()", "True py (GeV) of mu+", 100, -5, 5);
  DrawTrueVar("dim_true.mom_pos.Z()", "True pz (GeV) of mu+", 100,  0, 100);
  DrawTrueVar("dim_true.mom_neg.X()", "True px (GeV) of mu-", 100, -5, 5);
  DrawTrueVar("dim_true.mom_neg.Y()", "True py (GeV) of mu-", 100, -5, 5);
  DrawTrueVar("dim_true.mom_neg.Z()", "True pz (GeV) of mu-", 100,  0, 100);

  THStack* hs;
  TH1* h1_all = new TH1D("h1_all", "", 100, -1, 1);
  TH1* h1_rec = new TH1D("h1_rec", "", 100, -1, 1);
  tree->Project("h1_all", "(dim_true.mom_pos.Z() - dim_true.mom_neg.Z())/(dim_true.mom_pos.Z() + dim_true.mom_neg.Z())", "weight");
  tree->Project("h1_rec", "(dim_true.mom_pos.Z() - dim_true.mom_neg.Z())/(dim_true.mom_pos.Z() + dim_true.mom_neg.Z())", "weight * (rec_stat==0)");

  ostringstream oss;
  oss << " GMC;gpz+gpz (GeV) of tracks;N of tracks";
  hs = new THStack("hs", oss.str().c_str());
  hs->Add(h1_all);
  hs->Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs->Draw("nostack");
  c1->SaveAs("result/h1_trk_true_pz_asym.png");
}

void DrawTrueVar(const string varname, const string title_x, const int n_x, const double x_lo, const double x_hi)
{
  TH1* h1_all = new TH1D("h1_all", "", n_x, x_lo, x_hi);
  TH1* h1_rec = new TH1D("h1_rec", "", n_x, x_lo, x_hi);
  tree->Project("h1_all", varname.c_str(), "weight");
  tree->Project("h1_rec", varname.c_str(), "weight * (rec_stat==0)");

  ostringstream oss;
  oss << " GMC;" << title_x << ";Yield";
  THStack hs("hs", oss.str().c_str());
  hs.Add(h1_all);
  hs.Add(h1_rec);
  h1_rec->SetLineColor(kRed);
  hs.Draw("nostack");

  oss.str("");
  oss << "result/h1_";
  for (string::const_iterator it = varname.begin(); it != varname.end(); it++) {
    switch (*it) { // modify bad chars for file name
    case '.': case '*': case '/': oss << '_'; break;
    case '(': case ')': case ' ': /* omit */ break;
    default: oss << *it;
    }
  }
  oss << ".png";
  c1->SaveAs(oss.str().c_str());

  delete h1_all;
  delete h1_rec;
}

void Draw1D(const string name, const string var, const string title_x, const int n_x, const double x_lo, const double x_hi)
{
  TH1* h1 = new TH1D("h1", "", n_x, x_lo, x_hi);
  tree->Project("h1", var.c_str());
  ostringstream oss;
  oss << ";" << title_x << ";Yield";
  h1->SetTitle(oss.str().c_str());
  h1->SetLineColor(kRed);
  h1->Draw();
  c1->SetLogy(false);
  oss.str("");
  oss << "result/h1_" << name << ".png";
  c1->SaveAs(oss.str().c_str());
  c1->SetLogy(true);
  oss.str("");
  oss << "result/h1_" << name << "_log.png";
  c1->SaveAs(oss.str().c_str());
  delete h1;
}
