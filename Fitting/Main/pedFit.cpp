#include "Fitting.cpp"

pedFit::pedFit( const std::string name) : Fitting( name) {
  my_fit = new TF1( "Fit Function", this, (&pedFit::fitFunc), -30, 30, 2, "Fitting", "fitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (pedFit)" << std::endl;
    return;
  }
}

double pedFit::fitFunc( double *xcor, double *params) {
  test_ped->Reset();
  if (counter%100 == 0)
    std::cout << counter << " function calls" << std::endl;
  
  for (unsigned int i = 0; i < num_ped_events[0]; i++) {
    test_ped->Fill( (my_rand->Gaus( params[0], params[1])));
  }
  counter++;
  return test_ped->GetBinContent( test_ped->FindBin( xcor[0]));
}

void pedFit::drawHists() {
  my_fit->SetLineColor( kRed);
  leg->AddEntry( my_fit, "Fit", "l");
  Fitting::drawHists();
  my_fit->Draw("same");
  leg->Draw("same");
  canv->Print( Form( "%s/pedFit.png", dir_img));
  canv->SetLogx();
  canv->Print( Form( "%s/pedFit_log.png", dir_img));
}

void pedFit::theRealFit() {
  std::cout << "entered pedFit::theFit" << std::endl;
  my_fit->SetParNames( "Ped Gaus Mean ",
		       "Ped Gaus Sigma");

  std::cout << ped_params[0] << " and " << ped_params[1] << std::endl;
  my_fit->SetParameters( ped_params);

  // IMPORTANT CALL
  // uses the TF1 variable
  std::cout << "about to call MINUIT" << std::endl;
  real_hist->Fit( my_fit, "R0");
  std::cout << "leaving pedFit::theFit" << std::endl;
};

void pedFit::theTuneFit() {
  pedFit::otherFitFunc( ped_params); 
}

void pedFit::otherFitFunc( double *params) {  
  double one[1] = {0};
  pedFit::fitFunc( one, params);
};
