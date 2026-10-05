# 🚨 VANGUARD – Industrial Hazard Monitoring System

**VANGUARD** is a TinyML-powered Industrial IoT safety system designed for real-time hazard detection and early warning.

## 🎯 Overview

The system continuously monitors industrial environmental conditions using multiple sensors connected to an **ESP32**. Sensor data is analyzed locally to classify hazards and provide fast alerts.

## ✨ Features

* 🌡️ Temperature & humidity monitoring using **DHT22**
* 🔥 Gas detection using **MQ2**
* 🌫️ Air-quality monitoring using **MQ135**
* 🧠 **TinyML / Edge AI** hazard classification
* 📡 Long-range communication using **LoRa**
* 💾 Offline data logging using **SPIFFS**
* 📱 Mobile status monitoring using **Bluetooth**
* 🌐 Real-time **Web Dashboard**
* ☁️ **ThingSpeak** integration for remote monitoring
* 🚨 Local LED/LCD alerts

## ⚙️ Hazard Classification

The system can identify:

* ✅ SAFE
* ⚠️ WARNING
* 🔥 GAS LEAK
* 🌫️ AIR POLLUTION
* 🔥 FIRE RISK

## 🛠️ Technologies Used

**Hardware:** ESP32, ESP8266, DHT22, MQ2, MQ135, LoRa, LCD, LEDs

**Software:** Arduino IDE, C/C++, Python, TensorFlow/TinyML

**Connectivity:** LoRa, Bluetooth, Wi-Fi

**Storage & Monitoring:** SPIFFS, ThingSpeak, Web Dashboard

## 🔄 System Flow

'''text
Sensors
   ↓
ESP32
   ↓
TinyML / Hazard Classification
   ↓
Local Alert + SPIFFS Logging
   ↓
LoRa Transmission
   ↓
ESP8266 Receiver
   ↓
Web Dashboard / Mobile Monitoring
'''

## 🎯 Applications

* Chemical industries
* Pharmaceutical industries
* Warehouses
* Battery manufacturing
* Smart factories
* Industrial safety monitoring

## 🚀 Objective

To provide a **low-cost, intelligent, scalable, and reliable industrial safety solution** capable of detecting hazards quickly and supporting monitoring even when internet connectivity is limited.

## 👥 Project

**Project Name:** VANGUARD
**Domain:** Industrial IoT | TinyML | Edge AI | Embedded Systems
**Event:** CIH26 – Coimbatore Innovation Hackathon 2026
