#include "addFit.cpp"
#include "realFit.cpp"
#include "tuneFit.cpp"
#include "splitFit.cpp"
#include "pedFit.cpp"

void doPedFit() {
  Fitting *test_fit = new pedFit( "en_ped_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theRealFit();
  std::cout << "Mean  = " << (test_fit->my_fit->GetParameter(0)) << std::endl;
  std::cout << "Sigma  = " << (test_fit->my_fit->GetParameter(1)) << std::endl;
  test_fit->drawHists();
  
  test_fit->theTuneFit();
  test_fit->drawHists();
  // delete test_fit;
}

void doAddFit() {
  Fitting *test_fit = new addFit( "en_bins_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  delete test_fit;
}

void doTuneFit() {
  Fitting *test_fit = new tuneFit( "en_bins_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  delete test_fit;
}

void doRealFit() {
  Fitting *test_fit = new realFit( "en_bins_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  //delete test_fit;
}

void doSplitFit() {
  Fitting *test_fit = new splitFit( "en_bins_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->theFit();
  test_fit->drawHists();
  delete test_fit;
}

