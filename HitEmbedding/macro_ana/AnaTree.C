R__LOAD_LIBRARY(libana_embedding)
using namespace std;
TFile* file;
TTree* tree;
TCanvas* c1;
void Draw1D(const string name, const string var, const string title_x, const int n_x, const double x_lo, const double x_hi);

void AnaTree(const char* fname="ana_tree.root", const char* tname="tree")
{
  file = new TFile(fname);
  tree = (TTree*)file->Get(tname);
  gSystem->mkdir("result", true);
  c1 = new TCanvas("c1", "");
  c1->SetGrid();

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

  exit(0);
}

///
/// Functions below
///
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
