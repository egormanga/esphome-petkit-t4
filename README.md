# Petkit T4 ESPHome Firmware

https://github.com/user-attachments/assets/c50be11d-95dd-4898-9601-6adc21f130d6/#Firmware.Dreams.mp4


**Custom.** Fully replaces the stock firmware while introducing some extra neat features, as well as native Home Assistant integration.

It's able to work completely offline & without HA, although it's recommended to calibrate the load cell weight sensor before use.


## Extra features added

- Top (menu) button: Maintenance mode / Reboot
- Bottom (ok) button: Maintenance done & Level the litter
- Advanced status displaying
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


## Weight Sensor Calibration

1. Place the T4 on a reasonably flat surface, make sure it's standing on all four its feet, as load cells are attached to each of them.
2. Press the `Weight Calibration Zero` configuration button until the `Weight` diagnostic sensor reads positive zero exactly.
3. Put a precisely (to 0.01 kg) known weight on top of the T4.
4. Enter its exact weight in decimal kilograms to `Weight Calibration` configuration number input **and press enter**.
5. Remove the weight and check for exact positive zero again, press the button again to reach it.
6. Place the weight again and check for its weight drift, re-input the value if needed (you might need to add/remove a trailing non-significant zero in order for the value to be actually sent and saved).
7. Rinse, repeat as desired (a perfect 0.01 kg precision is known to be reachable on T4).


## Thanks to:

- @earlynerd with his [repo](https://github.com/earlynerd/petkit-pura-max-serial-bus) for the pioneering and _enormous_ help in the reverse engineering.
- @dwyschka for the idea and the upstart (earlynerd/petkit-pura-max-serial-bus#1).
- My loved girlfriend Sharea & my lovely kitty Xayah for the lots of testing.


---

https://github.com/user-attachments/assets/ad489f28-23f9-497a-a08b-c182428b3873/#Petkit.T4.ESPHome.Firmware.mp4
