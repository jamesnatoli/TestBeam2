#include <TMath.h>
#include <TCanvas.h>
#include <iostream>
#include <TF1.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TFile.h>

#define MPARS 4
#define GLPARS 3
#define PPARS 2

TF1 *func, *gfunc, *lfunc, *pfunc;
TRandom *my_rand;
TH1F *data;
TFile *file;
char const *dir_en;
double result;

double gausFunc( double *xcor, double *par) {
  result = TMath::Gaus( xcor[0], par[0], par[1]);
  return par[2] * result;
}

double landFunc( double *xcor, double *par) {
  result = TMath::Landau( xcor[0], par[0], par[1]);
  return par[2] * result;
}

double poisFunc( double *xcor, double *par) {
  result = (TMath::Poisson( xcor[0], par[0]) * par[1]);
  return result;
}
double myFunc( double *xcor, double *par) {
  double mean = my_rand->Poisson( par[0]);
  result = TMath::Gaus( xcor[0], (mean * par[2]), par[1]);
  return par[3] * result;
}

void functionPlot() {
  dir_en = "~/TB_Analysis_17/new_git_code/TestBeam2/energy_hists.root";
  file = new TFile( dir_en);
  data = (TH1F*)file->Get( "en_bins_EJ_260;1");
  my_rand = new TRandom();

  double (*ptr)(double*, double*) = &myFunc;
  double (*gptr)(double*, double*) = &gausFunc;
  double (*lptr)(double*, double*) = &landFunc;
  double (*pptr)(double*, double*) = &poisFunc;

  double xmin = 0, xmax = 1000;
  func = new TF1( "mainF", ptr, xmin, xmax, MPARS);
  gfunc = new TF1( "gausF", gptr, xmin, xmax, GLPARS);
  lfunc = new TF1( "landauF", lptr, xmin, xmax, GLPARS);
  pfunc = new TF1( "poissonF", pptr, xmin, xmax, PPARS);

  double mpars[MPARS] = {0};
  mpars[0] = 40.0;    // Poisson Mean
  mpars[1] = TMath::Sqrt( mpars[0]);     // Gaus Sig
  mpars[2] = 15.0;   // Scale Factor
  mpars[3] = 10.0; // Constant
  func->SetParameters( mpars);

  double gpars[GLPARS] = {0};
  gpars[0] = 50.0; // Mean
  gpars[1] = 7.0; // Sigma
  gpars[2] = 10.0; // Scale Factor
  gfunc->SetParameters( gpars);

  double lpars[GLPARS] = {0};
  lpars[0] = 40; // Mean
  lpars[1] = 10.0; // Sigma
  lpars[2] = 978; // Scale Factor
  lfunc->SetParameters( lpars);

  double ppars[PPARS] = {0};
  ppars[0] = 2.9; // Mean
  ppars[1] = 37.5; // Scale Factor
  pfunc->SetParameters( ppars);

  TCanvas *canv = new TCanvas( "canv", "Main", 500, 500);
  func->SetLineColor( kRed);
  func->Draw();
  func->GetYaxis()->SetRangeUser( 0, 10);
  canv->Print( "Images/functionPlot.png");

  TCanvas *gcanv = new TCanvas( "gaus canv", "Gaus", 500, 500);
  gfunc->SetLineColor( kRed);
  gfunc->Draw();
  gfunc->GetYaxis()->SetRangeUser( 0, 10);
  gcanv->Print( "Images/gausPlot.png");

  //TCanvas *lcanv = new TCanvas( "landau canv", "Landau", 500, 500);
  //lfunc->SetLineColor( kRed);
  //lfunc->Draw();
  //lfunc->GetYaxis()->SetRangeUser( 0, 1000);
  //lcanv->SetLogy();
  //lcanv->Print( "Images/landauPlot.png");

  //TCanvas *pcanv = new TCanvas( "poisson canv", "Poisson", 500, 500);
  //pfunc->SetLineColor( kRed);
  //pfunc->Draw();
  //pfunc->GetYaxis()->SetRangeUser( 0, 10);
  //pcanv->Print( "Images/poissonPlot.png");

  int bins = 100;
  TH1F *test = new TH1F( "Test", "", bins, 0, 100);
  TH1F *test_gaus = new TH1F( "Test_Gaussian", "", bins, 0, 100);
  TH1F *test_landau = new TH1F( "Test_Landau", "", bins, 0, 100);
  TH1F *test_poisson = new TH1F( "Test_Poisson", "", bins, 0, 1000);
  double gmean = 50, gsig = 7, lmpv = 40, lsig = 10, pmean = 10, pscale = 42;

  for (unsigned int i = 0; i < 10000; i++) {
    test_gaus->Fill( my_rand->Gaus( gmean, gsig));
    test_landau->Fill( my_rand->Landau( lmpv, lsig));
    test_poisson->Fill( my_rand->Poisson( pmean) * pscale);
  }

  //test_poisson->FillRandom( "poissonF", 20000);
  
  TCanvas *hcanv = new TCanvas( "Histogram Canvas", "Histogram", 500, 500);
  //test_gaus->Draw();
  //func->Draw( "same");
  //test_gaus->Fit( gfunc);
  //test_landau->Fit( lfunc);
  //test_poisson->Fit( pfunc);
  test_poisson->Draw();
  hcanv->Print( "Images/histPlot.png");
}
