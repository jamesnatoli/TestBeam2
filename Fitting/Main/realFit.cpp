#include "Fitting.cpp"

realFit::realFit( const std::string name) : Fitting( name) {
  Fitting::fptr = new Fitting();
}

void realFit::normFit() {
  std::cout << "entered realFit::normFit" << std::endl;
  // Set the Parameters
  Fitting::normFit();
  my_fit = new TF1( "Fit Function", Fitting::fptr, (&Fitting::normFitFunc), 0, 500, 5, "Fitting", "fitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (realFit)" << std::endl;
    return;
  }
  my_fit->SetParNames( "Landau mpv    ",
		       "Landau Sigma  ",
		       "Charge Param  ",
		       "Ped Gaus Mean ",
		       "Ped Gaus Sigma");
  my_fit->SetParameters( Fitting::params);

  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving realFit::normFit" << std::endl;
}

void realFit::pedFit() {
  std::cout << "entered realFit::pedFit" << std::endl;
  // Need to fix the range here
  my_fit = new TF1( "Fit Function", Fitting::fptr, (&Fitting::pedFitFunc), 0, 500, 2, "Fitting", "fitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (realFit)" << std::endl;
    return;
  }
  my_fit->SetParNames( "Ped Gaus Mean ",
		       "Ped Gaus Sigma");

  my_fit->SetParameters( Fitting::params);

  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving realFit::pedFit" << std::endl;
}

// NOT READY YET
void realFit::gpFit() {
  std::cout << "entered realFit::gpFit" << std::endl;
  my_fit = new TF1( "Fit Function", Fitting::fptr, (&Fitting::gpFitFunc), 0, 500, 5, "Fitting", "fitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (realFit)" << std::endl;
    return;
  }
  my_fit->SetParNames( "Landau mpv    ",
		       "Landau Sigma  ",
		       "Charge Param  ",
		       "Ped Gaus Mean ",
		       "Ped Gaus Sigma");
  my_fit->SetParameters( Fitting::params);

  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving realFit::gpFit" << std::endl;
}

void realFit::drawHists( ) {
  my_fit->SetLineColor( kRed);
  leg->AddEntry( my_fit, "Fit", "l");
  Fitting::drawHists();
  my_fit->Draw("same");
  leg->Draw("same");
  canv->Print( Form( "%s/%srealFit.png", dir_img, Fitting::getHist().c_str()));
  canv->SetLogx();
  canv->Print( Form( "%s/%srealFit_log.png", dir_img, Fitting::getHist().c_str()));
}
