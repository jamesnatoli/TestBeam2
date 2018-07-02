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

#define EVENTS 750000
#define BINS 150

class Fitting {
 public:
  TH1F *real_hist, *test_hist;
  TF1 *my_fit;
  TRandom *my_rand;
  TFile *my_file;
  TCanvas *canv;
  TLegend *leg;
  // We need this because we pass a member function to the TF1 constructor
  Fitting *fptr;
  int counter;
  char *dir_en;

  // Constructor
  Fitting( const std::string name = "");
  virtual void theFit() { /* nothing */ };
  virtual void drawHists();
  void work();
  const std::string getName();
  virtual double fitFunc(double *x, double *p);
  TF1* getFit();
  
 private:
  std::string my_name;
};

class realFit: public Fitting {
 public:
  realFit( const std::string name);
  void theFit();
  void drawHists();
};

class splitFit: public Fitting {
 public:
  splitFit( const std::string name);
  void theFit();
  void drawHists();
};

class addFit: public Fitting {
 public:
  addFit( const std::string name);
  void theFit();
  void drawHists();
  double fitFunc( double *x, double *p);
};

class tuneFit: public Fitting {
 public:
  tuneFit( const std::string name);
  void theFit();
  void drawHists();
  // Avoids hiding
  double fitFunc( double *x, double *p);
  void fitFunc( double *p);
};
#endif
