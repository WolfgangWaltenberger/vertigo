{
  TFile f("magneticfield.root");
  TCanvas c1;
  beamline.Draw("vecz:ptz","","l");
  c1.Print("plotz.eps");
}
