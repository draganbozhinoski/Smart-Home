package com.example.smarthomespringmqtt.mqtt_config;

import com.example.smarthomespringmqtt.domain.LightReading;
import com.example.smarthomespringmqtt.domain.PmReading;
import com.example.smarthomespringmqtt.domain.WindowReading;
import com.example.smarthomespringmqtt.service.UniversalService;
import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;

import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.Objects;
import java.util.UUID;

@Component
public class MqttSubscriberImpl extends MqttConfig implements MqttCallback {

    private static final String fota_fetch_record = "fota_fetch_record";
    private String brokerUrl = "0e2b872d30104e009127c52455bcd6be.s2.eu.hivemq.cloud";
    final private String colon = ":";
    final private String clientId = UUID.randomUUID().toString();

    private MqttClient mqttClient = null;
    private MqttConnectOptions connectionOptions = null;
    private MemoryPersistence persistence = null;

    private final UniversalService universalService;
    private static final Logger logger = LoggerFactory.getLogger(MqttSubscriberImpl.class);

    public MqttSubscriberImpl(UniversalService universalService) {
        this.universalService = universalService;
        logger.info("I am MqttSub impl");
        this.config();
    }

    @Override
    public void connectionLost(Throwable cause) {
        logger.info("Connection Lost" + cause);
        this.config();
    }

    @Override
    protected void config(String broker, Integer port, Boolean ssl, Boolean withUserNamePass) {
        logger.info("Inside Parameter Config");
        this.brokerUrl = this.TCP + this.broker + colon + port;
        this.persistence = new MemoryPersistence();
        this.connectionOptions = new MqttConnectOptions();
        try {
            this.mqttClient = new MqttClient(brokerUrl, clientId, persistence);
            this.connectionOptions.setCleanSession(true);
            this.connectionOptions.setPassword(this.password.toCharArray());
            this.connectionOptions.setUserName(this.userName);
            this.mqttClient.connect(this.connectionOptions);
            this.mqttClient.setCallback(this);
        } catch (MqttException me) {
            throw new RuntimeException("Not Connected");
        }
    }

    @Override
    protected void config() {
        logger.info("Inside Config with parameter");
        this.brokerUrl = this.SSL + this.broker + colon + this.port;
        this.persistence = new MemoryPersistence();
        this.connectionOptions = new MqttConnectOptions();
        System.out.println(brokerUrl + clientId + persistence);
        try {
            this.mqttClient = new MqttClient(brokerUrl, clientId, persistence);
            System.out.println("MQTT Client created." + mqttClient.toString());
            System.out.println(brokerUrl);
            this.connectionOptions.setCleanSession(true);
            this.connectionOptions.setPassword(this.password.toCharArray());
            this.connectionOptions.setUserName(this.userName);
            this.mqttClient.connect(this.connectionOptions);
            this.mqttClient.setCallback(this);
        } catch (MqttException me) {
            throw new RuntimeException("Not Connected");
        }
    }

    @Override
    public void subscribeMessage(String topic) {
        try {
            this.mqttClient.subscribe(topic, this.qos);
        } catch (MqttException me) {
            System.out.println("Not able to Read Topic  "+ topic);
            // me.printStackTrace();
        }
    }

    @Override
    public void disconnect() {
        try {
            this.mqttClient.disconnect();
        } catch (MqttException me) {
            logger.error("ERROR", me);
        }
    }


    @Override
    public void messageArrived(String mqttTopic, MqttMessage mqttMessage) throws Exception {
        String time = new Timestamp(System.currentTimeMillis()).toString();
        System.out.println("***********************************************************************");
        System.out.println("Message Arrived at Time: " + time + "  Topic: " + mqttTopic + "  Message: "
                + new String(mqttMessage.getPayload()));
        if(Objects.equals(mqttTopic, "light")) {
            JSONObject obj = new JSONObject(new String(mqttMessage.getPayload()));
            boolean lightOn = obj.getBoolean("lightOn");
            boolean movementDetected = obj.getBoolean("movementDetected");
            universalService.saveLightState(new LightReading(LocalDateTime.now(),lightOn,movementDetected));
        }
        if(Objects.equals(mqttTopic, "window")) {
            JSONObject obj = new JSONObject(new String(mqttMessage.getPayload()));
            boolean windowOpen = obj.getBoolean("windowOpen");
            boolean klimaOn = obj.getBoolean("klimaOn");
            universalService.saveWindowState(new WindowReading(LocalDateTime.now(),windowOpen,klimaOn));
        }
        if(Objects.equals(mqttTopic, "pm")) {
            JSONObject obj = new JSONObject(new String(mqttMessage.getPayload()));
            Long pm25 = obj.getLong("pm25");
            Long pm10 = obj.getLong("pm10");
            Long noise = obj.getLong("noise");
            Long temperature = obj.getLong("temperature");
            Long humidity = obj.getLong("humidity");
            Double pressure = obj.getDouble("pressure");
            universalService.savePmState(new PmReading(LocalDateTime.now(),pm25,pm10,noise,temperature,humidity,pressure));
        }

    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken token) {

    }
}