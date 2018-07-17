#include "Fitting.cpp"

class splitFit: public Fitting {
 public:
  splitFit( const std::string name);
  void theFit();
  void drawHists();
};

splitFit::splitFit( const std::string name) : Fitting( name) { };

void splitFit::drawHists() { 
  Fitting::drawHists(); 
  canv->Print("~/TestBeam/Fitting/Images/splitFit.png");
  canv->SetLogx();
  canv->Print("~/TestBeam/Fitting/Images/splitFit_log.png");
};

void splitFit::theFit() {
  TF1 *f1 = new TF1("1st Gaussian", "gaus", 20, 60);
  TF1 *f2 = new TF1("2nd Gaussian", "gaus", 60, 102);
  TF1 *f3 = new TF1("3rd Gaussian", "gaus", 102, 144);
  TF1 *f4 = new TF1("4th Gaussian", "gaus", 144, 186);
  TF1 *f5 = new TF1("5th Gaussian", "gaus", 186, 228);
  TF1 *f6 = new TF1("6th Gaussian", "gaus", 228, 270);
  TF1 *f7 = new TF1("7th Gaussian", "gaus", 270, 312);
  TF1 *f8 = new TF1("8th Gaussian", "gaus", 312, 354);
  TF1 *f9 = new TF1("9th Gaussian", "gaus", 354, 396);
  TF1 *f10 = new TF1("10th Gaussian", "gaus", 396, 438);
  TF1 *f11 = new TF1("11th Gaussian", "gaus", 438, 480);
  TF1 *f12 = new TF1("12th  Gaussian", "gaus", 480, 522);
  
  TF1 *fped = new TF1("Pedestal Gaussian", "gaus", -12, 12);

  real_hist->Fit(f1, "R+");
  real_hist->Fit(f2, "R+");
  real_hist->Fit(f3, "R+");
  real_hist->Fit(f4, "R+");
  real_hist->Fit(f5, "R+");
  real_hist->Fit(f6, "R+");
  real_hist->Fit(f7, "R+");
  real_hist->Fit(f8, "R+");
  real_hist->Fit(f9, "R+");
  real_hist->Fit(f10, "R+");
  real_hist->Fit(f11, "R+");
  real_hist->Fit(f12, "R+");
  
  real_hist->Fit(fped, "R+");
};
