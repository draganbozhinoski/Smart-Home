package com.example.smarthomespringmqtt.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Data;

import java.time.LocalDateTime;
@Entity
@Data
public class LightReading {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String localDateTime;
    private boolean lightOn;
    private boolean movementDetected;

    public LightReading() {
        this.lightOn = false;
        this.localDateTime = LocalDateTime.now().toString();
        this.movementDetected = false;
    }

    public LightReading(String localDateTime, boolean lightOn, boolean movementDetected) {
        this.localDateTime = localDateTime;
        this.lightOn = lightOn;
        this.movementDetected = movementDetected;
    }
}
