#include "Fun4CODA.C"

Fun4AllServer *se;
EvtFilter* evt_filter;
TGLViewer*  v;
TGNumberEntry *ne_evt_id;
TGNumberEntry *ne_trig;

class EvNavHandler
{
  public:
    void NextEvent()
    {
      printf("se->run(1)\n");
      se->run(1, true);
    }
    void ReqEvtID() {
      printf("ReqEvtID: %ld\n",ne_evt_id->GetNumberEntry()->GetIntNumber());
      evt_filter->set_event_id_req((int)(ne_evt_id->GetNumberEntry()->GetIntNumber()));
    }
    void ReqTrig() {
      printf("ReqTrig: %ld\n",ne_trig->GetNumberEntry()->GetIntNumber());
      evt_filter->set_trigger_req((int)(ne_trig->GetNumberEntry()->GetIntNumber()));
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
  frmMain->SetWindowName("Event Display");
  frmMain->SetCleanup(kDeepCleanup);

  TGVerticalFrame* frmVert = new TGVerticalFrame(frmMain);
  {

    TGTextButton* b = 0;
    EvNavHandler* handler = new EvNavHandler;
    TGHorizontalFrame* frm1 = 0;

    TGLabel* lab = 0;

    frm1 = new TGHorizontalFrame(frmVert);
    lab = new TGLabel(frm1, "Event ID");
    frm1->AddFrame(lab, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 5, 5, 3, 4));
    ne_evt_id = new TGNumberEntry(frm1, -1, 9, 999, TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -999999, 999999);
    ne_evt_id->Connect("ValueSet(Long_t)", "EvNavHandler", handler, "ReqEvtID()");
    frm1->AddFrame(ne_evt_id, new TGLayoutHints(kLHintsCenterY | kLHintsRight, 5, 5, 5, 5));
    frmVert->AddFrame(frm1);


    frm1 = new TGHorizontalFrame(frmVert);
    lab = new TGLabel(frm1, "Trigger");
    frm1->AddFrame(lab, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 5, 5, 3, 4));
    ne_trig = new TGNumberEntry(frm1, -1, 9, 999, TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEAAnyNumber,
        TGNumberFormat::kNELLimitMinMax,
        -999, 999);
    ne_trig->Connect("ValueSet(Long_t)", "EvNavHandler", handler, "ReqTrig()");
    frm1->AddFrame(ne_trig, new TGLayoutHints(kLHintsCenterY | kLHintsRight, 5, 5, 5, 5));
    frmVert->AddFrame(frm1);


    b = new TGTextButton(frmVert, "Next Event");
    frmVert->AddFrame(b, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    b->Connect("Clicked()", "EvNavHandler", handler, "NextEvent()");

    b = new TGTextButton(frmVert, "Top View");
    frmVert->AddFrame(b, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    b->Connect("Clicked()", "EvNavHandler", handler, "TopView()");

    b = new TGTextButton(frmVert, "Side View");
    frmVert->AddFrame(b, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    b->Connect("Clicked()", "EvNavHandler", handler, "SideView()");

    b = new TGTextButton(frmVert, "3D View");
    frmVert->AddFrame(b, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    b->Connect("Clicked()", "EvNavHandler", handler, "View3D()");

  }
  frmMain->AddFrame(frmVert);

  frmMain->MapSubwindows();
  frmMain->Resize();
  frmMain->MapWindow();

  browser->StopEmbedding();
  browser->SetTabTitle("Event Control", 0);
}


void eve_disp() {
  gROOT->LoadMacro("Fun4CODA.C");
  Fun4CODA(-1);

  se = Fun4AllServer::instance();

  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  PHEventDisplay *eve = (PHEventDisplay *) se->getSubsysReco("PHEventDisplay");
  evt_filter = (EvtFilter*) se->getSubsysReco("EvtFilter");

  g4->InitRun(se->topNode());
  eve->InitRun(se->topNode());
  v = gEve->GetDefaultGLViewer();

  make_gui();

  //se->run(1);
}

