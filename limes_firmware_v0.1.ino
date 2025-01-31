#include "esp_camera.h"
#include <WiFi.h>
#include "mpu6050_handler.h"  
#include "Arduino.h"

#define CAMERA_MODEL_XIAO_ESP32S3
#include "camera_pins.h"


const char* ap_ssid = "Archibald's iPhone";
const char* ap_password = "onetwothree123";

WiFiServer additionalServer(8080);

void startCameraServer();
void setupLedFlash(int pin);

const int M2IN1 = 8;
const int M2IN2 = 9;
const int M1IN1 = 1;
const int M1IN2 = 2;



void setup() {
  pinMode(M2IN1, OUTPUT);
  digitalWrite(M2IN1, LOW);
  pinMode(M2IN2, OUTPUT);
  digitalWrite(M2IN2, LOW);
  pinMode(M1IN1, OUTPUT);
  digitalWrite(M1IN1, LOW);
  pinMode(M1IN2, OUTPUT);
  digitalWrite(M1IN2, LOW);

    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    initMPU6050();

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access Point started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    startCameraServer();
    additionalServer.begin();
    Serial.println("Additional server started on port 8080");
}
void loop() {
    WiFiClient client = additionalServer.available();
    if (client) {
        Serial.println("New client connected on port 8080");
        String request = client.readStringUntil('\r');
        Serial.println(request);

        if (request.indexOf("GET /data.json") >= 0) {
            MPU6050Data data = getMPU6050Data();

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("{");
            client.printf("\"timestamp\": %lu,", millis());
            client.printf("\"temperature\": %.2f,", data.temperature);
            client.printf("\"acceleration\": {\"x\": %.2f, \"y\": %.2f, \"z\": %.2f},", 
                          data.accel_x, data.accel_y, data.accel_z);
            client.printf("\"gyroscope\": {\"x\": %.2f, \"y\": %.2f, \"z\": %.2f}", 
                          data.gyro_x, data.gyro_y, data.gyro_z);
            client.println("}");
        } 
        else if (request.indexOf("GET /forward") >= 0) {  
            digitalWrite(M2IN1, HIGH);
            digitalWrite(M2IN2, LOW);
            Serial.println("Pin 9 set to HIGH");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("FORWARD");
        } 
        else if (request.indexOf("GET /backward") >= 0) {  
            digitalWrite(M2IN1, LOW);
            digitalWrite(M2IN2, HIGH);
            Serial.println("Pin 9 set to HIGH");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("FORWARD");
        } 
        else if (request.indexOf("GET /halt") >= 0) {  

            digitalWrite(M2IN1, LOW);
            digitalWrite(M2IN2, LOW);
            Serial.println("Pin 10 set to HIGH");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("HALT");
        }
        else if (request.indexOf("GET /left") >= 0) {  

            digitalWrite(M1IN1, HIGH);
            digitalWrite(M1IN2, LOW);
            Serial.println("Pin 10 set to HIGH");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("HALT");
        }
        else if (request.indexOf("GET /right") >= 0) {  

            digitalWrite(M1IN1, LOW);
            digitalWrite(M1IN2, HIGH);
            Serial.println("Pin 10 set to HIGH");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("HALT");
        }
        else if (request.indexOf("GET /nor") >= 0) {  

            digitalWrite(M1IN1, LOW);
            digitalWrite(M1IN2, LOW);
            Serial.println("Pin 10 set to HIGH");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
            client.println("HALT");
        }
        else {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Access-Control-Allow-Origin: *");
            client.println();
        }

        client.stop();
        Serial.println("Client disconnected");
    }

    delay(10);
}
