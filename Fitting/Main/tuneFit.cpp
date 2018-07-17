#include "Fitting.cpp"

// This just calls the superclass constructor
tuneFit::tuneFit( const std::string name) : Fitting( name) { };

// This function is not used, but we still need to add it because the TF1 object
// needs this funciton to exit
double tuneFit::fitFunc( double *x, double *p) { 
  return Fitting::fitFunc( x, p);
};

void tuneFit::fitFunc( double *params) {  
  double one[1] = {0};
  Fitting::fitFunc( one, params);
};

// Just call the Base function
void tuneFit::theFit() { 
  tuneFit::fitFunc( params);
}

void tuneFit::drawHists() {
  leg->AddEntry( test_hist, "Test Hist", "l");
  test_hist->SetLineColor( kRed);
  Fitting::drawHists();
  test_hist->Draw("SAME");
  leg->Draw("SAME");
  char *help = Form( "/Images/%s.png", Fitting::getName().c_str());
  canv->Print( Form( "%s/tuneFit.png", dir_img));
  canv->SetLogx();
  canv->Print( Form( "%s/tuneFit_log.png", dir_img));
};
