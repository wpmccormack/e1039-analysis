# A Program to Run Full Chain of E1039 Simulation

## Environment

This program depends on the E1039 resource+share+core packages.
Unless you have a special reason, you are recommended to use "spinquestgpvm01.fnal.gov",
where the E1039 packages are ready to use.

You can just execute the following command to set up the environment properly;
```
source /e906/app/software/osg/software/e1039/this-e1039.sh
```
You have to do so when you start a new shell environment (i.e. text terminal).

## Download

You move to a working directory and check out the repository;
```
cd /path/to/your_working_directory
git clone https://github.com/E1039-Collaboration/e1039-analysis.git
```
If you are a member of the GitHub E1039 group, you better use the following command to obtain the write access;
```
git clone git@github.com:E1039-Collaboration/e1039-analysis.git
```

## Test Process

You run the following commands to run a small test.
It will take 10-20(?) minutes.
Note that you have to set up the environment beforehand, as explained above.

```
cd e1039-analysis/SimChainDev
root -l -q Fun4Sim.C
```

By default a simple muon-pair event (via PHG4SimpleEventGenerator) is simulated.
You can/should change various options to simulate what you need.

## Large Process

When you want a large set of simulated events,
you should run the simulation on the grid.
The procedure is explained in
[this Wiki page](https://github.com/E1039-Collaboration/e1039-wiki/wiki/Submit-jobs-to-the-grid).

## More information

You could refer to
[E1039-simulation-tutorial---Apr.-19](https://github.com/E1039-Collaboration/e1039-wiki/wiki/E1039-simulation-tutorial---Apr.-19).

## Event display

Below is an example how to interactively run the event display.

Open a ROOT session
```bash
root -l
```

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

