package com.example.smarthomespringmqtt.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Data;

import java.time.LocalDateTime;
@Entity
@Data
public class PmReading {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String localDateTime;
    private Long pm25;
    private Long pm10;
    private Long noise;
    private Long temperature;
    private Long humidity;

    public PmReading(String localDateTime, Long pm25, Long pm10, Long noise, Long temperature, Long humidity) {
        this.localDateTime = localDateTime;
        this.pm25 = pm25;
        this.pm10 = pm10;
        this.noise = noise;
        this.temperature = temperature;
        this.humidity = humidity;
    }

    public PmReading() {
        this.localDateTime = LocalDateTime.now().toString();
        this.pm25 = -1L;
        this.pm10 = -1L;
        this.noise = -1L;
        this.temperature = -1L;
        this.humidity = -1L;
    }
}
