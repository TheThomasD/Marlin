* ~~correct Z-home to be all positive and correct manual leveling to shift Z upward and rely on Z0 (not the position after homing)~~
    * set MANUAL_Z_HOME_POS to achieve this
* implement SD card handling for display
* correct/improve z-probe offset (set offset to 0, then measure/move)
    * this also requires a rewrite as subsquent presses on probe move the print head
* store values coming from display (e.g. temperatures) in a separate variable and return those to the frontend
    * update the values from time to time with values from the backend (every x seconds)
    * should make use of setting values in the FE more consistent (less "jumping" back to old values)
* correct probing? (can't remember what that means...)
* ~~switch on leveling page should be in line with M420 status, not simulated~~