#include "Fitting.cpp"

realFit::realFit( const std::string name) : Fitting( name) {
  Fitting::fptr = new Fitting();
  my_fit = new TF1( "Fit Function", fptr, (&Fitting::pedFitFunc), 0, 500, 2, "Fitting", "pedFitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (realFit)" << std::endl;
    return;
  }
}

// realFit::realFit( ) : Fitting( ) { }

void realFit::normFit() {
  std::cout << "entered realFit::normFit" << std::endl;
  my_fit->SetFunction( fptr, &Fitting::normFitFunc);
  my_fit->SetRange(0, 500);
  my_fit->SetTitle("Normal_Fit_Function");
  // Set the Parameters
  Fitting::normFit();
  std::cout << "here" << std::endl;
  std::cout << "params[0] = " << params[0] << std::endl;
  my_fit->SetParameters( params);
  std::cout << "here2" << std::endl;
  my_fit->SetParNames( "Landau mpv    ",
		       "Landau Sigma  ",
		       "Charge Param  ",
		       "Ped Gaus Mean ",
		       "Ped Gaus Sigma");
  std::cout << "here1" << std::endl;
  my_fit->FixParameter(3, 0);
  my_fit->FixParameter(4, 7);

  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving realFit::normFit" << std::endl;
}

void realFit::pedFit() {
  std::cout << "entered realFit::pedFit" << std::endl;
  // Set the Parameters
  // Fitting::pedFit();
  std::cout << "pls help" << std::endl;
  double ped_params[2] = {0, 0};
  params[0] = 0;
  params[1] = 7;
  my_fit->SetParameters( ped_params);
  std::cout << "pls help me" << std::endl;
  my_fit->SetParNames( "Ped Gaus Mean ",
		       "Ped Gaus Sigma");

  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving realFit::pedFit" << std::endl;
}

// NOT READY YET
void realFit::gpFit() {
  std::cout << "entered realFit::gpFit" << std::endl;
    my_fit->SetParameters( Fitting::gp_params);
  my_fit->SetParNames( "Landau mpv    ",
		       "Landau Sigma  ",
		       "Charge Param  ",
		       "Ped Gaus Mean ",
		       "Ped Gaus Sigma");

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
