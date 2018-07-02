from ROOT import TFile
from ROOT import *
from ROOT import TH1F
from ROOT import TCanvas
from ROOT import TF1
from ROOT import TMath
from ROOT import TRandom
from ROOT import TFormula

def testFunc(xcor, params):
    test_hist.Reset()
    for x in range(0, 750000):
        # The average number of photoelectrons is chosen from a Landau
        # This specific muon grabs from a Poisson
        # need a charge factor
        # The charge is "smeared" by a Gaussian
        smeared_charge = my_rand.Gaus( ( my_rand.Poisson(my_rand.Landau(params[0], params[1])) * params[2]), 
                                       TMath.Sqrt(my_rand.Poisson(my_rand.Landau(params[0], params[1])) * params[2]))
        # Pedestal 
        ped = my_rand.Gaus( params[3], params[4])
        # Fill the histograms
        test_hist.Fill(ped + smeared_charge)
    return test_hist.GetBinContent( test_hist.FindBin( xcor[0]))

# Create the histogram
test_hist = TH1F("Test_histogram", "", 1000, 0, 800)

# Create the TF1 objects
my_fit = TF1("Fit Object", testFunc, 0, 1000, 5)

# Create a TRandom object
my_rand = TRandom()

params = [0, 0, 0, 0, 0]
# The most probably value of the Landau, or is it?
params[0] = 2.75
# The sigma of the Landau
params[1] = 0.1
# The charge parameter
params[2] = 42
# Pedestal Gaussian Mean
params[3] = 0
# Pedestal Gaussian Width
params[4] = 2

# Draw Things
myfile = TFile("energy_hists.root")
real_hist = TH1F( myfile.Get("en_bins_EJ_260"))
real_hist.SetLineColor(kBlack)
real_hist.SetLineWidth(1)

# This makes one test histogram
testFunc( [0], params)

# This Fits
# real_hist.Fit(my_fit, "R0")

test_hist.SetLineColor(kRed)    
# test_hist.SetLineWidth(2)

leg = TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC")
leg.AddEntry( real_hist, "Data", "l")
leg.AddEntry( test_hist, "Test Func", "l")
leg.AddEntry( my_fit, "MINUIT Fit", "l")

canv = TCanvas("canv", "Test Function", 500, 500)
canv.SetLeftMargin(0.16)
canv.SetBottomMargin(0.16)
real_hist.Draw()
test_hist.Draw("same")
# my_fit.Draw("same")
leg.Draw()

canv.SetLogy()
canv.Print("myMIP.png")

canv.SetLogx()
canv.Print("myMIP_log.png")
