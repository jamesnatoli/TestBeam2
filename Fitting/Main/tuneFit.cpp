#include "Fitting.cpp"

// This just calls the superclass constructor
tuneFit::tuneFit( const std::string name) : Fitting( name) { };

// Just call the Base function
void tuneFit::normFit() { 
  Fitting::normFit();
  Fitting::normFitFunc( one, Fitting::params);
  drawHists( test_hist);
}

void tuneFit::pedFit() { 
  Fitting::pedFit();
  Fitting::pedFitFunc( one, Fitting::params);
  drawHists( test_ped);
}

void tuneFit::gpFit() { 
  Fitting::gpFit();
  Fitting::gpFitFunc( one, Fitting::params);
  drawHists( test_hist);
}

void tuneFit::drawHists( TH1F *histogram) {
  leg->AddEntry( histogram, "Test Hist", "l");
  histogram->SetLineColor( kRed);
  Fitting::drawHists();
  histogram->Draw("SAME");
  leg->Draw("SAME");
  canv->Print( Form( "%s/%s_tuneFit.png", dir_img, Fitting::getHist().c_str()));
  canv->SetLogx();
  canv->Print( Form( "%s/%s_tuneFit_log.png", dir_img, Fitting::getHist().c_str()));
  leg->Clear();
};
