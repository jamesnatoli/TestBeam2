#include "realFit.cpp"
#include "tuneFit.cpp"

void doTuneFit() {
  Fitting *test_fit = new tuneFit( "en_bins_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->normFit();
  test_fit->setHist("en_ped_EJ_260;1");
  test_fit->pedFit();
  delete test_fit;
}

void doRealFit() {
  Fitting *test_fit = new realFit( "en_bins_EJ_260;1");
  if (!test_fit) {
    std::cout << "BAD" << std::endl;
    return;
  }
  test_fit->normFit();
  test_fit->drawHists();
  delete test_fit;
}

