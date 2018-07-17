#include "Fitting.cpp"

class addFit: public Fitting {
 public:
  addFit( const std::string name);
  void theFit();
  void drawHists();
  double fitFunc( double *x, double *p);
};

addFit::addFit( const std::string name) : Fitting( name) {
  my_fit = new TF1( "Fit Function", Fitting::fptr, (&Fitting::fitFunc), 0, 1000, 6, "Fitting", "fitFunc");
  if (!my_fit) {
    std::cout << "Danger, Will Robinson (addFit)" << std::endl;
    return;
  }
}

void addFit::drawHists() {
  my_fit->SetLineColor( kRed);
  leg->AddEntry( my_fit, "Fit", "l");
  Fitting::drawHists();
  my_fit->Draw("same");
  leg->Draw("same");
  canv->Print("~/TestBeam/Fitting/Images/addFit.png");
  canv->SetLogx();
  canv->Print("~/TestBeam/Fitting/Images/addFit_log.png");
}

double addFit::fitFunc( double *xcor, double *params) {
  double retval = 0;
  if (counter % 1000 == 0)
    std::cout << counter << std::endl;
  counter++;
  retval =  params[2] * TMath::Landau( xcor[0], params[0], params[1]);
  retval += params[3] * TMath::Gaus( xcor[0], 1 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 2 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 3 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 4 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 5 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 6 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 7 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 8 * params[4], params[5]);
  retval += params[3] * TMath::Gaus( xcor[0], 9 * params[4], params[5]);
  
  // Return the function value
  return retval;
};

void addFit::theFit() {
  double pars[6] = { 0, 0, 0, 0, 0, 0};
  pars[0] = 84;
  pars[1] = 28;
  pars[2] = 12500;
  pars[3] = 1000;
  pars[4] = 42;
  pars[5] = 5;
  
  my_fit->SetParNames("Landau MPV   ", 
		      "Landau Sigma ", 
		      "Landau Norm  ",
		      "Gaus Norm    ", 
		      "Gaus Position", 
		      "Gaus Width   ");
  my_fit->SetParameters( pars);

  // Can either use the name or a pointer to the actual TF1 object
  if (!real_hist || !my_fit)
    std::cout << "ISSUE" << std::endl;
  real_hist->Fit( my_fit, "0");
};
