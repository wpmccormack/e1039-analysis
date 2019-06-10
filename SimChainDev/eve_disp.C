
gROOT->ProcessLine(".x Fun4Sim.C(-1)");

Fun4AllServer *se = Fun4AllServer::instance();

PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
PHEventDisplay *eve = (PHEventDisplay *) se->getSubsysReco("PHEventDisplay");

g4->InitRun(se->topNode());
eve->InitRun(se->topNode());

se->run(1);


gEve->FullRedraw3D(kTRUE);
TGLViewer*  v = gEve->GetDefaultGLViewer();
v->RefreshPadEditor(v);

v->CurrentCamera().RotateRad(-3.14/2,0);
v->CurrentCamera().Zoom(400, 0, 0);
v->CurrentCamera().Truck(3000,0);


v->DoDraw();
