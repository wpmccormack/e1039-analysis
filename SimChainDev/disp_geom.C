void disp_geom(){

  gGeoManager = TGeoManager::Import("geom.root");
  //gGeoManager = TGeoManager::Import("test.root");
  //gGeoManager->Export("geom.root");
  TGeoNode *current = gGeoManager->GetCurrentNode();
  current->GetVolume()->Draw("ogl");
}
