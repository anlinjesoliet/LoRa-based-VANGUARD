/*
  =========================================================
  VANGUARD - ESP8266 RECEIVER
  =========================================================

  Receives from ESP32 Transmitter:

  TEMP:32.5,HUM:76.7,MQ2:1400,MQ135:1390,SENSOR:SAFE,TINYML:SAFE

  ThingSpeak:
  Field 1 -> Temperature
  Field 2 -> Humidity
  Field 3 -> MQ2
  Field 4 -> MQ135
*/

// =========================================================
// LIBRARIES
// =========================================================

#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


// =========================================================
// WIFI
// =========================================================

const char* WIFI_SSID = "SJIT-Girls_Hostel2-First-Floor";
const char* WIFI_PASSWORD = "Tech@Sjit&gh1";


// =========================================================
// THINGSPEAK
// =========================================================

const char* TS_API_KEY = "FNWNFGZ9XXIH850V";

const char* TS_SERVER =
  "http://api.thingspeak.com/update";


// ThingSpeak minimum interval
#define THINGSPEAK_INTERVAL 16000


unsigned long lastThingSpeakTime = 0;


// =========================================================
// LORA PINS - ESP8266
// =========================================================

#define LORA_SCK   D5
#define LORA_MISO  D6
#define LORA_MOSI  D7
#define LORA_SS    D8
#define LORA_RST   D0
#define LORA_DIO0  D1


// =========================================================
// RECEIVED SENSOR VARIABLES
// =========================================================

float temperature = 0.0;

float humidity = 0.0;

int mq2 = 0;

int mq135 = 0;


// =========================================================
// PREDICTION VARIABLES
// =========================================================

String sensorPrediction = "SAFE";

String tinyMLPrediction = "SAFE";


// =========================================================
// DATA FLAG
// =========================================================

bool dataReceived = false;


// =========================================================
// WIFI CONNECTION
// =========================================================

void connectWiFi()
{

  if (WiFi.status() == WL_CONNECTED)
  {

    return;

  }


  Serial.println();

  Serial.println(
    "Connecting to WiFi..."
  );


  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );


  unsigned long startTime =
    millis();


  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - startTime < 15000
  )
  {

    delay(500);

    Serial.print(".");

  }


  Serial.println();


  if (WiFi.status() == WL_CONNECTED)
  {

    Serial.println(
      "WiFi Connected!"
    );


    Serial.print(
      "IP Address: "
    );

    Serial.println(
      WiFi.localIP()
    );

  }

  else
  {

    Serial.println(
      "WiFi Connection Failed!"
    );

    Serial.println(
      "LoRa will continue working."
    );

  }

}


// =========================================================
// THINGSPEAK UPDATE
// =========================================================

void sendToThingSpeak()
{

  // -------------------------------------------------------
  // CHECK WIFI
  // -------------------------------------------------------

  if (
    WiFi.status() != WL_CONNECTED
  )
  {

    Serial.println(
      "WiFi not connected!"
    );


    connectWiFi();


    if (
      WiFi.status() != WL_CONNECTED
    )
    {

      Serial.println(
        "ThingSpeak upload skipped."
      );

      return;

    }

  }


  // -------------------------------------------------------
  // CREATE CLIENT
  // -------------------------------------------------------

  WiFiClient client;

  HTTPClient http;


  // -------------------------------------------------------
  // CREATE URL
  // -------------------------------------------------------

  String url =
    String(TS_SERVER);


  url +=
    "?api_key=";

  url +=
    TS_API_KEY;


  // Field 1
  url +=
    "&field1=";

  url +=
    String(
      temperature,
      1
    );


  // Field 2
  url +=
    "&field2=";

  url +=
    String(
      humidity,
      1
    );


  // Field 3
  url +=
    "&field3=";

  url +=
    String(
      mq2
    );


  // Field 4
  url +=
    "&field4=";

  url +=
    String(
      mq135
    );


  // -------------------------------------------------------
  // PRINT UPLOAD DATA
  // -------------------------------------------------------

  Serial.println();

  Serial.println(
    "Uploading to ThingSpeak..."
  );


  Serial.print(
    "Temperature: "
  );

  Serial.println(
    temperature
  );


  Serial.print(
    "Humidity: "
  );

  Serial.println(
    humidity
  );


  Serial.print(
    "MQ2: "
  );

  Serial.println(
    mq2
  );


  Serial.print(
    "MQ135: "
  );

  Serial.println(
    mq135
  );


  // -------------------------------------------------------
  // HTTP REQUEST
  // -------------------------------------------------------

  http.begin(
    client,
    url
  );


  int httpCode =
    http.GET();


  // -------------------------------------------------------
  // CHECK RESULT
  // -------------------------------------------------------

  if (
    httpCode > 0
  )
  {

    Serial.print(
      "HTTP Response Code: "
    );

    Serial.println(
      httpCode
    );


    String response =
      http.getString();


    Serial.print(
      "ThingSpeak Response: "
    );

    Serial.println(
      response
    );


    if (
      httpCode == 200
    )
    {

      Serial.println(
        "ThingSpeak Update SUCCESS!"
      );

    }

  }

  else
  {

    Serial.print(
      "ThingSpeak Update FAILED: "
    );

    Serial.println(
      http.errorToString(
        httpCode
      )
    );

  }


  http.end();

}


// =========================================================
// SETUP
// =========================================================

void setup()
{

  // -------------------------------------------------------
  // SERIAL
  // -------------------------------------------------------

  Serial.begin(
    115200
  );


  delay(
    1000
  );


  Serial.println();

  Serial.println(
    "================================"
  );

  Serial.println(
    " VANGUARD ESP8266 RECEIVER"
  );

  Serial.println(
    "================================"
  );


  // -------------------------------------------------------
  // LORA SPI
  // -------------------------------------------------------

  Serial.println();

  Serial.println(
    "Initializing LoRa..."
  );


  SPI.begin();


  LoRa.setPins(
    LORA_SS,
    LORA_RST,
    LORA_DIO0
  );


  // IMPORTANT:
  // Same frequency as ESP32 transmitter

  if (
    !LoRa.begin(
      433E6
    )
  )
  {

    Serial.println();

    Serial.println(
      "ERROR: LoRa Initialization Failed!"
    );


    while (
      true
    )
    {

      delay(
        1000
      );

    }

  }


  Serial.println(
    "LoRa Initialized Successfully!"
  );


  Serial.println(
    "Frequency: 433 MHz"
  );


  // -------------------------------------------------------
  // WIFI
  // -------------------------------------------------------

  connectWiFi();


  // -------------------------------------------------------
  // READY
  // -------------------------------------------------------

  Serial.println();

  Serial.println(
    "================================"
  );

  Serial.println(
    "RECEIVER READY"
  );

  Serial.println(
    "Waiting for LoRa data..."
  );

  Serial.println(
    "================================"
  );

}


// =========================================================
// LOOP
// =========================================================

void loop()
{

  // =======================================================
  // CHECK WIFI
  // =======================================================

  if (
    WiFi.status() != WL_CONNECTED
  )
  {

    static unsigned long
      lastWiFiAttempt = 0;


    if (
      millis() -
      lastWiFiAttempt >
      10000
    )
    {

      lastWiFiAttempt =
        millis();


      connectWiFi();

    }

  }


  // =======================================================
  // CHECK LORA PACKET
  // =======================================================

  int packetSize =
    LoRa.parsePacket();


  if (
    packetSize > 0
  )
  {

    // -----------------------------------------------------
    // READ PACKET
    // -----------------------------------------------------

    String receivedPacket =
      "";


    while (
      LoRa.available()
    )
    {

      receivedPacket +=
        (char)LoRa.read();

    }


    int rssi =
      LoRa.packetRssi();


    // -----------------------------------------------------
    // DISPLAY RAW PACKET
    // -----------------------------------------------------

    Serial.println();

    Serial.println(
      "================================"
    );

    Serial.println(
      "       LORA PACKET RECEIVED"
    );

    Serial.println(
      "================================"
    );


    Serial.print(
      "Raw Packet: "
    );

    Serial.println(
      receivedPacket
    );


    Serial.print(
      "RSSI: "
    );

    Serial.println(
      rssi
    );


    // -----------------------------------------------------
    // PARSE TEMP
    // -----------------------------------------------------

    int tempStart =
      receivedPacket.indexOf(
        "TEMP:"
      );


    int tempEnd =
      receivedPacket.indexOf(
        ",HUM:"
      );


    // -----------------------------------------------------
    // PARSE HUMIDITY
    // -----------------------------------------------------

    int humStart =
      receivedPacket.indexOf(
        "HUM:"
      );


    int humEnd =
      receivedPacket.indexOf(
        ",MQ2:"
      );


    // -----------------------------------------------------
    // PARSE MQ2
    // -----------------------------------------------------

    int mq2Start =
      receivedPacket.indexOf(
        "MQ2:"
      );


    int mq2End =
      receivedPacket.indexOf(
        ",MQ135:"
      );


    // -----------------------------------------------------
    // PARSE MQ135
    // -----------------------------------------------------

    int mq135Start =
      receivedPacket.indexOf(
        "MQ135:"
      );


    int mq135End =
      receivedPacket.indexOf(
        ",SENSOR:"
      );


    // -----------------------------------------------------
    // PARSE SENSOR PREDICTION
    // -----------------------------------------------------

    int sensorStart =
      receivedPacket.indexOf(
        "SENSOR:"
      );


    int sensorEnd =
      receivedPacket.indexOf(
        ",TINYML:"
      );


    // -----------------------------------------------------
    // PARSE TINYML
    // -----------------------------------------------------

    int tinyStart =
      receivedPacket.indexOf(
        "TINYML:"
      );


    // =====================================================
    // CHECK PACKET FORMAT
    // =====================================================

    if (
      tempStart >= 0 &&
      tempEnd > tempStart &&

      humStart >= 0 &&
      humEnd > humStart &&

      mq2Start >= 0 &&
      mq2End > mq2Start &&

      mq135Start >= 0 &&
      mq135End > mq135Start &&

      sensorStart >= 0 &&
      sensorEnd > sensorStart &&

      tinyStart >= 0
    )
    {

      // ===================================================
      // EXTRACT TEMPERATURE
      // ===================================================

      temperature =
        receivedPacket.substring(
          tempStart + 5,
          tempEnd
        ).toFloat();


      // ===================================================
      // EXTRACT HUMIDITY
      // ===================================================

      humidity =
        receivedPacket.substring(
          humStart + 4,
          humEnd
        ).toFloat();


      // ===================================================
      // EXTRACT MQ2
      // ===================================================

      mq2 =
        receivedPacket.substring(
          mq2Start + 4,
          mq2End
        ).toInt();


      // ===================================================
      // EXTRACT MQ135
      // ===================================================

      mq135 =
        receivedPacket.substring(
          mq135Start + 6,
          mq135End
        ).toInt();


      // ===================================================
      // EXTRACT SENSOR PREDICTION
      // ===================================================

      sensorPrediction =
        receivedPacket.substring(
          sensorStart + 7,
          sensorEnd
        );


      // ===================================================
      // EXTRACT TINYML PREDICTION
      // ===================================================

      tinyMLPrediction =
        receivedPacket.substring(
          tinyStart + 7
        );


      // ===================================================
      // DATA RECEIVED
      // ===================================================

      dataReceived =
        true;


      // ===================================================
      // DISPLAY RECEIVED DATA
      // ===================================================

      Serial.println();

      Serial.println(
        "-------- RECEIVED DATA --------"
      );


      Serial.print(
        "Temperature : "
      );

      Serial.print(
        temperature,
        1
      );

      Serial.println(
        " C"
      );


      Serial.print(
        "Humidity    : "
      );

      Serial.print(
        humidity,
        1
      );

      Serial.println(
        " %"
      );


      Serial.print(
        "MQ2         : "
      );

      Serial.println(
        mq2
      );


      Serial.print(
        "MQ135       : "
      );

      Serial.println(
        mq135
      );


      Serial.print(
        "Sensor      : "
      );

      Serial.println(
        sensorPrediction
      );


      Serial.print(
        "TinyML      : "
      );

      Serial.println(
        tinyMLPrediction
      );


      Serial.print(
        "RSSI        : "
      );

      Serial.println(
        rssi
      );


      Serial.println(
        "-------------------------------"
      );


      Serial.println(
        "LoRa Data Received Successfully!"
      );


    }

    else
    {

      Serial.println();

      Serial.println(
        "ERROR: Invalid Packet Format!"
      );

      Serial.println(
        "Expected:"
      );

      Serial.println(
        "TEMP:32.5,HUM:76.7,MQ2:1400,MQ135:1390,SENSOR:SAFE,TINYML:SAFE"
      );

    }

  }


  // =======================================================
  // THINGSPEAK UPDATE
  // =======================================================

  if (
    dataReceived &&
    millis() -
    lastThingSpeakTime >=
    THINGSPEAK_INTERVAL
  )
  {

    lastThingSpeakTime =
      millis();


    Serial.println();

    Serial.println(
      "Sending latest data to ThingSpeak..."
    );


    sendToThingSpeak();


    // Data has been uploaded
    dataReceived =
      false;

  }


  // =======================================================
  // SMALL DELAY
  // =======================================================

  delay(
    10
  );

}