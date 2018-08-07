#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>
#include <TF1.h>
#include <TLegend.h>
#include <TVirtualFitter.h>
#include <TFitResult.h>

TFile *my_file;
TH1F *test_hist, *data_hist, *real_hist;
TCanvas *canv;
TRandom *my_rand;
TLegend *leg;
TF1 *my_fit;
char *my_name;
int counter;

double mathFunc(double *xcor, double *params) {
  if (counter%1000 == 0)
    std::cout << counter << " Function Calls" << std::endl;
  counter++;
  float xx = xcor[0];
  double res = TMath::Gaus( xcor[0], params[0], params[1]);
  return res;
}

double fitFunc(double *xcor, double *params) {
  test_hist->Reset();
  if (counter%1000 == 0)
    std::cout << counter << " Function Calls" << std::endl;
  for (unsigned int i = 0; i < 750000; i++) {
    //test_hist->Fill( my_rand->Gaus( 100, 15));
    test_hist->Fill( my_rand->Gaus( params[0], params[1]));
  }
  counter++;
  return test_hist->GetBinContent( test_hist->GetXaxis()->FindBin( xcor[0]));
}

void drawHists() {
  // ****** DRAW EVERYTHING ******

  data_hist->SetLineColor(kBlack);
  data_hist->Draw();

  // test_hist->SetLineColor( kRed);
  // test_hist->Draw( "same");
  // leg->AddEntry( test_hist, "Test", "l");
  
  my_fit->SetLineColor( kRed);
  //my_fit->Draw();
  my_fit->Draw("same");
  leg->AddEntry( my_fit, "Fit", "l");

  leg->AddEntry( data_hist, "Data", "l");
  // leg->Draw("same");

  canv->SetLeftMargin( 0.16);
  canv->SetBottomMargin( 0.16);
  canv->SetLogy();
  canv->SetLogx();
  canv->Print( "exampleFit_log.png");
  canv->SetLogx(0);
  canv->Print( "exampleFit.png");
  std::cout << "ENDING..." << std::endl;
}

void setPars( ) {
  test_hist = new TH1F("Test Hist", "", 300, 0, 300);
  data_hist = new TH1F("Data Hist", "", 300, 0, 300);
  
  canv = new TCanvas("theCanvas", "", 500, 500);
  my_rand = new TRandom();
  leg = new TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC");  
  double (*funcptr)(double*, double*) = &fitFunc;
  double (*mathptr)(double*, double*) = &mathFunc;
  //my_fit = new TF1( "Inline Func", "gaus", 0, 500);
  //my_fit = new TF1( "Inline Func", "TMath::Gaus( x, [0], [1])", 0, 500);
  my_fit = new TF1( "Math Func", &mathFunc, 0, 500, 2);
  //my_fit = new TF1( "Fit Function", funcptr, 50, 150, 2);
  counter = 0;

  if (!test_hist || !my_rand || !canv || !my_fit ||
      !data_hist || !leg) {
    std::cout << "Danger, Will Robinson" << std::endl;
    return;
  }

  my_fit->SetParNames( "Gaus Mean    ",
		       "Gaus Sigma  ");
  double fit_params[2] = {0, 0};
  fit_params[0] = 102;
  fit_params[1] = 13;

  my_fit->SetParameters( fit_params);
  // my_fit->SetParLimits( 0, 95, 105);
  // my_fit->SetParLimits( 1, 10, 20);
}

void makeData() {
  std::cout << "Making Data" << std::endl;
  for (unsigned int i = 0; i < 750000; i++){
    data_hist->Fill( my_rand->Gaus( 100, 15));
  }
  //data_hist->Sumw2();
  std::cout << "Made Data" << std::endl;
}

void exampleFit() {
  setPars();
  makeData();
  std::cout << "here" << std::endl;
  // std::cout << "Precision = " << TVirtualFitter::GetPrecision() << std::endl;
  // TVirtualFitter::SetPrecision(1.0);
  // std::cout << "Precision = " << TVirtualFitter::GetPrecision() << std::endl;
  // double uno[1] = {0};
  // double p[2] = { 100, 15};
  // fitFunc( uno, p);
  std::cout << "Entries = " << data_hist->GetEntries() << std::endl;
  data_hist->Fit( my_fit, "0");  
  drawHists();
}

