package com.example.smarthomespringmqtt.mqtt_config;

import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.context.annotation.Configuration;

public abstract class MqttConfig {

    protected final String broker = "0e2b872d30104e009127c52455bcd6be.s2.eu.hivemq.cloud";
    protected final int qos = 1;
    protected Boolean hasSSL = true; /*By default SSL is disabled */
    protected Integer port = 8883; /* Default port */
    protected final String userName = "dragan";
    protected final String password = "123456789Dd";
    protected final String TCP = "tcp://";
    protected final String SSL = "ssl://";

    /**
     * Custom Configuration
     *
     * @param broker
     * @param port
     * @param ssl
     * @param withUserNamePass
     */
    protected abstract void config(String broker, Integer port, Boolean ssl, Boolean withUserNamePass);

    /**
     * Default Configuration
     */
    protected abstract void config();


    public abstract void subscribeMessage(String topic);

    public abstract void disconnect();

    public abstract void messageArrived(String mqttTopic, MqttMessage mqttMessage) throws Exception;
}
