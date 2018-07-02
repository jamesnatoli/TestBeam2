from ROOT import TFile
from ROOT import *
from ROOT import TH1F
from ROOT import TCanvas
from ROOT import TF1
from ROOT import TMath
from ROOT import TRandom
import sys

def fitFunc(x, params):
    test_hist.Reset()
    for x in range(0, 750000):
        test_hist.Fill( TMath.Gaus( x[0], (TMath.Poisson( x[0], TMath.Landau(x[0], params[0], params[1])) * params[2]), 
                                    TMath.Sqrt( TMath.Poisson( x[0], TMath.Landau(x[0], params[0], params[1])) * params[2])) + 
                        rand.Gaus( params[3], params[4]))
    return test_hist.GetBinContent( test_hist.FindBin( xcor[0]))
    
myCan = TCanvas("can")
myFile = TFile("energy_hists.root")
dataHist = myFile.Get("en_bins_EJ_260;1")
myRand = TRandom()
testHist = TH1F("Tuning Histogram", "", 1000, 0, 1000)
myCan.SetLogy()
myCan.SetLogx()

theFit=TF1( "thefit", fitFunc, 0, 5000, 5)

theFit.SetParLimits(0,   1, 5000);
theFit.SetParLimits(1,    0,     5);
theFit.SetParLimits(2,   0.4, 1000);
theFit.SetParLimits(3,   10,  1000);
theFit.SetParLimits(4,  3,    1000);

theFit.SetParNames( "Landau mpv    ",
                    "Landau Sigma  ",
                    "Charge Param  ",
                    "Ped Gaus Sigma",
                    "Ped Gaus Mean ")
params = [3.6, 4.6, 50.6, 60.6, 7.6]
theFit.SetParameters( params)

dataHist.SetLineColor( kBlack)
dataHist.Draw("E1")
leg = TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC")
leg.AddEntry( realHist, "Data", "l")

if sys.argv[0] == "tune":
    testHist.SetLineColor(kRed)
    leg.AddEntry( testHist, "Fit Histogram", "l")
    testHist.Draw("SAME")
else:
    dataHist.Fit(theFit, "R0")
    leg.AddEntry( theFit, "Fit", "l")
    theFit.SetLineColor(kRed)
    theFit.Draw("SAME")

myCan.Print("t.png")
