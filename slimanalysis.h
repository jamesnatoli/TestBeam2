// This is a header file for the slimanalysis research
// Define everything here
#if !defined SLIMANALYSIS_2017_H
#define SLIMANALYSIS_2017_H
#include <string>
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <TH1F.h>
#include <TH2F.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>
#include <TLine.h>
#include <TLatex.h>
#include <TFitResult.h>
#include <TMath.h>
#include <initializer_list>
#include <THStack.h>
#include "edges.h"
#include <TStyle.h>
#include <TColor.h>

// Define these for making a centered plot easier
#define REG 1

// Correction to wire-chamber positions
const float wc_xab = +0.642158;
const float wc_xbc = +0.209902;
const float wc_xca = -0.830777;
const float wc_yab = -8.84928 ;
const float wc_ybc = +18.0467 ;
const float wc_yca = -9.20219 ;

// (Possibly) using sigma of delta-position to clean up more
const float wc_xab_res = 1.16192 ;
const float wc_xbc_res = 1.05491 ;
const float wc_xca_res = 1.54024 ;
const float wc_yab_res = 1.2041  ;
const float wc_ybc_res = 0.892824;
const float wc_yca_res = 1.33447 ;

const int NUMCHAN = 8;

const vector<int> TIMESLICES { 5, 6, 7, 8, 9};

// Holds the indeces for the sigma tiles to make things easier
vector<unsigned int> sigmas = {0, 1, 2, 7};
vector<unsigned int> fingers = {3, 4, 5, 6};

// Fiducial region: for each tile, 4 points are needed (8 numbers)
// TB: top, bottom; LR: left, right
// x,y BL; x,y BR; x,y TL; x,y TR
float fiducialX[NUMCHAN][4] = {
#if REG
  {-33,  32, -46,  16}, // EJ-260 CHECKED (TR)
  {-26,  31, -43,  31}, // EJ-260 2P CHECKED (TL)
  {-30,  35, -37,  30}, // EJ-200 CHECKED 
  // {-10,10,-10,10},   // Scint-X #signma
  // {  7,18,-6,  5},   // Scint-X finger
  {-32, -20, -42, -30}, // SCSN-81 finger 1
  {-16,  -4, -27, -15}, // SCSN-81 finger 2
  {  0,  13, -11,   2}, // SCSN-81 finger 3
  { 17,  30,   5,  18}, // SCSN-81 finger 4
  {-38,  34, -49,  22}  // SCSN-81 #sigma CHECKED
#else
  {-28,  29, -39.5,  18}, // EJ-260 CHECKED 
  {-29,  26.5, -43,  12.5}, // EJ-260 2P CHECKED
  {-30,  30, -38,  22}, // EJ-200 CHECKED 
  {-33.5, -21.5, -41, -29}, // SCSN-81 finger 1 CHECKED
  {-16, -4, -27, -15}, // SCSN-81 finger 2 CHECKED
  {0,  12, -11, 2}, // SCSN-81 finger 3 CHECKED
  { 14,  26.5, 5, 18}, // SCSN-81 finger 4 CHECKED
  {-29.5, 29.5, -39, 20}  // SCSN-81 #sigma CHECKED
#endif
};

float fiducialY[NUMCHAN][4] = {
#if REG
  {-52, -43, 15, 29}, // EJ-260 CHECKED (TR)
  {-54, -39, 27, 42}, // EJ-260 2P CHECKED (TL)
  {-47, -39, 34, 42}, // EJ-200 CHECKED 
  // {-10,-10,10,10}, // Scint-X #signma
  // {-42,-41,20,22}, // Scint-X finger
  {-50, -48, 23, 25}, // SCSN-81 finger 1
  {-50, -48, 25, 27}, // SCSN-81 finger 2
  {-50, -48, 28, 30}, // SCSN-81 finger 3
  {-46, -44, 30, 32}, // SCSN-81 finger 4
  {-52, -46, 21, 34}  // SCSN-81 #sigma CHECKED
#else
  {-29.5, -17, 19, 32}, // EJ-260 CHECKED
  {-41, -30, 27.5, 39}, // EJ-260 2P CHECKED
  {-48, -42, 30, 36}, // EJ-200 CHECKED
  {-39 , -38, 17, 18.5}, // SCSN-81 finger 1 CHECKED
  {-41, -39, 27, 29}, // SCSN-81 finger 2 CHECKED
  {-39, -37, 29.5, 31.5}, // SCSN-81 finger 3 CHECKED
  {-27, -25, 31, 33}, // SCSN-81 finger 4 CHECKED
  {-35.5, -26, 25, 34}  // SCSN-81 #sigma CHECKED
#endif
};

float anti_fiducialX[NUMCHAN][4] = {
  { -80, 80, -80, 80},
  { -80, 80, -80, 80},
  { -80, 80, -80, 80},
  { -80, 80, -80, 80},
  { -80, 80, -80, 80},
  { -80, 80, -80, 80},
  { -80, 80, -80, 80},
  { -80, 80, -80, 80}
};

float anti_fiducialY[NUMCHAN][4] = {
  { 50, 50, 90, 90},
  { 50, 50, 90, 90},
  { 50, 50, 90, 90},
  { 50, 50, 90, 90},
  { 50, 50, 90, 90},
  { 50, 50, 90, 90},
  { 50, 50, 90, 90},
  { 50, 50, 90, 90}
};
// Identify channels we need to use
struct channel {
  int chan;
  int ieta;
  int idepth;
  string name;
};

vector<channel> channels = 
  {{6,23,5,"EJ_260"},
   {9,23,6,"EJ_260_2P"},
   {12,23,7,"EJ_200"},
   // {3,23,4,"Scint_XS"},
   // {1,23,2,"Scint_XF"},
   {4,24,4,"SCSN_81F1"},
   {7,24,5,"SCSN_81F2"},
   {10,24,6,"SCSN_81F3"},
   {13,24,7,"SCSN_81F4"},
   {5,25,4,"SCSN_81S"}
  };

const string entry[NUMCHAN] = {
  "EJ-260",
  "EJ-260 2P",
  "EJ-200",
  // "Scint-X #sigma",
  // "Scint-X finger",
  "SCSN-81 F1",
  "SCSN-81 F2",
  "SCSN-81 F3",
  "SCSN-81 F4",
  "SCSN-81 #sigma"
};

const int color[NUMCHAN] = {
  kBlack,
  kGreen,
  kBlue,
  // kBlue+1,
  // kGreen+1,
  kRed,
  kRed+1,
  kRed+2,
  kRed+3,
  kViolet
};

const int style[NUMCHAN] = {
  kDotted,
  kSolid,
  kSolid,
  // kSolid,
  // kDashed,
  kDotted,
  kSolid,
  kSolid,
  kSolid,
  kDashed
};

const int NCH = 16;
const int NTS = 10;

float rot_fiducialX[NUMCHAN][4];
float rot_fiducialY[NUMCHAN][4];
float thetas[NUMCHAN];

// distance from wire-chamber A to plastic tiles, in [mm]
const double z_ex = 7300;

// Change this is you move to lxplus
const char* slim_dir = "~/TB_Analysis_17/DATA/new_SLIM/";

bool isFiducial(int tile, float x, float y);
void fill_Rot_Array();
double calc_theta(int channel_num);
double rotate_Point(double point_X, double point_Y, int channel_num, char xy);
void doAlignmentPlots(bool debug = false, const char* dir = slim_dir);
void doMaps(bool debug = false, const char* dir = slim_dir);
void doEnergyTS(bool debug = false, const char* dir = slim_dir);

#endif
