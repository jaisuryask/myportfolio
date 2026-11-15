// Task 5: control logic to add to the existing Firebase firmware (design snippet).
// Reads mode, bulbState and ldrThreshold written by the dashboard and drives the relay.
// Adapt the Firebase read calls to the FirebaseClient version used in the project.
// Polarity: the project relay is on GPIO 26; confirm active-HIGH or active-LOW on your module.

const bool RELAY_ACTIVE_LOW = true;       // set to false for an active-HIGH module

String modeValue   = "MANUAL";            // "MANUAL" or "AUTO"  (labAutomation/current/mode)
bool   bulbState   = false;               // labAutomation/current/bulbState
int    ldrThreshold = 2800;               // labAutomation/current/ldrThreshold
                                          // use the SAME number as the dashboard, not 400

void applyRelay(bool on) {
  digitalWrite(RELAY_PIN, (on ^ RELAY_ACTIVE_LOW) ? HIGH : LOW);
}

void updateLight(int ldrValue) {
  bool on;
  if (modeValue == "AUTO") {
    // Confirm the direction with your own readings (see calibration table in the report).
    on = (ldrValue > ldrThreshold);       // swap to '<' if your LDR circuit reads lower in the dark
  } else {
    on = bulbState;                       // MANUAL: follow the dashboard button
  }
  applyRelay(on);
  // The history record then logs: on ? "ON" : "OFF", and modeValue.
}
