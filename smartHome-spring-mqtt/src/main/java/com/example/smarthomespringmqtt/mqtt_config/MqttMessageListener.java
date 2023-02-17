package com.example.smarthomespringmqtt.mqtt_config;

import org.springframework.stereotype.Component;

@Component
public class MqttMessageListener implements Runnable {

    private final MqttSubscriberImpl windowSubscriber;
    private final MqttSubscriberImpl lightSubscriber;
    private final MqttSubscriberImpl pmSubscriber;

    public MqttMessageListener(MqttSubscriberImpl windowSubscriber, MqttSubscriberImpl lightSubscriber, MqttSubscriberImpl pmSubscriber) {
        this.windowSubscriber = windowSubscriber;
        this.lightSubscriber = lightSubscriber;
        this.pmSubscriber = pmSubscriber;
    }


    @Override
    public void run() {
        while (true) {
            windowSubscriber.subscribeMessage("window");
            lightSubscriber.subscribeMessage("light");
            pmSubscriber.subscribeMessage("pm");
        }
    }
}