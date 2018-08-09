#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>
#include <TF1.h>
#include <TLegend.h>
#include <TFractionFitter.h>
#include <TVirtualFitter.h>

char const *dir_en = "~/TB_Analysis_17/new_git_code/TestBeam2/energy_hists.root";

void fractionFit() {
  TFile *my_file = new TFile( dir_en );
  TRandom *my_rand = new TRandom();
  TH1F *data = (TH1F*)my_file->Get("en_bins_EJ_260;1");
  TH1F *mc0 = new TH1F( "Pedestal", "", 150, 0, 600);
  TH1F *mc1 = new TH1F( "Peaks", "", 150, 0, 600);
  TH1F *mcFull = new TH1F( "Full", "", 150, 0, 600);
  
  if (!my_rand || !data || !my_file || 
      !my_file->IsOpen() || !mc0 || !mcFull) {
    std::cout << "Danger, Will Robinson" << std::endl;
    return;
  }
  
  std::cout << "Setting Parameters" << std::endl;
  double params[5] = {0};
  params[0] = 2.7500;
  params[1] = 0.3000;
  params[2] = 42.000;
  params[3] = 0.0000;
  params[4] = 6.9000;

  std::cout << "Landau MPV: " << params[0] << "\n"
	    << "Landau Sig: " << params[1] << "\n"
	    << "Charge Par: " << params[2] << "\n"
	    << "Ped Mean:   " << params[3] << "\n"
	    << "Ped Sigma:  " << params[4] << std::endl;

  // Make the Monte Carlo
  unsigned int ped_events = 65000;
  std::cout << "mc0 events: " << data->GetEntries() << "\n"
	    << "mc1 events: " << ped_events << std::endl;
  for (unsigned int i = 0; i < data->GetEntries(); i++) {
    if (i < ped_events) {
      mc0->Fill( my_rand->Gaus( params[3], params[4]));
      mcFull->Fill( my_rand->Gaus( params[3], params[4]));
    }
    mc1->Fill( my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]),
			      TMath::Sqrt(my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2])));
    mcFull->Fill( my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]),
				 TMath::Sqrt(my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2])));
  }
  
  // MC histograms are put in this array
  std::cout << "Making TObjArray" << std::endl;
  TObjArray *mc = new TObjArray(2);
  mc->Add( mc0);
  mc->Add( mc1);
  
  std::cout << "Making TFraction Fitter" << std::endl;
  TFractionFitter* fit = new TFractionFitter( data, mc); // initialise
  fit->Constrain( 1, 0.0, 1.0);               // constrain fraction 1 to be between 0 and 1
  fit->UnConstrain( 1);
  fit->SetRangeX( 1, 15);                    // use only the first 15 bins in the fit
  fit->ReleaseRangeX();
  int status = fit->Fit();               // perform the fit
  std::cout << "fit status: " << status << std::endl;
  
  if (status == 0) {                       // check on fit status
    TH1F* result = (TH1F*) fit->GetPlot();
    TH1F* mc0_adj = (TH1F*) fit->GetMCPrediction(0);
    TH1F* mc1_adj = (TH1F*) fit->GetMCPrediction(1);
    TLegend *leg = new TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC");  

    if (!mc0_adj || !mc1_adj || !leg || !result) {
      std::cout << "Danger, Will Robinson" << std::endl;
      return;
    }
    std::cout << "Drawing Histograms" << std::endl;

    // **** Main Fit ****
    TCanvas *canv1 = new TCanvas( "theCanvas1", "", 500, 500);
    data->SetLineColor( kBlack);
    data->Draw();
    result->SetLineColor( kRed);
    result->Draw("same, c");
    
    leg->AddEntry( data, "Data", "l");
    leg->AddEntry( result, "Result", "l");
    leg->Draw( "same");

    canv1->SetLeftMargin(0.16);
    canv1->SetBottomMargin(0.16);
    canv1->SetLogy();
    canv1->SetLogx();
    canv1->Print( "fractionFit_log.png");
    canv1->SetLogx(0);
    canv1->Print( "fractionFit.png");

    // **** Pedestal ****
    TCanvas *canv2 = new TCanvas( "theCanvas2", "", 500, 500);
    mc0_adj->SetLineColor( kCyan);
    mc0_adj->Draw();
    mc0->SetLineColor( kGreen);
    mc0->Draw( "same");

    leg->Clear();
    leg->AddEntry( mc0_adj, "Adjusted MC", "l");
    leg->AddEntry( mc0, "Original MC", "l");
    leg->Draw( "same");

    canv2->SetLeftMargin(0.16);
    canv2->SetBottomMargin(0.16);
    canv2->SetLogy();
    canv2->Print( "pedestal.png");
    canv2->SetLogx();
    canv2->Print( "pedestal_log.png");

    // **** Main Peaks ****
    TCanvas *canv3 = new TCanvas( "theCanvas3", "", 500, 500);
    mc1_adj->SetLineColor( kOrange);
    mc1_adj->Draw();
    mc1->SetLineColor( kBlue);
    mc1->Draw( "same");

    leg->Clear();
    leg->AddEntry( mc1_adj, "Adjusted MC", "l");
    leg->AddEntry( mc1, "Original MC", "l");
    leg->Draw( "same");

    canv3->SetLeftMargin(0.16);
    canv3->SetBottomMargin(0.16);
    canv3->SetLogy();
    canv3->SetLogx();
    canv3->Print( "gaussianPeaks_log.png");
    canv3->SetLogx(0);
    canv3->Print( "gaussianPeaks.png");

    // **** Full Function ****
    TCanvas *canv4 = new TCanvas( "theCanvas4", "", 500, 500);
    data->SetLineColor( kBlack);
    data->Draw();
    mcFull->SetLineColor( kPink);
    mcFull->Draw("same");

    leg->Clear();
    leg->AddEntry( data, "Data", "l");
    leg->AddEntry( mcFull, "Full Function", "l");
    leg->Draw( "same");

    canv4->SetLeftMargin(0.16);
    canv4->SetBottomMargin(0.16);
    canv4->SetLogy();
    canv4->SetLogx();
    canv4->Print( "fullFunc_log.png");
    canv4->SetLogx(0);
    canv4->Print( "fullFunc.png");
    
    std::cout << "ENDING..." << std::endl;
  }
}
