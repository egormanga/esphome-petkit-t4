# Petkit T4 ESPHome Firmware

https://github.com/user-attachments/assets/c50be11d-95dd-4898-9601-6adc21f130d6/#Firmware.Dreams.mp4


**Custom.** Fully replaces the stock firmware while introducing some extra neat features, as well as native Home Assistant integration.

It's able to work completely offline & without HA, although it's recommended to calibrate the load cell weight sensor before use.


## Extra features added

- Top (menu) button: Maintenance mode / Reboot
- Bottom (ok) button: Maintenance done & Level the litter
- Advanced status displaying
- Quieter functioning
- Collected waste amount tracking
- Beep after reaching set amount of litter in maintenance mode
- QR code with a Wi-Fi AP connection shortcut
- Optional [Improv-BLE](https://improv-wifi.com) support
- Various internal sensors exposed to HA
- Additional safety checks compared to the stock fw
- Manual motor controls


## Yet to implement

* [x] Auto cleaning start (a.k.a. non-kitten mode)
* [ ] Pet weight saving
* [ ] Multiple pets support
* [ ] Extra unknown packets
* [ ] Better & more failsafe motor control
* [ ] Parametrized MCU init
* [ ] Automated calibration
* [ ] Dumping the stock calibration data from factory binaries (per-chip)
* [x] Weight as additional safety check
* [ ] Tune the timeouts and speeds


## Weight Sensor Calibration

1. Place the T4 on a reasonably flat surface, make sure it's standing on all four its feet, as load cells are attached to each of them.
2. Press the `Weight Calibration Zero` configuration button until the `Weight` diagnostic sensor reads positive zero exactly.
3. Put a precisely (to 0.01 kg) known weight on top of the T4.
4. Enter its exact weight in decimal kilograms to `Weight Calibration` configuration number input **and press enter**.
5. Remove the weight and check for exact positive zero again, press the button again to reach it.
6. Place the weight again and check for its weight drift, re-input the value if needed (you might need to add/remove a trailing non-significant zero in order for the value to be actually sent and saved).
7. Rinse, repeat as desired (a perfect 0.01 kg precision is known to be reachable on T4).


## Automatic Calibration (No Reference Weight)

Absolute scale calibration (getting the exact kg/grams factor) fundamentally requires *some* known reference mass.
However, for most real-world use of this firmware (pet presence detection, safety checks, and relative weight changes), the important part is keeping the scale **zero/tare** correct.

This repo now includes an **auto-zero** routine that does not require placing any reference weight:

- It waits for the unit to be idle and stable (no clean/maintenance running, no pet/approach, cover removed).
- After ~10 seconds of stable raw readings, it automatically sets the `weight_zero` offset.

If you still need maximum absolute accuracy (e.g. precise pet weight to 0.01 kg), you can optionally perform the manual reference-weight step to refine `weight_scale`.


## Thanks to:

- @earlynerd with his [repo](https://github.com/earlynerd/petkit-pura-max-serial-bus) for the pioneering and _enormous_ help in the reverse engineering.
- @dwyschka for the idea and the upstart (earlynerd/petkit-pura-max-serial-bus#1).
- My loved girlfriend Sharea & my lovely kitty Xayah for the lots of testing.


---

https://github.com/user-attachments/assets/ad489f28-23f9-497a-a08b-c182428b3873/#Petkit.T4.ESPHome.Firmware.mp4
