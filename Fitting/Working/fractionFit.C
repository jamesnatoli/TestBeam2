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
  TH1F *mc1 = new TH1F( "Shape", "", 150, 0, 600);
  
  if (!my_rand || !data || !my_file || 
      !my_file->IsOpen() || !mc0) {
    std::cout << "Danger, Will Robinson" << std::endl;
    return;
  }
  
  std::cout << "Setting Parameters" << std::endl;
  double params[5] = {0};
  params[0] = 2.75;
  params[1] = 0.1;
  params[2] = 42.0;
  params[3] = 0.0;
  params[4] = 7.0;

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
    if (i < ped_events)
      mc0->Fill( my_rand->Gaus( params[3], params[4]));
    mc1->Fill( my_rand->Gaus( (my_rand->Poisson( my_rand->Landau( params[0], params[1])) * params[2]),
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
    TCanvas *canv = new TCanvas("theCanvas", "", 500, 500);
    TLegend *leg = new TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC");  

    std::cout << "Drawing Histograms" << std::endl;
    
    data->SetLineColor( kBlack);
    data->Draw();
    result->SetLineColor( kRed);
    result->Draw("same");
    // mc0->SetLineColor( kGreen);
    // mc0->Draw( "same");
    // mc1->SetLineColor( kRed);
    // mc1->Draw( "same");
    
    leg->AddEntry( data, "Data", "l");
    leg->AddEntry( result, "Result", "l");
    // leg->AddEntry( mc0, "MC0", "l");
    // leg->AddEntry( mc1, "MC1", "l");
    // leg->Draw( "same");
    
    canv->SetLeftMargin(0.16);
    canv->SetBottomMargin(0.16);
    canv->SetLogy();
    canv->SetLogx();
    canv->Print( "fractionFit_log.png");
    canv->SetLogx(0);
    canv->Print( "fractionFit.png");
    
    std::cout << "ENDING..." << std::endl;
  }
}
