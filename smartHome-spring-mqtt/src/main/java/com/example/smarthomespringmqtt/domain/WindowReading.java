package com.example.smarthomespringmqtt.domain;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Data;

import java.time.LocalDateTime;

@Entity
@Data
public class WindowReading {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String localDateTime;
    private boolean windowOpen;
    private boolean klimaOn;

    public WindowReading(String localDateTime, boolean windowOpen, boolean klimaOn) {
        this.localDateTime = localDateTime;
        this.windowOpen = windowOpen;
        this.klimaOn = klimaOn;
    }

    public WindowReading() {
        this.localDateTime = LocalDateTime.now().toString();
        this.windowOpen = false;
        this.klimaOn = false;
    }
}
