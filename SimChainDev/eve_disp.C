
gROOT->ProcessLine(".x Fun4Sim.C(-1)");

Fun4AllServer *se = Fun4AllServer::instance();

PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
PHEventDisplay *eve = (PHEventDisplay *) se->getSubsysReco("PHEventDisplay");

g4->InitRun(se->topNode());
eve->InitRun(se->topNode());

se->run(1);


TGLViewer*  v = gEve->GetDefaultGLViewer();

// beam view
v->CurrentCamera().RotateRad(0, -3.14/2.0); // beam view
v->DoDraw();


//3D view
v->CurrentCamera().RotateRad(-3.14/4., -3.14/4.);
v->CurrentCamera().Zoom(350, 0, 0);
v->CurrentCamera().Truck(2000,-1500);
v->DoDraw();
