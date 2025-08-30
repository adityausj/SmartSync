# Smartsync: Intelligent Master Node for Predictive Home Automation

## 📌 Project Overview
Smartsync is a **home automation project** where multiple ESP32 nodes communicate in a master–slave network and connect to the **AWS IoT Cloud**.  
The system is being extended with **AI/ML models** to predict user behavior, optimize device usage, and save energy while improving user comfort.

---

## ✅ Current Progress
- **ESP32 Network Setup**
  - Configured **3 ESP32 boards**: 1 master + 2 slaves.
  - Slave 1 → Connected to **DHT11 sensor** (temperature & humidity).  
  - Slave 2 → Connected to **Light sensor**.  
  - Master → Receives sensor data from slaves and controls communication(**Using ESP-NOW & MQTT Protocols**).

- **Cloud Integration**
  - Master node publishes sensor readings to **AWS IoT Core**.
  - Data successfully saved in **Amazon Cloud** for analysis and logging.

---

## 🚧 Next Steps (Work in Progress)
- **AI/ML Integration**
  - Train predictive models to learn from sensor data and user patterns.
  
---

## 🔧 Features
- **Data Collection**: Reads sensor data (temperature, humidity, motion).
- **Prediction Engine**: Machine learning models for behavioral and environmental analysis.
- **Smart Control**: Automated adjustments to devices.
- **Feedback Loop**: Learns from user corrections to improve over time.

---

## 🏗️ Project Objectives
- Implement an **ESP32 master node** capable of real-time processing.
- Develop ML models to analyze user/device usage patterns.
- Design a prediction system for automated control.
- Integrate a feedback mechanism for improved adaptability.
- Validate predictive accuracy in a real home-like setup.

---

## 👨‍💻 Author
**Aditya Makwana**  
Master’s in Electrical and Electronics, Cal Poly Pomona  

