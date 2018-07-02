from ROOT import TFile
from ROOT import *
from ROOT import TH1F
from ROOT import TCanvas
from ROOT import TF1
from ROOT import TMath
from ROOT import TRandom
from ROOT import TFormula

# Create Functions
g1 = TF1("1st gaussian", "gaus", 18, 60)
g02 = TF1("2nd gaussian", "gaus", 60, 102)
g3 = TF1("3rd gaussian", "gaus", 102, 144)
g4 = TF1("4th gaussian", "gaus", 144, 188)
g05 = TF1("5th gaussian", "gaus", 188, 230)
g6 = TF1("6th gaussian", "gaus", 230, 272)
g07 = TF1("7th gaussian", "gaus", 272, 314)
g8 = TF1("8th gaussian", "gaus", 314, 356)
g9 = TF1("9th gaussian", "gaus", 356, 398)
g10 = TF1("10th gaussian", "gaus", 398, 440)
g11 = TF1("11th gaussian", "gaus", 440, 482)
g12 = TF1("12th gaussian", "gaus", 482, 524)
gped = TF1("Pedestal gaussian", "gaus", -18, 18)

# Create the Canvas
myfile = TFile("energy_hists.root")
real_hist = TH1F( myfile.Get("en_bins_EJ_260"))
real_hist.SetLineColor(kBlue)
real_hist.SetLineWidth(1)

# "R" uses the range specified in the function
# "+" adds this fitted function to the list of functions
real_hist.Fit(g1, "R+")
real_hist.Fit(g02, "R+")
real_hist.Fit(g3, "R+")
real_hist.Fit(g4, "R+")
real_hist.Fit(g05, "R+")
real_hist.Fit(g6, "R+")
real_hist.Fit(g07, "R+")
real_hist.Fit(g8, "R+")
real_hist.Fit(g9, "R+")
real_hist.Fit(g10, "R+")
real_hist.Fit(g11, "R+")
real_hist.Fit(g12, "R+")
real_hist.Fit(gped, "R+")

canv = TCanvas("canv", "Test Function", 500, 500)
canv.SetLeftMargin(0.16)
canv.SetBottomMargin(0.16)
real_hist.Draw()

canv.SetLogy()
canv.Print("mySumFit.png")

canv.SetLogx()
canv.Print("mySumFit_log.png")

print "ALL DONE"


