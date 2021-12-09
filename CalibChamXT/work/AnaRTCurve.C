/// AnaRTCurve.C:  ROOT macro to make the X-T curve.
R__LOAD_LIBRARY(libCalibChamXT)

int AnaRTCurve(const int iter, const char* fn_list_input)
{
  recoConsts* rc = recoConsts::instance();
  rc->init("cosmic");
  rc->set_BoolFlag("COARSE_MODE", true);
  rc->set_DoubleFlag("KMAGSTR", 0.);
  rc->set_DoubleFlag("FMAGSTR", 0.);
  //rc->Print();

  MakeRTCurve mrtc(iter);
  mrtc.Init();
  mrtc.AnalyzeListOfFiles(fn_list_input);
  mrtc.DrawHistEvent();
  mrtc.DrawHistHit();
  //mrtc.ExtractRT();
  //mrtc.WriteRT();
  //mrtc.DrawCalibResult();

  return 0;
}
