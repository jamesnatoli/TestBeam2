#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>
#include <TF1.h>
#include <TLegend.h>
#include <stdlib.h> 

#define SIZE 100

TH1F *background;
TRandom *my_rand;

double myLandau( double *xcor, double *par) {
  return TMath::Landau( xcor[0], par[0], par[1]);
}

double myGaus( double *xcor, double *par) {
  return TMath::Gaus( xcor[0], par[0], par[1]);
}

void histgen() {
  // generate the histogram background and save it to a file
  // background taken as Landau
  my_rand = new TRandom();
 
  // Landau background
  double (*landau_ptr)(double*, double*) = &myLandau;
  TF1 f1( "f1", landau_ptr, 0, SIZE, 2);
  // Landau needs 2 parameters
  f1.SetParameter( 0, 8);
  f1.SetParameter( 1, 5);
  f1.SetParameter( 2, 4);
  TH1F h( "background", "landau background", 100, 0, SIZE);
  // Fills the linear hist
  h.FillRandom( "f1", 10000);
  TFile f( "background.root", "recreate");
  //save the background histogram
  h.Write();
  //superimpose a gaussian signal to the background histogram
  double (*gaus_ptr)(double*, double*) = &myGaus;
  TF1 f2( "f2", gaus_ptr, 0, SIZE, 2);
  TF1 f3( "f3", gaus_ptr, 0, SIZE, 2);
  TF1 f4( "f4", gaus_ptr, 0, SIZE, 2);
  TF1 f5( "f5", gaus_ptr, 0, SIZE, 2);
  TF1 f6( "f6", gaus_ptr, 0, SIZE, 2);
  TF1 f7( "f7", gaus_ptr, 0, SIZE, 2);
  f2.SetParameters( 15, 2);
  f3.SetParameters( 30, 2);
  f4.SetParameters( 45, 2);
  f5.SetParameters( 60, 2);
  f6.SetParameters( 75, 2);
  f7.SetParameters( 90, 2);
  // Fill the histogram, now with the gaussian
  h.FillRandom( "f2", 2000);
  h.FillRandom( "f3", 2000);
  h.FillRandom( "f4", 2000);
  h.FillRandom( "f5", 2000);
  h.FillRandom( "f6", 2000);
  h.FillRandom( "f7", 2000);
  
  // This gets saved as a different thing
  h.SetName("result");
  h.Write();
}

// This function is passed to TF1 *ftot 
Double_t ftotal(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  // background defined in fitHist()
  Int_t bin = background->GetXaxis()->FindBin( xx);
  Double_t br = par[3]*background->GetBinContent( bin);
  Double_t arg = ( xx-par[1])/par[2];
  Double_t sr = par[0]*TMath::Exp(-0.5*arg*arg);
  return sr + br;
}

void fithist() {
  //fit function ftotal to signal + background
 
  histgen();
 
  // Open the file
  TFile *f = new TFile("background.root");
  background = (TH1F*)f->Get("background"); //pointer used in ftotal
  TH1F *result = (TH1F*)f->Get("result");
 
  TF1 *ftot = new TF1( "ftot", ftotal, 0, 10, 4);
  Double_t norm = result->GetMaximum();
  ftot->SetParameters( 0.5*norm, 5, 0.2, norm);
  ftot->SetParLimits( 0, 0.3*norm, norm);
 
  result->Fit( "ftot");
}
