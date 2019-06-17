#include "Fun4DB.C"

Fun4AllServer *se;
TGLViewer*  v;

class EvNavHandler
{
  public:
    void NextEvent()
    {
      printf("se->run(1)\n");
      se->run(1);
    }
    void TopView()
    {
      printf("Top View\n");
      v->ResetCurrentCamera();
      v->CurrentCamera().RotateRad(-3.14/2.0, 0);
      v->CurrentCamera().Zoom(400, 0, 0);
      v->CurrentCamera().Truck(2800,0);
      v->DoDraw();
    }
    void SideView()
    {
      printf("Side View\n");
      v->ResetCurrentCamera();
      v->CurrentCamera().Zoom(400, 0, 0);
      v->CurrentCamera().Truck(2800,0);
      v->DoDraw();
    }
    void View3D()
    {
      printf("3D View\n");
      v->ResetCurrentCamera();
      v->CurrentCamera().RotateRad(-3.14/4., -3.14/4.);
      v->CurrentCamera().Zoom(350, 0, 0);
      v->CurrentCamera().Truck(2000,-1500);
      v->DoDraw();
    }

};

void make_gui()
{
  // Create minimal GUI for event navigation.

  TEveBrowser* browser = gEve->GetBrowser();
  browser->StartEmbedding(TRootBrowser::kLeft);

  TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
  frmMain->SetWindowName("XX GUI");
  frmMain->SetCleanup(kDeepCleanup);

  TGVerticalFrame* hf = new TGVerticalFrame(frmMain);
  {

    TGTextButton* b = 0;
    EvNavHandler    *fh = new EvNavHandler;

    b = new TGTextButton(hf, "Next Event");
    hf->AddFrame(b);
    b->Connect("Clicked()", "EvNavHandler", fh, "NextEvent()");

    b = new TGTextButton(hf, "Top View");
    hf->AddFrame(b);
    b->Connect("Clicked()", "EvNavHandler", fh, "TopView()");

    b = new TGTextButton(hf, "Side View");
    hf->AddFrame(b);
    b->Connect("Clicked()", "EvNavHandler", fh, "SideView()");

    b = new TGTextButton(hf, "3D View");
    hf->AddFrame(b);
    b->Connect("Clicked()", "EvNavHandler", fh, "View3D()");

  }
  frmMain->AddFrame(hf);

  frmMain->MapSubwindows();
  frmMain->Resize();
  frmMain->MapWindow();

  browser->StopEmbedding();
  browser->SetTabTitle("Event Control", 0);
}


void eve_disp_db() {
  gROOT->LoadMacro("Fun4DB.C");
  Fun4DB(-1);

  se = Fun4AllServer::instance();

  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  PHEventDisplay *eve = (PHEventDisplay *) se->getSubsysReco("PHEventDisplay");

  g4->InitRun(se->topNode());
  eve->InitRun(se->topNode());
  v = gEve->GetDefaultGLViewer();

  make_gui();

  //se->run(1);
}

