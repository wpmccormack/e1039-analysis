
on a seaquest gpvm machine:

Setup enviroment
```
source /e906/app/users/yuhw/setup.sh
```

Download the e1039-analysis repository
```
cd <work-dir>
git clone https://github.com/E1039-Collaboration/e1039-analysis.git
```

Run the Fun4All macro
```
cd e1039-analysis/SimChainDev
root -l -q Fun4Sim.C
```


More information, refer to [Start up guide with an example](https://github.com/E1039-Collaboration/e1039-wiki/wiki/Start-up-guide-with-an-example)


Below is an eample how to interactively run the event display
```C++

gROOT->ProcessLine(".x Fun4Sim.C(-1)");

Fun4AllServer *se = Fun4AllServer::instance();

PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
PHEventDisplay *eve = (PHEventDisplay *) se->getSubsysReco("PHEventDisplay");

g4->InitRun(se->topNode());
eve->InitRun(se->topNode());

se->run(1);


// control TGLViewer from terminal
TGLViewer*  v = gEve->GetDefaultGLViewer();
v->CurrentCamera().RotateRad(0, -3.14/2.0); // beam view
v->DoDraw();
```

