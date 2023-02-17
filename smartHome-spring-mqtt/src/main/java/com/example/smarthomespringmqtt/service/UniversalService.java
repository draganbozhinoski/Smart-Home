package com.example.smarthomespringmqtt.service;

import com.example.smarthomespringmqtt.domain.LightReading;
import com.example.smarthomespringmqtt.domain.PmReading;
import com.example.smarthomespringmqtt.domain.WindowReading;
import com.example.smarthomespringmqtt.repository.LightRepository;
import com.example.smarthomespringmqtt.repository.PmRepository;
import com.example.smarthomespringmqtt.repository.WindowRepository;
import lombok.AllArgsConstructor;
import org.springframework.stereotype.Service;

import java.awt.*;
import java.time.LocalDateTime;
import java.util.List;

@Service
@AllArgsConstructor
public class UniversalService {
    private final LightRepository lightRepository;
    private final PmRepository pmRepository;
    private final WindowRepository windowRepository;

    public void saveLightState(LightReading lightReading) {
        lightRepository.save(lightReading);
    }
    public void savePmState(PmReading pmReading) {
        pmRepository.save(pmReading);
    }
    public void saveWindowState(WindowReading windowReading) {
        windowRepository.save(windowReading);
    }

    public LightReading findLastLightReading() {
        List<LightReading> readings = lightRepository.findAll();
        if(readings.size() > 0)
            return readings.get(readings.size() - 1);
        return new LightReading();
    }
    public PmReading findLastPmReading() {
        List<PmReading> readings = pmRepository.findAll();
        if(readings.size() > 0)
            return readings.get(readings.size() - 1);
        return new PmReading();
    }
    public WindowReading findLastWindowReading() {
        List<WindowReading> readings = windowRepository.findAll();
        if(readings.size() > 0)
            return readings.get(readings.size() - 1);
        return new WindowReading();
    }
}
