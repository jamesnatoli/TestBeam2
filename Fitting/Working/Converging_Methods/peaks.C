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
#include "TLine.h"
#include "TKey.h"
// #include "TClass.h"
#include <iostream>
#include <fstream>

Int_t npeaks = 30;
Int_t offset = 0;

// The function that goes with the TF1 objects
Double_t fpeaks(Double_t *x, Double_t *par) {
  //Double_t result = par[0] + par[1]*x[0];
  Double_t result = (par[0]*TMath::Landau( x[0], par[1], par[2]));
  // Loop over the number of peaks to generate them
  for (Int_t p = 0; p < npeaks; p++) {
    Double_t norm  = par[3*p+3];
    Double_t mean  = par[3*p+4];
    Double_t sigma = par[3*p+5];
    // Computes sums of Gaussians
    result += norm*TMath::Gaus( x[0], mean, sigma);
  }
  return result;
  // return result += (par[0]*TMath::Landau( x[0], par[1], par[2]));;
}

// pass in the number of peaks you want to fit
void peaks( Int_t np = 10) {
  npeaks = TMath::Abs(np);

  // Get the data histograms
  char const *dir_en = "~/TB_Analysis_17/new_git_code/TestBeam2/energy_hists.root";
  TFile *file = new TFile( dir_en);
  // Create an iterator for the histogram
  TIter next( file->GetListOfKeys());
  TKey *key;
  // Loop over all of the tiles
  while ((key = (TKey*)next())) {
    // TClass *cl = gROOT->GetClass(key->GetClassName());
    // if (!cl->InheritsFrom("TH1")) 
    //   continue;
    // Convert the key object to a TH1
    TH1 *h = (TH1*)key->ReadObj();
    // TH1 *h = (TH1*)file->Get("en_bins_SCSN_81S;1");
    TString *n = new TString(h->GetName());
    char const *name = n->Data();
    // Only want to fit the "bins" ones
    if ( !(TString(name).Contains("bins")) ) 
      continue;


    std::cout << "******** ANALYZING " << name << " ********" << std::endl;
  
    Double_t par[3000];
  
    Int_t p;
    TCanvas *c1 = new TCanvas("c1", "Fit Results", 750, 750);
    c1->Divide(1,2);
    c1->cd(1);

    h->Draw();
    // Create a copy
    TH1F *h2 = (TH1F*)h->Clone("h2");
    //Use TSpectrum to find the peak candidates
    TSpectrum *s = new TSpectrum(2*npeaks);
    Double_t search_sig = 8, threshold = 0.002;
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
    TF1 *fline = new TF1( "fline", "pol1", 0, 600);
    TF1 *fland = new TF1( "fland", "landau", 0, 500);
    fline->SetLineColor( kBlue);
    fland->SetLineColor( kGreen);
   
    // Q = quiet mode, N = do not store graphics, do not draw
    h->Fit( "fland", "rq");
    h->Fit( "fline", "rq+");

    // Get the PolyMarker
    TPolyMarker *polym = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
    polym->SetMarkerColor( kYellow);

    TLegend *leg = new TLegend( 0.5, 0.5, 0.9, 0.9);
    leg->AddEntry( h, name, "l");
    leg->AddEntry( hb, "Background", "l");
    leg->AddEntry( fline, "Linear Fit", "l");
    leg->AddEntry( fland, "Landau Fit" , "l");
    leg->AddEntry( polym, "Peaks" , "p");
    leg->Draw();

    // The Gaussian Peaks are added to this original background
    // Landau has 3 pars, Norm(0), MPV(1), Sigma(2)
    par[0] = fland->GetParameter(0);
    par[1] = fland->GetParameter(1);
    par[2] = fland->GetParameter(2);
  
    // Loop on all found peaks. Eliminate peaks at the background level
    npeaks = 0;

    // xpeaks is array of the x locations of the peaks
    Double_t *xpeaks = s->GetPositionX();

    // Create a vector so we can sort the peaks :) 
    std::vector<Double_t> sxpeaks ( xpeaks, xpeaks + nfound);
    std::sort( sxpeaks.begin(), sxpeaks.end());

    // Create a flag for if the pedestal peak was discarded
    bool noPed = false;

    // This is the xmax for finger fits
    int finger_max = 300;

    for ( p = 0; p < nfound; p++) {
      Double_t xp = sxpeaks[p];
      std::cout << "Checking peak " << p+1 << " at " << xp << std::endl;

      // Get the bin number of where the peak is
      Int_t bin = h->GetXaxis()->FindBin(xp);

      // Get the y value of that bin
      Double_t yp = h->GetBinContent(bin);

      // Compare with the Landau, rather than the line to get more peaks
      Int_t bbin = hb->GetXaxis()->FindBin( xp);
      Double_t byp = hb->GetBinContent( bbin);

      /*
      if (yp-TMath::Sqrt(yp) < byp ) {
	 if (yp-TMath::Sqrt(yp) < fland->Eval(xp)) {
	 if (yp < fland->Eval(xp)) { 
	if ( p == 0)
	  noPed = true;
	printf( "Eliminated peak %d at %f\n", (p + 1), xp);
	printf( "Value was %f, background was %f\n", yp-TMath::Sqrt(yp), byp);
	continue;
      } */
      
      // Don't want to add peaks that might be out of range of fit
      if ( TString(name).Contains("F") && (xp > finger_max)) {
	printf( "Eliminated peak %d at %f\n", (p + 1), xp);
	continue;
      }

      // Overwrite par array with new values of "useful" peaks
      par[3*npeaks+3] = yp; // Norm
      par[3*npeaks+4] = xp; // Mean
      par[3*npeaks+5] = 3;  // Sigma
      npeaks++;
    }

    Int_t diff = nfound - npeaks;
    Int_t npars = (npeaks * 3) + 3;
    // printf("Eliminated %d peaks that were at background level\n", diff);
    // printf( "Didn't try to eliminate any peaks\n");
    printf( "Eliminated Finger peaks above %d\n", finger_max);
    printf("Found %d useful peaks to fit\n", npeaks);
    printf("Now fitting: Be patient\n");

    c1->cd(2);
    // 3 from the landau lackground + (3 * 14 peaks) ==> 45 params
    Double_t xmin = h2->GetXaxis()->GetBinLowEdge( h2->GetXaxis()->GetFirst());
    std::cout << "xmin = " << xmin << std::endl;
    Double_t xmax = h2->GetXaxis()->GetBinUpEdge( h2->GetXaxis()->GetLast());
    std::cout << "xmax = " << xmax << std::endl;
    Double_t xmin1 = -1000.0;
    TF1 *fit;
    if ( TString( name).Contains("F"))
      fit = new TF1( "fit", fpeaks, xmin, finger_max, npars);
    else
      fit = new TF1( "fit", fpeaks, xmin, xmax, npars);
    std::cout << "We have " << npars << " parameters" << std::endl;

    // we may have more than the default 25 parameters
    // This uses the clone, h2 and sets the maximum amount of parameters
    TVirtualFitter::Fitter( h2, 15+3*npeaks);
    TVirtualFitter::SetMaxIterations( 200000);

    // Uses only the new "useful" peaks
    fit->SetParameters(par);
    fit->SetParName( 0, "BKG Norm");
    fit->SetParName( 1, "BKG MPV ");
    fit->SetParName( 2, "BKG Sig ");
    // fit->SetParName( 3, "Ped Norm");
    // fit->SetParName( 4, "Ped Mean");
    // fit->SetParName( 5, "Ped Sig ");

    // Constrain the Pedestal Mean
    if ( !noPed) {
      printf("Setting Pedestal Limits\n");
      // fit->SetParLimits( 4, -10, 4);
      fit->SetParameter( 5, 6);
      fit->SetParLimits( 5, 5, 10);
    }
    // fit->FixParameter( 43, par[43]);
    // fit->FixParameter( 44, par[44]);

    Int_t ngaus = 1;
    for (Int_t i = 3; i < fit->GetNpar(); i++){
      if (i%3 == 0)
	fit->SetParName( i, Form("Gaus %d Norm", ngaus));
      else if (i%3 == 1) 
	fit->SetParName( i, Form("Gaus %d Mean", ngaus));
      else if (i%3 == 2) {
	fit->SetParName( i, Form("Gaus %d Sig ", ngaus));
	ngaus++;
      }
    }


    fit->SetNpx(1000);
    fit->SetLineColor( kRed);
    h2->Fit("fit", "R");

    // Analyze the fitted parameters
    ngaus = 1;
    std::vector<Double_t> norms;
    std::vector<Double_t> means;
    std::vector<Double_t> widths;
    // Label the Gaussians
    for (Int_t i = 3; i < fit->GetNpar(); i++){
      if (i%3 == 0) {
	fit->SetParName( i, Form("Gaus %d Norm", ngaus));
	norms.push_back( fit->GetParameter(i));
      }
      else if (i%3 == 1) {
	fit->SetParName( i, Form("Gaus %d Mean", ngaus));
	means.push_back( fit->GetParameter(i));
      }
      else if (i%3 == 2) {
	fit->SetParName( i, Form("Gaus %d Sig ", ngaus));
	widths.push_back( fit->GetParameter(i));
	ngaus++;
      }
    }
  
    std::cout << std::endl << "Distance between peaks" << std::endl;
    for (unsigned int i = 0; i < means.size() - 1; i++)
      std::cout << i+1 << " and " << i+2 << " = " << means[i + 1] - means[i] << std::endl;

    std::cout << std::endl << "Means" << std::endl;
    for (unsigned int i = 0; i < means.size(); i++)
      std::cout << "Peak " << i+1 << " Mean: " << means[i] << std::endl;

    std::cout << std::endl << "Normalizations" << std::endl;
    for (unsigned int i = 0; i < norms.size(); i++)
      std::cout << "Peak " << i+1 << " Norm: " << norms[i] << std::endl;
  
    std::cout << std::endl << "Widths" << std::endl;
    for (unsigned int i = 0; i < widths.size(); i++)
      std::cout << "Peak " << i+1 << " Width: " << widths[i] << std::endl;
    std::cout << std::endl;

    TFile *output = new TFile( Form("Root_Results/fitResults_%s.root", name), "RECREATE");
    if (output->IsOpen()) {
      printf("Writing to ROOT file \n");
      h2->Write();
      fit->Write();
      output->Close();
    }
  
    printf("Writing to text file\n");
    ofstream fout;
    fout.open( Form("Text_Results/fitResults_%s.txt", name));
    fout << "\t\t\t*** FIT RESULTS ***" << std::endl;
    fout << setw(20) << left << "Name " 
	 << setw(20) << left << "Value" 
	 << setw(20) << left << "Error" 
	 << setw(20) << left << "Percent Error" 
	 << std::endl;
    for (int i = 0; i < fit->GetNpar(); i++) {
      fout << setw(20) << left << fit->GetParName(i) 
	   << setw(20) << left << fit->GetParameter(i) 
	   << setw(20) << left << fit->GetParError(i) 
	   << setw(20) << left << (fit->GetParError(i)/fit->GetParameter(i))*100
	   << std::endl;
    
    }
    fout.close();

    TLegend *leg1 = new TLegend( 0.7, 0.7, 0.9, 0.9);
    TLegend *leg2 = new TLegend( 0.2, 0.2, 0.4, 0.4);
    leg1->AddEntry( h2, name, "l");
    leg1->AddEntry( fit, "Fit", "l");
    leg2->AddEntry( h2, name, "l");
    leg2->AddEntry( fit, "Fit", "l");

    c1->Print( Form("Images/%s/peakFinderFull_%s.png", name, name));
    c1->cd(1);
    gPad->Print( Form("Images/%s/peakFinder_%s.png", name, name));
    c1->cd(2);
    leg1->Draw();
    gPad->Print( Form("Images/%s/peakFit_%s.png", name, name));
    delete leg1;
    gPad->SetLogy();
    leg2->Draw();
    gPad->SetLogx();
    gPad->Print( Form("Images/%s/peakFit_logxy_%s.png", name, name));
    gPad->SetLogx(0);
    gPad->Print( Form("Images/%s/peakFit_logy_%s.png", name, name));
  }
}

    
