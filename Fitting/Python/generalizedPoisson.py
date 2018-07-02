from ROOT import TFile
from ROOT import *
from ROOT import TH1F
from ROOT import TCanvas
from ROOT import TF1
from ROOT import TMath
from ROOT import TRandom
from ROOT import TFormula

def testFuncGenPoisson( xcor, params):
    test_hist_gp.Reset()
    for x in range(0, 750000):
        # The average number of photoelectrons is chosen from a Landau
        avg_pe = rand.Landau(params[0], params[1])

        # This specific muon grabs from a Poisson
        # The Poisson is actually slightly different
        # Grab a random value from the function
        # this is lambda, set this to the avg_pe
        gen_poisson.SetParameter(1, avg_pe)
        # need a charge factor
        # The charge is "smeared" by a Gaussian
        smeared_charge_gp = rand.Gaus( int(gen_poisson.GetRandom()), TMath.Sqrt( int(gen_poisson.GetRandom())))
        # Pedestal
        ped = rand.Gaus( params[3], params[4])
        # Fill the histograms
        test_hist_gp.Fill(smeared_charge_gp) 

# Create the histograms
test_hist_gp = TH1F("Test_histogram with GP", "", 1000, 1, 800)

# Create the TF1 objects
my_fit_gp = TF1("Fit Object GP", testFuncGenPoisson, 0, 1000, 5)
# Create a function for the Generalized Poisson
gen_poisson = TF1("Generalized Poisson", 
                  "(TMath::Power( [0]*([0]+([1]*x)), x-1) * TMath::Exp(-([0]+([1]*x)))) /TMath::Factorial(x)", 
                  0, 10)
gen_poisson.SetParName(0, "mu")
gen_poisson.SetParameter(0, params[5])
gen_poisson.SetParName(1, "lambda: average")
gen_poisson.SetLineColor(kGreen)

# Create a TRandom object
my_rand = TRandom()

params_gp = [0, 0, 0, 0, 0, 0]
params_gp[0] = 2.75
params_gp[0] = 0.1
params_gp[0] = 42
params_gp[0] = 0
params_gp[0] = 2
params_gp[5] = 3

# Draw Things
myfile = TFile("energy_hists.root")
real_hist = TH1F( myfile.Get("en_bins_EJ_260"))
real_hist.SetLineColor(kBlack)
real_hist.SetLineWidth(1)

test_hist_gp.SetLineColor(kGreen)    
test_hist_gp.SetLineWidth(2)

leg = TLegend( 0.2, 0.2, 0.4, 0.4,"","brNDC")
leg.AddEntry( real_hist, "Data", "l")
leg.AddEntry( test_hist_gp, "Test Func w/ GP", "l")

canv = TCanvas("canv", "Test Function", 500, 500)
canv.SetLeftMargin(0.16)
canv.SetBottomMargin(0.16)
real_hist.Draw()
test_hist_gp.Draw("same")
leg.Draw()

canv.SetLogy()
canv.Print("myMIP.png")

canv.SetLogx()
canv.Print("myMIP_log.png")



