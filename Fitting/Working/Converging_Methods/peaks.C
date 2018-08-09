// Illustrates how to find peaks in histograms.
// This script generates a random number of gaussian peaks
// on top of a linear background.
// The position of the peaks is found via TSpectrum and injected
// as initial values of parameters to make a global fit.
// The background is computed and drawn on top of the original histogram.
//
// To execute this example, do
//  root > .x peaks.C  (generate 10 peaks by default)
//  root > .x peaks.C++ (use the compiler)
//  root > .x peaks.C++(30) (generates 30 peaks)
//
// To execute only the first part of the script (without fitting)
// specify a negative value for the number of peaks, eg
//  root > .x peaks.C(-20)
//
//Author: Rene Brun

#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVirtualFitter.h"
#include "TFile.h"
#include "TLegend.h"
#include "TPolyMarker.h"

Int_t npeaks = 30;
// The function that goes with the TF1 objects
Double_t fpeaks(Double_t *x, Double_t *par) {
   Double_t result = par[0] + par[1]*x[0];
   // Loop over the number of peaks to generate them
   for (Int_t p=0;p<npeaks;p++) {
      Double_t norm  = par[3*p+2];
      Double_t mean  = par[3*p+3];
      Double_t sigma = par[3*p+4];
      // Computes sums of Gaussians
      result += norm*TMath::Gaus(x[0],mean,sigma);
   }
   return result;
}

// pass in the number of peaks you want to fit
void peaks(Int_t np=10) {
   npeaks = TMath::Abs(np);
   //generate n peaks at random

   Double_t par[3000];

   // par[0] = 0.8;
   // par[1] = -0.6/1000;
   Int_t p;
   /*
   // This fills the parameter array, which is later overritten
   for (p=0;p<npeaks;p++) {
     par[3*p+2] = 1;
     par[3*p+3] = 10+gRandom->Rndm()*980;
     par[3*p+4] = 3+2*gRandom->Rndm();
   }
   // 
   TF1 *f = new TF1("f",fpeaks,0,1000,2+3*npeaks);
   f->SetNpx(1000);
   f->SetParameters(par);
   */

   TCanvas *c1 = new TCanvas("c1", "Fit Results", 750, 750);
   c1->Divide(1,2);
   c1->cd(1);
   //h->FillRandom("f",200000);
   // Get the data histogram
   char const *dir_en = "~/TB_Analysis_17/new_git_code/TestBeam2/energy_hists.root";
   TFile *file = new TFile( dir_en);
   TH1F *h = (TH1F*)file->Get( "en_bins_EJ_260;1");
   h->Draw();
   // Create a copy
   TH1F *h2 = (TH1F*)h->Clone("h2");
   //Use TSpectrum to find the peak candidates
   TSpectrum *s = new TSpectrum(2*npeaks);
   Double_t search_sig = 2, threshold = 0.01;
   Int_t nfound = s->Search( h, search_sig, "", threshold);
   // This is so depricated but ok
   printf("Found %d candidate peaks to fit\n",nfound);
   //Estimate background using TSpectrum::Background
   Int_t iterations = 20;
   // hb is now the background histogram
   TH1 *hb = s->Background( h, iterations, "same");
   if (hb) 
     c1->Update();
   // This allows us to JUST extract the background, if we want
   if (np <0) 
     return;

   // estimate linear background using a fitting method
   // also use landau background to get more peaks
   // Here we make a line and see if any of the peaks are even with the line, then we eliminate them
   TF1 *fline = new TF1("fline","pol1",0,1000);
   TF1 *fland = new TF1("fland","landau", 0, 1000);
   fland->SetLineColor( kGreen);
   fline->SetLineColor( kBlue);
   // Q = quiet mode, N = do not store graphics, do not draw
   h->Fit( "fland", "q");
   h->Fit( "fline", "q+");

   // Get the PolyMarker
   TPolyMarker *polym = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");

   TLegend *leg = new TLegend( 0.5, 0.5, 0.9, 0.9);
   leg->AddEntry( h, "Data", "l");
   leg->AddEntry( hb, "Background", "l");
   leg->AddEntry( fline, "Linear Fit", "l");
   leg->AddEntry( fland, "Landau Fit" , "l");
   leg->AddEntry( polym, "Peaks" , "p");
   leg->Draw();

   // Loop on all found peaks. Eliminate peaks at the background level
   par[0] = fline->GetParameter(0);
   par[1] = fline->GetParameter(1);
  
   npeaks = 0;
   // xpeaks is array of the x locations of the peaks
   Double_t *xpeaks = s->GetPositionX();
   for (p=0;p<nfound;p++) {
     Double_t xp = xpeaks[p];
     // Get the bin number of where the peak is
     Int_t bin = h->GetXaxis()->FindBin(xp);
     // Get the y value of that bin
     Double_t yp = h->GetBinContent(bin);
     // Compare with the Landau, rather than the line to get more peaks
     if (yp-TMath::Sqrt(yp) < fland->Eval(xp)) 
       continue;
     // Overwrite par array with new values of "useful" peaks
     par[3*npeaks+2] = yp; // Mean
     par[3*npeaks+3] = xp; // Norm
     par[3*npeaks+4] = 3;  // Sigma
     npeaks++;
   }
   Int_t diff = nfound - npeaks;
   printf("Eliminated %d peaks that were at background level\n", diff);
   printf("Found %d useful peaks to fit\n", npeaks);
   printf("Now fitting: Be patient\n");

   c1->cd(2);
   // 2 parameters from the background line, and 3 for each subsequent peak = 32 parameters
   TF1 *fit = new TF1( "fit", fpeaks, 0, 1000, 2+3*npeaks);

   //we may have more than the default 25 parameters
   // This uses the clone, h2
   TVirtualFitter::Fitter(h2,10+3*npeaks);

   // Uses only the new "useful" peaks
   fit->SetParameters(par);
   fit->SetNpx(1000);
   fit->SetLineColor( kRed);
   h2->Fit("fit");

   TLegend *leg1 = new TLegend( 0.7, 0.7, 0.9, 0.9);
   TLegend *leg2 = new TLegend( 0.2, 0.2, 0.4, 0.4);
   leg1->AddEntry( h2, "Data", "l");
   leg1->AddEntry( fit, "Fit", "l");
   leg2->AddEntry( h2, "Data", "l");
   leg2->AddEntry( fit, "Fit", "l");

   c1->Print( "peakFinderFull.png");
   c1->cd(1);
   gPad->Print( "peakFinder.png");
   c1->cd(2);
   leg1->Draw();
   gPad->Print( "peakFit.png");
   delete leg1;
   gPad->SetLogy();
   leg2->Draw();
   gPad->Print( "peakFit_logy.png");
   gPad->SetLogx();
   gPad->Print( "peakFit_logxy.png");
}
