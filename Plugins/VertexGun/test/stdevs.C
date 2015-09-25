{
  TFile f("devs.root");
  d.Fit("gaus","rho");
  c1.Print("rho.png");
  /*
  d.Fit("gaus","dx");
  c1.Print("dx.png");
  d.Fit("gaus","dy");
  c1.Print("dy.png");
  d.Fit("gaus","dxdir");
  c1.Print("dxdir.png");
  d.Fit("gaus","dydir");
  c1.Print("dydir.png");
  */
  
  st.Fit("gaus","rho");
  c1.Print("st.rho.png");
  /*
  st.Fit("gaus","dx");
  c1.Print("st.dx.png");
  st.Fit("gaus","dy");
  c1.Print("st.dy.png");
  st.Fit("gaus","dxdir");
  c1.Print("st.dxdir.png");
  st.Fit("gaus","dydir");
  c1.Print("st.dydir.png");
  */

  TFile g("tracks.root");
  Tk.Fit("gaus","rho");
  c1.Print("rho.TPH.png");
  Tk.Fit("gaus","strho");
  c1.Print("st.rho.TPH.png");
  
  /*
  Tk.Fit("gaus","zimp");
  c1.Print("rho.TPH.png");
  Tk.Fit("gaus","strho");
  c1.Print("st.rho.TPH.png");
  */
}
