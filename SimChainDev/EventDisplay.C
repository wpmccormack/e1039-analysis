#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <pheve_interface/PHEventDisplay.h>
#include <TTimer.h>
class SubsysReco;
R__LOAD_LIBRARY(libpheve_display)
R__LOAD_LIBRARY(libpheve_modules)
R__LOAD_LIBRARY(libpheve_interface)
#endif

int EventDisplay(const int nevent = 0)
{

  Fun4AllServer* se = Fun4AllServer::instance();

  //---------------
  // Event display
  //---------------
  //gSystem->Exec("/bin/env");

  PHEventDisplay* event_display
    = new PHEventDisplay(
    1400, 800, // width height
    false, // use_fieldmap
    false, // use_geofile
    "", // field-map name
    "geom.root" //geo-file name
    );

  // location of geometry file : /gpfs/mnt/gpfs02/sphenix/user/shlee/svtx/stage1_jobs
  event_display->set_jet_pt_threshold(10.);//GeV/c
  event_display->set_jet_e_scale(30.);//GeV
  event_display->set_calo_e_threshold(0.0009);//GeV
  event_display->set_svtx_on(true);
  event_display->set_cemc_on(true);
  event_display->set_hcalin_on(false);
  event_display->set_hcalout_on(false);
  event_display->set_jet_on(false);
  event_display->set_truth_on(false);
  event_display->set_verbosity(3);

  se->registerSubsystem(event_display);
  //event_display->start_rotation();

  //TTimer* timer = new TTimer();
  //timer->Connect("Timeout()", "PHEventDisplay", event_display,"run_evt_in_thread()");
  //timer->Start(100000,kFALSE);

  return 0;
}
