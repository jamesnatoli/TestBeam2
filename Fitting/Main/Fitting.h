// Header file for Fitting class
#ifndef FITTING_H
#define FITTING_H
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>
#include <TF1.h>
#include <TLegend.h>
#include <string>

#define NUMCHAN 8

class Fitting {
 public:
  TH1F *real_hist, *test_hist, *test_ped;
  TF1 *my_fit;
  TRandom *my_rand;
  TFile *my_file;
  TCanvas *canv;
  TLegend *leg;
  // We need this because we pass a member function to the TF1 constructor
  Fitting *fptr;
  int counter;
  char const *dir_en  = "~/TB_Analysis_17/new_git_code/TestBeam2/energy_hists.root";
  char const *dir_img = "~/TB_Analysis_17/new_git_code/TestBeam2/Fitting/Images";
  double params[5];
  double one[1] = {0};
  float num_events[NUMCHAN] = 
    { 667504,
      800133,
      744374,
      24361,
      53388,
      67250,
      44640,
      759491 };
  float num_ped_events[NUMCHAN] = 
    { 4060,
      4060,
      4060,
      1659,
      1659,
      1659,
      1659,
      4060 };
  float gaus_mean[NUMCHAN] = 
    { -0.12817,
      -0.03207,
      -0.67574,
      -0.13574,
      -0.36461,
      -0.22439,
      -0.72199,
      -0.83230 }; 
  float gaus_sigma[NUMCHAN] = 
    { 7.84473,
      8.20680,
      8.39783,
      7.36830,
      6.80696,
      6.98477,
      6.37060,
      6.79495 };
  
  // Constructor
  Fitting( const std::string name = "");
  virtual void normFit();
  virtual void pedFit();
  virtual void gpFit();
  virtual void drawHists();
  virtual double normFitFunc(double *x, double *p);
  virtual double pedFitFunc(double *x, double *p);
  virtual double gpFitFunc(double *x, double *p);
  const std::string getHist();
  void setHist( const std::string name);
  TF1* getFit();
  
 private:
  std::string my_name;
};

class tuneFit: public Fitting {
 public:
  tuneFit( const std::string name);
  void normFit();
  void pedFit();
  void gpFit();
  void drawHists( TH1F *hist);
  void drawHists() { /* avoid hiding */};
};

class realFit: public Fitting {
 public:
  realFit( const std::string name);
  void normFit();
  void pedFit();
  void gpFit();
  void drawHists( );
};

#endif
