#include "Fitting.cpp"

realFit::realFit( const std::string name) : Fitting( name) {
  Fitting::fptr = new Fitting();
  my_fit = new TF1( "Fit Function", Fitting::fptr, (&Fitting::fitFunc), 0, 500, 3, "Fitting", "fitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (realFit)" << std::endl;
    return;
  }
}

void realFit::drawHists() {
  my_fit->SetLineColor( kRed);
  leg->AddEntry( my_fit, "Fit", "l");
  Fitting::drawHists();
  my_fit->Draw("same");
  leg->Draw("same");
  canv->Print( Form( "%s/realFit.png", dir_img));
  canv->SetLogx();
  canv->Print( Form( "%s/realFit_log.png", dir_img));
}

void realFit::theFit() {
  std::cout << "entered realFit::theFit" << std::endl;
  // Parameter Numbering starts at 0
  my_fit->SetParNames( "Landau mpv    ",
		       "Landau Sigma  ",
		       "Charge Param  ");
		       // "Ped Gaus Sigma",
		       // "Ped Gaus Mean ");
  double params[5] = {0, 0, 0, 0, 0};
  params[0] = 2.75;
  params[1] = 0.1;
  params[2] = 42;
  // params[3] = 0;
  // params[4] = 7;
  my_fit->SetParameters( params);
  // my_fit->FixParameter( 2, 42);

  // IMPORTANT CALL
  // uses the TF1 variable
  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving realFit::theFit" << std::endl;
};
