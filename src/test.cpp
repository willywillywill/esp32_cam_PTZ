#include "Arduino.h"
#include "WiFi.h"
#include "esp_https_server.h"
#include "Wire.h"
#include "ESP32Servo.h"
#include "esp_camera.h"
#include "BH1750.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define I2C_SDA  GPIO_NUM_14
#define I2C_SCL  GPIO_NUM_15
#define MAX_LUX 150
BH1750 lightMeter;

esp_err_t test_handler(httpd_req_t *req) {
    while (true) {

        char data[4];
        float lux = lightMeter.readLightLevel();

        if (lux > MAX_LUX){
            data[0] = '1';
        }
        else {
            data[0] = '0';
        }

        Serial.print("Light: ");
        Serial.print(lux);
        Serial.println(" lx");
        httpd_resp_set_type(req, "application/json");
        httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
        httpd_resp_send(req, data, strlen(data));


    }
    

    return ESP_OK;
}

httpd_handle_t stream_httpd = NULL;
void startCameraServer(){
    httpd_config_t config_http = HTTPD_DEFAULT_CONFIG();
    config_http.server_port = 80;


    httpd_uri_t test_uri = {
            .uri      ="/test",
            .method   = HTTP_GET,
            .handler  = test_handler,
            .user_ctx = NULL
    };

    if (httpd_start(&stream_httpd, &config_http) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &test_uri);
    }


}


void setup() {
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    WiFi.begin("A8", "123456789");
    Wire.begin(I2C_SDA, I2C_SCL);
    lightMeter.begin();
    Serial.begin(115200);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }

    Serial.println("");
    Serial.println("*WiFi-STA-Mode*");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    startCameraServer();

}
void loop() {
    

}