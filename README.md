# ESP32 Dino Game on SSD1306 OLED

A simple Chrome Dino-style game built using ESP32 and SSD1306 OLED display.

---

## Features

- Side-scrolling Dino gameplay
- Obstacle generation
- Jump physics
- Score counter
- OLED graphics rendering
- Touch/button input support
- Smooth animation on 128x64 display

---

## Hardware Used

| Component | Details |
|---|---|
| Microcontroller | ESP32 |
| Display | SSD1306 OLED 128x64 |
| Interface | I2C |
| Input | Push button / touch sensor |

---

## Wiring

| ESP32 Pin | OLED Pin |
|---|---|
| 3.3V | VCC |
| GND | GND |
| GPIO21 | SDA |
| GPIO22 | SCL |

---

## Libraries Used

- Adafruit SSD1306
- Adafruit GFX

Install from Arduino Library Manager.

---

## How to Run

1. Open `Dino_Game.ino`
2. Select ESP32 board
3. Install required libraries
4. Upload the code
5. Press button to jump

---

## Future Improvements

- Multiple obstacle types
- Sound effects
- High score saving
- Difficulty levels
