// Implements the functions defined in Fitting.h and used in Driver.cpp
#ifndef FITTING_CPP
#define FITTING_CPP
#include "Fitting.h"

double Fitting::fitFunc(double *xcor, double *params) {
  test_hist->Reset();
  if (counter%100 == 0) 
    std::cout << counter << " function calls" << std::endl;
  
  for (unsigned int i = 0; i < num_events[0]; i++) {
    test_hist->Fill( (my_rand->Gaus( gaus_mean[0], gaus_sigma[0])) + 
		     (my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]),
				     TMath::Sqrt(my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]))));
  }
  counter++;
  return test_hist->GetBinContent( test_hist->FindBin( xcor[0]));
}

// Constructor
Fitting::Fitting( const std::string name) {
  // Set up the file and the histogram
  // Eventually I need to add something to this to allow other files to be fit
  my_file = new TFile( dir_en);
  test_hist = new TH1F("Test Hist", "", 100, 0, 600);
  real_hist = (TH1F*)my_file->Get("en_bins_EJ_260;1");
  my_rand = new TRandom();
  leg = new TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC");  
  
  counter = 0;

  if (!test_hist || !my_rand || !real_hist 
      || !my_file->IsOpen() || !leg) {
    std::cout << "Danger, Will Robinson" << std::endl;
    return;
  }

  my_name = name;
}

void Fitting::drawHists() {
  canv = new TCanvas("theCanvas", "", 500, 500);
  real_hist->SetLineColor(kBlack);
  real_hist->Draw();
  leg->AddEntry( real_hist, "Data", "l");
  leg->SetTextSize(.05);

  canv->SetLeftMargin(0.16);
  canv->SetBottomMargin(0.16);
  canv->SetLogy();
}

TF1* Fitting::getFit() {
  return my_fit;
}

const std::string Fitting::getName() {
  return my_name;
}
#endif
