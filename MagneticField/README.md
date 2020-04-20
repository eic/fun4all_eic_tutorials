This tutorial shows the different available fieldmaps (Babar, Beast and Cleo), also compared to constant solenoidal fields. The magnet bore is empty - you can put in your own detector and see how this would work. Edit Fun4All_G4_Magnet.C to select the fieldmap you are interested in. By default the macro sets the number of processed events to -1, so it will not start the event loop. By default it runs 4 charged geantinos with py=0.5, 1, 2 and 3 GeV. It sets pz to 1GeV/c to prevent loopers (the low momentum geantinos otherwise curl up in the field and won't stop)

Usage to see one event:

```
root.exe
.x Fun4All_G4_Magnet.C
.L DisplayOn.C
PHG4Reco *g4 = DisplayOn();
Fun4AllServer *se = Fun4AllServer::instance();
se->run(1);
```
