#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>
#include <TF1.h>
#include <TLegend.h>

TFile *my_file;
TH1F *test_hist, *data_hist;
TCanvas *canv;
TRandom *my_rand;
TLegend *leg;
TF1 *my_fit;
char *my_name;
int counter;
char *dir_en = "~/TestBeam/Fitting/energy_hists.root";

double fitFunc(double *xcor, double *params) {
  test_hist->Reset();
  if (counter%100 == 0)
    std::cout << counter << std::endl;
  for (unsigned int i = 0; i < 750000; i++) {
    test_hist->Fill( (my_rand->Gaus( params[3], params[4])) + 
		     (my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]),
				     TMath::Sqrt(my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]))));
  }
  counter++;
  return test_hist->GetBinContent( test_hist->FindBin( xcor[0]));
}

void drawHists() {
  // ****** DRAW EVERYTHING ******

  data_hist->SetLineColor(kBlack);
  data_hist->Draw("same");

  if (test_hist->GetEntries() == 0) {
    std::cout << "test_hist was empty" << endl;
    my_fit->SetLineColor( kRed);
    my_fit->Draw();
    leg->AddEntry( my_fit, "Fit", "l");
  }
  else {
    std::cout << "test_hist was NOT empty" << endl;
    leg->AddEntry( test_hist, "Test Hist", "l");
    test_hist->SetLineColor( kRed);
    test_hist->Draw("same");
  }

  leg->AddEntry( data_hist, "Data", "l");
  leg->Draw("same");


  canv->SetLeftMargin(0.16);
  canv->SetBottomMargin(0.16);
  canv->SetLogy();
  canv->Print( "speedFit.png");
  canv->SetLogx();
  canv->Print( "speedFit_log.png");

  std::cout << "ENDING..." << std::endl;
}

void setPars( int type = 1) {
  my_file = new TFile( dir_en );
  test_hist = new TH1F("Test Hist", "", 1000, 0, 800);
  data_hist = (TH1F*)my_file->Get("en_bins_EJ_260;1");
  canv = new TCanvas("theCanvas", "", 500, 500);
  my_rand = new TRandom();
  leg = new TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC");  
  double (*funcptr)(double*, double*) = &fitFunc;
  my_fit = new TF1( "Fit Function", funcptr, 0, 1000, 5);
  counter = 0;

  if (!test_hist || !my_rand || !canv || !my_fit ||
      !data_hist || !my_file->IsOpen() || !leg) {
    std::cout << "Danger, Will Robinson" << std::endl;
    return;
  }

  double single[1] = {0};
  my_fit->SetParNames( "Landau mpv    ",
		       "Landau Sigma  ",
		       "Charge Param  ",
		       "Ped Gaus Sigma",
		       "Ped Gaus Mean ");
  double fit_params[5] = {0, 0, 0, 0, 0};
  fit_params[0] = 2.75;
  fit_params[1] = 0.1;
  fit_params[2] = 42;
  fit_params[3] = 0;
  fit_params[4] = 7;
  my_fit->SetParameters( fit_params);
  my_fit->FixParameter( 2, 42);

  if (type == 0) 
    fitFunc( single, fit_params);
}

void realFit() {
  setPars();
  data_hist->Fit( my_fit, "R0");
  test_hist->Reset();
  drawHists();
}

void tuneFit() {
  setPars(0);
  drawHists();
}


