{
  TFile f("b.root");
  AllTk->Fit("gaus","10000.*d_tip");
  // AllTk->Fit("gaus","sttip");
}
