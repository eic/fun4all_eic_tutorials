R__LOAD_LIBRARY(libeicsmear.so)

void ConvertFile(const char *fname="MILOU_5x100_TOTAL_01_20evt.out", const int nevnt = 0)
{
// suppress warnings from far away particles
  erhic::DisKinematics::BoundaryWarning=false;
// translate input to standard root file
  BuildTree(fname,".",nevnt);
// all done - quit the show
  gSystem->Exit(0);
}
