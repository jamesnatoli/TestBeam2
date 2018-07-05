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

void tuneFit::theFit() {
  double params[7] = {0, 0, 0, 0, 0, 0, 0};
  // The most probably value of the Landau, or is it?                                             
  params[0] = 2.75;
  // The sigma of the Landau                                                                      
  params[1] = 0.1;
  // The charge parameter                                                                         
  params[2] = 42;
  // Pedestal Gaussian Mean                                                                       
  params[3] = 0;
  // Pedestal Gaussian Width                                                                      
  params[4] = 7;
  // Generalized Poisson                                                          
  params[5] = 3;

  tuneFit::fitFunc( params);  
};

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
}
