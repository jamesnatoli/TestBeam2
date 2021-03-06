// Implements the functions defined in Fitting.h and used in Driver.cpp
#ifndef FITTING_CPP
#define FITTING_CPP
#include "Fitting.h"

double Fitting::normFitFunc(double *xcor, double *pars) {
  test_hist->Reset();
  counter++;
  for (unsigned int i = 0; i < num_events[0]; i++) {
    /*
    test_hist->Fill( (my_rand->Gaus( gaus_mean[0], gaus_sigma[0])) + 
		     (my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( pars[0], pars[1])) * pars[2]),
				     TMath::Sqrt(my_rand->Poisson( my_rand->Landau( pars[0], pars[1])) * pars[2]))));
    */
    test_hist->Fill( (my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( pars[0], pars[1])) * pars[2]),
				     TMath::Sqrt(my_rand->Poisson( my_rand->Landau( pars[0], pars[1])) * pars[2]))));
  }
  if (counter%100 == 0) {
    std::cout << counter << " function calls" << std::endl;
  }
  return test_hist->GetBinContent( test_hist->FindBin( xcor[0]));
}

double Fitting::pedFitFunc( double *xc, double *p) {
  test_ped->Reset();
  counter++;
  for (unsigned int i = 0; i < num_ped_events[0]; i++) {
    test_ped->Fill( (my_rand->Gaus( p[0], p[1])));
  }
  if (counter%100 == 0) {
    //std::cout << counter << " function calls" << std::endl;
  }
  return test_ped->GetBinContent( test_ped->FindBin( xc[0]));
}

// NOT READY YET
double Fitting::gpFitFunc(double *xcor, double *pars) {
  test_hist->Reset();
  if (counter%100 == 0) 
    std::cout << counter << " function calls" << std::endl;
  
  for (unsigned int i = 0; i < num_events[0]; i++) {
    test_hist->Fill( (my_rand->Gaus( gaus_mean[0], gaus_sigma[0])) + 
		     (my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( pars[0], pars[1])) * pars[2]),
				     TMath::Sqrt(my_rand->Poisson( my_rand->Landau( pars[0], pars[1])) * pars[2]))));
  }
  counter++;
  return test_hist->GetBinContent( test_hist->FindBin( xcor[0]));
}

// Constructor
Fitting::Fitting( const std::string name) {
  std::cout << "entered Fitting::Fitting " << std::endl;
  // Set up the file and the histogram
  my_file = new TFile( dir_en);
  real_hist = (TH1F*)my_file->Get( name.c_str());
  test_ped = new TH1F("Test Ped Hist", "", 37, -30, 30);
  TAxis *axe = real_hist->GetXaxis();
  std::cout << "x low = " << axe->GetBinLowEdge( axe->GetFirst()) << std::endl;
  std::cout << "x up = " << axe->GetBinUpEdge( axe->GetLast()) << std::endl;
  xlow = axe->GetBinLowEdge( axe->GetFirst());
  xup = axe->GetBinUpEdge( axe->GetLast());
  test_hist = new TH1F("Test Hist", "", 150, xlow, xup);
  my_rand = new TRandom();
  leg = new TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC");  
  my_name = name;
  counter = 0;

  if (!test_hist || !my_rand || !real_hist || !my_file->IsOpen() || !leg || !test_ped) {
    std::cout << "Danger, Will Robinson (fitting.cpp)" << std::endl;
    return;
  }

  std::cout << "leaving Fitting::Fitting " << std::endl;
}

Fitting::Fitting( ) {
  std::cout << "entered special constructor " << std::endl;
  my_file = new TFile();
  real_hist = new TH1F();
  test_hist = new TH1F();
  test_ped = new TH1F();
  my_rand = new TRandom();
  leg = new TLegend();
  counter = 0;
  my_name = "test";
  std::cout << "leaving special constructor" << std::endl;
}

void Fitting::normFit() {
  std::cout << "entered Fitting::normFit()" << std::endl;
  // The most probably value of the Landau, or is it?                                             
  params[0] = 2.75;
  // The sigma of the Landau                                                                      
  params[1] = 0.1;
  // The charge parameter                                                                         
  params[2] = 42;
  // Pedestal Mean
  /*
  params[3] = 0;
  // Pedestal Sigma
  params[4] = 7;
  */
  std::cout << "leaving Fitting::normFit()" << std::endl;
}

void Fitting::pedFit() {
  // Pedestal Mean
  ped_params[0] = 0;
  // Pedestal Sigma
  ped_params[1] = 7;
}

// NOT READY YET
void Fitting::gpFit() {
  // The most probably value of the Landau, or is it?                                             
  gp_params[0] = 2.75;
  // The sigma of the Landau                                                                      
  gp_params[1] = 0.1;
  // The charge parameter                                                                         
  gp_params[2] = 42;
  // Pedestal Mean
  gp_params[3] = 0;
  // Pedestal Sigma
  gp_params[4] = 7;
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

const std::string Fitting::getHist() {
  return real_hist->GetName();
}

const std::string Fitting::getName() {
  return my_name;
}

void Fitting::setHist( const std::string name) {
  my_name = name;
  real_hist = (TH1F*)my_file->Get( name.c_str());
}
#endif
